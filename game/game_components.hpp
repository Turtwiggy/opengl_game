#pragma once

// other libs
#include <glm/glm.hpp>

// engine headers
#include "engine/maths.hpp"

namespace game2d {

struct ClickToDestroyComponent
{
  bool placeholder = true;
};

struct CursorComponent
{
  int cursor_ltrd = 0; // 0=l, 1=t, 2=r, 3=d
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
  float hp = 5.0f;
};

struct ParryComponent
{
  bool placeholder = false;
};

struct Player
{
  bool placeholder = false;
};

struct VelocityInBoundingboxComponent
{
  bool placeholder = false;
};

//
// singletons
//

struct SINGLETON_GamePausedComponent
{
  bool paused = false;
};

struct SINGLETON_GridSizeComponent
{
  const int size_xy = 16;
};

struct SINGLETON_ResourceComponent
{
  engine::RandomState rnd;
};

} // namespace game2d