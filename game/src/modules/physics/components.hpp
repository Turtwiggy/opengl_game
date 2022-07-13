#pragma once

// c++ headers
#include <cstdint>
#include <map>
#include <vector>

namespace game2d {

// enum class BodyType
// {
//   Static = 0,
//   Dynamic,
//   Kinematic
// };

// struct Rigidbody2DComponent
// {
//   BodyType Type = BodyType::Static;
//   bool fixed_rotation = false;
//   void* runtime_body = nullptr;
// };

// struct BoxCollider2DComponent
// {
//   glm::vec2 offset = { 0.0f, 0.0f };
//   glm::vec2 size = { 0.5f, 0.5f };

//   float density = 1.0f;
//   float friction = 0.5f;
//   float restitution = 0.0f;
//   float restitution_threshold = 0.5f;

//   void* runtime_fixture = nullptr;
// };

// Note; for no collision, don't attach a CollidableComponent
enum class GameCollisionLayer
{
  ACTOR_UNIT_GROUP = 1,
  ACTOR_U = 2,
  ACTOR_OBJECTIVE = 3,
  ACTOR_CURSOR = 4,
  ACTOR_PROJECTILE = 5,
  SOLID_WALL = 6,

  COUNT = 7,
};

struct PhysicsActorComponent
{
  GameCollisionLayer layer_id;
};

struct PhysicsSolidComponent
{
  GameCollisionLayer layer_id;
};

struct PhysicsSizeComponent
{
  int w = 0;
  int h = 0;

  PhysicsSizeComponent() = default;
  PhysicsSizeComponent(int w, int h)
    : w(w)
    , h(h){};
};

struct VelocityComponent
{
  float x = 0.0f;
  float y = 0.0f;
};

// specific for actor-actor collisions

// A collision occurs between two entities
struct Collision2D
{
  uint32_t ent_id_0 = 0;
  uint32_t ent_id_1 = 0;
  bool collision_x = false;
  bool collision_y = false;
  bool dirty = false; // dirty means it occurred last frame
};

// -- singleton components

struct SINGLETON_PhysicsComponent
{
  // resulting collisions from all registered objects
  std::map<uint64_t, Collision2D> frame_collisions;

  // persistent buffer, meaning the events enter, stay and exit can be tracked.
  std::map<uint64_t, Collision2D> persistent_collisions;

  // collision states
  std::vector<Collision2D> collision_enter;
  std::vector<Collision2D> collision_stay;
  std::vector<Collision2D> collision_exit;
};

} // game2d