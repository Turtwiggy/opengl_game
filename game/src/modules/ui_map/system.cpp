// your header
#include "modules/ui_map/system.hpp"

// components
#include "gameplay_components/components.hpp"
#include "modules/animation/components.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/ui_map/components.hpp"

// helpers
#include "gameplay_helpers/physics_layers.hpp"
#include "modules/ui_map/helpers.hpp"

// other engine headers
#include "engine/grid.hpp"

// other libs
#include "imgui.h"

// std lib
#include <chrono>
#include <string>
#include <vector>

namespace game2d {

entt::entity
create_grid_block(entt::registry& registry,
                  int x,
                  int y,
                  sprite::type& sprite_type,
                  TileType tile_type,
                  SINGLETON_MapComponent& map,
                  const SINGLETON_GridSizeComponent& grid,
                  const SINGLETON_ColoursComponent& colours)
{
  const int GRID_SIZE = grid.size_xy;

  // add a block
  entt::entity r = registry.create();
  registry.emplace<TagComponent>(r, std::string("blocks"));
  // rendering
  registry.emplace<ColourComponent>(r, colours.green);
  registry.emplace<PositionIntComponent>(r, x * GRID_SIZE, y * GRID_SIZE);
  registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  registry.emplace<SpriteComponent>(r, sprite_type);
  // gameplay
  registry.emplace<GridPositionComponent>(r, x, y);
  // registry.emplace<GridTypeComponent>(r, tile_type);
  registry.emplace<ClickToDestroyComponent>(r);
  registry.emplace<HealthComponent>(r, 1.0f);

  add_entity_to_map(map, r, x, y);
  return r;
}

struct RectangularRoom
{
public:
  RectangularRoom(entt::registry& r,
                  int x,
                  int y,
                  int width,
                  int height,
                  SINGLETON_MapComponent& map,
                  SINGLETON_ResourceComponent& res,
                  const SINGLETON_ColoursComponent& colours,
                  const SINGLETON_GridSizeComponent& grid)
    : x1(x)
    , y1(y)
    , x2(x + width)
    , y2(y + height)
    , center_x(static_cast<int>((x1 + x2) / 2))
    , center_y(static_cast<int>((y1 + y2) / 2))
  {
    auto floor_sprite = sprite::type::BUSH_0;
    auto floor_type = TileType::FLOOR;
    auto wall_sprite = sprite::type::EMPTY;
    auto wall_type = TileType::WALL;

    // draw walls!
    int modulo_x = 0;
    int modulo_y = 0;

    for (int y = y1; y < y2; y++) {
      for (int x = x1; x < x2; x++) {
        modulo_x = (x - x1) % width;
        modulo_y = (y - y1) % height;

        // is block on the edges?
        bool create_block = false;
        create_block |= modulo_x == 0;
        create_block |= modulo_x == width - 1;
        create_block |= modulo_y == 0;
        create_block |= modulo_y == height - 1;

        if (create_block)
          create_grid_block(r, x, y, wall_sprite, wall_type, map, grid, colours);
        else
          create_grid_block(r, x, y, floor_sprite, floor_type, map, grid, colours);
      }
    }
  }

public:
  int x1 = 0;
  int y1 = 0;
  int x2 = 0;
  int y2 = 0;
  int center_x = 0;
  int center_y = 0;
};

class MapGenerator
{
public:
  void create_map(entt::registry& registry,
                  SINGLETON_MapComponent& map,
                  SINGLETON_ResourceComponent& res,
                  const SINGLETON_ColoursComponent& colours,
                  const SINGLETON_GridSizeComponent& grid)
  {
    // Credit: https://gist.github.com/munificent/b1bcd969063da3e6c298be070a22b604
    // Robert Nystrom @munificentbob for Ginny 2008-2019

    // std::vector<RectangularRoom> rooms;

    const auto add_rooms = [&](int start) {
      //
      const int w = engine::rand_det_s(res.rnd.rng, 5, 14);
      const int h = engine::rand_det_s(res.rnd.rng, 3, 8);
      const int left = engine::rand_det_s(res.rnd.rng, 0, map.size_x - w - 2);
      const int top = engine::rand_det_s(res.rnd.rng, 0, map.size_y - h - 2);
      const int right = left + w + 2;
      const int bottom = top + h + 2;

      for (int y = top; y <= bottom; ++y)
        for (int x = left; x <= right; ++x) {
          if (grid_cell_contains_tile_type(registry, map, x, y, TileType::FLOOR))
            return;
        }

      int door_count = 0;
      int dx = 0;
      int dy = 0;

      if (!start) {
        for (int y = top; y <= bottom; ++y) {
          for (int x = left; x <= right; ++x) {
            const int s = x == left || x == right;
            const int t = y == top || y == bottom;
            bool cell_contains_wall = grid_cell_contains_tile_type(registry, map, x, y, TileType::WALL);
            if (s ^ t && cell_contains_wall) {
              int random_door = engine::rand_det_s(res.rnd.rng, 0, ++door_count);
              if (random_door == 0) {
                dx = x;
                dy = y;
              }
            }
          }
        }
        if (door_count == 0)
          return;
      }

      for (int y = top; y <= bottom; ++y)
        for (int x = left; x <= right; ++x) {
          const int s = x == left || x == right;
          const int t = y == top || y == bottom;

          auto& entities = get_entities(map, x, y);

          auto floor_sprite = sprite::type::CASTLE_FLOOR;
          auto floor_type = TileType::FLOOR;
          auto wall_sprite = sprite::type::EMPTY;
          auto wall_type = TileType::WALL;
          auto door_sprite = sprite::type::BOAT;
          auto door_type = TileType::DOOR;
          auto unknown_sprite = sprite::type::CAMPFIRE;
          auto unknown_type = TileType::UNKNOWN;

          bool first = s && t;
          bool second = s ^ t;
          TileType tile_type = first ? unknown_type : second ? wall_type : floor_type;
          sprite::type tile_sprite = first ? unknown_sprite : second ? wall_sprite : floor_sprite;

          if (entities.size() > 1) {
            // problem!
            throw std::runtime_error("honk!");
          } else if (entities.size() == 1) {
            // update tile
            auto& entity = entities[0];
            update_grid_entity(registry, entity, tile_type, tile_sprite);
          } else {
            // create tile
            create_grid_block(registry, x, y, tile_sprite, tile_type, map, grid, colours);
          }
        }

      if (door_count > 0) {
        // set dx, dy to a door
        auto tile_type = TileType::DOOR;
        auto tile_sprite = sprite::type::BOAT;
        auto& entities = get_entities(map, dx, dy);

        if (entities.size() > 1) {
          // problem!
          throw std::runtime_error("honk!");
        } else if (entities.size() == 1) {
          // update tile
          auto& entity = entities[0];
          update_grid_entity(registry, entity, tile_type, tile_sprite);
        } else {
          // create tile
          create_grid_block(registry, dx, dy, tile_sprite, tile_type, map, grid, colours);
        }
      }
    };

    for (int i = 0; i < map.size_x * map.size_y; ++i) {
      int start = i == 0;
      add_rooms(start);
    }

    // set if the tile is passable

    // for (int y = 0; y < map.size_y; ++y)
    //   for (int x = 0; x < map.size_x; ++x) {
    //     Tile& tile = map.at(x, y);
    //     if (tile.ch == '!')
    //       tile.ch = '#';
    //     tile.passable = (tile.ch != '#');
    //   }
    // }
  }
};

} // namespace game2d

