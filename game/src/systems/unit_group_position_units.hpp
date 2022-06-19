#pragma once

// engine headers
#include "engine/app/application.hpp"

// other lib headers
#include <entt/entt.hpp>

namespace game2d {

void
update_unit_group_position_units_system(entt::registry& registry, engine::Application& app, float dt);

} // namespace game2d