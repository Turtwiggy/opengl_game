#include "modules/ui_game/system.hpp"

// game
// #include "modules/game_combat"
#include "create_entities.hpp"
#include "game_modules/components/units.hpp"
#include "resources/colour.hpp"

// other lib headers
#include <imgui.h>
#include <iostream> // temp

void
game2d::update_ui_game_system(entt::registry& registry)
{
  const auto colours = registry.ctx<SINGLETON_ColoursComponent>();

  // ImGui::Begin("Game", NULL, ImGuiWindowFlags_NoFocusOnAppearing);

  // if (ImGui::Button("Create Unit"))
  //   std::cout << "todo: create unit" << std::endl;
  // // create_player(registry,
  // //               25,                   // x
  // //               25,                   // y
  // //               50,                   // sx
  // //               50,                   // sy
  // //               std::string("UNIT"),  // name
  // //               std::string("EMPTY"), // sprite
  // //               colours.cyan,
  // //               colours.dblue);

  // ImGui::End();
};