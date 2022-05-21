// header
#include "system.hpp"

// components
#include "modules/physics/components.hpp"

// other lib headers
#include <imgui.h>

void
game2d::update_ui_physics_system(entt::registry& registry, engine::Application& app)
{
  const auto& p = registry.ctx<SINGLETON_PhysicsComponent>();

  int solids = 0;
  int actors = 0;
  const auto& coll_view = registry.view<CollidableComponent>();
  coll_view.each([&solids, &actors](const auto& coll) {
    if (coll.type == PhysicsType::SOLID)
      solids += 1;
    else {
      actors += 1;
    }
  });

  ImGui::Begin("Physics", NULL, ImGuiWindowFlags_NoFocusOnAppearing);
  ImGui::Text("Solids %i", solids);
  ImGui::Text("Actors %i", actors);
  ImGui::Text("collision_enter %i", p.collision_enter.size());
  ImGui::Text("collision_stay %i", p.collision_stay.size());
  ImGui::Text("collision_exit %i", p.collision_exit.size());
  ImGui::End();
};