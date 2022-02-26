// your header
#include "modules/ui_map/system.hpp"

// components
#include "game_components.hpp"
#include "modules/animation/components.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/ui_map/components.hpp"

// helpers
#include "helpers/physics_layers.hpp"
#include "modules/ui_map/helpers.hpp"

// other engine headers
#include "engine/grid.hpp"

// other libs
#include "imgui.h"

// std lib
#include <chrono>
#include <string>

void
game2d::init_ui_map_system(entt::registry& registry, const engine::Application& app)
{
  const auto colours = registry.ctx<SINGLETON_ColoursComponent>();
  const int GRID_SIZE = registry.ctx<SINGLETON_GridSizeComponent>().size_xy;
  const auto ri = registry.ctx<SINGLETON_RendererInfo>();
  auto res = registry.ctx<SINGLETON_ResourceComponent>();

  // obtain a seed from the timer
  const auto now = std::chrono::high_resolution_clock::now();
  const auto d = now - app.app_start;
  res.rnd.rng.seed(d.count());

  auto map = SINGLETON_MapComponent();

  // do fun map things
  const auto viewport_size = ri.viewport_size_current;
  map.size_x = viewport_size.x / GRID_SIZE;
  map.size_y = viewport_size.y / GRID_SIZE;
  // map.size_x = 10;
  // map.size_y = 10;
  map.entities.resize(map.size_x * map.size_y);

  for (int y = 0; y < map.size_y; y++) {
    for (int x = 0; x < map.size_x; x++) {
      int random = static_cast<int>(engine::rand_det_s(res.rnd.rng, 0.0f, 100.0f));

      if (random < 5) // 5%
      {
        // add a block
        entt::entity r = registry.create();
        registry.emplace<TagComponent>(r, std::string("blocks"));
        // rendering
        registry.emplace<ColourComponent>(r, colours.green);
        registry.emplace<PositionIntComponent>(r, x * GRID_SIZE, y * GRID_SIZE);
        registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
        registry.emplace<SpriteComponent>(r, sprite::type::EMPTY);
        // gameplay
        registry.emplace<GridPositionComponent>(r, x, y);

        add_entity_to_map(map, r, x, y);
      }
    }
  }

  registry.set<SINGLETON_MapComponent>(map);
};

void
game2d::update_ui_map_system(entt::registry& registry, engine::Application& app, float dt)
{
  const auto& gs = registry.ctx<SINGLETON_GridSizeComponent>();
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();
  const int GRID_SIZE = gs.size_xy;
  auto& map = registry.ctx<SINGLETON_MapComponent>();

  glm::ivec2 imgui_mouse_pos = app.get_input().get_mouse_pos();

  ImGui::Begin("Map Editor");

  if (map.selected_entity != std::nullopt) {
    ImGui::Text("You have clicked a cell with an entity in it");
  } else {
    ImGui::Text("No clicked cell");
  }

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
  ImGui::End();

  // temp
  std::string map_path = "assets/scenes/default.entt";

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
