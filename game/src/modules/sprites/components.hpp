#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace game2d {

struct sprite
{
  std::string name;
  int x = 0;
  int y = 0;
  float angle = 0.0f;
};

// A sprite tag is attached to a object,
// then the system will remove the spritetag
// and replace itself with a sprite component
struct SpriteTagComponent
{
  std::string tag;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SpriteTagComponent, tag)
};

struct SpriteComponent
{
  int x = 0;
  int y = 0;
  float offset = 0.0f;

  // do not serialize
};

}