void
game2d::init_ui_map_system(entt::registry& registry, const engine::Application& app)
{
  const auto colours = registry.ctx<SINGLETON_ColoursComponent>();
  const auto grid = registry.ctx<SINGLETON_GridSizeComponent>();
  const auto ri = registry.ctx<SINGLETON_RendererInfo>();
  auto res = registry.ctx<SINGLETON_ResourceComponent>();
  const auto GRID_SIZE = grid.size_xy;

  // obtain a seed from the timer
  const auto now = std::chrono::high_resolution_clock::now();
  const auto d = now - app.app_start;
  res.rnd.rng.seed(d.count());

  auto map = SINGLETON_MapComponent();

  // do fun map things
  const auto viewport_size = ri.viewport_size_current;
  map.size_x = 40;
  map.size_y = 40;
  // map.size_x = viewport_size.x / GRID_SIZE;
  // map.size_y = viewport_size.y / GRID_SIZE;
  map.entities.resize(map.size_x * map.size_y);

  MapGenerator r;
  r.create_map(registry, map, res, colours, grid);

  registry.set<SINGLETON_MapComponent>(map);
};

void
game2d::update_ui_map_system(entt::registry& registry, engine::Application& app, float dt)
{
  const auto& gs = registry.ctx<SINGLETON_GridSizeComponent>();
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();
  const int GRID_SIZE = gs.size_xy;
  auto& map = registry.ctx<SINGLETON_MapComponent>();

  // Check entities in entity-map are still valid
  {
    for (const auto& eids : map.entities) {
      for (const auto& eid : eids) {
        if (!registry.valid(eid)) {
          remove_entity_from_map(map, eid);
        }
      }
    }
  }

  glm::ivec2 imgui_mouse_pos = app.get_input().get_mouse_pos();

  ImGui::Begin("Map Editor");
  ImVec2 vmin = ImGui::GetWindowContentRegionMin();
  ImVec2 vmax = ImGui::GetWindowContentRegionMax();
  vmin.x += ImGui::GetWindowPos().x;
  vmin.y += ImGui::GetWindowPos().y;
  vmax.x += ImGui::GetWindowPos().x;
  vmax.y += ImGui::GetWindowPos().y;
  ImGui::GetForegroundDrawList()->AddRect(vmin, vmax, IM_COL32(255, 255, 0, 255));
  // ImGui::Text("vmin: %f %f", vmin.x, vmin.y);
  // ImGui::Text("vmax: %f %f", vmax.x, vmax.y);
  ImGui::Text("window: %i %i", ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
  ImGui::Text("mouse %i %i", imgui_mouse_pos.x, imgui_mouse_pos.y);
  ImGui::Text("Objects on map: %i", map.objects_on_map);
  ImGui::Text("map x:%i y:%i", map.size_x, map.size_y);
  ImGui::End();

  // std::string map_path = "assets/scenes/default.entt"; // temp

  // ImGui::Begin("Map Editor", NULL, ImGuiWindowFlags_NoFocusOnAppearing);
  // if (ImGui::Button("Save map")) {
  //   serialize_to_text(registry, map_path);
  // }
  // if (ImGui::Button("Load map")) {
  //   deserialize_text_to_registry(registry, map_path);
  // }

  // ImGui::Checkbox("Place Sprite", &map.place_sprite);
  // if (map.place_sprite) {
  //   auto sprite = map.sprite_to_place.value_or(sprite::type::EMPTY);
  //   int sprite_int = static_cast<int>(sprite);
  //   const int sprite_max = static_cast<int>(sprite::type::_COUNT);
  //   ImGui::DragInt("Sprite", &sprite_int, 1.0f, 0, sprite_max);
  //   map.sprite_to_place = static_cast<sprite::type>(sprite_int);
  // }

  // auto uv = convert_sprite_to_uv(icon);
  // ImGui::Image((ImTextureID)ri.loaded_texture_ids[0], ImVec2(768, 352));
  // // ImGui::Image((ImTextureID)kenny_texture_id, { shop_icon_size.x, shop_icon_size.y }, uv[0], uv[1]);
  // ImVec2 vmin = ImGui::GetWindowContentRegionMin();
  // ImVec2 vmax = ImGui::GetWindowContentRegionMax();
  // vmin.x += ImGui::GetWindowPos().x;
  // vmin.y += ImGui::GetWindowPos().y;
  // vmax.x += ImGui::GetWindowPos().x;
  // vmax.y += ImGui::GetWindowPos().y;
  // ImGui::GetForegroundDrawList()->AddRect(vmin, vmax, IM_COL32(255, 255, 0, 255));
  // ImGui::Text("vmin: %f %f", vmin.x, vmin.y);
  // ImGui::Text("vmax: %f %f", vmax.x, vmax.y);
  // ImGui::End();
}
