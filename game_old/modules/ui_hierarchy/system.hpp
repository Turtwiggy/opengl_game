#pragma once

// engine headers
#include "engine/application.hpp"

// other lib headers
#include "ecs.hpp"

namespace game2d {

void
init_ui_hierarchy_system(game2d::Coordinator& registry);

void
update_ui_hierarchy_system(game2d::Coordinator& registry, engine::Application& app);

} // namespace game2d