#include "player.hpp"

#include <imgui.h>

namespace game2d {

void
update_ui_highscore_system(entt::registry& r)
{

  // TODO: this should be in a system
  // update_score_system
  // and then update this ui based off that state
  static float score = 0.0f;
  {
    const float dt = ImGui::GetIO().DeltaTime;
    score += dt;
  }

  ImGui::Begin("Highscore");
  ImGui::Text("Score: %0.2f", score);
  ImGui::Text("Highscore: %0.2f", 0.0f);
  ImGui::End();
};

} // namespace game2d