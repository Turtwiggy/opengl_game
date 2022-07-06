#pragma once

// engine libs
#include "engine/colour.hpp"
#include "engine/maths/maths.hpp"

// other libs
#include <glm/glm.hpp>

namespace game2d {

struct SINGLETON_GamePausedComponent
{
  bool paused = false;
};

struct SINGLETON_ResourceComponent
{
  engine::RandomState rnd;
};

}; // namespace game2d