// your header
#include "systems/player_move_on_grid.hpp"

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
      if (vx == 0 && vy == 0) // no need to update entity in map if it hasn't moved
        return;

      int index = map.size_x * gridpos.y + gridpos.x;
      auto& entities = map.entities[index];
      int neighbour_index = map.size_x * glm::max(gridpos.y - vy, 0) + glm::max(gridpos.x + vx, 0);
      auto& neighbour_entities = map.entities[neighbour_index];

      if (neighbour_entities.size() > 0) {
        printf("blocked!");
        return; // skip -- blocked
      }

      auto it = entities.begin();
      while (it != entities.end()) {
        const entt::entity& e = *it;

        bool removed = false;
        if (e == entity) {
          it = entities.erase(it); // erase old entity in old grid slot
          removed = true;
        } else {
          ++it;
        }

        if (removed) {

          gridpos.x += vx;
          gridpos.y -= vy;

          // put entity in new grid slot
          neighbour_entities.push_back(entity);
        }
      }
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
