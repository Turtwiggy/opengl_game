#pragma once

// game2d
#include "components.hpp"
#include "ecs.hpp"
#include "modules/ui_profiler/components.hpp"

// systems
#include "modules/renderer/system.hpp"
#include "modules/ui_profiler/system.hpp"
#include "systems/cursor.hpp"

// engine headers
#include "engine/application.hpp"
#include "engine/util.hpp"
using namespace engine;

// other lib
#include "glm/glm.hpp"

// std lib
#include <memory>

namespace game2d {

class Game
{
public:
  Game(const glm::vec2& screen_wh);
  void init();
  void update(Application& app, float dt);

private:
  Coordinator registry;
  RendererInfo renderer_info;

private: // game systems
  std::shared_ptr<game2d::CursorSystem> cursor;
  std::shared_ptr<game2d::RenderSpriteSystem> renderer;
  std::shared_ptr<game2d::ProfilerSystem> profiler;
};

} // namespace game2d