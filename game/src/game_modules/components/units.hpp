#pragma once

// other lib
#include "entt/entt.hpp"

// std lib
#include <vector>

namespace game2d {

struct UnitGroupComponent
{
  std::vector<entt::entity> units;
};

}; // namespace game2d