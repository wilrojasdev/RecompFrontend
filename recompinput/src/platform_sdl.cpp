// SDL2 backend for the recompinput platform abstraction.
//
// Compiled into recompinput on Windows / macOS / Linux. Translates
// SDL_Events to recompinput::Event and wraps SDL polling APIs in the
// recompinput::platform namespace.
//
// The static_asserts at the top of this file lock the numerical alignment
// between SDL_Scancode / SDL_GameController* and our public enums in
// recompinput/event.h. If SDL ever renumbers, the build breaks here.

#if !defined(__ANDROID__)

#include "recompinput/event.h"
#include "recompinput/platform.h"

#include <SDL.h>

#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace recompinput::platform {

// ---------------------------------------------------------------------------
// Compile-time alignment proofs
// ---------------------------------------------------------------------------

#define ASSERT_EQ(a, b) static_assert(static_cast<int32_t>(a) == static_cast<int32_t>(b), #a " != " #b)

ASSERT_EQ(Scancode::Unknown,      SDL_SCANCODE_UNKNOWN);
ASSERT_EQ(Scancode::A,            SDL_SCANCODE_A);
ASSERT_EQ(Scancode::Z,            SDL_SCANCODE_Z);
ASSERT_EQ(Scancode::N0,           SDL_SCANCODE_0);
ASSERT_EQ(Scancode::N9,           SDL_SCANCODE_9);
ASSERT_EQ(Scancode::Return,       SDL_SCANCODE_RETURN);
ASSERT_EQ(Scancode::Escape,       SDL_SCANCODE_ESCAPE);
ASSERT_EQ(Scancode::Backspace,    SDL_SCANCODE_BACKSPACE);
ASSERT_EQ(Scancode::Tab,          SDL_SCANCODE_TAB);
ASSERT_EQ(Scancode::Space,        SDL_SCANCODE_SPACE);
ASSERT_EQ(Scancode::Capslock,     SDL_SCANCODE_CAPSLOCK);
ASSERT_EQ(Scancode::F1,           SDL_SCANCODE_F1);
ASSERT_EQ(Scancode::F12,          SDL_SCANCODE_F12);
ASSERT_EQ(Scancode::F15,          SDL_SCANCODE_F15);
ASSERT_EQ(Scancode::F17,          SDL_SCANCODE_F17);
ASSERT_EQ(Scancode::F24,          SDL_SCANCODE_F24);
ASSERT_EQ(Scancode::PrintScreen,  SDL_SCANCODE_PRINTSCREEN);
ASSERT_EQ(Scancode::ScrollLock,   SDL_SCANCODE_SCROLLLOCK);
ASSERT_EQ(Scancode::Pause,        SDL_SCANCODE_PAUSE);
ASSERT_EQ(Scancode::Insert,       SDL_SCANCODE_INSERT);
ASSERT_EQ(Scancode::Home,         SDL_SCANCODE_HOME);
ASSERT_EQ(Scancode::PageUp,       SDL_SCANCODE_PAGEUP);
ASSERT_EQ(Scancode::Delete,       SDL_SCANCODE_DELETE);
ASSERT_EQ(Scancode::End,          SDL_SCANCODE_END);
ASSERT_EQ(Scancode::PageDown,     SDL_SCANCODE_PAGEDOWN);
ASSERT_EQ(Scancode::Right,        SDL_SCANCODE_RIGHT);
ASSERT_EQ(Scancode::Left,         SDL_SCANCODE_LEFT);
ASSERT_EQ(Scancode::Down,         SDL_SCANCODE_DOWN);
ASSERT_EQ(Scancode::Up,           SDL_SCANCODE_UP);
ASSERT_EQ(Scancode::NumLockClear, SDL_SCANCODE_NUMLOCKCLEAR);
ASSERT_EQ(Scancode::LCtrl,        SDL_SCANCODE_LCTRL);
ASSERT_EQ(Scancode::LShift,       SDL_SCANCODE_LSHIFT);
ASSERT_EQ(Scancode::LAlt,         SDL_SCANCODE_LALT);
ASSERT_EQ(Scancode::LGui,         SDL_SCANCODE_LGUI);
ASSERT_EQ(Scancode::RCtrl,        SDL_SCANCODE_RCTRL);
ASSERT_EQ(Scancode::RShift,       SDL_SCANCODE_RSHIFT);
ASSERT_EQ(Scancode::RAlt,         SDL_SCANCODE_RALT);
ASSERT_EQ(Scancode::RGui,         SDL_SCANCODE_RGUI);

