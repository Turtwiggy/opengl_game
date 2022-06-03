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

void
game2d::update_select_objects_move_system(entt::registry& registry, engine::Application& app, const float dt)
{
  const auto& colours = registry.ctx<SINGLETON_ColoursComponent>();
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();
  auto& rnd = registry.ctx<SINGLETON_ResourceComponent>();
  glm::ivec2 imgui_mouse_pos = app.get_input().get_mouse_pos();
  glm::vec2 imgui_viewport_tl = ri.viewport_pos;
  glm::ivec2 mouse_pos = imgui_mouse_pos - glm::ivec2(imgui_viewport_tl.x, imgui_viewport_tl.y);

  const auto& view =
    registry.view<const SelectableComponent, const PositionIntComponent, VelocityComponent, DestinationComponent>();
  view.each(
    [&registry, &app, &colours, &mouse_pos, &rnd](const auto& s, const auto& pos, auto& vel, auto& destinationC) {
      if (!s.is_selected)
        return;

      if (app.get_input().get_mouse_rmb_press()) {

        // Generate a random destination around mouse_pos
        // const auto random_dir_xyz = engine::rand_unit_vector(rnd.rnd);
        // int destination_x = mouse_pos.x + static_cast<int>(random_dir_xyz.x * 32.0f);
        // int destination_y = mouse_pos.y + static_cast<int>(random_dir_xyz.y * 32.0f);
        // glm::ivec2 destination = { destination_x, destination_y };
        glm::ivec2 destination = { mouse_pos.x, mouse_pos.y };

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

        // delete line if it already existed
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
      }
    });
};
