// your header
#include "systems/player_input.hpp"

// components
#include "game_components.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"

// helpers
#include "helpers/physics_layers.hpp"

// other engine headers
#include "engine/grid.hpp"

// other lib headers
#include <glm/glm.hpp>
#include <imgui.h>

#include <algorithm>
#include <stdio.h>

void
game2d::update_player_input_system(entt::registry& registry, engine::Application& app, float dt)
{
  const int& GRID_SIZE = registry.ctx<SINGLETON_GridSizeComponent>().size_xy;
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();

  {
    const auto& view = registry.view<PlayerComponent, PlayerInputComponent>();
    view.each([&app](const auto& player, auto& input) {
      //
      input.move_up = false;
      input.move_down = false;
      input.move_left = false;
      input.move_right = false;
      input.jump = false;

      auto& input_system = app.get_input();

      // convert keyboard inputs to movement actions for player

      if (input.use_keyboard) {
        input.move_up = input_system.get_key_held(input.kb_w);
        input.move_down = input_system.get_key_held(input.kb_s);
        input.move_left = input_system.get_key_held(input.kb_a);
        input.move_right = input_system.get_key_held(input.kb_d);
        input.jump = input_system.get_key_down(input.kb_space);
      }

      // convert controller inputs to movement actions for player

      if (input.use_controller) {

        int num_controllers = input_system.controllers.size();
        if (num_controllers == 0)
          return;
        auto controller = input_system.controllers[0]; // hmm, only supports one controller atm
        // auto controller = app.get_input().controllers[input.controller_index]

        float c_left_stick_x = input_system.get_axis_dir(controller, input.c_left_stick_x);
        float c_left_stick_y = input_system.get_axis_dir(controller, input.c_left_stick_y);
        input.move_up = c_left_stick_y > 0.0f;
        input.move_down = c_left_stick_y < 0.0f;
        input.move_left = c_left_stick_x < 0.0f;
        input.move_right = c_left_stick_x > 0.0f;
        input.jump = input_system.get_button_down(controller, input.c_a);
      }
    });
  }

  glm::ivec2 imgui_mouse_pos = app.get_input().get_mouse_pos();
  glm::vec2 imgui_viewport_tl = ri.viewport_pos;
  glm::ivec2 mouse_pos = imgui_mouse_pos - glm::ivec2(imgui_viewport_tl.x, imgui_viewport_tl.y);
  glm::vec2 mouse_pos_adjusted_in_worldspace = mouse_pos;
  mouse_pos_adjusted_in_worldspace.x += GRID_SIZE / 2.0f;
  mouse_pos_adjusted_in_worldspace.y += GRID_SIZE / 2.0f;

  {
    const auto& view = registry.view<PlayerComponent, PlayerInputComponent, PositionIntComponent, VelocityComponent>();
    view.each([&registry, &app, &mouse_pos_adjusted_in_worldspace, &GRID_SIZE, &dt](
                const auto& player, const auto& input, auto& pos, auto& vel) {
      //
      // Action: Move, Convert WASD to input
      int vx = 0;
      int vy = 0;
      if (input.move_up)
        vy = 1;
      else if (input.move_down)
        vy = -1;
      if (input.move_left)
        vx = -1;
      else if (input.move_right)
        vx = 1;

      // Move left and right (non-grid)
      int x_speed = 50;
      vel.x = vx * x_speed;

      // Move up and down (non-grid)
      // int y_speed = 50;
      // vel.y = vy * y_speed;

      // Action: Update player position with RMB
      // ImGui::Text("player grid %i %i", grid_slot.x, grid_slot.y);
      glm::ivec2 grid_slot = engine::grid::world_space_to_grid_space({ pos.x, pos.y }, GRID_SIZE);
      if (app.get_input().get_mouse_rmb_down()) {
        glm::ivec2 grid_slot = engine::grid::world_space_to_grid_space(mouse_pos_adjusted_in_worldspace, GRID_SIZE);
        glm::ivec2 world_space = grid_slot * GRID_SIZE;
        pos.x = world_space.x;
        pos.y = world_space.y;
      }
    });
  };
}

//
// Action: spawn object with arrow keys
//
// const float bullet_speed = 10.0f;

// int arrow_vx = 0, arrow_vy = 0;
// if (app.get_input().get_key_down(SDL_SCANCODE_RIGHT))
//   arrow_vx = 1;
// else if (app.get_input().get_key_down(SDL_SCANCODE_LEFT))
//   arrow_vx = -1;
// if (app.get_input().get_key_down(SDL_SCANCODE_UP))
//   arrow_vy = -1;
// else if (app.get_input().get_key_down(SDL_SCANCODE_DOWN))
//   arrow_vy = 1;

// if (arrow_vx != 0 || arrow_vy != 0) {
//   glm::ivec2 player_pos = { pos.x, pos.y };
//   glm::vec2 vel = glm::vec2(arrow_vx, arrow_vy);
//   if (!(vel.x == 0.0f && vel.y == 0.0f))
//     vel = glm::normalize(vel);
//   vel *= bullet_speed;
//   // Note: this shouldn't be here
//   // but im not sure where it should be yet
//   { // spawn projectile
//     entt::entity r = registry.create();
//     registry.emplace<TagComponent>(r, "Arrow");
//     // rendering
//     registry.emplace<ColourComponent>(r, 1.0f, 1.0f, 1.0f, 1.0f);
//     registry.emplace<PositionIntComponent>(r, pos.x, pos.y);
//     registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
//     registry.emplace<SpriteComponent>(r, sprite::type::WEAPON_ARROW_1);
//     // physics
//     registry.emplace<CollidableComponent>(r, static_cast<uint32_t>(GameCollisionLayer::ACTOR_PROJECTILE));
//     registry.emplace<PhysicsSizeComponent>(r, GRID_SIZE, GRID_SIZE);
//     registry.emplace<VelocityComponent>(r, vel.x, vel.y);
//     // gameplay
//     registry.emplace<DestroyAfterTimeComponent>(r, 5.0f);
//   }