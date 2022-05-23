#include "create_entities.hpp"

// my libs
#include "components/app.hpp"
#include "components/cursor.hpp"
#include "components/pathfinding.hpp"
#include "components/selectable.hpp"
#include "helpers/physics_layers.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/sprites/components.hpp"

// std libs
#include <string>

namespace game2d {

const int SPRITE_SIZE = 32;

entt::entity
create_renderable(entt::registry& r, const std::string& name, const glm::vec4& colour)
{
  entt::entity e = r.create();
  r.emplace<TagComponent>(e, name);
  // rendering
  r.emplace<PositionIntComponent>(e);
  r.emplace<RenderSizeComponent>(e, SPRITE_SIZE, SPRITE_SIZE);
  r.emplace<ColourComponent>(e, colour);
  r.emplace<SpriteTagComponent>(e, "EMPTY");
  r.emplace<TextureComponent>(e, tex_unit_kenny_nl);
  return e;
}

void
create_cursor(entt::registry& r)
{
  const auto& colours = r.ctx<SINGLETON_ColoursComponent>();

  CursorComponent c;
  c.line_u = create_renderable(r, std::string("cursor_line_u"), colours.red);
  c.line_d = create_renderable(r, std::string("cursor_line_d"), colours.red);
  c.line_l = create_renderable(r, std::string("cursor_line_l"), colours.red);
  c.line_r = create_renderable(r, std::string("cursor_line_r"), colours.red);
  c.backdrop = create_renderable(r, std::string("cursor_backdrop"), colours.backdrop_red);

  entt::entity e = r.create();
  r.emplace<TagComponent>(e, std::string("cursor_parent"));
  r.emplace<CursorComponent>(e, c);
};

void
create_player(entt::registry& r,
              int x,
              int y,
              const std::string& sprite,
              const glm::vec4& start_colour,
              const glm::vec4& highlight_colour)
{
  entt::entity e = r.create();
  r.emplace<TagComponent>(e, "player");

  // rendering
  r.emplace<ColourComponent>(e, start_colour);
  r.emplace<PositionIntComponent>(e, x * SPRITE_SIZE, y * SPRITE_SIZE);
  r.emplace<RenderSizeComponent>(e, SPRITE_SIZE, SPRITE_SIZE);
  r.emplace<SpriteTagComponent>(e, sprite);
  r.emplace<TextureComponent>(e, tex_unit_custom_spaceships);
  // physics
  r.emplace<CollidableComponent>(e, static_cast<uint32_t>(GameCollisionLayer::ACTOR_PLAYER));
  r.emplace<PhysicsSizeComponent>(e, SPRITE_SIZE, SPRITE_SIZE);
  r.emplace<VelocityComponent>(e, 0.0f, 0.0f);
  // gameplay
  r.emplace<SelectableComponent>(e, false);
  r.emplace<HighlightComponent>(e, start_colour, highlight_colour);
  r.emplace<DestinationComponent>(e);

  // input
  // PlayerInputComponent pic;
  // pic.use_keyboard = true;
  // registry.emplace<PlayerInputComponent>(r, pic);
  // gameplay
};

} // namespace game2d