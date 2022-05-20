#pragma once

// other libs
#include <entt/entt.hpp>
#include <glm/glm.hpp>

// std libs
#include <vector>

namespace game2d {

struct CursorComponent
{
  entt::entity line_u = entt::null;
  entt::entity line_d = entt::null;
  entt::entity line_l = entt::null;
  entt::entity line_r = entt::null;
  entt::entity backdrop = entt::null;

  glm::ivec2 mouse_click = glm::ivec2(0);
  glm::ivec2 mouse_held = glm::ivec2(0);
  glm::ivec2 mouse_wh = glm::ivec2(0);
};

} // namespace game2d