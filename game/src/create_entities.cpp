#include "create_entities.hpp"

// my libs
#include "components/animation.hpp"
#include "components/app.hpp"
#include "components/cursor.hpp"
#include "components/debug.hpp"
#include "components/objectives.hpp"
#include "components/pathfinding.hpp"
#include "components/selectable.hpp"
#include "components/units.hpp"
#include "modules/camera/components.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/sprites/components.hpp"
#include "modules/sprites/helpers.hpp"
#include "modules/ui_hierarchy/components.hpp"
#include "textures.hpp"

// std libs
#include <string>

namespace game2d {

const int SPRITE_SIZE = 32; // 16 is native

void
create_empty(entt::registry& r)
{
  auto& h = r.ctx<SINGLETON_HierarchyComponent>();
  auto& h_root = r.get<EntityHierarchyComponent>(h.root_node);

  entt::entity e = r.create();
  h_root.children.push_back(e);
  r.emplace<TagComponent>(e, "empty-entity");
  r.emplace<EntityHierarchyComponent>(e, h.root_node);
}

void
create_camera(entt::registry& r, int x, int y)
{
  auto& h = r.ctx<SINGLETON_HierarchyComponent>();

  entt::entity e = r.create();
  r.emplace<TagComponent>(e, "camera");
  r.emplace<EntityHierarchyComponent>(e, h.root_node);

  // rendering
  TransformComponent transform;
  transform.position.x = x;
  transform.position.y = y;
  r.emplace<TransformComponent>(e, transform);
  r.emplace<CameraComponent>(e);
}

entt::entity
create_renderable(entt::registry& r,
                  const entt::entity& parent,
                  const std::string& name,
                  const engine::SRGBColour& colour)
{
  const auto& slots = r.ctx<SINGLETON_Textures>();

  entt::entity e = r.create();
  r.emplace<TagComponent>(e, name);

  // hierarchy
  auto& parent_hierarchy = r.get<EntityHierarchyComponent>(parent);
  parent_hierarchy.children.push_back(e);
  r.emplace<EntityHierarchyComponent>(e, parent);

  TransformComponent transform;
  transform.scale.x = SPRITE_SIZE;
  transform.scale.y = SPRITE_SIZE;
  r.emplace<TransformComponent>(e, transform);

  SpriteComponent sprite;
  sprite.colour = engine::SRGBToLinear(colour);
  sprite.tex_unit = slots.tex_unit_kenny;
  sprite.x = 0;
  sprite.y = 0;
  r.emplace<SpriteComponent>(e, sprite);

  return e;
}

void
create_cursor(entt::registry& r)
{
  const auto& colours = r.ctx<SINGLETON_ColoursComponent>();
  auto& h = r.ctx<SINGLETON_HierarchyComponent>();
  auto& h_root = r.get<EntityHierarchyComponent>(h.root_node);

  entt::entity e = r.create();
  r.emplace<TagComponent>(e, std::string("cursor_parent"));

  // hierarchy cursor-root
  r.emplace<EntityHierarchyComponent>(e, h.root_node);
  h_root.children.push_back(e); // cursor parent

  // cursor
  CursorComponent c;
  c.line_u = create_renderable(r, e, "line_u", colours.red);
  c.line_d = create_renderable(r, e, "line_d", colours.red);
  c.line_l = create_renderable(r, e, "line_l", colours.red);
  c.line_r = create_renderable(r, e, "line_r", colours.red);
  c.backdrop = create_renderable(r, e, "backdrop", colours.backdrop_red);
  r.emplace<CursorComponent>(e, c);

  // physics
  r.emplace<PhysicsActorComponent>(c.backdrop, GameCollisionLayer::ACTOR_CURSOR);
  r.emplace<PhysicsSizeComponent>(c.backdrop, SPRITE_SIZE, SPRITE_SIZE);
  r.emplace<VelocityComponent>(c.backdrop);
};

entt::entity
create_unit_group(entt::registry& r,
                  int x,
                  int y,
                  int size_x,
                  int size_y,
                  const std::string& name,
                  const engine::SRGBColour& start_colour,
                  const engine::SRGBColour& highlight_colour)
{
  auto& h = r.ctx<SINGLETON_HierarchyComponent>();
  auto& h_root = r.get<EntityHierarchyComponent>(h.root_node);
  const auto& colours = r.ctx<SINGLETON_ColoursComponent>();

  entt::entity e = r.create();
  h_root.children.push_back(e);
  r.emplace<TagComponent>(e, name);
  r.emplace<EntityHierarchyComponent>(e, h.root_node);

  // rendering
  TransformComponent transform;
  transform.position.x = x;
  transform.position.y = y;
  transform.scale.x = size_x / 2;
  transform.scale.y = size_y / 2;
  transform.rotation.z = 0.0f;
  r.emplace<TransformComponent>(e, transform);
  SpriteComponent sprite;
  sprite.colour = engine::SRGBToLinear(colours.red);
  sprite.x = 0;
  sprite.y = 0;
  r.emplace<SpriteComponent>(e, sprite);
  // physics
  r.emplace<PhysicsActorComponent>(e, GameCollisionLayer::ACTOR_UNIT_GROUP);
  r.emplace<PhysicsSizeComponent>(e, size_x, size_y);
  r.emplace<VelocityComponent>(e);
  // gameplay
  r.emplace<SelectableComponent>(e);
  // r.emplace<HighlightComponent>(e, start_colour, highlight_colour);
  r.emplace<DestinationComponent>(e);
  r.emplace<UnitGroupComponent>(e);

  return e;
};

entt::entity
create_unit(entt::registry& registry, const std::string& name, const engine::SRGBColour& colour)
{
  const auto& colours = registry.ctx<SINGLETON_ColoursComponent>();
  const auto& slots = registry.ctx<SINGLETON_Textures>();

  // entity
  entt::entity e = registry.create();
  registry.emplace<TagComponent>(e, name);
  // hierarchy
  auto& h = registry.ctx<SINGLETON_HierarchyComponent>();
  auto& h_root = registry.get<EntityHierarchyComponent>(h.root_node);
  h_root.children.push_back(e);
  registry.emplace<EntityHierarchyComponent>(e, h.root_node);
  // auto& parent_hierarchy = registry.get<EntityHierarchyComponent>(parent);
  // parent_hierarchy.children.push_back(e);
  // registry.emplace<EntityHierarchyComponent>(e, parent);
  // rendering
  TransformComponent transform;
  transform.scale.x = SPRITE_SIZE;
  transform.scale.y = SPRITE_SIZE;
  registry.emplace<TransformComponent>(e, transform);
  SpriteComponent sprite;
  sprite.colour = engine::SRGBToLinear(colour);
  sprite.tex_unit = slots.tex_unit_sprout;
  sprite.x = 1;
  sprite.y = 0;
  registry.emplace<SpriteComponent>(e, sprite);
  // animation
  registry.emplace<SpriteAnimationComponent>(e);
  registry.emplace<AnimationSetByVelocityComponent>(e);
  // physics
  registry.emplace<PhysicsActorComponent>(e, GameCollisionLayer::ACTOR_U);
  registry.emplace<PhysicsSizeComponent>(e, SPRITE_SIZE, SPRITE_SIZE);
  registry.emplace<VelocityComponent>(e);
  // gameplay
  registry.emplace<DestinationComponent>(e);

  return e;
}

void
create_debug_square(entt::registry& registry)
{
  auto& h = registry.ctx<SINGLETON_HierarchyComponent>();

  const auto& colours = registry.ctx<SINGLETON_ColoursComponent>();
  auto e = create_renderable(registry, h.root_node, std::string("debug_square"), colours.white);
  // debug square
  registry.emplace<DebugSquareComponent>(e);
}

} // namespace game2d