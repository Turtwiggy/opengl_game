#pragma once

// other lib headers
#include <entt/entt.hpp>

namespace game2d {

void
init_ui_hierarchy_system(entt::registry& registry);

void
update_ui_hierarchy_system(entt::registry& registry);

} // namespace game2d