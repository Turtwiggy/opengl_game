#pragma once

// other lib
#include "entt/entt.hpp"
#include "glm/glm.hpp"

// std lib
#include <map>
#include <optional>
#include <vector>

namespace game2d {

struct PathCostComponent
{
  int cost = 0;
};

// std::optional<std::vector<glm::ivec2>>
// a_star(entt::registry& registry, SINGLETON_TilemapComponent& tmap, glm::ivec2 first, glm::ivec2 finish);

}