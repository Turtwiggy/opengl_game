#pragma once

// game2d headers
#include "ecs.hpp"

// engine headers
#include "engine/application.hpp"

// other project headers
#include <glm/glm.hpp>

// std lib
#include <memory>

namespace game2d {

class RenderSystem : public game2d::System
{
public:
  void update(game2d::Coordinator& registry);
};

std::shared_ptr<RenderSystem>
init_render_system(game2d::Coordinator& registry, const glm::ivec2& screen_wh);

void
end_frame_render_system(game2d::Coordinator& registry);

} // namespace game2d
