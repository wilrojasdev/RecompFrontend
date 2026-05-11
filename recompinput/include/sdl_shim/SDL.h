// =========================================================================
// SDL2 shim for Android
// =========================================================================
//
// Android NDK ships no SDL2, but recompinput/recompui were written against
// SDL2. Rather than refactor every call site, we ship this header that
// declares SDL's surface as recompinput/recompui consumes it, and a sibling
// translation unit (sdl_shim.cpp) implements the functions in terms of
// recompinput::platform::* and Android NDK APIs.
//
// Numerical values for SDL_Scancode / SDL_GameControllerButton / etc. are
// kept identical to upstream SDL2 (verified by static_asserts in
// platform_sdl.cpp on desktop). Profile JSON written on Windows therefore
// loads byte-identical on Android.
//
// This shim is selected on Android by placing
//   lib/RecompFrontend/recompinput/include/sdl_shim/
// on the Android-only include path *before* any real SDL2 include dir. On
// desktop builds, this file is not on the include path and the upstream
// SDL2 header is used.

#ifndef __RECOMPINPUT_SDL_SHIM_H__
#define __RECOMPINPUT_SDL_SHIM_H__

#if !defined(__ANDROID__)
#error "sdl_shim/SDL.h is the Android-only stand-in for SDL2. Do not include on desktop."
#endif

#include <cstdint>
#include <cstddef>

// -------------------------------------------------------------------------
// Primitive types & booleans
// -------------------------------------------------------------------------

typedef int8_t   Sint8;
typedef uint8_t  Uint8;
typedef int16_t  Sint16;
typedef uint16_t Uint16;
typedef int32_t  Sint32;
typedef uint32_t Uint32;
typedef int64_t  Sint64;
typedef uint64_t Uint64;

typedef enum { SDL_FALSE = 0, SDL_TRUE = 1 } SDL_bool;

#define SDL_ENABLE   1
#define SDL_DISABLE  0

#define SDL_STANDARD_GRAVITY 9.80665f

#define SDLCALL

// -------------------------------------------------------------------------
// Scancodes (USB HID Usage Page 0x07, identical to SDL2)
// -------------------------------------------------------------------------

