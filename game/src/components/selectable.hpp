#pragma once

#include <glm/glm.hpp>

namespace game2d {

struct SelectableComponent
{
  bool is_selected = false;
};

struct HighlightComponent
{
  glm::vec4 start_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
  glm::vec4 highlight_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
};

} // namespace game2d
