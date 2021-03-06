#include "system.hpp"

#include "modules/events/components.hpp"
#include "modules/events/helpers/keyboard.hpp"
#include "modules/events/helpers/mouse.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <imgui.h>

#include <iostream>

void
game2d::init_input_system(entt::registry& registry)
{
  auto& input = registry.set<SINGLETON_InputComponent>();
  input.state = SDL_GetKeyboardState(NULL);
}

void
game2d::update_input_system(entt::registry& registry, engine::Application& app)
{
  auto& input = registry.ctx<SINGLETON_InputComponent>();

  input.mouse_position_in_worldspace = mouse_position_in_worldspace(registry);

  if (get_mouse_lmb_press())
    input.mouse_click = input.mouse_position_in_worldspace;
  if (!get_mouse_lmb_held())
    input.mouse_click = glm::ivec2(0);

  if (get_mouse_lmb_press() || get_mouse_lmb_held()) {
    // draw expanding square
    int width = input.mouse_position_in_worldspace.x - input.mouse_click.x;
    int height = input.mouse_position_in_worldspace.y - input.mouse_click.y;
    input.mouse_drag_boundingbox.x = width;
    input.mouse_drag_boundingbox.y = height;
  } else {
    // draw regular square
    input.mouse_drag_boundingbox.x = input.CURSOR_SIZE;
    input.mouse_drag_boundingbox.y = input.CURSOR_SIZE;
  }

  input.keys_pressed.clear();
  input.keys_released.clear();
  input.sdl_events.clear();

  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    input.sdl_events.push_back(e);

    // Events to quit
    if (e.type == SDL_QUIT)
      app.shutdown();

    // https://wiki.libsdl.org/SDL_WindowEvent
    if (e.type == SDL_WINDOWEVENT) {
      switch (e.window.event) {
        case SDL_WINDOWEVENT_CLOSE:
          app.shutdown();
        case SDL_WINDOWEVENT_FOCUS_GAINED:
          SDL_Log("Window %d gained keyboard focus \n", e.window.windowID);
          break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
          SDL_Log("Window %d lost keyboard focus \n", e.window.windowID);
          break;
      }
    }

    // Pass UI the event
    // Note: should move imgui manager to a System...
    if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard) {
      app.imgui_manager.process_event(&e);
      // continue; // Imgui stole the event
    }

    // keyboard specific
    if (e.type == SDL_KEYDOWN)
      process_key_down(input, e.key.keysym.scancode, e.key.repeat);
    if (e.type == SDL_KEYUP)
      process_key_up(input, e.key.keysym.scancode, e.key.repeat);

    // controller specific
    // if (e.type == SDL_JOYHATMOTION)
    //   input_manager.process_controller_dpad(e.jhat);
    // if (e.type == SDL_JOYBUTTONDOWN)
    //   input_manager.process_controller_button_down(e.jbutton);
    // if (e.type == SDL_JOYBUTTONUP)
    //   input_manager.process_controller_button_up(e.jbutton);
    //   if (e.type == SDL_JOYDEVICEADDED)
    //     input_manager.process_controller_added();
    //   if (e.type == SDL_JOYDEVICEREMOVED)
    //     input_manager.process_controller_removed();
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