typedef enum {
    SDL_SCANCODE_UNKNOWN      = 0,

    SDL_SCANCODE_A            = 4,
    SDL_SCANCODE_B            = 5,
    SDL_SCANCODE_C            = 6,
    SDL_SCANCODE_D            = 7,
    SDL_SCANCODE_E            = 8,
    SDL_SCANCODE_F            = 9,
    SDL_SCANCODE_G            = 10,
    SDL_SCANCODE_H            = 11,
    SDL_SCANCODE_I            = 12,
    SDL_SCANCODE_J            = 13,
    SDL_SCANCODE_K            = 14,
    SDL_SCANCODE_L            = 15,
    SDL_SCANCODE_M            = 16,
    SDL_SCANCODE_N            = 17,
    SDL_SCANCODE_O            = 18,
    SDL_SCANCODE_P            = 19,
    SDL_SCANCODE_Q            = 20,
    SDL_SCANCODE_R            = 21,
    SDL_SCANCODE_S            = 22,
    SDL_SCANCODE_T            = 23,
    SDL_SCANCODE_U            = 24,
    SDL_SCANCODE_V            = 25,
    SDL_SCANCODE_W            = 26,
    SDL_SCANCODE_X            = 27,
    SDL_SCANCODE_Y            = 28,
    SDL_SCANCODE_Z            = 29,

    SDL_SCANCODE_1            = 30,
    SDL_SCANCODE_2            = 31,
    SDL_SCANCODE_3            = 32,
    SDL_SCANCODE_4            = 33,
    SDL_SCANCODE_5            = 34,
    SDL_SCANCODE_6            = 35,
    SDL_SCANCODE_7            = 36,
    SDL_SCANCODE_8            = 37,
    SDL_SCANCODE_9            = 38,
    SDL_SCANCODE_0            = 39,

    SDL_SCANCODE_RETURN       = 40,
    SDL_SCANCODE_ESCAPE       = 41,
    SDL_SCANCODE_BACKSPACE    = 42,
    SDL_SCANCODE_TAB          = 43,
    SDL_SCANCODE_SPACE        = 44,

    SDL_SCANCODE_CAPSLOCK     = 57,

    SDL_SCANCODE_F1           = 58,
    SDL_SCANCODE_F2           = 59,
    SDL_SCANCODE_F3           = 60,
    SDL_SCANCODE_F4           = 61,
    SDL_SCANCODE_F5           = 62,
    SDL_SCANCODE_F6           = 63,
    SDL_SCANCODE_F7           = 64,
    SDL_SCANCODE_F8           = 65,
    SDL_SCANCODE_F9           = 66,
    SDL_SCANCODE_F10          = 67,
    SDL_SCANCODE_F11          = 68,
    SDL_SCANCODE_F12          = 69,

    SDL_SCANCODE_PRINTSCREEN  = 70,
    SDL_SCANCODE_SCROLLLOCK   = 71,
    SDL_SCANCODE_PAUSE        = 72,
    SDL_SCANCODE_INSERT       = 73,
    SDL_SCANCODE_HOME         = 74,
    SDL_SCANCODE_PAGEUP       = 75,
    SDL_SCANCODE_DELETE       = 76,
    SDL_SCANCODE_END          = 77,
    SDL_SCANCODE_PAGEDOWN     = 78,
    SDL_SCANCODE_RIGHT        = 79,
    SDL_SCANCODE_LEFT         = 80,
    SDL_SCANCODE_DOWN         = 81,
    SDL_SCANCODE_UP           = 82,

    SDL_SCANCODE_NUMLOCKCLEAR = 83,

    SDL_SCANCODE_F13          = 104,
    SDL_SCANCODE_F14          = 105,
    SDL_SCANCODE_F15          = 106,
    SDL_SCANCODE_F16          = 107,
    SDL_SCANCODE_F17          = 108,
    SDL_SCANCODE_F18          = 109,
    SDL_SCANCODE_F19          = 110,
    SDL_SCANCODE_F20          = 111,
    SDL_SCANCODE_F21          = 112,
    SDL_SCANCODE_F22          = 113,
    SDL_SCANCODE_F23          = 114,
    SDL_SCANCODE_F24          = 115,

    SDL_SCANCODE_LCTRL        = 224,
    SDL_SCANCODE_LSHIFT       = 225,
    SDL_SCANCODE_LALT         = 226,
    SDL_SCANCODE_LGUI         = 227,
    SDL_SCANCODE_RCTRL        = 228,
    SDL_SCANCODE_RSHIFT       = 229,
    SDL_SCANCODE_RALT         = 230,
    SDL_SCANCODE_RGUI         = 231,

    SDL_NUM_SCANCODES         = 512
} SDL_Scancode;

// -------------------------------------------------------------------------
// Key codes (SDLK_*). Values match upstream SDL2 SDL_keycode.h so that any
// keysym pushed into the shim event queue by android_main matches the values
// RmlUi_Platform_SDL.cpp and recompui's key_map table compare against.
// Letters/digits = ASCII; everything else = SDL_SCANCODE_TO_KEYCODE(scancode).
// -------------------------------------------------------------------------

typedef Sint32 SDL_Keycode;

#define SDLK_SCANCODE_MASK         (1 << 30)
#define SDL_SCANCODE_TO_KEYCODE(X) ((X) | SDLK_SCANCODE_MASK)

#define SDLK_UNKNOWN     0
#define SDLK_RETURN      '\r'
#define SDLK_ESCAPE      '\x1B'
#define SDLK_BACKSPACE   '\b'
#define SDLK_TAB         '\t'
#define SDLK_SPACE       ' '

