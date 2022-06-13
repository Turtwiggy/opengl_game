#include "create_entities.hpp"

// my libs
#include "components/app.hpp"
#include "components/cursor.hpp"
#include "components/debug.hpp"
#include "components/hierarchy.hpp"
#include "components/objectives.hpp"
#include "components/pathfinding.hpp"
#include "components/selectable.hpp"
#include "modules/camera/components.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/sprites/components.hpp"

// std libs
#include <string>

namespace game2d {

const int SPRITE_SIZE = 32;

void
create_camera(entt::registry& r, int x, int y)
{
  entt::entity e = r.create();
  r.emplace<TagComponent>(e, "camera");
  // rendering
  r.emplace<PositionIntComponent>(e, x, y);
  r.emplace<CameraComponent>(e);
}

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
  // physics
  r.emplace<PhysicsActorComponent>(c.backdrop, GameCollisionLayer::ACTOR_CURSOR);
  r.emplace<PhysicsSizeComponent>(c.backdrop, SPRITE_SIZE, SPRITE_SIZE);
  r.emplace<VelocityComponent>(c.backdrop, 0.0f, 0.0f);

  entt::entity e = r.create();
  r.emplace<TagComponent>(e, std::string("cursor_parent"));
  r.emplace<CursorComponent>(e, c);

  // update hierarchy view
  std::vector<entt::entity> children;
  children.push_back(c.line_u);
  children.push_back(c.line_d);
  children.push_back(c.line_l);
  children.push_back(c.line_r);
  children.push_back(c.backdrop);
  r.emplace<EntityHierarchyComponent>(e, children);
};

void
create_objective(entt::registry& r, int x, int y, int size_x, int size_y, const std::string& sprite)
{
  const auto colours = r.ctx<SINGLETON_ColoursComponent>();

  entt::entity e = r.create();
  r.emplace<TagComponent>(e, "objective");
  r.emplace<EntityHierarchyComponent>(e);

  // rendering
  r.emplace<ColourComponent>(e, colours.feint_white);
  r.emplace<PositionIntComponent>(e, x, y);
  r.emplace<RenderSizeComponent>(e, size_x, size_y);
  r.emplace<RenderAngleComponent>(e, 0.0f);
  r.emplace<SpriteTagComponent>(e, sprite);
  r.emplace<TextureComponent>(e, tex_unit_custom_spaceships);
  // physics
  r.emplace<PhysicsActorComponent>(e, GameCollisionLayer::ACTOR_OBJECTIVE);
  r.emplace<PhysicsSizeComponent>(e, size_x, size_y);
  r.emplace<VelocityComponent>(e, 0.0f, 0.0f);
  // gameplay
  r.emplace<ObjectiveComponent>(e);
  r.emplace<HighlightComponent>(e, colours.feint_white, colours.backdrop_red);
}

void
create_player(entt::registry& r,
              int x,
              int y,
              int size_x,
              int size_y,
              const std::string& name,
              const std::string& sprite,
              const glm::vec4& start_colour,
              const glm::vec4& highlight_colour)
{
  entt::entity e = r.create();
  r.emplace<TagComponent>(e, name);
  r.emplace<EntityHierarchyComponent>(e);

  // rendering
  r.emplace<ColourComponent>(e, start_colour);
  r.emplace<PositionIntComponent>(e, x, y);
  r.emplace<RenderSizeComponent>(e, size_x, size_y);
  r.emplace<RenderAngleComponent>(e, 0.0f);
  r.emplace<SpriteTagComponent>(e, sprite);
  r.emplace<TextureComponent>(e, tex_unit_custom_spaceships);
  // physics
  r.emplace<PhysicsActorComponent>(e, GameCollisionLayer::ACTOR_UNIT);
  r.emplace<PhysicsSizeComponent>(e, size_x, size_y);
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

void
create_debug_square(entt::registry& registry)
{
  const auto& colours = registry.ctx<SINGLETON_ColoursComponent>();
  auto e = create_renderable(registry, std::string("debug_square"), colours.white);
  // debug square
  registry.emplace<DebugSquareComponent>(e);
}

} // namespace game2d