#pragma once

// game2d
#include "helpers/spritemap.hpp"

// other libs
#include "entt/entt.hpp"

// c++ lib headers
#include <optional>
#include <vector>

namespace game2d {

struct SINGLETON_MapComponent
{
  int size_x = 10;
  int size_y = 10;
  std::vector<entt::entity> entities;

  // editor?
  bool place_sprite = false;
  std::optional<sprite::type> sprite_to_place = std::nullopt;
};

} // namespace game2d