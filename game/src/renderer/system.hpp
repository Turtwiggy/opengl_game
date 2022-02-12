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

class RenderSpriteSystem : public game2d::System
{
public:
  void update(game2d::Coordinator& registry);
};

RendererInfo
init_render_system(const glm::ivec2& screen_wh);

void
prep_for_render(RendererInfo& ri);

void
end_frame_render_system();

} // namespace game2d
