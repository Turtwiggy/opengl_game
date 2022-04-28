#pragma once

// other libs
#include <glm/glm.hpp>

// engine headers
#include "engine/input.hpp"
#include "engine/maths.hpp"

// std lib headers
#include <vector>

namespace game2d {

struct ClickToDestroyComponent
{
  bool placeholder = true;
};

struct CursorComponent
{
  int cursor_ltrd = 0; // 0=l, 1=t, 2=r, 3=d
};

struct DestroyAfterTimeComponent
{
  float time = 0.0f;
};

struct DoubleJumpComponent
{
  bool able_to_jump = true;
};

struct FlashColourComponent
{
  glm::vec4 start_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
  glm::vec4 flash_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
};

struct HealthComponent
{
  int hp = 5;
};

struct ParryComponent
{
  bool placeholder = false;
};

struct PlayerComponent
{
  int player = 0;
};

//
// singletons
//

struct SINGLETON_ColoursComponent
{
  const glm::vec4 red = glm::vec4(232 / 255.0f, 80 / 255.0f, 100 / 255.0f, 1.0f);
  const glm::vec4 cyan = glm::vec4(8 / 255.0f, 177 / 255.0f, 190 / 255.0f, 1.0f);
  const glm::vec4 dblue = glm::vec4(49 / 255.0f, 99 / 255.0f, 188 / 255.0f, 1.0f);
  const glm::vec4 white = glm::vec4(1.0f);
  const glm::vec4 green = glm::vec4(100 / 255.0f, 188 / 255.0f, 49 / 255.0f, 1.0f);
};

struct SINGLETON_GamePausedComponent
{
  bool paused = false;
};

struct SINGLETON_ResourceComponent
{
  engine::RandomState rnd;
};

} // namespace game2d