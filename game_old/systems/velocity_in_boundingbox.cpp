// your header
#include "systems/velocity_in_boundingbox.hpp"

// components
#include "game_components.hpp"
#include "physics/components.hpp"
#include "renderer/components.hpp"

// other engine headers
#include "engine/grid.hpp"

// other lib headers
#include <glm/glm.hpp>
#include <imgui.h>

void
game2d::update_velocity_in_boundingbox_system(game2d::Coordinator& registry, engine::Application& app, float dt)
{
  const auto& ri = registry.ctx<RendererInfo>();

  glm::ivec2 boundary = ri.viewport_size_render_at;

  const auto& view = registry.view<VelocityComponent,
                                   const PositionIntComponent,
                                   const PhysicsSizeComponent,
                                   const VelocityInBoundingboxComponent>();

  view.each([&boundary](auto& vel, const auto& pos, const auto& size, const auto& vib) {
    if (pos.x - (size.w / 2.0f) <= 0.0f)
      vel.x = abs(vel.x);
    if (pos.x + (size.w / 2.0f) >= boundary.x)
      vel.x = -abs(vel.x);
    if (pos.y - (size.h / 2.0f) <= 0.0f)
      vel.y = abs(vel.y);
    if (pos.y + (size.h / 2.0f) >= boundary.y)
      vel.y = -abs(vel.y);
  });
}