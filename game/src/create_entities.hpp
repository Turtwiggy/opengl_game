#pragma once

#include <entt/entt.hpp>

namespace game2d {

struct AsteroidComponent
{
  bool initialized = false;
  float time_alive = 0.0f;
  float max_time_alive = 0.0f;
  float spin_amount = 0.0f;
};

struct PlayerComponent
{
  float speed = 0.0f;
};

entt::entity
create_camera(entt::registry& r);

entt::entity
create_hierarchy_root_node(entt::registry& r);

entt::entity
create_player(entt::registry& r);

entt::entity
create_asteroid(entt::registry& r);

entt::entity
create_bullet(entt::registry& r);

} // namespace game2d