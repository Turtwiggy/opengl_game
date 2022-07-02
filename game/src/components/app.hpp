#pragma once

// engine libs
#include "engine/colour.hpp"
#include "engine/maths/maths.hpp"

// other libs
#include <glm/glm.hpp>

namespace game2d {

struct SINGLETON_ColoursComponent
{
  // palette
  const engine::SRGBColour red = { 232, 80, 100, 0.5f };
  const engine::SRGBColour desat_red = { 255, 121, 121, 1.0f };
  const engine::SRGBColour backdrop_red = { 232, 80, 100, 0.1f };
  const engine::SRGBColour cyan = { 8, 177, 190, 1.0f };
  const engine::SRGBColour dblue = { 49, 99, 188, 1.0f };
  const engine::SRGBColour white = { 255, 255, 255, 1.0f };
  const engine::SRGBColour feint_white = { 1.0f, 1.0f, 1.0f, 0.15f };
  const engine::SRGBColour green = { 100, 188, 49, 1.0f };
  const engine::SRGBColour grey = { 82, 88, 98, 1.0f };
  const engine::SRGBColour black = { 0, 0, 0, 1.0f };

  // game colours
  const engine::SRGBColour background = grey;
  const engine::SRGBColour player_unit = feint_white;
  const engine::SRGBColour enemy_unit = red;
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