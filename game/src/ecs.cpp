#include "ecs.hpp"

namespace game2d {

//
// EntityManager
//

EntityManager::EntityManager()
{
  for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
    available_entities.push(entity);
  }
};

Entity
EntityManager::create_entity()
{
  assert(living_entities < MAX_ENTITIES && "Too many entities.");

  Entity id = available_entities.front();
  available_entities.pop();
  ++living_entities;

  return id;
};

void
EntityManager::destroy_entity(const Entity e)
{
  assert(e < MAX_ENTITIES && "Entity out of range.");

  // invalidate the destroyed entity signature
  signatures[e].reset();

  // put the destroyed id at the back of the queue
  available_entities.push(e);
  --living_entities;
};

void
EntityManager::set_signature(const Entity e, const Signature s)
{
  assert(e < MAX_ENTITIES && "Entity out of range.");

  // put the entity signature in to the array
  signatures[e] = s;
};

Signature
EntityManager::get_signature(const Entity e)
{
  assert(e < MAX_ENTITIES && "Entity out of range.");

  return signatures[e];
};

//
// ComponentArray
//

// ...

//
// ComponentManager
//

// ...

}; // namespace game2d