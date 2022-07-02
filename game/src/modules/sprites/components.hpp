#pragma once

#include "engine/colour.hpp"

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace game2d {

struct SpriteAnimation
{
  std::string name;
  std::vector<glm::ivec2> animation_frames;
  float animation_frames_per_second = 1;
  float animation_angle = 0.0f;
};

struct SpriteAnimationComponent
{
  std::string playing_animation_name = "down_idle";
  int frame = 0;
  float frame_dt = 0.0f;
};

struct SpriteComponent
{
  engine::LinearColour colour;
  int x = 0;
  int y = 0;
  int tex_unit = 0; // do not serialize
  float angle = 0.0f;
};

struct SINGLETON_Animations
{
  std::vector<SpriteAnimation> animations;
};

} // namespace game2d