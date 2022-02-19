#pragma once

// other libs
#include "entt/entt.hpp"

// c++ lib headers
#include <vector>

namespace game2d {

struct SINGLETON_MapComponent
{
  int size_x = 10;
  int size_y = 10;
  std::vector<entt::entity> entities;
};

} // namespace game2d