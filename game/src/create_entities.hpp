#pragma once

#include <entt/entt.hpp>

namespace game2d {

entt::entity
create_camera(entt::registry& r);

entt::entity
create_hierarchy_root_node(entt::registry& r);

entt::entity
create_player(entt::registry& r);

entt::entity
create_asteroid(entt::registry& r);

} // namespace game2d