#include "system.hpp"

#include "modules/input/components.hpp"
#include "modules/input/mouse.hpp"

void
game2d::update_input_system(entt::registry& registry, engine::Application& app)
{
  auto& c = registry.ctx<SINGLETON_InputComponent>();
  c.mouse_position_in_worldspace = mouse_position_in_worldspace(registry, app);

  c.cursor_click = false;
  if (app.get_input().get_mouse_lmb_press()) {
    c.mouse_click = c.mouse_position_in_worldspace;
    c.cursor_click = true;
  }

  c.cursor_held = false;
  if (app.get_input().get_mouse_lmb_held()) {
    c.mouse_held = c.mouse_position_in_worldspace;
    c.cursor_held = true;
  }

  c.cursor_release = false;
  if (app.get_input().get_mouse_lmb_release()) {
    c.mouse_click = glm::ivec2(0);
    c.mouse_held = glm::ivec2(0);
    c.cursor_release = true;
  }

  const int CURSOR_SIZE = 16;

  if (c.cursor_click || c.cursor_held || c.cursor_release) {
    // draw expanding square
    int width = c.mouse_held.x - c.mouse_click.x;
    int height = c.mouse_held.y - c.mouse_click.y;
    c.mouse_wh.x = width;
    c.mouse_wh.y = height;
  } else {
    // draw regular square
    c.mouse_wh.x = CURSOR_SIZE;
    c.mouse_wh.y = CURSOR_SIZE;
    int offset = static_cast<int>(CURSOR_SIZE / 2.0f);
  }
};

//
// input.move_up = false;
// input.move_down = false;
// input.move_left = false;
// input.move_right = false;
// input.jump = false;

//   // convert keyboard inputs to movement actions for player

//   if (input.use_keyboard) {
//     input.move_up = input_system.get_key_down(input.kb_w);
//     input.move_down = input_system.get_key_down(input.kb_s);
//     input.move_left = input_system.get_key_down(input.kb_a);
//     input.move_right = input_system.get_key_down(input.kb_d);
//     input.jump = input_system.get_key_down(input.kb_space);
//   }

//   // convert controller inputs to movement actions for player

//   if (input.use_controller) {
//     int num_controllers = input_system.controllers.size();
//     if (num_controllers == 0)
//       return;
//     auto controller = input_system.controllers[0]; // LIMITATION: only supports one controller atm
//     // auto controller = app.get_input().controllers[input.controller_index]
//     float c_left_stick_x = input_system.get_axis_dir(controller, input.c_left_stick_x);
//     float c_left_stick_y = input_system.get_axis_dir(controller, input.c_left_stick_y);
//     input.move_up = c_left_stick_y > 0.0f;
//     input.move_down = c_left_stick_y < 0.0f;
//     input.move_left = c_left_stick_x < 0.0f;
//     input.move_right = c_left_stick_x > 0.0f;
//     input.jump = input_system.get_button_down(controller, input.c_a);
