#pragma once

#include "engine/colour.hpp"

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

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

struct SINGLETON_SpriteTextures
{
  // texture unit (slot on gpu)
  const int tex_unit_kenny = 2;
  const int tex_unit_custom = 3;
  const int tex_unit_sprout = 4;
  // texture ids (id after bound)
  unsigned int tex_id_kenny = 0;
  unsigned int tex_id_custom = 0;
  unsigned int tex_id_sprout = 0;
  // resource paths
  std::string sheet_kenny = "assets/textures/kennynl_1bit_pack/monochrome_transparent_packed.png";
  std::string sheet_custom = "assets/textures/custom_spaceships.png";
  std::string sheet_sprout = "assets/textures/sprout_lands/chars/basic_char.png";
  // sprite info
  std::string yml_kenny = "assets/config/spritemap_kennynl.yml";
  std::string yml_custom = "assets/config/spritemap_custom.yml";
  std::string yml_sprout = "assets/config/spritemap_sprout.yml";

  std::vector<SpriteAnimation> animations;
};

} // namespace game2d