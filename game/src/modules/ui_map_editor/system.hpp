#pragma once

// engine headers
#include "engine/application.hpp"

// other lib headers
#include <entt/entt.hpp>

namespace game2d {

void
update_ui_map_editor_system(entt::registry& registry, engine::Application& app, float dt);

}; // namespace game2d