// Printable ASCII punctuation/symbols.
#define SDLK_PLUS        '+'
#define SDLK_COMMA       ','
#define SDLK_MINUS       '-'
#define SDLK_PERIOD      '.'
#define SDLK_SLASH       '/'
#define SDLK_SEMICOLON   ';'
#define SDLK_LEFTBRACKET '['
#define SDLK_BACKSLASH   '\\'
#define SDLK_RIGHTBRACKET ']'
#define SDLK_BACKQUOTE   '`'
#define SDLK_QUOTEDBL    '"'

// Digits 0-9.
#define SDLK_0 '0'
#define SDLK_1 '1'
#define SDLK_2 '2'
#define SDLK_3 '3'
#define SDLK_4 '4'
#define SDLK_5 '5'
#define SDLK_6 '6'
#define SDLK_7 '7'
#define SDLK_8 '8'
#define SDLK_9 '9'

// Letters a-z (lowercase ASCII).
#define SDLK_a 'a'
#define SDLK_b 'b'
#define SDLK_c 'c'
#define SDLK_d 'd'
#define SDLK_e 'e'
#define SDLK_f 'f'
#define SDLK_g 'g'
#define SDLK_h 'h'
#define SDLK_i 'i'
#define SDLK_j 'j'
#define SDLK_k 'k'
#define SDLK_l 'l'
#define SDLK_m 'm'
#define SDLK_n 'n'
#define SDLK_o 'o'
#define SDLK_p 'p'
#define SDLK_q 'q'
#define SDLK_r 'r'
#define SDLK_s 's'
#define SDLK_t 't'
#define SDLK_u 'u'
#define SDLK_v 'v'
#define SDLK_w 'w'
#define SDLK_x 'x'
#define SDLK_y 'y'
#define SDLK_z 'z'

// Recompui references SDLK_F (uppercase) as a synonym for the F key alongside
// SDLK_f; keep both for backwards compatibility with the static_assert table.
#define SDLK_F SDLK_f

// Navigation cluster.
#define SDLK_CAPSLOCK    SDL_SCANCODE_TO_KEYCODE(0x39)
#define SDLK_F1          SDL_SCANCODE_TO_KEYCODE(0x3A)
#define SDLK_F2          SDL_SCANCODE_TO_KEYCODE(0x3B)
#define SDLK_F3          SDL_SCANCODE_TO_KEYCODE(0x3C)
#define SDLK_F4          SDL_SCANCODE_TO_KEYCODE(0x3D)
#define SDLK_F5          SDL_SCANCODE_TO_KEYCODE(0x3E)
#define SDLK_F6          SDL_SCANCODE_TO_KEYCODE(0x3F)
#define SDLK_F7          SDL_SCANCODE_TO_KEYCODE(0x40)
#define SDLK_F8          SDL_SCANCODE_TO_KEYCODE(0x41)
#define SDLK_F9          SDL_SCANCODE_TO_KEYCODE(0x42)
#define SDLK_F10         SDL_SCANCODE_TO_KEYCODE(0x43)
#define SDLK_F11         SDL_SCANCODE_TO_KEYCODE(0x44)
#define SDLK_F12         SDL_SCANCODE_TO_KEYCODE(0x45)
#define SDLK_F13         SDL_SCANCODE_TO_KEYCODE(0x68)
#define SDLK_F14         SDL_SCANCODE_TO_KEYCODE(0x69)
#define SDLK_F15         SDL_SCANCODE_TO_KEYCODE(0x6A)
#define SDLK_F16         SDL_SCANCODE_TO_KEYCODE(0x6B)
#define SDLK_F17         SDL_SCANCODE_TO_KEYCODE(0x6C)
#define SDLK_F18         SDL_SCANCODE_TO_KEYCODE(0x6D)
#define SDLK_F19         SDL_SCANCODE_TO_KEYCODE(0x6E)
#define SDLK_F20         SDL_SCANCODE_TO_KEYCODE(0x6F)
#define SDLK_F21         SDL_SCANCODE_TO_KEYCODE(0x70)
#define SDLK_F22         SDL_SCANCODE_TO_KEYCODE(0x71)
#define SDLK_F23         SDL_SCANCODE_TO_KEYCODE(0x72)
#define SDLK_F24         SDL_SCANCODE_TO_KEYCODE(0x73)

