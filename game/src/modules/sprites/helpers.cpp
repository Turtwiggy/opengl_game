// header
#include "modules/sprites/helpers.hpp"

// other libs
#include <yaml-cpp/yaml.h>

// std libs
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace game2d {

void
load_sprite_yml(std::vector<sprite>& sprites, const std::string path)
{
  std::cout << "loading yml: " << path << std::endl;
  YAML::Node config = YAML::LoadFile(path)["sprites"];
  sprite s;
  for (int i = 0; i < config.size(); i++) {
    auto node = config[i];
    s.name = node["name"].as<std::string>();
    s.x = node["x"].as<int>();
    s.y = node["y"].as<int>();
    if (node["angle"])
      s.angle = node["angle"].as<float>();
    sprites.push_back(s);
  }
};

sprite
find_sprite(const std::vector<sprite>& sprites, const std::string name)
{
  auto s = std::find_if(sprites.begin(), sprites.end(), [&name](const sprite& spr) { return spr.name == name; });
  if (s != std::end(sprites))
    return *s;
  else {
    std::cerr << "sprite not found: " << name << std::endl;
    exit(1);
  }
}

// void
// print_sprite_info(sprite& sprite)
// {
//   std::cout << "sprite: " << sprite.name << " " << sprite.x << " " << sprite.y << std::endl;
// }

} // namespace game2d

// std::vector<sprite::type>
// convert_int_to_sprites(int damage)
// {
//   std::vector<sprite::type> numbers;

//   if (damage == 0) {
//     numbers.push_back(sprite::type::NUMBER_0);
//     return numbers;
//   }

//   int number = damage;
//   // this iterates over number from right to left.
//   // e.g. 1230 will iterate as 0, 3, 2, 1
//   while (number > 0) {
//     int digit = number % 10;
//     number /= 10;
//     if (digit == 9)
//       numbers.push_back(sprite::type::NUMBER_9);
//     if (digit == 8)
//       numbers.push_back(sprite::type::NUMBER_8);
//     if (digit == 7)
//       numbers.push_back(sprite::type::NUMBER_7);
//     if (digit == 6)
//       numbers.push_back(sprite::type::NUMBER_6);
//     if (digit == 5)
//       numbers.push_back(sprite::type::NUMBER_5);
//     if (digit == 4)
//       numbers.push_back(sprite::type::NUMBER_4);
//     if (digit == 3)
//       numbers.push_back(sprite::type::NUMBER_3);
//     if (digit == 2)
//       numbers.push_back(sprite::type::NUMBER_2);
//     if (digit == 1)
//       numbers.push_back(sprite::type::NUMBER_1);
//     if (digit == 0)
//       numbers.push_back(sprite::type::NUMBER_0);
//   }

//   // now reverse numbers in to e.g. 1230
//   std::reverse(numbers.begin(), numbers.end());
//   return numbers;
// }

// std::array<ImVec2, 2>
// convert_sprite_to_uv(sprite::type type, float pixels, glm::ivec2 wh)
// {
//   // these are for the full texture
//   // ImVec2 tl = ImVec2(0.0f, 0.0f);
//   // ImVec2 br = ImVec2(1.0f, 1.0f);

//   // this is for part of the texture
//   auto offset = sprite::spritemap::get_sprite_offset(type);
//   ImVec2 tl = ImVec2(((offset.x * pixels + 0.0f) / wh.x), ((offset.y * pixels + 0.0f) / wh.y));
//   ImVec2 br = ImVec2(((offset.x * pixels + pixels) / wh.x), ((offset.y * pixels + pixels) / wh.y));

//   std::array<ImVec2, 2> coords = { tl, br };
//   return coords;
// };
