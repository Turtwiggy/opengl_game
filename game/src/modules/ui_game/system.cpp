#include "modules/ui_game/system.hpp"

// game
// #include "modules/game_combat"
#include "components/app.hpp"
#include "create_entities.hpp"

// other lib headers
#include <imgui.h>
#include <iostream> // temp
#include <string>

void
game2d::update_ui_game_system(entt::registry& registry, engine::Application& app)
{
  const auto colours = registry.ctx<SINGLETON_ColoursComponent>();

  ImGui::Begin("Game");
  ImGui::Text("Hello");

  if (ImGui::Button("Create Unit")) {
    std::cout << "creating unit..!" << std::endl;

    // ..!
    create_player(registry,
                  25,                   // x
                  25,                   // y
                  50,                   // sx
                  50,                   // sy
                  std::string("UNIT"),  // name
                  std::string("EMPTY"), // sprite
                  colours.cyan,
                  colours.dblue);
  }

  ImGui::End();
};