#define SDLK_PRINTSCREEN SDL_SCANCODE_TO_KEYCODE(0x46)
#define SDLK_SCROLLLOCK  SDL_SCANCODE_TO_KEYCODE(0x47)
#define SDLK_PAUSE       SDL_SCANCODE_TO_KEYCODE(0x48)
#define SDLK_INSERT      SDL_SCANCODE_TO_KEYCODE(0x49)
#define SDLK_HOME        SDL_SCANCODE_TO_KEYCODE(0x4A)
#define SDLK_PAGEUP      SDL_SCANCODE_TO_KEYCODE(0x4B)
#define SDLK_DELETE      0x7F
#define SDLK_END         SDL_SCANCODE_TO_KEYCODE(0x4D)
#define SDLK_PAGEDOWN    SDL_SCANCODE_TO_KEYCODE(0x4E)
#define SDLK_RIGHT       SDL_SCANCODE_TO_KEYCODE(0x4F)
#define SDLK_LEFT        SDL_SCANCODE_TO_KEYCODE(0x50)
#define SDLK_DOWN        SDL_SCANCODE_TO_KEYCODE(0x51)
#define SDLK_UP          SDL_SCANCODE_TO_KEYCODE(0x52)
#define SDLK_NUMLOCKCLEAR SDL_SCANCODE_TO_KEYCODE(0x53)
#define SDLK_CLEAR       SDL_SCANCODE_TO_KEYCODE(0x9C)
#define SDLK_HELP        SDL_SCANCODE_TO_KEYCODE(0x75)

// Keypad.
#define SDLK_KP_DIVIDE   SDL_SCANCODE_TO_KEYCODE(0x54)
#define SDLK_KP_MULTIPLY SDL_SCANCODE_TO_KEYCODE(0x55)
#define SDLK_KP_MINUS    SDL_SCANCODE_TO_KEYCODE(0x56)
#define SDLK_KP_PLUS     SDL_SCANCODE_TO_KEYCODE(0x57)
#define SDLK_KP_ENTER    SDL_SCANCODE_TO_KEYCODE(0x58)
#define SDLK_KP_1        SDL_SCANCODE_TO_KEYCODE(0x59)
#define SDLK_KP_2        SDL_SCANCODE_TO_KEYCODE(0x5A)
#define SDLK_KP_3        SDL_SCANCODE_TO_KEYCODE(0x5B)
#define SDLK_KP_4        SDL_SCANCODE_TO_KEYCODE(0x5C)
#define SDLK_KP_5        SDL_SCANCODE_TO_KEYCODE(0x5D)
#define SDLK_KP_6        SDL_SCANCODE_TO_KEYCODE(0x5E)
#define SDLK_KP_7        SDL_SCANCODE_TO_KEYCODE(0x5F)
#define SDLK_KP_8        SDL_SCANCODE_TO_KEYCODE(0x60)
#define SDLK_KP_9        SDL_SCANCODE_TO_KEYCODE(0x61)
#define SDLK_KP_0        SDL_SCANCODE_TO_KEYCODE(0x62)
#define SDLK_KP_PERIOD   SDL_SCANCODE_TO_KEYCODE(0x63)
#define SDLK_KP_EQUALS   SDL_SCANCODE_TO_KEYCODE(0x67)

// Modifier keys.
#define SDLK_LCTRL       SDL_SCANCODE_TO_KEYCODE(0xE0)
#define SDLK_LSHIFT      SDL_SCANCODE_TO_KEYCODE(0xE1)
#define SDLK_LALT        SDL_SCANCODE_TO_KEYCODE(0xE2)
#define SDLK_LGUI        SDL_SCANCODE_TO_KEYCODE(0xE3)
#define SDLK_RCTRL       SDL_SCANCODE_TO_KEYCODE(0xE4)
#define SDLK_RSHIFT      SDL_SCANCODE_TO_KEYCODE(0xE5)
#define SDLK_RALT        SDL_SCANCODE_TO_KEYCODE(0xE6)
#define SDLK_RGUI        SDL_SCANCODE_TO_KEYCODE(0xE7)
// SDL1 aliases retained by some code paths.
#define SDLK_LSUPER      SDLK_LGUI
#define SDLK_RSUPER      SDLK_RGUI

