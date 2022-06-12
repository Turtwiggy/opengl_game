// header
#include "system.hpp"

// components/systems
#include "modules/camera/components.hpp"
#include "modules/renderer/components.hpp"

namespace game2d {

void
update_camera_system(entt::registry& registry, engine::Application& app)
{
  const auto& cameras = registry.view<CameraComponent, PositionIntComponent>();
  const auto& main_camera = cameras.front();
  auto& main_camera_position = registry.get<PositionIntComponent>(main_camera);

  const int CAM_SPEED = 2;

  if (app.get_input().get_key_held(SDL_SCANCODE_W))
    main_camera_position.y += 1 * CAM_SPEED;
  if (app.get_input().get_key_held(SDL_SCANCODE_S))
    main_camera_position.y -= 1 * CAM_SPEED;
  if (app.get_input().get_key_held(SDL_SCANCODE_A))
    main_camera_position.x += 1 * CAM_SPEED;
  if (app.get_input().get_key_held(SDL_SCANCODE_D))
    main_camera_position.x -= 1 * CAM_SPEED;
};

} // namespace game2d