// your header
#include "select_objects_move.hpp"

// components
#include "components/selectable.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"

void
game2d::update_select_objects_move_system(entt::registry& registry, engine::Application& app, const float dt)
{
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();
  glm::ivec2 imgui_mouse_pos = app.get_input().get_mouse_pos();
  glm::vec2 imgui_viewport_tl = ri.viewport_pos;
  glm::ivec2 mouse_pos = imgui_mouse_pos - glm::ivec2(imgui_viewport_tl.x, imgui_viewport_tl.y);

  const auto& view = registry.view<SelectableComponent, PositionIntComponent, VelocityComponent>();
  view.each([&app, &dt, &mouse_pos](const auto& s, const auto& pos, auto& vel) {
    if (!s.is_selected) {
      // TEMP hack
      vel.x = 0.0f;
      vel.y = 0.0f;
      return; // only interested in selected objects
    }

    if (app.get_input().get_mouse_rmb_press()) {
      glm::vec2 dir = mouse_pos - glm::ivec2(pos.x, pos.y);
      glm::vec2 n = glm::normalize(dir);
      const int speed = 200;
      vel.x = n.x * speed;
      vel.y = n.y * speed;
    }
  });
};