// -------------------------------------------------------------------------
// Keymod (modifier key bitmask). Plain enum so users can OR values.
// -------------------------------------------------------------------------

typedef enum {
    KMOD_NONE     = 0x0000,
    KMOD_LSHIFT   = 0x0001,
    KMOD_RSHIFT   = 0x0002,
    KMOD_LCTRL    = 0x0040,
    KMOD_RCTRL    = 0x0080,
    KMOD_LALT     = 0x0100,
    KMOD_RALT     = 0x0200,
    KMOD_LGUI     = 0x0400,
    KMOD_RGUI     = 0x0800,
    KMOD_NUM      = 0x1000,
    KMOD_CAPS     = 0x2000,
    KMOD_MODE     = 0x4000,
    KMOD_SCROLL   = 0x8000,
    KMOD_CTRL     = KMOD_LCTRL  | KMOD_RCTRL,
    KMOD_SHIFT    = KMOD_LSHIFT | KMOD_RSHIFT,
    KMOD_ALT      = KMOD_LALT   | KMOD_RALT,
    KMOD_GUI      = KMOD_LGUI   | KMOD_RGUI,
    KMOD_RESERVED = 0x8000
} SDL_Keymod;

// -------------------------------------------------------------------------
// Mouse button indices
// -------------------------------------------------------------------------

#define SDL_RELEASED      0
#define SDL_PRESSED       1

#define SDL_BUTTON_LEFT   1
#define SDL_BUTTON_MIDDLE 2
#define SDL_BUTTON_RIGHT  3
#define SDL_BUTTON_X1     4
#define SDL_BUTTON_X2     5

// -------------------------------------------------------------------------
// Game controller button & axis enums
// -------------------------------------------------------------------------

typedef enum {
    SDL_CONTROLLER_BUTTON_INVALID       = -1,
    SDL_CONTROLLER_BUTTON_A             = 0,
    SDL_CONTROLLER_BUTTON_B             = 1,
    SDL_CONTROLLER_BUTTON_X             = 2,
    SDL_CONTROLLER_BUTTON_Y             = 3,
    SDL_CONTROLLER_BUTTON_BACK          = 4,
    SDL_CONTROLLER_BUTTON_GUIDE         = 5,
    SDL_CONTROLLER_BUTTON_START         = 6,
    SDL_CONTROLLER_BUTTON_LEFTSTICK     = 7,
    SDL_CONTROLLER_BUTTON_RIGHTSTICK    = 8,
    SDL_CONTROLLER_BUTTON_LEFTSHOULDER  = 9,
    SDL_CONTROLLER_BUTTON_RIGHTSHOULDER = 10,
    SDL_CONTROLLER_BUTTON_DPAD_UP       = 11,
    SDL_CONTROLLER_BUTTON_DPAD_DOWN     = 12,
    SDL_CONTROLLER_BUTTON_DPAD_LEFT     = 13,
    SDL_CONTROLLER_BUTTON_DPAD_RIGHT    = 14,
    SDL_CONTROLLER_BUTTON_MISC1         = 15,
    SDL_CONTROLLER_BUTTON_PADDLE1       = 16,
    SDL_CONTROLLER_BUTTON_PADDLE2       = 17,
    SDL_CONTROLLER_BUTTON_PADDLE3       = 18,
    SDL_CONTROLLER_BUTTON_PADDLE4       = 19,
    SDL_CONTROLLER_BUTTON_TOUCHPAD      = 20,
    SDL_CONTROLLER_BUTTON_MAX           = 21
} SDL_GameControllerButton;

