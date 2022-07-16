#pragma once

#include "engine/maths/maths.hpp"

namespace game2d {

struct SINGLETON_GamePausedComponent
{
  bool paused = false;
};

struct SINGLETON_GameOverComponent
{
  bool over = false;
};

struct SINGLETON_ResourceComponent
{
  engine::RandomState rnd;
};

} // namespace game2d