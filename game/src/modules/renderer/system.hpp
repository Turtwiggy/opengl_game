#pragma once

// game2d headers
#include "components.hpp"
#include "ecs.hpp"

// engine headers
#include "engine/application.hpp"

// other project headers
#include <glm/glm.hpp>

// std lib
#include <memory>

namespace game2d {

RendererInfo
init_render_system(const glm::ivec2& screen_wh);

class RenderSpriteSystem : public game2d::System
{
public:
  void update(const game2d::Coordinator& registry, RendererInfo& ri);
};

void
end_frame_render_system();

} // namespace game2d
