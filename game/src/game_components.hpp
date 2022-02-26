#pragma once

// other libs
#include <glm/glm.hpp>

// engine headers
#include "engine/input.hpp"
#include "engine/maths.hpp"

// std lib headers
#include <memory>

namespace game2d {

struct ClickToDestroyComponent
{
  bool placeholder = true;
};

struct CursorComponent
{
  int cursor_ltrd = 0; // 0=l, 1=t, 2=r, 3=d
};

struct DestroyAfterTimeComponent
{
  float time = 0.0f;
};

struct DoubleJumpComponent
{
  bool able_to_jump = true;
};

struct FlashColourComponent
{
  glm::vec4 start_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
  glm::vec4 flash_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
};

struct GridPositionComponent
{
  int x = 0;
  int y = 0;
};

struct HealthComponent
{
  float hp = 5.0f;
};

struct ParryComponent
{
  bool placeholder = false;
};

struct PlayerComponent
{
  int player = 0;
};

struct PlayerInputComponent
{
  bool use_keyboard = false;
  bool use_controller = false;

  // keyboard bindings
  SDL_Scancode kb_start = SDL_SCANCODE_RETURN;
  SDL_Scancode kb_w = SDL_SCANCODE_W;
  SDL_Scancode kb_a = SDL_SCANCODE_A;
  SDL_Scancode kb_s = SDL_SCANCODE_S;
  SDL_Scancode kb_d = SDL_SCANCODE_D;
  SDL_Scancode kb_space = SDL_SCANCODE_SPACE;

  // controller bindings
  SDL_GameControllerAxis c_left_stick_x = SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX;
  SDL_GameControllerAxis c_left_stick_y = SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY;
  SDL_GameControllerAxis c_right_stick_x = SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX;
  SDL_GameControllerAxis c_right_stick_y = SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY;
  SDL_GameControllerAxis c_left_trigger = SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERLEFT;
  SDL_GameControllerAxis c_right_trigger = SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT;
  SDL_GameControllerButton c_start = SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START;
  SDL_GameControllerButton c_a = SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A;
  SDL_GameControllerButton c_b = SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B;
  SDL_GameControllerButton c_x = SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X;
  SDL_GameControllerButton c_y = SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y;
  SDL_GameControllerButton c_dpad_up = SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP;
  SDL_GameControllerButton c_dpad_down = SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN;
  SDL_GameControllerButton c_dpad_left = SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT;
  SDL_GameControllerButton c_dpad_right = SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT;

  // game movement state
  bool move_up = false;
  bool move_down = false;
  bool move_left = false;
  bool move_right = false;
  bool jump = false;
};

//
// singletons
//

struct SINGLETON_ColoursComponent
{
  const glm::vec4 red = glm::vec4(232 / 255.0f, 80 / 255.0f, 100 / 255.0f, 1.0f);
  const glm::vec4 cyan = glm::vec4(8 / 255.0f, 177 / 255.0f, 190 / 255.0f, 1.0f);
  const glm::vec4 dblue = glm::vec4(49 / 255.0f, 99 / 255.0f, 188 / 255.0f, 1.0f);
  const glm::vec4 white = glm::vec4(1.0f);
  const glm::vec4 green = glm::vec4(100 / 255.0f, 188 / 255.0f, 49 / 255.0f, 1.0f);
};

struct SINGLETON_GamePausedComponent
{
  bool paused = false;
};

struct SINGLETON_GridSizeComponent
{
  const int size_xy = 16;
};

struct SINGLETON_ResourceComponent
{
  engine::RandomState rnd;
};

} // namespace game2d