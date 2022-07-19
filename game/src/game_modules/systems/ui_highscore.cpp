#include "player.hpp"

#include "game_modules/components/game.hpp"

#include <imgui.h>

namespace game2d {

void
update_ui_highscore_system(entt::registry& r)
{
  auto& gs = r.ctx<SINGLETON_AsteroidGameStateComponent>();

  // TODO: this should be in a system
  // update_score_system
  // and then update this ui based off that state
  const float dt = ImGui::GetIO().DeltaTime;
  gs.score += dt;

  ImGui::Begin("Highscore");
  ImGui::Text("Score: %0.2f", gs.score);
  ImGui::Text("Highscore: %0.2f", 0.0f);
  ImGui::End();
};

} // namespace game2d