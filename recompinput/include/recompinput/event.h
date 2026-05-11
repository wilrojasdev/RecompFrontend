#ifndef __RECOMP_INPUT_EVENT_H__
#define __RECOMP_INPUT_EVENT_H__

#include <cstdint>

namespace recompinput {

// ---------------------------------------------------------------------------
// Scancode / Keymod / Gamepad enums
// ---------------------------------------------------------------------------
//
// All numeric values are deliberately identical to SDL2's SDL_Scancode /
// SDL_Keymod / SDL_GameControllerButton / SDL_GameControllerAxis.
//
// Rationale: InputField::input_id is stored as a raw int32_t and serialized to
// user-profile JSON. Keeping the values byte-identical to the SDL enums means
// existing Windows profiles continue to load on Android without migration.
//
// platform_sdl.cpp issues static_asserts to prove the alignment is correct.
// Only the codes the codebase actually uses are enumerated. Codes that never
// appear in input_mapping.cpp or as bind targets are intentionally absent.

enum class Scancode : int32_t {
    Unknown      = 0,

    A            = 4,
    B            = 5,
    C            = 6,
    D            = 7,
    E            = 8,
    F            = 9,
    G            = 10,
    H            = 11,
    I            = 12,
    J            = 13,
    K            = 14,
    L            = 15,
    M            = 16,
    N            = 17,
    O            = 18,
    P            = 19,
    Q            = 20,
    R            = 21,
    S            = 22,
    T            = 23,
    U            = 24,
    V            = 25,
    W            = 26,
    X            = 27,
    Y            = 28,
    Z            = 29,

    N1           = 30,
    N2           = 31,
    N3           = 32,
    N4           = 33,
    N5           = 34,
    N6           = 35,
    N7           = 36,
    N8           = 37,
    N9           = 38,
    N0           = 39,

    Return       = 40,
    Escape       = 41,
    Backspace    = 42,
    Tab          = 43,
    Space        = 44,

    Capslock     = 57,

    F1           = 58,
    F2           = 59,
    F3           = 60,
    F4           = 61,
    F5           = 62,
    F6           = 63,
    F7           = 64,
    F8           = 65,
    F9           = 66,
    F10          = 67,
    F11          = 68,
    F12          = 69,

    PrintScreen  = 70,
    ScrollLock   = 71,
    Pause        = 72,
    Insert       = 73,
    Home         = 74,
    PageUp       = 75,
    Delete       = 76,
    End          = 77,
    PageDown     = 78,
    Right        = 79,
    Left         = 80,
    Down         = 81,
    Up           = 82,

    NumLockClear = 83,

    // F13-F24 are USB HID codes 0x68..0x73. Only F15-F17 are used as "fake
    // mapped" sentinels in the menu input system (see ui_state.cpp
    // is_sdl_input_fake_mapped); they are never produced by a real Android
    // key event.
    F13          = 104,
    F14          = 105,
    F15          = 106,
    F16          = 107,
    F17          = 108,
    F18          = 109,
    F19          = 110,
    F20          = 111,
    F21          = 112,
    F22          = 113,
    F23          = 114,
    F24          = 115,

    LCtrl        = 224,
    LShift       = 225,
    LAlt         = 226,
    LGui         = 227,
    RCtrl        = 228,
    RShift       = 229,
    RAlt         = 230,
    RGui         = 231,
};

// SDL_Keymod is a bitmask. Values mirror KMOD_* exactly.
enum Keymod : uint16_t {
    KMOD_None     = 0x0000,
    KMOD_LShift   = 0x0001,
    KMOD_RShift   = 0x0002,
    KMOD_LCtrl    = 0x0040,
    KMOD_RCtrl    = 0x0080,
    KMOD_LAlt     = 0x0100,
    KMOD_RAlt     = 0x0200,
    KMOD_LGui     = 0x0400,
    KMOD_RGui     = 0x0800,
    KMOD_Num      = 0x1000,
    KMOD_Caps     = 0x2000,
    KMOD_Mode     = 0x4000,
    KMOD_Scroll   = 0x8000,
    KMOD_Ctrl     = KMOD_LCtrl  | KMOD_RCtrl,
    KMOD_Shift    = KMOD_LShift | KMOD_RShift,
    KMOD_Alt      = KMOD_LAlt   | KMOD_RAlt,
    KMOD_Gui      = KMOD_LGui   | KMOD_RGui,
};

// Mirrors SDL_GameControllerButton. SDL2 lacks SOUTH/EAST/WEST/NORTH aliases
// in versions we ship; we provide them here as constants matching the existing
// shim in input_mapping.h.
enum class GamepadButton : int32_t {
    Invalid       = -1,
    A             = 0,
    B             = 1,
    X             = 2,
    Y             = 3,
    Back          = 4,
    Guide         = 5,
    Start         = 6,
    LeftStick     = 7,
    RightStick    = 8,
    LeftShoulder  = 9,
    RightShoulder = 10,
    DpadUp        = 11,
    DpadDown      = 12,
    DpadLeft      = 13,
    DpadRight     = 14,
    Misc1         = 15,
    Paddle1       = 16,
    Paddle2       = 17,
    Paddle3       = 18,
    Paddle4       = 19,
    Touchpad      = 20,
    Max           = 21,

