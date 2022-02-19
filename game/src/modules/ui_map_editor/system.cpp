// your header
#include "modules/ui_map_editor/system.hpp"

// components
#include "modules/renderer/components.hpp"
#include "modules/ui_map_editor/components.hpp"

// engine
#include "engine/util.hpp"

// helpers
#include "modules/ui_map_editor/helpers.hpp"

// other lib headers
#include <imgui.h>

// std lib
#include <stdio.h>
#include <string>

void
game2d::update_ui_map_editor_system(entt::registry& registry, engine::Application& app, float dt)
{
  SINGLETON_MapEditorComponent& i = registry.ctx<SINGLETON_MapEditorComponent>();
  SINGLETON_RendererInfo& ri = registry.ctx<SINGLETON_RendererInfo>();

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

  // temp
  std::string map_path = "assets/scenes/default.entt";

  ImGui::Begin("Map Editor", NULL, ImGuiWindowFlags_NoFocusOnAppearing);

  if (ImGui::Button("Save map")) {
    serialize_to_text(registry, map_path);
  }

  if (ImGui::Button("Load map")) {
    deserialize_text_to_registry(registry, map_path);
  }

  ImGui::Checkbox("Place Sprite", &i.place_sprite);

  if (i.place_sprite) {
    auto sprite = i.sprite_to_place.value_or(sprite::type::EMPTY);
    int sprite_int = static_cast<int>(sprite);
    const int sprite_max = static_cast<int>(sprite::type::_COUNT);
    ImGui::DragInt("Sprite", &sprite_int, 1.0f, 0, sprite_max);
    i.sprite_to_place = static_cast<sprite::type>(sprite_int);
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
}
