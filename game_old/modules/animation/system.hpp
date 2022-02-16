#pragma once

// engine headers
#include "engine/application.hpp"

// other lib headers
#include "ecs.hpp"

namespace game2d {

void
update_animation_system(game2d::Coordinator& registry, engine::Application& app, float dt);

} // namespace game2d