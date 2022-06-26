// header
#include "modules/input/helpers/keyboard.hpp"

namespace game2d {

// InputManager()
// {
//   state = SDL_GetKeyboardState(NULL);
// }

// void
// new_frame(const uint64_t& frame)
// {
//   mousewheel_y = 0.0f;

//   keys_pressed.clear();
//   keys_released.clear();

//   this->frame = frame;
// }

// //
// // keyboard
// //

// void
// process_key_down(const SDL_Scancode button, const Uint8 is_repeat)
// {
//   if (is_repeat == 1)
//     return; // dont process held
//   keys_pressed.push_back(button);
// }

// void
// process_key_up(const SDL_Scancode button, const Uint8 is_repeat)
// {
//   if (is_repeat == 1)
//     return; // dont process held
//   keys_released.push_back(button);
// }

// bool
// get_key_down(const SDL_Scancode button)
// {
//   const auto& button_pressed = std::find(keys_pressed.begin(), keys_pressed.end(), button);
//   return button_pressed != keys_pressed.end();
// }

// bool
// get_key_up(const SDL_Scancode button)
// {
//   const auto& button_pressed = std::find(keys_released.begin(), keys_released.end(), button);
//   return button_pressed != keys_released.end();
// }

// bool
// get_key_held(SDL_Scancode button) const
// {
//   return state[button];
// }

} // namespace game2d