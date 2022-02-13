#pragma once

// engine headers
#include "engine/application.hpp"
#include "engine/grid.hpp"

// game headers
#include "components.hpp"
#include "ecs.hpp"
#include "modules/renderer/components.hpp"

namespace game2d {

class CursorSystem : public System
{
public:
  void update(game2d::Coordinator& registry,
              engine::Application& app,
              const RendererInfo& ri,
              const SINGLETON_GridSizeComponent& grid);
};

} // namespace game2d