// your header
#include "systems/cursor.hpp"

// other engine headers
#include "engine/grid.hpp"
#include "engine/maths.hpp"

// other lib headers
#include <glm/glm.hpp>
#include <imgui.h>

void
game2d::CursorSystem::update(game2d::Coordinator& registry,
                             engine::Application& app,
                             const RendererInfo& ri,
                             const SINGLETON_GridSizeComponent& grid)
{
  const int& GRID_SIZE = grid.size_xy;

  // dont process game events if the viewport says so
  if (!ri.viewport_process_events)
    return;

  glm::ivec2 imgui_mouse_pos = app.get_input().get_mouse_pos();
  glm::vec2 imgui_viewport_tl = ri.viewport_pos;
  glm::ivec2 mouse_pos = imgui_mouse_pos - glm::ivec2(imgui_viewport_tl.x, imgui_viewport_tl.y);
  glm::vec2 mouse_pos_adjusted_in_worldspace = mouse_pos;
  mouse_pos_adjusted_in_worldspace.x += GRID_SIZE / 2.0f;
  mouse_pos_adjusted_in_worldspace.y += GRID_SIZE / 2.0f;

  // Update cursor

  for (const auto& entity : entities) {
    const auto& c = registry.get_component<CursorComponent>(entity);
    auto& pos = registry.get_component<PositionIntComponent>(entity);
    auto& size = registry.get_component<RenderSizeComponent>(entity);

    glm::ivec2 grid_slot = engine::grid::world_space_to_grid_space(mouse_pos_adjusted_in_worldspace, GRID_SIZE);
    // ImGui::Text("mouse grid %i %i", grid_slot.x, grid_slot.y);

    glm::ivec2 world_space = grid_slot * GRID_SIZE;
    pos.x = grid_slot.x * GRID_SIZE;
    pos.y = grid_slot.y * GRID_SIZE;

    size.w = 1;
    size.h = 1;

    if (c.cursor_ltrd == 0) // left
    {
      size.h = GRID_SIZE;
      pos.x -= static_cast<int>(GRID_SIZE / 2.0f);
    }
    if (c.cursor_ltrd == 1) // top
    {
      size.w = GRID_SIZE;
      pos.y -= static_cast<int>(GRID_SIZE / 2.0f);
    }
    if (c.cursor_ltrd == 2) // right
    {
      size.h = GRID_SIZE;
      pos.x += static_cast<int>(GRID_SIZE / 2.0f) - 1;
    }
    if (c.cursor_ltrd == 3) // down
    {
      size.w = GRID_SIZE;
      pos.y += static_cast<int>(GRID_SIZE / 2.0f) - 1;
    }
  }
};
