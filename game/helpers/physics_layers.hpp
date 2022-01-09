#pragma once

namespace game2d {

// Note; for no collision, don't attach a CollidableComponent
enum class GameCollisionLayer
{
  ACTOR_PLAYER = 1,
  ACTOR_GOAL = 2,
  ACTOR_BALL = 3,
  ACTOR_NO_OXY_ZONE = 4,
  ACTOR_ARROW = 5,
  SOLID_WALL = 6,
};

} // namespace game2d