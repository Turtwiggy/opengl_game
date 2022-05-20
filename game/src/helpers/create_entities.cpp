#include "create_entities.hpp"

// my libs
#include "components/app.hpp"
#include "components/cursor.hpp"
#include "components/selectable.hpp"
#include "helpers/physics_layers.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/sprites/components.hpp"

// std libs
#include <string>

namespace game2d {

entt::entity
create_renderable(entt::registry& r, const std::string& name, const glm::vec4& colour)
{
  const int GRID_SIZE = 16; // temp
  entt::entity e = r.create();
  r.emplace<TagComponent>(e, name);
  // rendering
  r.emplace<PositionIntComponent>(e);
  r.emplace<RenderSizeComponent>(e, GRID_SIZE, GRID_SIZE);
  r.emplace<ColourComponent>(e, colour);
  r.emplace<SpriteTagComponent>(e, "EMPTY");
  r.emplace<TextureComponent>(e, tex_unit_kenny_nl);
  return e;
}

void
create_cursor(entt::registry& registry)
{
  const auto colours = registry.ctx<SINGLETON_ColoursComponent>();

  CursorComponent c;
  c.line_u = create_renderable(registry, std::string("cursor_line_u"), colours.red);
  c.line_d = create_renderable(registry, std::string("cursor_line_d"), colours.red);
  c.line_l = create_renderable(registry, std::string("cursor_line_l"), colours.red);
  c.line_r = create_renderable(registry, std::string("cursor_line_r"), colours.red);
  c.backdrop = create_renderable(registry, std::string("cursor_backdrop"), colours.backdrop_red);

  entt::entity e = registry.create();
  registry.emplace<TagComponent>(e, std::string("cursor_parent"));
  registry.emplace<CursorComponent>(e, c);
};

void
create_player(entt::registry& registry,
              int x,
              int y,
              const std::string& sprite,
              const glm::vec4& start_colour,
              const glm::vec4& highlight_colour)
{
  const int GRID_SIZE = 16;

  entt::entity r = registry.create();
  registry.emplace<TagComponent>(r, "player");

  // rendering
  registry.emplace<ColourComponent>(r, start_colour);
  registry.emplace<PositionIntComponent>(r, x * GRID_SIZE, y * GRID_SIZE);
  registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  registry.emplace<SpriteTagComponent>(r, sprite);
  registry.emplace<TextureComponent>(r, tex_unit_custom_spaceships);
  // physics
  registry.emplace<CollidableComponent>(r, static_cast<uint32_t>(GameCollisionLayer::ACTOR_PLAYER));
  registry.emplace<PhysicsSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  registry.emplace<VelocityComponent>(r, 0.0f, 0.0f);
  // gameplay
  registry.emplace<SelectableComponent>(r, false);
  registry.emplace<HighlightComponent>(r, start_colour, highlight_colour);
  // input
  // PlayerInputComponent pic;
  // pic.use_keyboard = true;
  // registry.emplace<PlayerInputComponent>(r, pic);
  // gameplay
};

} // namespace game2d