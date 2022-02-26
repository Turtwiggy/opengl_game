// your header
#include "systems/player_move_on_grid.hpp"

// helpers
#include "modules/ui_map/helpers.hpp"

// engine
#include "engine/grid.hpp"

// components
#include "game_components.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/ui_map/components.hpp"

void
game2d::update_player_move_on_grid(entt::registry& registry, engine::Application& app, float dt)
{
  const int& GRID_SIZE = registry.ctx<SINGLETON_GridSizeComponent>().size_xy;
  auto& map = registry.ctx<SINGLETON_MapComponent>();

  // Process player input to update gridpos
  {
    const auto& view = registry.view<PlayerComponent, PlayerInputComponent, GridPositionComponent>();
    view.each([&map](const auto& entity, const auto& player, const auto& input, auto& gridpos) {
      //
      int vx = 0;
      int vy = 0;
      if (input.move_up)
        vy = 1;
      else if (input.move_down)
        vy = -1;
      if (input.move_left)
        vx = -1;
      else if (input.move_right)
        vx = 1;

      // no need to update entity in map if it hasn't moved
      if (vx == 0 && vy == 0)
        return;

      int old_x = gridpos.x;
      int old_y = gridpos.y;
      int new_x = gridpos.x + vx;
      int new_y = gridpos.y - vy;
      printf("%i %i\n", old_x, old_y);

      // temp: fake collisions?
      {
        auto& neighbour_cell = get_entities(map, new_x, new_y);
        if (neighbour_cell.size() > 0) {
          printf("blocked!");
          return; // skip -- blocked
        }
      }

      move_entity_on_map(map, entity, old_x, old_y, new_x, new_y);
      gridpos.x = new_x;
      gridpos.y = new_y;
    });
  }

  // Process all gridpos to convert it to worldpos
  {
    map.objects_on_map = 0;

    for (const auto& eids : map.entities) {
      for (auto eid : eids) {
        bool has_pos = registry.all_of<PositionIntComponent>(eid);
        bool has_grid_pos = registry.all_of<GridPositionComponent>(eid);

        if (has_pos && has_grid_pos) {
          auto& pos = registry.get<PositionIntComponent>(eid);
          auto& gpos = registry.get<GridPositionComponent>(eid);

          // assign pos based on grid position
          const auto converted_pos = engine::grid::grid_space_to_world_space(glm::ivec2(gpos.x, gpos.y), GRID_SIZE);
          pos.x = static_cast<int>(converted_pos.x);
          pos.y = static_cast<int>(converted_pos.y);

          map.objects_on_map++;
        }
      }
    }
  }
};
