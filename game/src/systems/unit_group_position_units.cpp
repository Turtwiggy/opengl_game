#include "systems/unit_group_position_units.hpp"

#include "components/pathfinding.hpp"
#include "components/units.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"

#include "engine/maths/maths.hpp"

#include "imgui.h" // temp

namespace game2d {

void
update_unit_group_position_units_system(entt::registry& registry, engine::Application& app, float dt)
{
  const auto& groups =
    registry
      .view<const UnitGroupComponent, const PositionIntComponent, const PhysicsSizeComponent, RenderAngleComponent>();
  groups.each([&registry, &dt](auto entity, const auto& group, const auto& pos, const auto& physics_size, auto& angle) {
    int group_units = group.units.size();
    if (group_units == 0)
      return;
    float& group_angle = angle.angle_radians;

    // TEMP: slowly rotate
    // group_angle += engine::PI / 8.0f * dt;

    int half_units = group_units / 2;
    int unit_index = -half_units; // go from -half_units to half_units

    int larger_side = physics_size.w > physics_size.h ? physics_size.w : physics_size.h;
    int unit_offset = (larger_side / 2) / half_units;

    for (const auto& unit : group.units) {
      auto& unitpos = registry.get<PositionIntComponent>(unit);
      auto& unitpath = registry.get<DestinationComponent>(unit);
      auto& unitvel = registry.get<VelocityComponent>(unit);

      glm::vec2 unitdir = engine::angle_radians_to_direction(group_angle);

      int destination_x = pos.x - unitpos.x + (unitdir.x * unit_index * unit_offset);
      int destination_y = pos.y - unitpos.y + (unitdir.y * unit_index * unit_offset);
      unit_index += 1;

      glm::ivec2 destination{ destination_x, destination_y };

      // check same spot not clicked
      glm::ivec2 d = { destination.x, destination.y };
      if (d.x == 0 && d.y == 0)
        continue;

      // dont go back and forth on a pixel
      if (glm::abs(d.x) <= 1 && glm::abs(d.y) <= 1)
        continue;

      // set velocity to get to destination
      glm::vec2 dir = glm::vec2(d.x, d.y);
      glm::vec2 n = glm::normalize(dir);
      const float speed = 100.0f;
      unitvel.x = n.x * speed;
      unitvel.y = n.y * speed;

      // set destination
      unitpath.xy = destination;
    }
  });
};

} // namespace game2d