typedef enum {
    SDL_CONTROLLER_AXIS_INVALID      = -1,
    SDL_CONTROLLER_AXIS_LEFTX        = 0,
    SDL_CONTROLLER_AXIS_LEFTY        = 1,
    SDL_CONTROLLER_AXIS_RIGHTX       = 2,
    SDL_CONTROLLER_AXIS_RIGHTY       = 3,
    SDL_CONTROLLER_AXIS_TRIGGERLEFT  = 4,
    SDL_CONTROLLER_AXIS_TRIGGERRIGHT = 5,
    SDL_CONTROLLER_AXIS_MAX          = 6
} SDL_GameControllerAxis;

typedef enum {
    SDL_SENSOR_INVALID = -1,
    SDL_SENSOR_UNKNOWN = 0,
    SDL_SENSOR_ACCEL   = 1,
    SDL_SENSOR_GYRO    = 2
} SDL_SensorType;

// -------------------------------------------------------------------------
// Joystick / GameController opaque types
// -------------------------------------------------------------------------

typedef Sint32 SDL_JoystickID;

typedef struct SDL_Joystick       SDL_Joystick;
typedef struct SDL_GameController SDL_GameController;

typedef struct {
    Uint8 data[16];
} SDL_JoystickGUID;

// -------------------------------------------------------------------------
// Window (opaque). recompui calls SDL_GetWindowSizeInPixels.
// -------------------------------------------------------------------------

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Cursor SDL_Cursor;

#define SDL_WINDOW_VULKAN 0x10000000u

// -------------------------------------------------------------------------
// Events
// -------------------------------------------------------------------------

typedef enum {
    SDL_FIRSTEVENT = 0,

    SDL_QUIT       = 0x100,

    SDL_WINDOWEVENT = 0x200,
    SDL_KEYDOWN     = 0x300,
    SDL_KEYUP       = 0x301,
    SDL_TEXTINPUT   = 0x303,

    SDL_MOUSEMOTION     = 0x400,
    SDL_MOUSEBUTTONDOWN = 0x401,
    SDL_MOUSEBUTTONUP   = 0x402,
    SDL_MOUSEWHEEL      = 0x403,

    SDL_CONTROLLERAXISMOTION     = 0x650,
    SDL_CONTROLLERBUTTONDOWN     = 0x651,
    SDL_CONTROLLERBUTTONUP       = 0x652,
    SDL_CONTROLLERDEVICEADDED    = 0x653,
    SDL_CONTROLLERDEVICEREMOVED  = 0x654,
    SDL_CONTROLLERSENSORUPDATE   = 0x657,

    SDL_DROPFILE     = 0x1000,
    SDL_DROPTEXT     = 0x1001,
    SDL_DROPBEGIN    = 0x1002,
    SDL_DROPCOMPLETE = 0x1003,

    SDL_USEREVENT    = 0x8000,

    SDL_LASTEVENT    = 0xFFFF
} SDL_EventType;

#define SDL_WINDOWEVENT_LEAVE         8
#define SDL_WINDOWEVENT_SIZE_CHANGED  6

#define SDL_MOUSEWHEEL_NORMAL  0
#define SDL_MOUSEWHEEL_FLIPPED 1

typedef struct {
    SDL_Scancode scancode;
    SDL_Keycode  sym;
    Uint16       mod;
    Uint32       unused;
} SDL_Keysym;

typedef struct {
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    Uint8  state;
    Uint8  repeat;
    Uint8  padding2;
    Uint8  padding3;
    SDL_Keysym keysym;
} SDL_KeyboardEvent;

typedef struct {
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    Uint8  state;
    Uint8  padding1;
    Uint8  padding2;
    Uint8  padding3;
    char   text[32];
} SDL_TextInputEvent;

typedef struct {
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    Uint32 which;
    Uint32 state;
    Sint32 x;
    Sint32 y;
    Sint32 xrel;
    Sint32 yrel;
} SDL_MouseMotionEvent;

typedef struct {
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    Uint32 which;
    Uint8  button;
    Uint8  state;
    Uint8  clicks;
    Uint8  padding1;
    Sint32 x;
    Sint32 y;
} SDL_MouseButtonEvent;

typedef struct {
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    Uint32 which;
    Sint32 x;
    Sint32 y;
    Uint32 direction;
    float  preciseX;
    float  preciseY;
    Sint32 mouseX;
    Sint32 mouseY;
} SDL_MouseWheelEvent;

