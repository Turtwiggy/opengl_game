// your header
#include "select_objects_move.hpp"

// components
#include "components/app.hpp"
#include "components/debug.hpp"
#include "components/pathfinding.hpp"
#include "components/selectable.hpp"
#include "modules/camera/components.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"

// engine headers
#include "engine/maths/maths.hpp"

void
game2d::update_select_objects_move_system(entt::registry& registry, engine::Application& app, const float dt)
{
  const auto& colours = registry.ctx<SINGLETON_ColoursComponent>();
  auto& rnd = registry.ctx<SINGLETON_ResourceComponent>();

  glm::ivec2 mouse_pos_adjusted_in_worldspace;
  {
    const auto& ri = registry.ctx<SINGLETON_RendererInfo>();
    const auto& cameras = registry.view<CameraComponent, PositionIntComponent>();
    const auto& main_camera = cameras.front();
    const auto& main_camera_position = registry.get<PositionIntComponent>(main_camera);
    const auto& camera_pos = glm::ivec2{ main_camera_position.x, main_camera_position.y };
    glm::ivec2 imgui_mouse_pos = app.get_input().get_mouse_pos();
    glm::ivec2 mouse_pos = imgui_mouse_pos - ri.viewport_pos;
    mouse_pos_adjusted_in_worldspace = { mouse_pos.x + (camera_pos.x * -1), mouse_pos.y + (camera_pos.y * -1) };
  }

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
  }

  //
  // Process Input...
  // Move all the selected units, and try and keep them in formation
  //
  entt::entity random_chosen_entity_parent = entt::null;

  if (app.get_input().get_mouse_rmb_press()) {
    const auto& selectable =
      registry.view<const SelectableComponent, const PositionIntComponent, VelocityComponent, DestinationComponent>();
    selectable.each(
      [&registry, &colours, &avg_pos_x, &avg_pos_y, &mouse_pos_adjusted_in_worldspace, &random_chosen_entity_parent](
        const auto entity, const auto& s, const auto& pos, auto& vel, auto& destinationC) {
        if (!s.is_selected)
          return;
        random_chosen_entity_parent = entity; // doesn't matter which one

        // calculate current offset from avg position
        glm::ivec2 offset = { pos.x - avg_pos_x, pos.y - avg_pos_y };
        glm::ivec2 destination = mouse_pos_adjusted_in_worldspace + offset;

        // set velocity to get to destination
        glm::ivec2 d = { destination.x - pos.x, destination.y - pos.y };
        if (d.x == 0 && d.y == 0) // check same spot not clicked
          return;
        glm::vec2 dir = glm::vec2(d.x, d.y);
        glm::vec2 n = glm::normalize(dir);
        const float speed = 200.0f;
        vel.x = n.x * speed;
        vel.y = n.y * speed;

        // set destination
        destinationC.xy = destination;
      });

    //
    // Draw a line from the chosen entity to the avg movement point
    //

    if (random_chosen_entity_parent != entt::null) {
      auto& destinationC = registry.get<DestinationComponent>(random_chosen_entity_parent);

      glm::ivec2 d = { mouse_pos_adjusted_in_worldspace.x - avg_pos_x, mouse_pos_adjusted_in_worldspace.y - avg_pos_y };
      if (d.x == 0 && d.y == 0) // check same spot not clicked
        return;
      glm::vec2 dir = glm::vec2(d.x, d.y);
      glm::vec2 n = glm::normalize(dir);

      // convert dir to angle
      const float radius = glm::length(dir);
      const float angle = atan2(dir.y, dir.x) + engine::PI;
      const float x = avg_pos_x + (n.x * radius / 2.0f);
      const float y = avg_pos_y + (n.y * radius / 2.0f);

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
    }
  }
};
