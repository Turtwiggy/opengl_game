// your header
#include "cursor.hpp"

// components
#include "components/cursor.hpp"
#include "modules/camera/components.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"

// other engine headers
#include "engine/maths/grid.hpp"
#include "engine/maths/maths.hpp"

// other lib headers
#include <glm/glm.hpp>
#include <imgui.h>

namespace game2d {
void
update_renderable(entt::registry& r, const entt::entity& e, const int x, const int y, const int w, const int h)
{
  auto& pos = r.get<PositionIntComponent>(e);
  auto& size = r.get<RenderSizeComponent>(e);
  pos.x = x;
  pos.y = y;
  size.w = w;
  size.h = h;
};

} // namespace game2d

void
game2d::update_cursor_system(entt::registry& registry, engine::Application& app)
{
  const int CURSOR_SIZE = 16;

  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();
  const auto& cameras = registry.view<CameraComponent, PositionIntComponent>();
  const auto& main_camera = cameras.front();
  const auto& main_camera_position = registry.get<PositionIntComponent>(main_camera);
  const auto& camera_pos = glm::ivec2{ main_camera_position.x, main_camera_position.y };
  glm::ivec2 imgui_mouse_pos = app.get_input().get_mouse_pos();
  glm::ivec2 mouse_pos = imgui_mouse_pos - ri.viewport_pos;
  glm::ivec2 mouse_pos_adjusted_in_worldspace = { mouse_pos.x + (camera_pos.x * -1),
                                                  mouse_pos.y + (camera_pos.y * -1) };

  {
    const auto& view = registry.view<CursorComponent>();
    view.each([&registry, &app, &CURSOR_SIZE, &mouse_pos_adjusted_in_worldspace](auto& c) {
      //
      // Input...
      //

      c.click = false;
      if (app.get_input().get_mouse_lmb_press()) {
        c.mouse_click = mouse_pos_adjusted_in_worldspace;
        c.click = true;
      }

      c.held = false;
      if (app.get_input().get_mouse_lmb_held()) {
        c.mouse_held = mouse_pos_adjusted_in_worldspace;
        c.held = true;
      }

      c.release = false;
      if (app.get_input().get_mouse_lmb_release()) {
        c.mouse_click = glm::ivec2(0);
        c.mouse_held = glm::ivec2(0);
        c.release = true;
      }

      //
      // Rendering...
      //

      if (c.click || c.held || c.release) {
        // draw expanding square
        int width = c.mouse_held.x - c.mouse_click.x;
        int height = c.mouse_held.y - c.mouse_click.y;
        c.mouse_wh.x = width;
        c.mouse_wh.y = height;

        { // top
          int x = c.mouse_click.x + (width / 2.0f);
          int y = c.mouse_click.y;
          update_renderable(registry, c.line_u, x, y, width, 1);
        }
        { // bottom
          int x = c.mouse_click.x + (width / 2.0f);
          int y = c.mouse_click.y + height;
          update_renderable(registry, c.line_d, x, y, width, 1);
        }
        { // left
          int x = c.mouse_click.x;
          int y = c.mouse_click.y + (height / 2.0f);
          update_renderable(registry, c.line_l, x, y, 1, height);
        }
        { // right
          int x = c.mouse_click.x + width;
          int y = c.mouse_click.y + (height / 2.0f);
          update_renderable(registry, c.line_r, x, y, 1, height);
        }
        { // backdrop
          int x = c.mouse_click.x + (width / 2.0f);
          int y = c.mouse_click.y + (height / 2.0f);
          update_renderable(registry, c.backdrop, x, y, width, height);
          // backdrop has physics size, update that too
          auto& ps = registry.get<PhysicsSizeComponent>(c.backdrop);
          ps.w = width;
          ps.h = height;
        }
      } else {
        // draw regular square
        c.mouse_wh.x = CURSOR_SIZE;
        c.mouse_wh.y = CURSOR_SIZE;

        glm::ivec2 world_pos = mouse_pos_adjusted_in_worldspace;
        int offset = static_cast<int>(CURSOR_SIZE / 2.0f);

        { // top
          int x = world_pos.x;
          int y = world_pos.y - offset;
          update_renderable(registry, c.line_u, x, y, CURSOR_SIZE, 1);
        }
        { // bottom
          int x = world_pos.x;
          int y = world_pos.y + offset;
          update_renderable(registry, c.line_d, x, y, CURSOR_SIZE, 1);
        }
        { // left
          int x = world_pos.x - offset;
          int y = world_pos.y;
          update_renderable(registry, c.line_l, x, y, 1, CURSOR_SIZE);
        }
        { // right
          int x = world_pos.x + offset;
          int y = world_pos.y;
          update_renderable(registry, c.line_r, x, y, 1, CURSOR_SIZE);
        }
        { // backdrop
          int x = world_pos.x;
          int y = world_pos.y;
          update_renderable(registry, c.backdrop, x, y, CURSOR_SIZE, CURSOR_SIZE);
          // backdrop has physics size, update that too
          auto& ps = registry.get<PhysicsSizeComponent>(c.backdrop);
          ps.w = CURSOR_SIZE;
          ps.h = CURSOR_SIZE;
        }
      }
    });
  }
};
