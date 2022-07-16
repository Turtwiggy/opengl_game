#pragma once

#include <entt/entt.hpp>

#include <vector>

namespace game2d {

struct SINGLETON_EntityBinComponent
{
  std::vector<entt::entity> dead;
};

};