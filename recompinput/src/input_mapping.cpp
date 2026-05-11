#include <unordered_map>
#include "recompinput.h"

namespace recompinput {

    using default_game_input_device_mapping = std::unordered_map<GameInput, std::vector<InputField>>;

    default_game_input_device_mapping default_n64_mappings_keyboard = {
        { GameInput::A,              { InputField::keyboard(Scancode::Space) } },
        { GameInput::B,              { InputField::keyboard(Scancode::LShift) } },
        { GameInput::L,              { InputField::keyboard(Scancode::E) } },
        { GameInput::B,              { InputField::keyboard(Scancode::LShift) } },
        { GameInput::R,              { InputField::keyboard(Scancode::R) } },
        { GameInput::Z,              { InputField::keyboard(Scancode::Q) } },
        { GameInput::START,          { InputField::keyboard(Scancode::Return) } },
        { GameInput::C_LEFT,         { InputField::keyboard(Scancode::Left) } },
        { GameInput::C_RIGHT,        { InputField::keyboard(Scancode::Right) } },
        { GameInput::C_UP,           { InputField::keyboard(Scancode::Up) } },
        { GameInput::C_DOWN,         { InputField::keyboard(Scancode::Down) } },
        { GameInput::DPAD_LEFT,      { InputField::keyboard(Scancode::J) } },
        { GameInput::DPAD_RIGHT,     { InputField::keyboard(Scancode::L) } },
        { GameInput::DPAD_UP,        { InputField::keyboard(Scancode::I) } },
        { GameInput::DPAD_DOWN,      { InputField::keyboard(Scancode::K) } },
        { GameInput::X_AXIS_NEG,     { InputField::keyboard(Scancode::A) } },
        { GameInput::X_AXIS_POS,     { InputField::keyboard(Scancode::D) } },
        { GameInput::Y_AXIS_POS,     { InputField::keyboard(Scancode::W) } },
        { GameInput::Y_AXIS_NEG,     { InputField::keyboard(Scancode::S) } },

        { GameInput::TOGGLE_MENU,    { InputField::keyboard(Scancode::Escape) } },
        { GameInput::ACCEPT_MENU,    { InputField::keyboard(Scancode::Return) } },
        { GameInput::BACK_MENU,      { InputField::keyboard(Scancode::F15) } },
        { GameInput::APPLY_MENU,     { InputField::keyboard(Scancode::F) } },
        { GameInput::TAB_LEFT_MENU,  { InputField::keyboard(Scancode::F16) } },
        { GameInput::TAB_RIGHT_MENU, { InputField::keyboard(Scancode::F17) } }
    };

    default_game_input_device_mapping default_n64_mappings_controller = {
        { GameInput::A,              { InputField::controller_digital(GamepadButton::South) } },
        { GameInput::B,              { InputField::controller_digital(GamepadButton::West) } },
        { GameInput::L,              { InputField::controller_digital(GamepadButton::LeftShoulder) } },
        { GameInput::R,              { InputField::controller_analog( GamepadAxis::TriggerRight, true) } },
        { GameInput::Z,              { InputField::controller_analog( GamepadAxis::TriggerLeft, true) } },
        { GameInput::START,          { InputField::controller_digital(GamepadButton::Start) } },
        { GameInput::C_LEFT,         { InputField::controller_analog( GamepadAxis::RightX, false), InputField::controller_digital(GamepadButton::North) } },
        { GameInput::C_RIGHT,        { InputField::controller_analog( GamepadAxis::RightX, true),  InputField::controller_digital(GamepadButton::East) } },
        { GameInput::C_UP,           { InputField::controller_analog( GamepadAxis::RightY, false), InputField::controller_digital(GamepadButton::RightStick) } },
        { GameInput::C_DOWN,         { InputField::controller_analog( GamepadAxis::RightY, true),  InputField::controller_digital(GamepadButton::RightShoulder) } },
        { GameInput::DPAD_LEFT,      { InputField::controller_digital(GamepadButton::DpadLeft) } },
        { GameInput::DPAD_RIGHT,     { InputField::controller_digital(GamepadButton::DpadRight) } },
        { GameInput::DPAD_UP,        { InputField::controller_digital(GamepadButton::DpadUp) } },
        { GameInput::DPAD_DOWN,      { InputField::controller_digital(GamepadButton::DpadDown) } },
        { GameInput::X_AXIS_NEG,     { InputField::controller_analog( GamepadAxis::LeftX, false) } },
        { GameInput::X_AXIS_POS,     { InputField::controller_analog( GamepadAxis::LeftX, true) } },
        { GameInput::Y_AXIS_POS,     { InputField::controller_analog( GamepadAxis::LeftY, false) } },
        { GameInput::Y_AXIS_NEG,     { InputField::controller_analog( GamepadAxis::LeftY, true) } },

        { GameInput::TOGGLE_MENU,    { InputField::controller_digital(GamepadButton::Back) } },
        { GameInput::ACCEPT_MENU,    { InputField::controller_digital(GamepadButton::South) } },
        { GameInput::BACK_MENU,      { InputField::controller_digital(GamepadButton::West) } },
        { GameInput::APPLY_MENU,     { InputField::controller_digital(GamepadButton::North), InputField::controller_digital(GamepadButton::Start) } },
        { GameInput::TAB_LEFT_MENU,  { InputField::controller_digital(GamepadButton::LeftShoulder) } },
        { GameInput::TAB_RIGHT_MENU, { InputField::controller_digital(GamepadButton::RightShoulder) } },
    };

    // Flag to prevent the base game from changing default mappings after they have been used.
    static bool default_mappings_used = false;

    const std::vector<InputField>& get_default_mapping_for_input(recompinput::InputDevice device, const GameInput input) {
        static const std::vector<InputField> empty_input_field{};
        default_mappings_used = true;
        default_game_input_device_mapping *mapping = nullptr;

        switch (device) {
            case InputDevice::Keyboard: mapping = &default_n64_mappings_keyboard; break;
            case InputDevice::Controller: mapping = &default_n64_mappings_controller; break;
            default: return empty_input_field;
        }

        auto it = mapping->find(input);
        if (it != mapping->end()) {
            return it->second;
        }
        return empty_input_field;
    }

    void set_default_mapping_for_controller(GameInput input, const std::vector<InputField>& fields) {
        if (default_mappings_used) {
            throw std::runtime_error("Cannot set default mapping after it has been used.");
        }
        default_n64_mappings_controller[input] = fields;
    }

    void set_default_mapping_for_keyboard(GameInput input, const std::vector<InputField>& fields) {
        if (default_mappings_used) {
            throw std::runtime_error("Cannot set default mapping after it has been used.");
        }
        default_n64_mappings_keyboard[input] = fields;
    }
}
