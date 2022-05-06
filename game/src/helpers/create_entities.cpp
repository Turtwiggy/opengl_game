#include "create_entities.hpp"

// my libs
#include "components/cursor.hpp"
#include "helpers/physics_layers.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/sprites/components.hpp"

// std libs
#include <string>

namespace game2d {

void
create_cursor(entt::registry& registry)
{
  const int GRID_SIZE = 16;

  for (int i = 0; i < 4; i++) {
    entt::entity r = registry.create();
    registry.emplace<TagComponent>(r, std::string("cursor" + std::to_string(i)));
    // rendering
    registry.emplace<PositionIntComponent>(r);
    registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
    registry.emplace<ColourComponent>(r, 1.0f, 0.0f, 0.0f, 0.5f);
    registry.emplace<SpriteTagComponent>(r, "EMPTY");
    registry.emplace<TextureComponent>(r, tex_unit_kenny_nl);
    // gameplay
    registry.emplace<CursorComponent>(r, i);
  }
};

void
create_player(entt::registry& registry, int x, int y, const glm::vec4& colour)
{
  const int GRID_SIZE = 16;

  entt::entity r = registry.create();
  registry.emplace<TagComponent>(r, "player");

  // rendering
  registry.emplace<ColourComponent>(r, colour);
  registry.emplace<PositionIntComponent>(r, x * GRID_SIZE, y * GRID_SIZE);
  registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  registry.emplace<SpriteTagComponent>(r, "EMPTY");
  registry.emplace<TextureComponent>(r, tex_unit_custom_spaceships);
  // physics
  registry.emplace<CollidableComponent>(r, static_cast<uint32_t>(GameCollisionLayer::ACTOR_PLAYER));
  registry.emplace<PhysicsSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  registry.emplace<VelocityComponent>(r, 0.0f, 0.0f);
  // input
  // PlayerInputComponent pic;
  // pic.use_keyboard = true;
  // registry.emplace<PlayerInputComponent>(r, pic);
  // gameplay
};

} // namespace game2d