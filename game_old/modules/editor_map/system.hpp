#pragma once

// engine headers
#include "engine/application.hpp"

// other lib headers
#include "ecs.hpp"

namespace game2d {

void
init_map_editor_system(game2d::Coordinator& registry);

void
update_map_editor_system(game2d::Coordinator& registry, engine::Application& app, float dt);

}; // namespace game2d