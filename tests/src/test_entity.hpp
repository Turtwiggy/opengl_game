#pragma once

// header
#include "ecs.hpp"

// other libs
#include <gtest/gtest.h>

//
// EntityManager...
//

TEST(TestSuite, TestCreateEntity)
{
  // setup
  game2d::EntityManager em;

  // act
  auto entity = em.create_entity();

  // assert
  ASSERT_EQ(em.living_entities, 1);
  ASSERT_EQ(em.available_entities.size(), game2d::MAX_ENTITIES - 1);
}

TEST(TestSuite, TestDestroyEntity)
{
  // setup
  game2d::EntityManager em;
  auto entity = em.create_entity();

  // act
  em.destroy_entity(entity);

  // assert
  ASSERT_EQ(em.living_entities, 0);                              // no living entities
  ASSERT_EQ(em.signatures[entity].none(), true);                 // should be all 0's
  ASSERT_EQ(em.available_entities.size(), game2d::MAX_ENTITIES); // no entities
}

//
// ComponentArray...
//

TEST(TestSuite, TestComponentArray)
{
  // setup
  game2d::EntityManager em;
  auto entity_0 = em.create_entity();
  auto entity_1 = em.create_entity();
  auto entity_2 = em.create_entity();

  game2d::ComponentArray<int> array;
  array.insert_data(entity_0, 5);
  array.insert_data(entity_1, 6);
  array.insert_data(entity_2, 7);
  array.remove_data(entity_1); // destroy comp attached to entity

  // act
  auto val_0 = array.get_data(entity_0);
  auto val_2 = array.get_data(entity_2);

  // assert
  ASSERT_EQ(val_0, 5);
  ASSERT_EQ(val_2, 7);
}

//
// ComponentManager...
//

struct FakeGravity
{
  float value = 9.8f;
};

struct FakeTransform
{
  float x = 0.0f;
  float y = 1.0f;
  float z = 2.0f;
};

class PhysicsSystem : public game2d::System
{
public:
  inline void update(game2d::Coordinator& registry, float dt)
  {
    for (const auto& entity : entities) {
      auto& gravity = registry.get_component<FakeGravity>(entity);
      auto& transform = registry.get_component<FakeTransform>(entity);

      gravity.value = 100.0f;
    }
  }
};

TEST(TestSuite, TestComponentManager)
{
  // setup
  game2d::Coordinator registry;

  // register all components
  registry.register_component<FakeGravity>();
  registry.register_component<FakeTransform>();

  // register all systems
  auto physics_system = registry.register_system<PhysicsSystem>();
  {
    game2d::Signature signature;
    signature.set(registry.get_component_type<FakeGravity>());
    signature.set(registry.get_component_type<FakeTransform>());
    registry.set_system_signature<PhysicsSystem>(signature);
  }

  // create some entities with components
  auto e1 = registry.create_entity();
  registry.add_component<FakeGravity>(e1, FakeGravity());
  registry.add_component<FakeTransform>(e1, FakeTransform());
  auto e2 = registry.create_entity();
  auto e3 = registry.create_entity();

  // act
  physics_system->update(registry, 0.0f);

  // assert
  auto component = registry.get_component<FakeGravity>(e1);
  ASSERT_EQ(component.value, 100.0f);
}