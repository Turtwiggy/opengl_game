#include "ui_place_entity.hpp"

#include "create_entities.hpp"
#include "engine/maths/grid.hpp"
#include "modules/events/components.hpp"
#include "modules/events/helpers/mouse.hpp"
#include "modules/renderer/components.hpp"

#include <imgui.h>

namespace game2d {

void
update_ui_place_entity_system(entt::registry& r)
{
  const auto& input = r.ctx<SINGLETON_InputComponent>();

  if (get_mouse_rmb_press()) {

    const int GRID_SIZE = 32;
    glm::ivec2 pos = engine::grid::world_space_to_clamped_world_space(input.mouse_position_in_worldspace, GRID_SIZE);

    auto e = create_turret(r);
    auto& transform = r.get<TransformComponent>(e);
    transform.position.x = pos.x;
    transform.position.y = pos.y;
  }

  ImGui::Begin("EntityPlacer");
  ImGui::Text("Turret");
  ImGui::End();
}

} // namespace game2d