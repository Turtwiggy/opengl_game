// your header
#include "select_objects_highlight.hpp"

// components
#include "components/selectable.hpp"
#include "modules/renderer/components.hpp"

void
game2d::update_select_objects_highlight_system(entt::registry& registry, engine::Application& app)
{
  const auto& view = registry.view<SelectableComponent, HighlightComponent, ColourComponent>();
  view.each([](auto entity, const auto& s, const auto& highlight, auto& colour) {
    if (s.is_selected)
      colour.colour = highlight.highlight_colour;
    else
      colour.colour = highlight.start_colour;
  });
};
