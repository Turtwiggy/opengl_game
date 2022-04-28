// your header
#include "player_input.hpp"

// components
#include "gameplay_components/components.hpp"
#include "gameplay_components/grid.hpp"
#include "gameplay_components/input.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"

// other lib headers
#include <glm/glm.hpp>
#include <imgui.h>

namespace game2d {

struct GameMovementComponent
{
  bool move_up = false;
  bool move_down = false;
  bool move_left = false;
  bool move_right = false;
  bool jumping = false;
};

} // namespace game2d

void
game2d::update_player_input_system(entt::registry& registry, engine::Application& app, float dt)
{
  const int& GRID_SIZE = registry.ctx<SINGLETON_GridSizeComponent>().size_xy;
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();

  GameMovementComponent input;

  {
    const auto& view = registry.view<PlayerComponent, PlayerKeyboardComponent>();
    view.each([&app, &input](const auto& player, auto& input) {
      //

      ImGui::Begin("Player Input");
      {
        ImGui::Text("honk");
      }
      ImGui::End();

      // input.move_up = false;
      // input.move_down = false;
      // input.move_left = false;
      // input.move_right = false;
      // input.jump = false;

      //   auto& input_system = app.get_input();

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
    });
  }

  {
    const auto& view = registry.view<PlayerComponent, PlayerControllerComponent>();
    view.each([&app](const auto& player, auto& input) {
      //
    });
  }
};