    // Aliases matching the existing recompinput::SDL_CONTROLLER_BUTTON_*
    // constants in input_mapping.h
    South         = A,
    East          = B,
    West          = X,
    North         = Y,
};

// Mirrors SDL_GameControllerAxis.
enum class GamepadAxis : int32_t {
    Invalid       = -1,
    LeftX         = 0,
    LeftY         = 1,
    RightX        = 2,
    RightY        = 3,
    TriggerLeft   = 4,
    TriggerRight  = 5,
    Max           = 6,
};

// Mirrors SDL_SensorType (values used by recompinput SDL_CONTROLLERSENSORUPDATE).
enum class SensorType : int32_t {
    Invalid       = -1,
    Unknown       = 0,
    Accel         = 1,
    Gyro          = 2,
};

// Mouse button indices (mirrors SDL_BUTTON_* mask bits' indices).
enum class MouseButton : uint8_t {
    Left   = 1,
    Middle = 2,
    Right  = 3,
    X1     = 4,
    X2     = 5,
};

// ---------------------------------------------------------------------------
// Event type tags & payloads
// ---------------------------------------------------------------------------
//
// Replaces SDL_Event for everything recompinput consumes. The platform layer
// translates the host's native events into recompinput::Event and pushes them
// onto the recompui queue.

enum class EventType : int32_t {
    None = 0,

    KeyDown,
    KeyUp,

    MouseMotion,
    MouseButtonDown,
    MouseButtonUp,
    MouseWheel,

    GamepadAdded,
    GamepadRemoved,
    GamepadButtonDown,
    GamepadButtonUp,
    GamepadAxisMotion,
    GamepadSensorUpdate,

    QuitRequested,

    DropBegin,
    DropFile,
    DropComplete,

    TextInput,

    // Synthetic event emitted by recompinput when an axis crosses the digital
    // threshold during binding mode. Replaces the SDL_USEREVENT abuse in the
    // original event filter.
    UserStickReturn,

    WindowSizeChanged,
    WindowMouseLeave,
};

struct KeyEvent {
    Scancode scancode;
    uint16_t mod;   // bitmask of Keymod
    bool     repeat;
};

struct MouseMotionEvent {
    int32_t x;
    int32_t y;
    int32_t xrel;
    int32_t yrel;
};

struct MouseButtonEvent {
    MouseButton button;
    int32_t     x;
    int32_t     y;
    uint8_t     clicks;
};

struct MouseWheelEvent {
    int32_t y;
    bool    flipped;
};

struct GamepadDeviceEvent {
    int32_t which;  // joystick instance id
};

struct GamepadButtonEvent {
    int32_t       which;
    GamepadButton button;
};

struct GamepadAxisEvent {
    int32_t     which;
    GamepadAxis axis;
    int16_t     value;  // raw -32768..32767, matches SDL
};

struct GamepadSensorEvent {
    int32_t    which;
    SensorType sensor;
    float      data[3];
    uint64_t   timestamp_ns;
};

struct DropFileEvent {
    // Heap-allocated UTF-8 path. Receiver takes ownership and must free with
    // recompinput::platform::free_drop_path.
    char* path;
};

struct TextInputEvent {
    // Up to 32 UTF-8 bytes, null-terminated. Matches SDL_TextInputEvent layout.
    char text[32];
};

struct UserStickReturnEvent {
    GamepadAxis axis;
};

struct WindowSizeChangedEvent {
    int32_t width;
    int32_t height;
};

struct Event {
    EventType type;
    uint32_t  timestamp_ms;

    union {
        KeyEvent               key;
        MouseMotionEvent       motion;
        MouseButtonEvent       button;
        MouseWheelEvent        wheel;
        GamepadDeviceEvent     cdevice;
        GamepadButtonEvent     cbutton;
        GamepadAxisEvent       caxis;
        GamepadSensorEvent     csensor;
        DropFileEvent          drop;
        TextInputEvent         text;
        UserStickReturnEvent   user_stick;
        WindowSizeChangedEvent window_size;
    };

    Event() : type(EventType::None), timestamp_ms(0), key{} {}
};

} // namespace recompinput

#endif // __RECOMP_INPUT_EVENT_H__
