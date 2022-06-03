// your header
#include "select_objects_move.hpp"

// components
#include "components/app.hpp"
#include "components/debug.hpp"
#include "components/pathfinding.hpp"
#include "components/selectable.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"

// engine headers
#include "engine/maths/maths.hpp"

void
game2d::update_select_objects_move_system(entt::registry& registry, engine::Application& app, const float dt)
{
  const auto& colours = registry.ctx<SINGLETON_ColoursComponent>();
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();
  auto& rnd = registry.ctx<SINGLETON_ResourceComponent>();
  glm::ivec2 imgui_mouse_pos = app.get_input().get_mouse_pos();
  glm::ivec2 mouse_pos = imgui_mouse_pos - ri.viewport_pos;

  //
  // Calculate average position of selected objects
  //

  int count = 0;
  int avg_pos_x = 0;
  int avg_pos_y = 0;
  {
    const auto& selectable = registry.view<const SelectableComponent, const PositionIntComponent>();
    selectable.each([&avg_pos_x, &avg_pos_y, &count](const auto& s, const auto& pos) {
      if (!s.is_selected)
        return;
      count += 1;
      avg_pos_x += pos.x;
      avg_pos_y += pos.y;
    });
    if (count != 0) {
      avg_pos_x = avg_pos_x / count;
      avg_pos_y = avg_pos_y / count;
    }

    // const auto& debug_square = registry.view<DebugSquareComponent, PositionIntComponent>();
    // debug_square.each([&avg_pos_x, &avg_pos_y](const auto entity, const auto& debug, auto& pos) {
    //   pos.x = avg_pos_x;
    //   pos.y = avg_pos_y;
    // });
  }

  //
  // Process Input...
  // Move all the selected units, and try and keep them in formation
  //

  if (app.get_input().get_mouse_rmb_press()) {
    const auto& selectable =
      registry.view<const SelectableComponent, const PositionIntComponent, VelocityComponent, DestinationComponent>();
    selectable.each([&registry, &colours, &avg_pos_x, &avg_pos_y, &mouse_pos](
                      const auto entity, const auto& s, const auto& pos, auto& vel, auto& destinationC) {
      if (!s.is_selected)
        return;

      //
      // calculate current offset from avg position
      glm::ivec2 offset = { pos.x - avg_pos_x, pos.y - avg_pos_y };
      glm::ivec2 destination = mouse_pos + offset;

      // set velocity to get to destination
      glm::vec2 dir = destination - glm::ivec2(pos.x, pos.y);
      glm::vec2 n = glm::normalize(dir);
      const int speed = 200;
      vel.x = n.x * speed;
      vel.y = n.y * speed;

      // set destination
      destinationC.xy = destination;

      // convert dir to angle
      const float radius = glm::length(dir);
      const float angle = atan2(dir.y, dir.x) + engine::PI;
      const float x = pos.x + (n.x * radius / 2.0f);
      const float y = pos.y + (n.y * radius / 2.0f);

      // delete destination line if it already existed
      if (destinationC.destination_line != entt::null) {
        registry.destroy(destinationC.destination_line);
        destinationC.destination_line = entt::null;
      }

      // create line from current position to end position
      entt::entity e = registry.create();
      registry.emplace<TagComponent>(e, "SPAWNED LINE");
      registry.emplace<PositionIntComponent>(e, x, y);
      registry.emplace<RenderSizeComponent>(e, radius, 1);
      registry.emplace<RenderAngleComponent>(e, angle);
      registry.emplace<ColourComponent>(e, colours.red);
      registry.emplace<SpriteTagComponent>(e, "EMPTY");
      registry.emplace<TextureComponent>(e, tex_unit_custom_spaceships);

      destinationC.destination_line = e;
    });
  }
};
