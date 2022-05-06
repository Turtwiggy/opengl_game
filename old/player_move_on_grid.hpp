#pragma once

// engine headers
#include "engine/app/application.hpp"

// other lib headers
#include <entt/entt.hpp>

namespace game2d {

void
update_player_move_on_grid(entt::registry& registry, engine::Application& app, float dt);

} // namespace game2d