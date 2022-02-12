#pragma once

// game2d headers
#include <ecs.hpp>

// engine headers
#include "engine/application.hpp"

namespace game2d {

void
init_ui_profiler_system(game2d::Coordinator& registry);

void
update_ui_profiler_system(game2d::Coordinator& registry, engine::Application& app);

} // namespace game2d