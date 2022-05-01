// your header
#include "player_move_on_grid.hpp"

// helpers
#include "grid.hpp"

// components
#include "gameplay_components/components.hpp"
#include "gameplay_components/grid.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"

void
game2d::update_player_move_on_grid(entt::registry& registry, engine::Application& app, float dt)
{
  const int& GRID_SIZE = registry.ctx<SINGLETON_GridSizeComponent>().size_xy;

  // Process player input to update gridpos
  {
    // const auto& view =
    //   registry.view<PlayerComponent, PlayerInputComponent, PositionIntComponent, GridPositionComponent>();
    // view.each([&registry, &map, &GRID_SIZE](
    //             const auto& entity, const auto& player, const auto& input, auto& pos, auto& gridpos) {
    //   //
    //   int vx = 0;
    //   int vy = 0;
    //   if (input.move_up)
    //     vy = 1;
    //   else if (input.move_down)
    //     vy = -1;
    //   if (input.move_left)
    //     vx = -1;
    //   else if (input.move_right)
    //     vx = 1;

    //   // no need to update entity in map if it hasn't moved
    //   if (vx == 0 && vy == 0)
    //     return;

    //   int old_x = gridpos.x;
    //   int old_y = gridpos.y;
    //   int new_x = gridpos.x + vx;
    //   int new_y = gridpos.y - vy;
    //   new_x = glm::min(glm::max(new_x, 0), map.size_x - 1);
    //   new_y = glm::min(glm::max(new_y, 0), map.size_y - 1);

    //   // temp: cant collide with walls?
    //   {
    //     auto& neighbour_cell = get_entities(map, new_x, new_y);
    //     for (const auto& entity : neighbour_cell) {
    //       if (registry.all_of<GridTypeComponent>(entity)) {
    //         const auto& gtc = registry.get<GridTypeComponent>(entity);
    //         if (gtc.type == TileType::WALL) {
    //           return; // skip -- blocked
    //         }
    //       }
    //     }
    //   }

    //   move_entity_on_map(map, entity, old_x, old_y, new_x, new_y);
    //   gridpos.x = new_x;
    //   gridpos.y = new_y;

    //   // assign pos based on grid position
    //   const auto converted_pos = engine::grid::grid_space_to_world_space(glm::ivec2(gridpos.x, gridpos.y),
    //   GRID_SIZE); pos.x = static_cast<int>(converted_pos.x); pos.y = static_cast<int>(converted_pos.y);
    // });
  }

  // Process all gridpos to convert it to worldpos
  // {
  //   glm::ivec2 ipos;
  //   const auto& view = registry.view<PositionIntComponent, GridPositionComponent>();
  //   view.each([&map, &GRID_SIZE, &ipos](const auto& input, auto& pos, auto& gridpos) {
  //     ipos.x = gridpos.x;
  //     ipos.y = gridpos.y;
  //     const auto converted_pos = engine::grid::grid_space_to_world_space(ipos, GRID_SIZE);
  //     pos.x = static_cast<int>(converted_pos.x);
  //     pos.y = static_cast<int>(converted_pos.y);
  //   });
  // }
};
