#pragma once

// game2d headers
#include "ecs.hpp"

// engine headers
#include "engine/application.hpp"

// other project headers
#include <glm/glm.hpp>

namespace game2d {

void
init_render_system(game2d::Coordinator& registry, const glm::ivec2& screen_wh);

void
update_render_system(game2d::Coordinator& registry, engine::Application& app);

void
end_frame_render_system(game2d::Coordinator& registry);

} // namespace game2d