typedef struct {
    Uint32 type;
    Uint32 timestamp;
    SDL_JoystickID which;
} SDL_ControllerDeviceEvent;

typedef struct {
    Uint32 type;
    Uint32 timestamp;
    SDL_JoystickID which;
    Uint8 button;
    Uint8 state;
    Uint8 padding1;
    Uint8 padding2;
} SDL_ControllerButtonEvent;

typedef struct {
    Uint32 type;
    Uint32 timestamp;
    SDL_JoystickID which;
    Uint8 axis;
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    Sint16 value;
    Uint16 padding4;
} SDL_ControllerAxisEvent;

typedef struct {
    Uint32 type;
    Uint32 timestamp;
    SDL_JoystickID which;
    Sint32 sensor;
    float  data[3];
    Uint64 timestamp_us;
} SDL_ControllerSensorEvent;

typedef struct {
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    Uint8  event;
    Uint8  padding1;
    Uint8  padding2;
    Uint8  padding3;
    Sint32 data1;
    Sint32 data2;
} SDL_WindowEvent;

typedef struct {
    Uint32 type;
    Uint32 timestamp;
    char*  file;
    Uint32 windowID;
} SDL_DropEvent;

typedef struct {
    Uint32 type;
    Uint32 timestamp;
} SDL_QuitEvent;

typedef struct {
    Uint32 type;
    Uint32 timestamp;
    Uint32 windowID;
    Sint32 code;
    void*  data1;
    void*  data2;
} SDL_UserEvent;

typedef struct {
    Uint32 type;
    Uint32 timestamp;
} SDL_CommonEvent;

typedef union {
    Uint32                    type;
    SDL_CommonEvent           common;
    SDL_KeyboardEvent         key;
    SDL_TextInputEvent        text;
    SDL_MouseMotionEvent      motion;
    SDL_MouseButtonEvent      button;
    SDL_MouseWheelEvent       wheel;
    SDL_ControllerDeviceEvent cdevice;
    SDL_ControllerButtonEvent cbutton;
    SDL_ControllerAxisEvent   caxis;
    SDL_ControllerSensorEvent csensor;
    SDL_WindowEvent           window;
    SDL_DropEvent             drop;
    SDL_QuitEvent             quit;
    SDL_UserEvent             user;
    Uint8                     padding[128];
} SDL_Event;

// RT64's application_window.h stores a function pointer of this type and
// installs a filter callback. Shim doesn't dispatch through filters today —
// the typedef is enough to compile; SDL_AddEventWatch / SDL_SetEventFilter
// are stubbed as no-ops.
typedef int (*SDL_EventFilter)(void* userdata, SDL_Event* event);

// -------------------------------------------------------------------------
// Misc constants / hints
// -------------------------------------------------------------------------

#define SDL_MESSAGEBOX_ERROR 0x10

#define SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS \
    "SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS"

// -------------------------------------------------------------------------
// Function declarations
// -------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

// Event polling
int  SDL_PollEvent(SDL_Event* event);
int  SDL_PushEvent(SDL_Event* event);

// RT64::ApplicationWindow installs a global SDL event watch to forward window
// events into its own filter. On Android our shim doesn't dispatch through
// watches yet — the symbols just need to exist so dlopen() accepts the .so.
int  SDL_AddEventWatch(SDL_EventFilter filter, void* userdata);
void SDL_DelEventWatch(SDL_EventFilter filter, void* userdata);

// Keyboard
const Uint8* SDL_GetKeyboardState(int* numkeys);
SDL_Keymod   SDL_GetModState(void);

// Game controller
SDL_GameController* SDL_GameControllerOpen(int joystick_index);
SDL_GameController* SDL_GameControllerFromInstanceID(SDL_JoystickID joyid);
SDL_Joystick*       SDL_GameControllerGetJoystick(SDL_GameController* gamecontroller);
const char*         SDL_GameControllerName(SDL_GameController* gamecontroller);
Uint8               SDL_GameControllerGetButton(SDL_GameController* gamecontroller,
                                                SDL_GameControllerButton button);
