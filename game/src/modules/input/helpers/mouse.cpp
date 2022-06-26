// header
#include "mouse.hpp"

#include "modules/camera/helpers.hpp"

#include "modules/camera/components.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"

namespace game2d {

glm::ivec2
mouse_position_in_worldspace(entt::registry& registry, engine::Application& app)
{
  glm::ivec2 mouse_pos_adjusted_in_worldspace;
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();
  const auto main_camera = get_main_camera(registry);
  const auto& transform = registry.get<TransformComponent>(main_camera);
  const auto& camera_pos = glm::ivec2{ transform.position.x, transform.position.y };
  const auto imgui_mouse_pos = app.get_input().get_mouse_pos();
  const auto mouse_pos = imgui_mouse_pos - ri.viewport_pos;
  mouse_pos_adjusted_in_worldspace = { mouse_pos.x + (camera_pos.x * -1), mouse_pos.y + (camera_pos.y * -1) };
  return mouse_pos_adjusted_in_worldspace;
};

} // namespace game2d