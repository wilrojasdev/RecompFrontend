// =========================================================================
// SDL2 shim implementation for Android
// =========================================================================
//
// Companion to sdl_shim/SDL.h. Implements the declared SDL symbols on top
// of:
//   - An internal thread-safe SDL_Event queue (used by SDL_PollEvent /
//     SDL_PushEvent). banjo_android::touch and android_main push synthetic
//     mouse / key / gamepad events onto this queue.
//   - A keyboard state byte array, indexed by SDL_Scancode. Set/cleared by
//     KEYDOWN / KEYUP events that pass through SDL_PushEvent.
//   - Stubs for clipboard / message box / cursor / gamepad — these will
//     gain real behavior incrementally; today's launcher does not depend
//     on them to be visible & navigable.
//
// Real keyboard / gamepad ingestion lives in android_main.cpp's
// handle_input() callback, which translates AInputEvent into SDL_Event and
// feeds it through SDL_PushEvent.

#if defined(__ANDROID__)

#include "SDL.h"

#include <atomic>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <mutex>
#include <queue>
#include <string>
#include <vector>

#include <android/log.h>

#define SHIM_TAG "BK64-Online::sdl_shim"
#define SHIM_LOG(fmt, ...) __android_log_print(ANDROID_LOG_INFO, SHIM_TAG, fmt, ##__VA_ARGS__)

// External viewport accessor — defined in android_touch.cpp. The launcher
// asks SDL_GetWindowSizeInPixels at startup, and we serve the cached
// ANativeWindow dimensions android_main published via set_viewport.
namespace banjo_android::touch {
    void get_viewport(int& width, int& height);
}

// -------------------------------------------------------------------------
// Event queue
// -------------------------------------------------------------------------

namespace {

std::mutex             g_event_mutex;
std::queue<SDL_Event>  g_event_queue;

// 512 bytes is the SDL_NUM_SCANCODES upper bound used by SDL2 internally.
// We index this array by SDL_Scancode (integer 0..511).
std::array<Uint8, 512> g_key_state{};
std::atomic<Uint16>    g_mod_state{KMOD_NONE};

void apply_key_event_to_state(const SDL_Event& ev) {
    SDL_Scancode s = ev.key.keysym.scancode;
    if (s < 0 || static_cast<size_t>(s) >= g_key_state.size()) return;
    g_key_state[s] = (ev.type == SDL_KEYDOWN) ? 1u : 0u;

    // Keep mod state coarse-tracked. The android_main translator sets
    // ev.key.keysym.mod each event with the current meta state, so we just
    // mirror the most recent value here.
    g_mod_state.store(ev.key.keysym.mod);
}

} // namespace

// -------------------------------------------------------------------------
// Public SDL function implementations
// -------------------------------------------------------------------------

extern "C" {

int SDL_PollEvent(SDL_Event* event) {
    if (!event) return 0;
    std::lock_guard lock{g_event_mutex};
    if (g_event_queue.empty()) return 0;
    *event = g_event_queue.front();
    g_event_queue.pop();
    if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
        apply_key_event_to_state(*event);
    }
    return 1;
}

int SDL_PushEvent(SDL_Event* event) {
    if (!event) return 0;
    std::lock_guard lock{g_event_mutex};
    g_event_queue.push(*event);
    return 1;
}

const Uint8* SDL_GetKeyboardState(int* numkeys) {
    if (numkeys) *numkeys = static_cast<int>(g_key_state.size());
    return g_key_state.data();
}

SDL_Keymod SDL_GetModState(void) {
    return static_cast<SDL_Keymod>(g_mod_state.load());
}

// ---- Game controller ----------------------------------------------------
//
// Stubbed for now. The Bluetooth gamepad path is on the Android port
// backlog (see todo_android_port_remaining). When we add it, replace these
// stubs with calls into a banjo_android::gamepad namespace.

SDL_GameController* SDL_GameControllerOpen(int /*joystick_index*/)            { return nullptr; }
SDL_GameController* SDL_GameControllerFromInstanceID(SDL_JoystickID /*id*/)   { return nullptr; }
SDL_Joystick*       SDL_GameControllerGetJoystick(SDL_GameController* /*c*/)  { return nullptr; }
const char*         SDL_GameControllerName(SDL_GameController* /*c*/)         { return "unknown"; }

