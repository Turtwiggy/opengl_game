#pragma once

// engine headers
#include "engine/app/application.hpp"

// other lib headers
#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace game2d {

glm::ivec2
mouse_position_in_worldspace(entt::registry& registry, engine::Application& app);

} // namespace game2d