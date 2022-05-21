// your header
#include "pathfinding.hpp"

// components
#include "components/pathfinding.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"

// other lib headers
#include <glm/glm.hpp>

void
game2d::update_pathfinding_system(entt::registry& registry, engine::Application& app)
{
  const auto& view = registry.view<PositionIntComponent, DestinationComponent, VelocityComponent>();
  view.each([&app](const auto& pos, const auto& destination, auto& vel) {
    //
    // Stop object if at or past destination
    //
    bool at_x = destination.xy.x == pos.x;
    bool at_y = destination.xy.y == pos.y;
    bool past_x = glm::sign(vel.x) > 0 ? pos.x > destination.xy.x : pos.x < destination.xy.x;
    bool past_y = glm::sign(vel.y) > 0 ? pos.y > destination.xy.y : pos.y < destination.xy.y;
    if (at_x || past_x)
      vel.x = 0.0f;
    if (at_y || past_y)
      vel.y = 0.0f;
  });
};
