// your header
#include "cursor.hpp"

// components
#include "game_modules/components/cursor.hpp"
#include "modules/events/components.hpp"
#include "modules/events/helpers/mouse.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"

// other engine headers
#include "engine/maths/grid.hpp"
#include "engine/maths/maths.hpp"

// other lib headers
#include <glm/glm.hpp>

namespace game2d {
void
update_renderable(entt::registry& r, const entt::entity& e, const int x, const int y, const int w, const int h)
{
  auto& transform = r.get<TransformComponent>(e);
  transform.position.x = x;
  transform.position.y = y;
  transform.scale.x = w;
  transform.scale.y = h;
};

} // namespace game2d

void
game2d::update_cursor_system(entt::registry& registry)
{
  const auto& input = registry.ctx<SINGLETON_InputComponent>();

  {
    const auto& view = registry.view<CursorComponent>();
    view.each([&registry, &input](auto entity, auto& c) {
      //
      // Rendering...
      //
      int x_offset = input.mouse_drag_boundingbox.x;
      int x_offset_half = input.mouse_drag_boundingbox.x / 2.0f;
      int y_offset = input.mouse_drag_boundingbox.y;
      int y_offset_half = input.mouse_drag_boundingbox.y / 2.0f;

      int tx = 0;
      int ty = 0;
      int bx = 0;
      int by = 0;
      int lx = 0;
      int ly = 0;
      int rx = 0;
      int ry = 0;
      if (get_mouse_lmb_press() || get_mouse_lmb_held() || get_mouse_lmb_release()) {
        // draw a cursor from the clicked point as the top left
        tx = input.mouse_click.x + x_offset_half;
        ty = input.mouse_click.y;
        bx = input.mouse_click.x + x_offset_half;
        by = input.mouse_click.y + y_offset;
        lx = input.mouse_click.x;
        ly = input.mouse_click.y + y_offset_half;
        rx = input.mouse_click.x + x_offset;
        ry = input.mouse_click.y + y_offset_half;
      } else {
        // draw a cursor around the cursor as the center point
        tx = input.mouse_position_in_worldspace.x;
        ty = input.mouse_position_in_worldspace.y - y_offset_half;
        bx = input.mouse_position_in_worldspace.x;
        by = input.mouse_position_in_worldspace.y + y_offset_half;
        lx = input.mouse_position_in_worldspace.x - x_offset_half;
        ly = input.mouse_position_in_worldspace.y;
        rx = input.mouse_position_in_worldspace.x + x_offset_half;
        ry = input.mouse_position_in_worldspace.y;
      }
      update_renderable(registry, c.line_u, tx, ty, input.mouse_drag_boundingbox.x, 1);
      update_renderable(registry, c.line_d, bx, by, input.mouse_drag_boundingbox.x, 1);
      update_renderable(registry, c.line_l, lx, ly, 1, input.mouse_drag_boundingbox.y);
      update_renderable(registry, c.line_r, rx, ry, 1, input.mouse_drag_boundingbox.y);
      update_renderable(registry, c.backdrop, tx, ly, input.mouse_drag_boundingbox.x, input.mouse_drag_boundingbox.y);
      auto& ps = registry.get<PhysicsSizeComponent>(c.backdrop);
      ps.w = input.mouse_drag_boundingbox.x;
      ps.h = input.mouse_drag_boundingbox.y;
    });
  }
};
