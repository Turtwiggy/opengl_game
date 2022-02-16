#pragma once

// game2d headers
#include "ecs.hpp"

// engine headers
#include "engine/application.hpp"

namespace game2d {

class ProfilerSystem : public System
{
public:
  static void init(game2d::Coordinator& registry);
  void update(game2d::Coordinator& registry, engine::Application& app);
};

} // namespace game2d