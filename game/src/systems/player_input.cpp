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
game2d::update_player_input_system(entt::registry& registry, engine::Application& app)
{
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();
  const int& GRID_SIZE = registry.ctx<SINGLETON_GridSizeComponent>().size_xy;

  glm::ivec2 imgui_mouse_pos = app.get_input().get_mouse_pos();
  glm::vec2 imgui_viewport_tl = ri.viewport_pos;
  glm::ivec2 mouse_pos = imgui_mouse_pos - glm::ivec2(imgui_viewport_tl.x, imgui_viewport_tl.y);
  glm::vec2 mouse_pos_adjusted_in_worldspace = mouse_pos;
  mouse_pos_adjusted_in_worldspace.x += GRID_SIZE / 2.0f;
  mouse_pos_adjusted_in_worldspace.y += GRID_SIZE / 2.0f;

  {
    const auto& view = registry.view<Player, PositionIntComponent, VelocityComponent>();
    view.each(
      [&registry, &app, &mouse_pos_adjusted_in_worldspace, &GRID_SIZE](const auto& player, auto& pos, auto& vel) {
        //
        // Action: Move, Convert WASD to input
        int vx = 0, vy = 0;
        if (app.get_input().get_key_held(SDL_SCANCODE_S))
          vy = 1;
        else if (app.get_input().get_key_held(SDL_SCANCODE_W))
          vy = -1;
        if (app.get_input().get_key_held(SDL_SCANCODE_A))
          vx = -1;
        else if (app.get_input().get_key_held(SDL_SCANCODE_D))
          vx = 1;

        // Move left and right (non-grid)
        int x_speed = 50;
        vel.x = vx * x_speed;
        // Move up and down (non-grid)
        int y_speed = 50;
        vel.y = vy * y_speed;

        // apply gravity
        // float gravity = 1.0f;
        // vel.y += gravity;

        // Action: Update player position with RMB
        // ImGui::Text("player grid %i %i", grid_slot.x, grid_slot.y);
        glm::ivec2 grid_slot = engine::grid::world_space_to_grid_space({ pos.x, pos.y }, GRID_SIZE);
        if (app.get_input().get_mouse_rmb_down()) {
          glm::ivec2 grid_slot = engine::grid::world_space_to_grid_space(mouse_pos_adjusted_in_worldspace, GRID_SIZE);
          glm::ivec2 world_space = grid_slot * GRID_SIZE;
          pos.x = world_space.x;
          pos.y = world_space.y;
        }

        //
        // Action: spawn object with arrow keys
        //
        const float bullet_speed = 10.0f;

        int arrow_vx = 0, arrow_vy = 0;
        if (app.get_input().get_key_down(SDL_SCANCODE_RIGHT))
          arrow_vx = 1;
        else if (app.get_input().get_key_down(SDL_SCANCODE_LEFT))
          arrow_vx = -1;
        if (app.get_input().get_key_down(SDL_SCANCODE_UP))
          arrow_vy = -1;
        else if (app.get_input().get_key_down(SDL_SCANCODE_DOWN))
          arrow_vy = 1;

        if (arrow_vx != 0 || arrow_vy != 0) {

          glm::ivec2 player_pos = { pos.x, pos.y };

          glm::vec2 vel = glm::vec2(arrow_vx, arrow_vy);
          if (!(vel.x == 0.0f && vel.y == 0.0f))
            vel = glm::normalize(vel);
          vel *= bullet_speed;

          // Note: this shouldn't be here
          // but im not sure where it should be yet

          { // spawn projectile
            entt::entity r = registry.create();
            registry.emplace<TagComponent>(r, "Arrow");
            // rendering
            registry.emplace<ColourComponent>(r, 1.0f, 1.0f, 1.0f, 1.0f);
            registry.emplace<PositionIntComponent>(r, pos.x, pos.y);
            registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
            registry.emplace<SpriteComponent>(r, sprite::type::WEAPON_ARROW_1);
            // physics
            registry.emplace<CollidableComponent>(r, static_cast<uint32_t>(GameCollisionLayer::ACTOR_PROJECTILE));
            registry.emplace<PhysicsSizeComponent>(r, GRID_SIZE, GRID_SIZE);
            registry.emplace<VelocityComponent>(r, vel.x, vel.y);
            // gameplay
            registry.emplace<DestroyAfterTimeComponent>(r, 5.0f);
          }
        }

        // end
      });
  }
};

// // temp
// const auto& controllers = app.get_input().controllers;
// if (controllers.size() > 0) {
//   SDL_GameController* controller_0 = controllers[0];

//   bool start_button =
//     app.get_input().get_button_down(controller_0, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START);
//   bool a_pressed = app.get_input().get_button_down(controller_0, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A);
//   bool b_pressed = app.get_input().get_button_down(controller_0, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B);
//   bool x_pressed = app.get_input().get_button_down(controller_0, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X);
//   bool y_pressed = app.get_input().get_button_down(controller_0, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y);

//   if (start_button)
//     printf("start pressed... \n");
//   if (a_pressed)
//     printf("a pressed... \n");
//   if (b_pressed)
//     printf("b pressed... \n");
//   if (x_pressed)
//     printf("x pressed... \n");
//   if (y_pressed)
//     printf("y pressed... \n");

//   bool dpad_up =
//     app.get_input().get_button_down(controller_0, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP);
//   bool dpad_down =
//     app.get_input().get_button_down(controller_0, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN);
//   bool dpad_left =
//     app.get_input().get_button_down(controller_0, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT);
//   bool dpad_right =
//     app.get_input().get_button_down(controller_0, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
//   if (dpad_up)
//     printf("dpad_up pressed... \n");
//   if (dpad_down)
//     printf("dpad_down pressed... \n");
//   if (dpad_left)
//     printf("dpad_left pressed... \n");
//   if (dpad_right)
//     printf("dpad_right pressed... \n");
// }

// {
//   const auto& view = registry.view<Player, VelocityComponent, DoubleJumpComponent>();
//   view.each([&app](const auto& player, auto& vel, auto& dd) {
//     if (dd.able_to_jump) {
//       // Jump simple
//       // const auto UP = glm::vec2(0.0f, -1.0f);
//       // const auto JUMP_VEL = 150.0f;
//       // bool jump_pressed = app.get_input().get_key_down(SDL_SCANCODE_SPACE);
//       // if (jump_pressed) {
//       //   dd.able_to_jump = false;
//       //   vel.y = (UP * JUMP_VEL).y;
//       // }
//     }
//   });
// }