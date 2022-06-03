// your header
#include "select_objects.hpp"

// components
#include "components/cursor.hpp"
#include "components/selectable.hpp"
#include "modules/physics/components.hpp"
#include "modules/physics/helpers.hpp"
#include "modules/renderer/components.hpp"

// other lib headers
#include <glm/glm.hpp>

// std lib headers
#include <vector>

void
game2d::update_select_objects_system(entt::registry& registry, engine::Application& app)
{
  // .. get the list of all collisions
  const auto& p = registry.ctx<SINGLETON_PhysicsComponent>();

  {
    const auto& view = registry.view<CursorComponent>();
    view.each([&registry, &p](auto entity, auto& c) {
      //... for each cursor ...

      if (!c.click && !c.held) {
        // user is not holding...
        return;
      }

      const auto& selectable = registry.view<SelectableComponent>();
      selectable.each([&c](auto entity, auto& selectable) {
        if (c.click) {
          // user clicked... remove all the old selected
          // (i.e. keep persistent until new click)
          selectable.is_selected = false;
        }
      });

      // Now check physics systems for cursor-unit collisions

      for (const auto& coll : p.collision_stay) {

        const auto e0_id = static_cast<entt::entity>(coll.ent_id_0);
        const auto& e0_layer = registry.get<PhysicsActorComponent>(e0_id);
        const auto e0_layer_id = e0_layer.layer_id;

        const auto e1_id = static_cast<entt::entity>(coll.ent_id_1);
        const auto& e1_layer = registry.get<PhysicsActorComponent>(e1_id);
        const auto e1_layer_id = e1_layer.layer_id;

        entt::entity e0_cursor = entt::null;
        entt::entity e1_unit = entt::null;

        // Collisions are bi-directional, but only one collision exists
        if (e0_layer_id == GameCollisionLayer::ACTOR_UNIT && e1_layer_id == GameCollisionLayer::ACTOR_CURSOR) {
          e0_cursor = e1_id;
          e1_unit = e0_id;
        }
        if (e0_layer_id == GameCollisionLayer::ACTOR_CURSOR && e1_layer_id == GameCollisionLayer::ACTOR_UNIT) {
          e0_cursor = e0_id;
          e1_unit = e1_id;
        }

        if (e0_cursor != entt::null && e1_unit != entt::null) {
          auto& sc = registry.get<SelectableComponent>(e1_unit);
          sc.is_selected = true;
        }
      }
    });
  }
};