ASSERT_EQ(KMOD_None,   KMOD_NONE);
ASSERT_EQ(KMOD_LShift, ::KMOD_LSHIFT);
ASSERT_EQ(KMOD_RShift, ::KMOD_RSHIFT);
ASSERT_EQ(KMOD_LCtrl,  ::KMOD_LCTRL);
ASSERT_EQ(KMOD_RCtrl,  ::KMOD_RCTRL);
ASSERT_EQ(KMOD_LAlt,   ::KMOD_LALT);
ASSERT_EQ(KMOD_RAlt,   ::KMOD_RALT);
ASSERT_EQ(KMOD_LGui,   ::KMOD_LGUI);
ASSERT_EQ(KMOD_RGui,   ::KMOD_RGUI);
ASSERT_EQ(KMOD_Num,    ::KMOD_NUM);
ASSERT_EQ(KMOD_Caps,   ::KMOD_CAPS);
ASSERT_EQ(KMOD_Mode,   ::KMOD_MODE);

ASSERT_EQ(GamepadButton::Invalid,       SDL_CONTROLLER_BUTTON_INVALID);
ASSERT_EQ(GamepadButton::A,             SDL_CONTROLLER_BUTTON_A);
ASSERT_EQ(GamepadButton::B,             SDL_CONTROLLER_BUTTON_B);
ASSERT_EQ(GamepadButton::X,             SDL_CONTROLLER_BUTTON_X);
ASSERT_EQ(GamepadButton::Y,             SDL_CONTROLLER_BUTTON_Y);
ASSERT_EQ(GamepadButton::Back,          SDL_CONTROLLER_BUTTON_BACK);
ASSERT_EQ(GamepadButton::Guide,         SDL_CONTROLLER_BUTTON_GUIDE);
ASSERT_EQ(GamepadButton::Start,         SDL_CONTROLLER_BUTTON_START);
ASSERT_EQ(GamepadButton::LeftStick,     SDL_CONTROLLER_BUTTON_LEFTSTICK);
ASSERT_EQ(GamepadButton::RightStick,    SDL_CONTROLLER_BUTTON_RIGHTSTICK);
ASSERT_EQ(GamepadButton::LeftShoulder,  SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
ASSERT_EQ(GamepadButton::RightShoulder, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
ASSERT_EQ(GamepadButton::DpadUp,        SDL_CONTROLLER_BUTTON_DPAD_UP);
ASSERT_EQ(GamepadButton::DpadDown,      SDL_CONTROLLER_BUTTON_DPAD_DOWN);
ASSERT_EQ(GamepadButton::DpadLeft,      SDL_CONTROLLER_BUTTON_DPAD_LEFT);
ASSERT_EQ(GamepadButton::DpadRight,     SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
ASSERT_EQ(GamepadButton::Misc1,         SDL_CONTROLLER_BUTTON_MISC1);
ASSERT_EQ(GamepadButton::Paddle1,       SDL_CONTROLLER_BUTTON_PADDLE1);
ASSERT_EQ(GamepadButton::Paddle2,       SDL_CONTROLLER_BUTTON_PADDLE2);
ASSERT_EQ(GamepadButton::Paddle3,       SDL_CONTROLLER_BUTTON_PADDLE3);
ASSERT_EQ(GamepadButton::Paddle4,       SDL_CONTROLLER_BUTTON_PADDLE4);
ASSERT_EQ(GamepadButton::Touchpad,      SDL_CONTROLLER_BUTTON_TOUCHPAD);
ASSERT_EQ(GamepadButton::Max,           SDL_CONTROLLER_BUTTON_MAX);

ASSERT_EQ(GamepadAxis::Invalid,      SDL_CONTROLLER_AXIS_INVALID);
ASSERT_EQ(GamepadAxis::LeftX,        SDL_CONTROLLER_AXIS_LEFTX);
ASSERT_EQ(GamepadAxis::LeftY,        SDL_CONTROLLER_AXIS_LEFTY);
ASSERT_EQ(GamepadAxis::RightX,       SDL_CONTROLLER_AXIS_RIGHTX);
ASSERT_EQ(GamepadAxis::RightY,       SDL_CONTROLLER_AXIS_RIGHTY);
ASSERT_EQ(GamepadAxis::TriggerLeft,  SDL_CONTROLLER_AXIS_TRIGGERLEFT);
ASSERT_EQ(GamepadAxis::TriggerRight, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
ASSERT_EQ(GamepadAxis::Max,          SDL_CONTROLLER_AXIS_MAX);

ASSERT_EQ(SensorType::Invalid, SDL_SENSOR_INVALID);
ASSERT_EQ(SensorType::Unknown, SDL_SENSOR_UNKNOWN);
ASSERT_EQ(SensorType::Accel,   SDL_SENSOR_ACCEL);
ASSERT_EQ(SensorType::Gyro,    SDL_SENSOR_GYRO);

ASSERT_EQ(MouseButton::Left,   SDL_BUTTON_LEFT);
ASSERT_EQ(MouseButton::Middle, SDL_BUTTON_MIDDLE);
ASSERT_EQ(MouseButton::Right,  SDL_BUTTON_RIGHT);
ASSERT_EQ(MouseButton::X1,     SDL_BUTTON_X1);
ASSERT_EQ(MouseButton::X2,     SDL_BUTTON_X2);

#undef ASSERT_EQ

// ---------------------------------------------------------------------------
// SDL_Event → recompinput::Event translation
// ---------------------------------------------------------------------------

namespace {

bool translate(const SDL_Event& src, Event& dst) {
    dst.timestamp_ms = src.common.timestamp;

    switch (src.type) {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        dst.type = (src.type == SDL_KEYDOWN) ? EventType::KeyDown : EventType::KeyUp;
        dst.key.scancode = static_cast<Scancode>(src.key.keysym.scancode);
        dst.key.mod      = static_cast<uint16_t>(src.key.keysym.mod);
        dst.key.repeat   = src.key.repeat != 0;
        return true;

    case SDL_MOUSEMOTION:
        dst.type = EventType::MouseMotion;
        dst.motion.x    = src.motion.x;
        dst.motion.y    = src.motion.y;
        dst.motion.xrel = src.motion.xrel;
        dst.motion.yrel = src.motion.yrel;
        return true;

    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        dst.type = (src.type == SDL_MOUSEBUTTONDOWN) ? EventType::MouseButtonDown
                                                     : EventType::MouseButtonUp;
        dst.button.button = static_cast<MouseButton>(src.button.button);
        dst.button.x      = src.button.x;
        dst.button.y      = src.button.y;
        dst.button.clicks = src.button.clicks;
        return true;

    case SDL_MOUSEWHEEL:
        dst.type = EventType::MouseWheel;
        dst.wheel.y       = src.wheel.y;
        dst.wheel.flipped = src.wheel.direction == SDL_MOUSEWHEEL_FLIPPED;
        return true;

    case SDL_CONTROLLERDEVICEADDED:
    case SDL_CONTROLLERDEVICEREMOVED:
        dst.type = (src.type == SDL_CONTROLLERDEVICEADDED) ? EventType::GamepadAdded
                                                           : EventType::GamepadRemoved;
        dst.cdevice.which = src.cdevice.which;
        return true;

    case SDL_CONTROLLERBUTTONDOWN:
    case SDL_CONTROLLERBUTTONUP:
        dst.type = (src.type == SDL_CONTROLLERBUTTONDOWN) ? EventType::GamepadButtonDown
                                                          : EventType::GamepadButtonUp;
        dst.cbutton.which  = src.cbutton.which;
        dst.cbutton.button = static_cast<GamepadButton>(src.cbutton.button);
        return true;

    case SDL_CONTROLLERAXISMOTION:
        dst.type = EventType::GamepadAxisMotion;
        dst.caxis.which = src.caxis.which;
        dst.caxis.axis  = static_cast<GamepadAxis>(src.caxis.axis);
        dst.caxis.value = src.caxis.value;
        return true;

    case SDL_CONTROLLERSENSORUPDATE:
        dst.type = EventType::GamepadSensorUpdate;
        dst.csensor.which        = src.csensor.which;
        dst.csensor.sensor       = static_cast<SensorType>(src.csensor.sensor);
        dst.csensor.data[0]      = src.csensor.data[0];
        dst.csensor.data[1]      = src.csensor.data[1];
        dst.csensor.data[2]      = src.csensor.data[2];
        dst.csensor.timestamp_ns = src.csensor.timestamp;
        return true;

    case SDL_QUIT:
        dst.type = EventType::QuitRequested;
        return true;

    case SDL_DROPBEGIN:
        dst.type = EventType::DropBegin;
        return true;

    case SDL_DROPFILE:
        dst.type = EventType::DropFile;
        // SDL hands us a heap-allocated UTF-8 path; transfer ownership to the
        // caller, who must release with free_drop_path (which SDL_free's it).
        dst.drop.path = src.drop.file;
        return true;

    case SDL_DROPCOMPLETE:
        dst.type = EventType::DropComplete;
        return true;

    case SDL_TEXTINPUT:
        dst.type = EventType::TextInput;
        std::memcpy(dst.text.text, src.text.text, sizeof(dst.text.text));
        dst.text.text[sizeof(dst.text.text) - 1] = '\0';
        return true;

    case SDL_WINDOWEVENT:
        if (src.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            dst.type = EventType::WindowSizeChanged;
            dst.window_size.width  = src.window.data1;
            dst.window_size.height = src.window.data2;
            return true;
        }
        if (src.window.event == SDL_WINDOWEVENT_LEAVE) {
            dst.type = EventType::WindowMouseLeave;
            return true;
        }
        return false;

    default:
        return false;
    }
}

} // namespace

// ---------------------------------------------------------------------------
// Public interface
// ---------------------------------------------------------------------------

bool init() {
    // SDL is initialized elsewhere (recompui boot path); nothing platform-
    // specific to do here.
    return true;
}

void shutdown() {}

bool poll_event(Event& out) {
    SDL_Event sdl_ev;
    // Skip event types we don't translate, so the caller sees a clean stream
    // and doesn't have to filter EventType::None.
    while (SDL_PollEvent(&sdl_ev)) {
        if (translate(sdl_ev, out)) {
            return true;
        }
    }
    return false;
}

const bool* get_keyboard_state(int32_t* num_keys_out) {
    int n = 0;
    const Uint8* state = SDL_GetKeyboardState(&n);
    if (num_keys_out) *num_keys_out = n;
    // SDL_GetKeyboardState returns Uint8 (0/1) — bit-compatible with bool on
    // every platform we ship.
    return reinterpret_cast<const bool*>(state);
}

uint16_t get_mod_state() {
    return static_cast<uint16_t>(SDL_GetModState());
}

GamepadHandle gamepad_from_instance(int32_t instance_id) {
    return reinterpret_cast<GamepadHandle>(
        SDL_GameControllerFromInstanceID(instance_id));
}

const char* gamepad_path(GamepadHandle h) {
    if (!h) return "";
    SDL_GameController* c = reinterpret_cast<SDL_GameController*>(h);
    SDL_Joystick* j = SDL_GameControllerGetJoystick(c);
    const char* p = SDL_JoystickPath(j);
    return p ? p : "";
}

const char* gamepad_name(GamepadHandle h) {
    if (!h) return "";
    const char* n = SDL_GameControllerName(reinterpret_cast<SDL_GameController*>(h));
    return n ? n : "";
}

bool gamepad_get_button(GamepadHandle h, GamepadButton button) {
    if (!h) return false;
    return SDL_GameControllerGetButton(
        reinterpret_cast<SDL_GameController*>(h),
        static_cast<SDL_GameControllerButton>(button)) != 0;
}

int16_t gamepad_get_axis_raw(GamepadHandle h, GamepadAxis axis) {
    if (!h) return 0;
    return SDL_GameControllerGetAxis(
        reinterpret_cast<SDL_GameController*>(h),
        static_cast<SDL_GameControllerAxis>(axis));
}

void gamepad_rumble(GamepadHandle h, uint16_t low_freq, uint16_t high_freq,
                    uint32_t duration_ms) {
    if (!h) return;
    SDL_GameControllerRumble(reinterpret_cast<SDL_GameController*>(h),
                             low_freq, high_freq, duration_ms);
}

bool gamepad_has_sensor(GamepadHandle h, SensorType sensor) {
    if (!h) return false;
    return SDL_GameControllerHasSensor(
        reinterpret_cast<SDL_GameController*>(h),
        static_cast<SDL_SensorType>(sensor)) == SDL_TRUE;
}

void gamepad_set_sensor_enabled(GamepadHandle h, SensorType sensor, bool enabled) {
    if (!h) return;
    SDL_GameControllerSetSensorEnabled(
        reinterpret_cast<SDL_GameController*>(h),
        static_cast<SDL_SensorType>(sensor),
        enabled ? SDL_TRUE : SDL_FALSE);
}

void show_cursor(bool visible) {
    SDL_ShowCursor(visible ? SDL_ENABLE : SDL_DISABLE);
}

void set_relative_mouse_mode(bool relative) {
    SDL_SetRelativeMouseMode(relative ? SDL_TRUE : SDL_FALSE);
}

char* get_clipboard_text() {
    char* t = SDL_GetClipboardText();
    if (t && t[0] == '\0') {
        SDL_free(t);
        return nullptr;
    }
    return t;
}

void set_clipboard_text(const char* text_utf8) {
    SDL_SetClipboardText(text_utf8 ? text_utf8 : "");
}

void free_clipboard_text(char* text) {
    if (text) SDL_free(text);
}

void free_drop_path(char* path) {
    if (path) SDL_free(path);
}

void show_error_box(const char* title, const char* message) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             title ? title : "Error",
                             message ? message : "",
                             nullptr);
}

} // namespace recompinput::platform

#endif // !__ANDROID__
