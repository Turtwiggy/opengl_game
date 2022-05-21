// your header
#include "select_objects_move.hpp"

// components
#include "components/app.hpp"
#include "components/pathfinding.hpp"
#include "components/selectable.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"

// engine headers
#include "engine/maths/maths.hpp"

#include <iostream>

void
game2d::update_select_objects_move_system(entt::registry& registry, engine::Application& app, const float dt)
{
  const auto& colours = registry.ctx<SINGLETON_ColoursComponent>();
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();
  glm::ivec2 imgui_mouse_pos = app.get_input().get_mouse_pos();
  glm::vec2 imgui_viewport_tl = ri.viewport_pos;
  glm::ivec2 mouse_pos = imgui_mouse_pos - glm::ivec2(imgui_viewport_tl.x, imgui_viewport_tl.y);

  const auto& view =
    registry.view<SelectableComponent, PositionIntComponent, VelocityComponent, DestinationComponent>();
  view.each([&registry, &app, &colours, &dt, &mouse_pos](const auto& s, const auto& pos, auto& vel, auto& destination) {
    if (!s.is_selected) {
      return; // only interested in selected objects
    }

    if (app.get_input().get_mouse_rmb_press()) {

      // set velocity to get to destination
      glm::vec2 dir = mouse_pos - glm::ivec2(pos.x, pos.y);
      glm::vec2 n = glm::normalize(dir);
      const int speed = 200;
      vel.x = n.x * speed;
      vel.y = n.y * speed;

      // set destination
      destination.xy = mouse_pos;
      std::cout << "destination x: " << destination.xy.x << " y: " << destination.xy.y;

      // convert dir to angle
      const float radius = glm::length(dir);
      const float angle = atan2(dir.y, dir.x) + engine::PI;
      const float x = pos.x + (n.x * radius / 2.0f);
      const float y = pos.y + (n.y * radius / 2.0f);

      // create line from current position to end position
      entt::entity e = registry.create();
      registry.emplace<TagComponent>(e, "SPAWNED LINE");
      registry.emplace<PositionIntComponent>(e, x, y);
      registry.emplace<RenderSizeComponent>(e, radius, 1);
      registry.emplace<RenderAngleComponent>(e, angle);
      registry.emplace<ColourComponent>(e, colours.red);
      registry.emplace<SpriteTagComponent>(e, "EMPTY");
      registry.emplace<TextureComponent>(e, tex_unit_custom_spaceships);
    }
  });
};
