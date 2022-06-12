#pragma once

// engine headers
#include "engine/app/application.hpp"

// other project headers
#include <entt/entt.hpp>

namespace game2d {

void
update_camera_system(entt::registry& registry, engine::Application& app);

} // namespace game2d