Sint16              SDL_GameControllerGetAxis(SDL_GameController* gamecontroller,
                                              SDL_GameControllerAxis axis);
SDL_bool            SDL_GameControllerHasSensor(SDL_GameController* gamecontroller,
                                                SDL_SensorType type);
int                 SDL_GameControllerSetSensorEnabled(SDL_GameController* gamecontroller,
                                                       SDL_SensorType type,
                                                       SDL_bool enabled);
int                 SDL_GameControllerRumble(SDL_GameController* gamecontroller,
                                             Uint16 low_frequency_rumble,
                                             Uint16 high_frequency_rumble,
                                             Uint32 duration_ms);

// Joystick
SDL_JoystickID   SDL_JoystickInstanceID(SDL_Joystick* joystick);
const char*      SDL_JoystickPath(SDL_Joystick* joystick);
SDL_JoystickGUID SDL_JoystickGetGUID(SDL_Joystick* joystick);
const char*      SDL_JoystickGetSerial(SDL_Joystick* joystick);
int              SDL_JoystickRumble(SDL_Joystick* joystick,
                                    Uint16 low_frequency_rumble,
                                    Uint16 high_frequency_rumble,
                                    Uint32 duration_ms);
void             SDL_GetJoystickGUIDInfo(SDL_JoystickGUID guid,
                                         Uint16* vendor,
                                         Uint16* product,
                                         Uint16* version,
                                         Uint16* crc16);

// Mouse / cursor
int   SDL_ShowCursor(int toggle);
int   SDL_SetRelativeMouseMode(SDL_bool enabled);
int   SDL_CaptureMouse(SDL_bool enabled);

// System cursor enum + create/free/set. RmlUi's SDL backend builds a small
// pool of cursors at startup even though Android has no pointer hardware;
// returning nullptr is fine because RmlUi only forwards the handle back to
// SDL_SetCursor which we also stub out.
typedef enum {
    SDL_SYSTEM_CURSOR_ARROW     = 0,
    SDL_SYSTEM_CURSOR_IBEAM     = 1,
    SDL_SYSTEM_CURSOR_WAIT      = 2,
    SDL_SYSTEM_CURSOR_CROSSHAIR = 3,
    SDL_SYSTEM_CURSOR_WAITARROW = 4,
    SDL_SYSTEM_CURSOR_SIZENWSE  = 5,
    SDL_SYSTEM_CURSOR_SIZENESW  = 6,
    SDL_SYSTEM_CURSOR_SIZEWE    = 7,
    SDL_SYSTEM_CURSOR_SIZENS    = 8,
    SDL_SYSTEM_CURSOR_SIZEALL   = 9,
    SDL_SYSTEM_CURSOR_NO        = 10,
    SDL_SYSTEM_CURSOR_HAND      = 11,
    SDL_NUM_SYSTEM_CURSORS      = 12
} SDL_SystemCursor;

SDL_Cursor* SDL_CreateSystemCursor(SDL_SystemCursor id);
void        SDL_FreeCursor(SDL_Cursor* cursor);
void        SDL_SetCursor(SDL_Cursor* cursor);

// Performance counter — RmlUi's elapsed-time helper relies on these.
Uint64 SDL_GetPerformanceCounter(void);
Uint64 SDL_GetPerformanceFrequency(void);

// Clipboard
char* SDL_GetClipboardText(void);
int   SDL_SetClipboardText(const char* text);
int   SDL_HasClipboardText(void);

// Window
void  SDL_GetWindowSizeInPixels(SDL_Window* window, int* w, int* h);

// Misc
void  SDL_free(void* mem);
int   SDL_ShowSimpleMessageBox(Uint32 flags, const char* title,
                               const char* message, SDL_Window* window);
SDL_bool SDL_SetHint(const char* name, const char* value);

#ifdef __cplusplus
}
#endif

#endif // __RECOMPINPUT_SDL_SHIM_H__
