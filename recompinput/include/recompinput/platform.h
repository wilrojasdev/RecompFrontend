#ifndef __RECOMP_INPUT_PLATFORM_H__
#define __RECOMP_INPUT_PLATFORM_H__

#include <cstdint>
#include "recompinput/event.h"

namespace recompinput::platform {

// ---------------------------------------------------------------------------
// Platform abstraction for the input layer
// ---------------------------------------------------------------------------
//
// Each host OS implements this interface in a single translation unit:
//   - platform_sdl.cpp    : Windows/macOS/Linux (wraps SDL2)
//   - platform_android.cpp: Android (wraps AInputEvent / Sensor APIs)
//
// Only one TU is compiled into recompinput per build, selected by the
// recompinput CMakeLists based on RECOMPINPUT_BACKEND.

// Initialize the backend. Called once during recompinput startup.
// Returns false on fatal init failure.
bool init();

// Tear down the backend. Called once at shutdown.
void shutdown();

// Drain the next pending event. Returns false when the queue is empty.
// Equivalent to SDL_PollEvent(&ev) returning non-zero.
bool poll_event(Event& out);

// ---------------------------------------------------------------------------
// Keyboard state polling
// ---------------------------------------------------------------------------

// Returns a pointer to a static array indexed by Scancode integer value.
// `num_keys_out` receives the array length. The pointer is stable for the
// lifetime of the platform; callers re-read each call (state may be cached
// or freshly polled depending on backend).
const bool* get_keyboard_state(int32_t* num_keys_out);

// Current modifier keys as a Keymod bitmask.
uint16_t get_mod_state();

// ---------------------------------------------------------------------------
// Gamepad enumeration and state polling
// ---------------------------------------------------------------------------

// Opaque platform handle (SDL_GameController* on desktop, custom struct on
// Android). nullptr means "no gamepad".
struct Gamepad_;
using GamepadHandle = Gamepad_*;

// Look up a gamepad by joystick instance id (the int that arrives in
// GamepadDeviceEvent::which).
GamepadHandle gamepad_from_instance(int32_t instance_id);

// Path/name strings for debug logging. Caller does not own the returned ptr.
const char* gamepad_path(GamepadHandle);
const char* gamepad_name(GamepadHandle);

// Read digital button state.
bool gamepad_get_button(GamepadHandle, GamepadButton);

// Read axis state as raw int16, identical range to SDL_GameControllerGetAxis
// (-32768..32767 for sticks, 0..32767 for triggers).
int16_t gamepad_get_axis_raw(GamepadHandle, GamepadAxis);

// Trigger rumble. `duration_ms` 0 stops rumble. Both motors get the same
// strength on platforms that don't expose separate channels.
void gamepad_rumble(GamepadHandle, uint16_t low_freq, uint16_t high_freq,
                    uint32_t duration_ms);

// Sensor support.
bool gamepad_has_sensor(GamepadHandle, SensorType);
void gamepad_set_sensor_enabled(GamepadHandle, SensorType, bool enabled);

// ---------------------------------------------------------------------------
// Cursor & relative mouse mode (no-op on Android)
// ---------------------------------------------------------------------------

void show_cursor(bool visible);
void set_relative_mouse_mode(bool relative);

// ---------------------------------------------------------------------------
// Clipboard (no-op on Android initially; can use ClipboardManager later)
// ---------------------------------------------------------------------------

// Caller takes ownership of returned UTF-8 string (heap-allocated) and must
// call free_clipboard_text on it. Returns nullptr if no clipboard text.
char* get_clipboard_text();
void  set_clipboard_text(const char* text_utf8);
void  free_clipboard_text(char* text);

// Free a path returned by a DropFileEvent.
void free_drop_path(char* path);

// ---------------------------------------------------------------------------
// Error reporting
// ---------------------------------------------------------------------------

void show_error_box(const char* title, const char* message);

} // namespace recompinput::platform

#endif // __RECOMP_INPUT_PLATFORM_H__