Uint8 SDL_GameControllerGetButton(SDL_GameController* /*c*/, SDL_GameControllerButton /*b*/) {
    return 0;
}

Sint16 SDL_GameControllerGetAxis(SDL_GameController* /*c*/, SDL_GameControllerAxis /*a*/) {
    return 0;
}

SDL_bool SDL_GameControllerHasSensor(SDL_GameController* /*c*/, SDL_SensorType /*t*/) {
    return SDL_FALSE;
}

int SDL_GameControllerSetSensorEnabled(SDL_GameController* /*c*/, SDL_SensorType /*t*/, SDL_bool /*on*/) {
    return -1;
}

int SDL_GameControllerRumble(SDL_GameController* /*c*/, Uint16 /*l*/, Uint16 /*h*/, Uint32 /*ms*/) {
    return -1;
}

// ---- Joystick -----------------------------------------------------------

SDL_JoystickID   SDL_JoystickInstanceID(SDL_Joystick* /*j*/) { return -1; }
const char*      SDL_JoystickPath(SDL_Joystick* /*j*/)       { return ""; }
SDL_JoystickGUID SDL_JoystickGetGUID(SDL_Joystick* /*j*/)    { return SDL_JoystickGUID{}; }
const char*      SDL_JoystickGetSerial(SDL_Joystick* /*j*/)  { return ""; }
int              SDL_JoystickRumble(SDL_Joystick* /*j*/, Uint16, Uint16, Uint32) { return -1; }
void SDL_GetJoystickGUIDInfo(SDL_JoystickGUID /*guid*/, Uint16* vendor, Uint16* product,
                             Uint16* version, Uint16* crc16) {
    if (vendor)  *vendor  = 0;
    if (product) *product = 0;
    if (version) *version = 0;
    if (crc16)   *crc16   = 0;
}

// ---- Mouse / cursor -----------------------------------------------------

int SDL_AddEventWatch(SDL_EventFilter /*filter*/, void* /*userdata*/) { return 0; }
void SDL_DelEventWatch(SDL_EventFilter /*filter*/, void* /*userdata*/) {}

int SDL_ShowCursor(int /*toggle*/)            { return SDL_DISABLE; }
int SDL_SetRelativeMouseMode(SDL_bool /*on*/) { return 0; }
int SDL_CaptureMouse(SDL_bool /*on*/)         { return 0; }

// No mouse pointer on Android — RmlUi calls these on startup but its result
// flows back into SDL_SetCursor which we also no-op.
SDL_Cursor* SDL_CreateSystemCursor(SDL_SystemCursor /*id*/) { return nullptr; }
void        SDL_FreeCursor(SDL_Cursor* /*cursor*/)          {}
void        SDL_SetCursor(SDL_Cursor* /*cursor*/)           {}

// CLOCK_MONOTONIC ticks at nanosecond resolution; report frequency as 1e9.
Uint64 SDL_GetPerformanceCounter(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (Uint64)ts.tv_sec * 1000000000ull + (Uint64)ts.tv_nsec;
}
Uint64 SDL_GetPerformanceFrequency(void) { return 1000000000ull; }

// ---- Clipboard ----------------------------------------------------------
//
// Android clipboard requires JNI access to ClipboardManager. Deferred — for
// the launcher launch flow we don't need it; only the in-game pause menu
// would.

char* SDL_GetClipboardText(void)              { return nullptr; }
int   SDL_SetClipboardText(const char* /*t*/) { return -1; }
int   SDL_HasClipboardText(void)              { return 0; }

// ---- Window -------------------------------------------------------------

void SDL_GetWindowSizeInPixels(SDL_Window* /*window*/, int* w, int* h) {
    int vw = 0, vh = 0;
    banjo_android::touch::get_viewport(vw, vh);
    if (w) *w = vw;
    if (h) *h = vh;
}

// ---- Misc ---------------------------------------------------------------

void SDL_free(void* mem) {
    std::free(mem);
}

int SDL_ShowSimpleMessageBox(Uint32 /*flags*/, const char* title,
                             const char* message, SDL_Window* /*window*/) {
    SHIM_LOG("MessageBox: [%s] %s", title ? title : "", message ? message : "");
    return 0;
}

SDL_bool SDL_SetHint(const char* /*name*/, const char* /*value*/) {
    return SDL_TRUE;
}

} // extern "C"

#endif // __ANDROID__
