#pragma once

// engine headers
#include "engine/application.hpp"

// other lib headers
#include "ecs.hpp"

namespace game2d {

void
update_actor_actor_collision_system(game2d::Coordinator& registry, engine::Application& app, float dt);

}; // namespace game2d