#include "player.hpp"

#include "create_entities.hpp"
#include "game_modules/components/game.hpp"
#include "modules/events/components.hpp"
#include "modules/events/helpers/keyboard.hpp"
#include "modules/lifecycle/components.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"

#include "engine/maths/maths.hpp"

void
game2d::update_player_system(entt::registry& r)
{
  const auto& input = r.ctx<SINGLETON_InputComponent>();
  const auto& p = r.ctx<SINGLETON_PhysicsComponent>();
  auto& gameover = r.ctx<SINGLETON_GameOverComponent>();
  auto& eb = r.ctx<SINGLETON_EntityBinComponent>();

  //
  // Move player(s)
  //

  const auto& view = r.view<const PlayerComponent, VelocityComponent, TransformComponent>();
  view.each([&input](auto entity, const auto& player, auto& vel, auto& transform) {
    if (get_key_held(input, SDL_SCANCODE_W))
      vel.y = -1 * player.speed;
    if (get_key_held(input, SDL_SCANCODE_S))
      vel.y = 1 * player.speed;
    if (get_key_held(input, SDL_SCANCODE_A))
      vel.x = -1 * player.speed;
    if (get_key_held(input, SDL_SCANCODE_D))
      vel.x = 1 * player.speed;

    if (get_key_up(input, SDL_SCANCODE_A) || get_key_up(input, SDL_SCANCODE_D))
      vel.x = 0.0f;
    if (get_key_up(input, SDL_SCANCODE_W) || get_key_up(input, SDL_SCANCODE_S))
      vel.y = 0.0f;

    // .. rotate to velocity
    // .. IMPROVEMENT
    // Could improve this by not immediately setting rotation.z
    // and by setting a target angle that is lerped towards over multiple frames
    // const float EPSILON = 0.005f;
    // if (glm::abs(vel.x) > EPSILON || glm::abs(vel.y) > EPSILON)
    //   transform.rotation.z = engine::dir_to_angle_radians({ vel.x, vel.y }) - engine::HALF_PI;
  });

  //
  // Resolve any player collisions
  //

  entt::entity e0_player = entt::null;
  entt::entity e1_asteroid = entt::null;

  for (const auto& coll : p.collision_enter) {

    const auto e0_id = static_cast<entt::entity>(coll.ent_id_0);
    const auto& e0_layer = r.get<PhysicsActorComponent>(e0_id);
    const auto e0_layer_id = e0_layer.layer_id;

    const auto e1_id = static_cast<entt::entity>(coll.ent_id_1);
    const auto& e1_layer = r.get<PhysicsActorComponent>(e1_id);
    const auto e1_layer_id = e1_layer.layer_id;

    // Collisions are bi-directional, but only one collision exists
    if (e0_layer_id == GameCollisionLayer::ACTOR_PLAYER && e1_layer_id == GameCollisionLayer::ACTOR_ASTEROID) {
      e0_player = e0_id;
      e1_asteroid = e1_id;
      break;
    }
    if (e0_layer_id == GameCollisionLayer::ACTOR_ASTEROID && e1_layer_id == GameCollisionLayer::ACTOR_PLAYER) {
      e0_player = e1_id;
      e1_asteroid = e0_id;
      break;
    }
  }

  // Resolve asteroid-player collision

  if (e0_player != entt::null && e1_asteroid != entt::null) {
    gameover.over = true;
    eb.dead.push_back(e0_player); // kill the player
  }
};