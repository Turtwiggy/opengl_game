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
        // physics
        registry.emplace<CollidableComponent>(
          r, static_cast<uint32_t>(GameCollisionLayer::SOLID_WALL), PhysicsType::SOLID);
        registry.emplace<PhysicsSizeComponent>(r, GRID_SIZE, GRID_SIZE);
        // gameplay
        FlashColourComponent f;
        f.start_colour = colours.dblue;
        f.flash_colour = colours.green;
        registry.emplace<FlashColourComponent>(r, f);
        registry.emplace<HealthComponent>(r, 3.0f);
        registry.emplace<ClickToDestroyComponent>(r);
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

  // temp
  std::string map_path = "assets/scenes/default.entt";

  ImGui::Begin("Map Editor", NULL, ImGuiWindowFlags_NoFocusOnAppearing);

  if (ImGui::Button("Save map")) {
    serialize_to_text(registry, map_path);
  }

  if (ImGui::Button("Load map")) {
    deserialize_text_to_registry(registry, map_path);
  }

  ImGui::Checkbox("Place Sprite", &map.place_sprite);

  if (map.place_sprite) {
    auto sprite = map.sprite_to_place.value_or(sprite::type::EMPTY);
    int sprite_int = static_cast<int>(sprite);
    const int sprite_max = static_cast<int>(sprite::type::_COUNT);
    ImGui::DragInt("Sprite", &sprite_int, 1.0f, 0, sprite_max);
    map.sprite_to_place = static_cast<sprite::type>(sprite_int);
  }

  // auto uv = convert_sprite_to_uv(icon);
  ImGui::Image((ImTextureID)ri.loaded_texture_ids[0], ImVec2(768, 352));
  // ImGui::Image((ImTextureID)kenny_texture_id, { shop_icon_size.x, shop_icon_size.y }, uv[0], uv[1]);

  ImVec2 vmin = ImGui::GetWindowContentRegionMin();
  ImVec2 vmax = ImGui::GetWindowContentRegionMax();
  vmin.x += ImGui::GetWindowPos().x;
  vmin.y += ImGui::GetWindowPos().y;
  vmax.x += ImGui::GetWindowPos().x;
  vmax.y += ImGui::GetWindowPos().y;
  ImGui::GetForegroundDrawList()->AddRect(vmin, vmax, IM_COL32(255, 255, 0, 255));

  ImGui::Text("vmin: %f %f", vmin.x, vmin.y);
  ImGui::Text("vmax: %f %f", vmax.x, vmax.y);

  ImGui::End();

  // urgh.. x3
  // glm::ivec2 imgui_mouse_pos = app.get_input().get_mouse_pos();
  // glm::vec2 imgui_viewport_tl = ri.viewport_pos;
  // glm::ivec2 mouse_pos = imgui_mouse_pos - glm::ivec2(imgui_viewport_tl.x, imgui_viewport_tl.y);
  // // adjust pos to keep square visible while mouse is in 4 corners
  // glm::vec2 mouse_pos_adjusted_in_worldspace = mouse_pos;
  // mouse_pos_adjusted_in_worldspace.x += GRID_SIZE / 2.0f;
  // mouse_pos_adjusted_in_worldspace.y += GRID_SIZE / 2.0f;

  // if (map.place_sprite && app.get_input().get_mouse_lmb_down()) {
  //   glm::ivec2 grid_slot = engine::grid::world_space_to_grid_space(mouse_pos_adjusted_in_worldspace, GRID_SIZE);
  //   glm::ivec2 world_space = grid_slot * GRID_SIZE;
  //   // The entity to spawn!
  //   // Note: this shouldn't be here
  //   {
  //     entt::entity r = registry.create();
  //     registry.emplace<TagComponent>(r, "entity");
  //     registry.emplace<ColourComponent>(r, 1.0f, 1.0f, 1.0f, 1.0f);
  //     registry.emplace<PositionIntComponent>(r, world_space.x, world_space.y);
  //     registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  //     registry.emplace<PhysicsSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  //     const auto& sprite = map_editor_info.sprite_to_place.value_or(sprite::type::EMPTY);
  //     registry.emplace<SpriteComponent>(r, sprite);
  //     registry.emplace<VelocityComponent>(r);
  //     registry.emplace<CollidableComponent>(
  //       r, static_cast<uint32_t>(GameCollisionLayer::SOLID_WALL), PhysicsType::SOLID);
  //   }
  // }
}

// ImGui::Begin("Menu...");
// if (ImGui::Button("Open File")) {
//   printf("open file clicked...\n");
//   std::string filepath = engine::open_file(app.get_window(), "All\0*.*\0Text\0*.txt\0");
//   if (!filepath.empty()) {
//     printf("user wants to open file: %s \n", filepath.c_str());
//   }
// }
// if (ImGui::Button("Save File")) {
//   printf("save file clicked...\n");
//   std::string filepath = engine::save_file(app.get_window(), "All\0*.*\0.txt\0");
//   if (!filepath.empty()) {
//     printf("user wants to save file: %s \n", filepath.c_str());
//   }
// }
// ImGui::End();