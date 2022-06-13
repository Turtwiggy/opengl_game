#pragma once

#include <entt/entt.hpp>

// c++ lib headers
#include <vector>

namespace game2d {

struct EntityHierarchyComponent
{
  std::vector<entt::entity> children;
};

} // namespace game2d