#pragma once

// my libs
#include "modules/sprites/components.hpp"

// std libs
#include <string>
#include <vector>

namespace game2d {

void
load_sprite_yml(std::vector<sprite>& sprites, const std::string path);

sprite
find_sprite(const std::vector<sprite>& sprites, const std::string name);

// std::vector<sprite::type>
// convert_int_to_sprites(int damage);

// std::array<ImVec2, 2>
// convert_sprite_to_uv(sprite::type type, float pixels, glm::ivec2 wh);

}; // namespace game2d