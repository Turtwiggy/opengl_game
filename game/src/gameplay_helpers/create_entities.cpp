#include "create_entities.hpp"

// my libs
#include "gameplay_components/components.hpp"
#include "gameplay_components/grid.hpp"
#include "gameplay_helpers/physics_layers.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/sprites/components.hpp"

// std libs
#include <string>

namespace game2d {

void
create_cursor(entt::registry& registry)
{
  const int GRID_SIZE = registry.ctx<SINGLETON_GridSizeComponent>().size_xy;

  for (int i = 0; i < 4; i++) {
    entt::entity r = registry.create();
    registry.emplace<TagComponent>(r, std::string("cursor" + std::to_string(i)));
    // rendering
    registry.emplace<PositionIntComponent>(r);
    registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
    registry.emplace<ColourComponent>(r, 1.0f, 0.0f, 0.0f, 0.5f);
    registry.emplace<TextureComponent>(r, tex_unit_custom_spaceships);
    registry.emplace<SpriteTagComponent>(r, "EMPTY");
    // gameplay
    registry.emplace<CursorComponent>(r, i);
  }
};

void
create_player(entt::registry& registry, int x, int y, const glm::vec4& colour)
{
  const int GRID_SIZE = registry.ctx<SINGLETON_GridSizeComponent>().size_xy;

  entt::entity r = registry.create();
  registry.emplace<TagComponent>(r, "player");

  // rendering
  registry.emplace<ColourComponent>(r, colour);
  registry.emplace<PositionIntComponent>(r, x * GRID_SIZE, y * GRID_SIZE);
  registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  registry.emplace<TextureComponent>(r, tex_unit_custom_spaceships);
  registry.emplace<SpriteTagComponent>(r, "SHIP_0");
  // physics
  registry.emplace<CollidableComponent>(r, static_cast<uint32_t>(GameCollisionLayer::ACTOR_PLAYER));
  registry.emplace<PhysicsSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  registry.emplace<VelocityComponent>(r, 0.0f, 0.0f);
  // input
  registry.emplace<PlayerComponent>(r, 0);
  // PlayerInputComponent pic;
  // pic.use_keyboard = true;
  // registry.emplace<PlayerInputComponent>(r, pic);
  // gameplay
  registry.emplace<GridPositionComponent>(r, x, y);
  registry.emplace<HealthComponent>(r);
};

} // namespace game2d