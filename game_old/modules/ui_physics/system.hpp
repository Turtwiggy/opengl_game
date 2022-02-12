#pragma once

// engine headers
#include "engine/application.hpp"

// other lib headers
#include "ecs.hpp"

namespace game2d {

void
update_ui_physics_system(game2d::Coordinator& registry, engine::Application& app);

} // namespace game2d