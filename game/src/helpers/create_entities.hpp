#pragma once

// other libs
#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace game2d {

void
create_cursor(entt::registry& registry);

void
create_player(entt::registry& registry, int x, int y, const glm::vec4& colour);

} // namespace game2d