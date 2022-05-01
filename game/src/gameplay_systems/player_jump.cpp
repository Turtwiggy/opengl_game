// your header
#include "player_jump.hpp"

// components
#include "gameplay_components/components.hpp"
#include "gameplay_components/grid.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"

// helpers
#include "gameplay_helpers/physics_layers.hpp"
#include "grid.hpp"

// other lib headers
#include <glm/glm.hpp>
#include <imgui.h>

void
game2d::update_player_jump_system(entt::registry& registry, engine::Application& app, float dt)
{
  const int& GRID_SIZE = registry.ctx<SINGLETON_GridSizeComponent>().size_xy;
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();

  //
  // ... process jump

  const auto UP = glm::vec2(0.0f, -1.0f);
  const auto JUMP_VEL = 100.0f;
  {
    // const auto& view = registry.view<PlayerComponent, PlayerInputComponent, VelocityComponent,
    // DoubleJumpComponent>(); view.each([&app, &UP, &JUMP_VEL](const auto& player, const auto& input, auto& vel, auto&
    // dd) {
    //   if (input.jump && dd.able_to_jump) {
    //     dd.able_to_jump = false;
    //     vel.y = (UP * JUMP_VEL).y;
    //   }
    // });
  }

  //
  // ... give visual feedback

  {
    const auto& view = registry.view<PlayerComponent, DoubleJumpComponent, ColourComponent, FlashColourComponent>();
    view.each([&app, &UP, &JUMP_VEL](const auto& player, const auto& dd, auto& colour, const auto& flash_colours) {
      if (dd.able_to_jump) {
        colour.colour = flash_colours.flash_colour;
      } else {
        colour.colour = flash_colours.start_colour;
      }
    });
  }
};
