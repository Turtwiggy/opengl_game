// your header
#include "collisions_actor_actor.hpp"

// components
#include "modules/physics/components.hpp"

// helpers
#include "gameplay_helpers/physics_layers.hpp"

// c++ lib headers
#include <algorithm>
#include <iostream>

void
game2d::update_actor_actor_collision_system(entt::registry& registry, engine::Application& app, float dt)
{
  SINGLETON_PhysicsComponent& p = registry.ctx<SINGLETON_PhysicsComponent>();

  for (const auto& coll : p.collision_enter) {
    CollidableComponent& e0_layer = registry.get<CollidableComponent>(static_cast<entt::entity>(coll.ent_id_0));
    auto e0_layer_id = e0_layer.layer_id;
    CollidableComponent& e1_layer = registry.get<CollidableComponent>(static_cast<entt::entity>(coll.ent_id_1));
    auto e1_layer_id = e1_layer.layer_id;

    // Now run through resolutions?
    // HmmmmmMMMMMMMMmmmmmmmmMMMMMM this seems bad
    //
    auto min_id = static_cast<GameCollisionLayer>(std::min(e0_layer_id, e1_layer_id));
    auto max_id = static_cast<GameCollisionLayer>(std::max(e0_layer_id, e1_layer_id));

    // Note: actor-solid collisons should not happen here

    if (min_id == GameCollisionLayer::ACTOR_PLAYER && max_id == GameCollisionLayer::ACTOR_PLAYER) {
      std::cout << "PLAYER-PLAYER collision..." << std::endl;
    } else if (min_id == GameCollisionLayer::ACTOR_PLAYER && max_id == GameCollisionLayer::ACTOR_PROJECTILE) {
      std::cout << "PLAYER-PROJECTILE collision..." << std::endl;
    }
  }
}