// your header
#include "ui_map_editor/system.hpp"

// components
#include "renderer/components.hpp"
#include "ui_map_editor/components.hpp"

// engine
#include "engine/util.hpp"

// helpers
#include "ui_map_editor/helpers.hpp"

// other lib headers
#include <imgui.h>

// std lib
#include <stdio.h>
#include <string>

void
game2d::init_ui_map_editor_system(game2d::Coordinator& registry)
{
  registry.set<SINGLETON_MapEditorInfo>(SINGLETON_MapEditorInfo());
};

void
game2d::update_ui_map_editor_system(game2d::Coordinator& registry, engine::Application& app, float dt)
{
  SINGLETON_MapEditorInfo& i = registry.ctx<SINGLETON_MapEditorInfo>();

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
  std::string map_path = "assets/scenes/default.yml";

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

  ImGui::End();
}
