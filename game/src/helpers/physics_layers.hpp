#pragma once

namespace game2d {

// Note; for no collision, don't attach a CollidableComponent
enum class GameCollisionLayer
{
  ACTOR_PLAYER = 1,
  ACTOR_PROJECTILE = 2,
  SOLID_WALL = 3,
};

} // namespace game2d