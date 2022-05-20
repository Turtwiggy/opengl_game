// your header
#include "select_objects.hpp"

// components
#include "components/cursor.hpp"
#include "components/grid.hpp"
#include "components/selectable.hpp"
#include "modules/physics/components.hpp"
#include "modules/physics/helpers.hpp"
#include "modules/renderer/components.hpp"

// other lib headers
#include <glm/glm.hpp>
#include <imgui.h>

// std lib headers
#include <iostream>
#include <utility>
#include <vector>

void
game2d::update_select_objects_system(entt::registry& registry, engine::Application& app)
{
  // .. get the list of all entities in the game with a physics bounding box
  std::vector<PhysicsObject> selectable;

  {
    const auto& view = registry.view<PositionIntComponent, PhysicsSizeComponent, SelectableComponent>();
    view.each([&selectable](auto entity, const auto& pos, const auto& size, auto& s) {
      s.is_selected = false;

      PhysicsObject po;
      po.ent_id = static_cast<uint32_t>(entity);
      po.x_tl = static_cast<int>(pos.x - size.w / 2.0f);
      po.y_tl = static_cast<int>(pos.y - size.h / 2.0f);
      po.w = size.w;
      po.h = size.h;

      selectable.push_back(po);
    });
  }

  {
    const auto& view = registry.view<CursorComponent>();
    view.each([&registry, &selectable](auto entity, auto& c) {
      //... for each cursor...

      const glm::ivec2 zero = glm::ivec2(0);
      if (c.mouse_click == zero || c.mouse_held == zero) {
        // user is not holding...
        return;
      }

      PhysicsObject cursor;
      cursor.ent_id = static_cast<uint32_t>(entity);
      cursor.x_tl = glm::min(c.mouse_held.x, c.mouse_click.x);
      cursor.y_tl = glm::min(c.mouse_held.y, c.mouse_click.y);
      cursor.w = glm::abs(c.mouse_wh.x);
      cursor.h = glm::abs(c.mouse_wh.y);

      std::vector<entt::entity> selected;

      // generate collisions
      // could be better in the physics system... somehow
      for (const auto s : selectable)
        if (collide(s, cursor))
          selected.push_back(static_cast<entt::entity>(s.ent_id));

      // dont need second for loop... but eh
      // if need the performance, remove it
      for (const auto entity : selected) {
        auto& s = registry.get<SelectableComponent>(entity);
        s.is_selected = true;
      }
    });
  }
};
