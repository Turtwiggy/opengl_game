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
  std::vector<std::vector<entt::entity>> entities;
  int objects_on_map = 0;
  int size_x = 10;
  int size_y = 10;

  // editor?
  // bool place_sprite = false;
  // std::optional<sprite::type> sprite_to_place = std::nullopt;
  std::optional<entt::entity> selected_entity;
};

} // namespace game2d