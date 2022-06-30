#pragma once

// engine libs
#include "engine/maths/maths.hpp"

// other libs
#include <glm/glm.hpp>

namespace game2d {

struct SINGLETON_ColoursComponent
{
  // palette
  const glm::vec4 red = glm::vec4(232 / 255.0f, 80 / 255.0f, 100 / 255.0f, 1.0f);
  const glm::vec4 desat_red = glm::vec4(255 / 255.0f, 121 / 255.0f, 121 / 255.0f, 1.0f);
  const glm::vec4 backdrop_red = glm::vec4(232 / 255.0f, 80 / 255.0f, 100 / 255.0f, 0.1f);
  const glm::vec4 cyan = glm::vec4(8 / 255.0f, 177 / 255.0f, 190 / 255.0f, 1.0f);
  const glm::vec4 dblue = glm::vec4(49 / 255.0f, 99 / 255.0f, 188 / 255.0f, 1.0f);
  const glm::vec4 white = glm::vec4(1.0f);
  const glm::vec4 feint_white = glm::vec4(1.0f, 1.0f, 1.0f, 0.15f);
  const glm::vec4 whiteish = glm::vec4(198 / 255.0f, 201 / 255.0f, 208 / 255.0f, 1.0f);
  const glm::vec4 green = glm::vec4(100 / 255.0f, 188 / 255.0f, 49 / 255.0f, 1.0f);
  const glm::vec4 grey = glm::vec4(82 / 255.0f, 88 / 255.0f, 98 / 255.0f, 1.0f);

  // game colours
  const glm::vec4 background = grey;
  const glm::vec4 player_unit = whiteish;
  const glm::vec4 enemy_unit = red;
};

struct SINGLETON_GamePausedComponent
{
  bool paused = false;
};

struct SINGLETON_ResourceComponent
{
  engine::RandomState rnd;
};

}; // namespace game2d