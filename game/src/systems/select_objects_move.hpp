#pragma once

// engine headers
#include "engine/app/application.hpp"

// other lib headers
#include <entt/entt.hpp>

namespace game2d {

void
update_select_objects_move_system(entt::registry& registry, engine::Application& app, const float dt);

} // namespace game2d