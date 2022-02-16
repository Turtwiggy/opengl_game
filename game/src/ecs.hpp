#pragma once

#include <array>
#include <bitset>
#include <cassert>
#include <memory>
#include <queue>
#include <set>
#include <type_traits>
#include <unordered_map>

namespace game2d {

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;

//
// Entities...
// ( i.e. have a list of components )
//

// The EntityManager distributes entity IDs,
// keeping a record of which IDs are in use,
// and which are not.
class EntityManager
{
public:
  EntityManager();
  [[nodiscard]] Entity create_entity();
  void destroy_entity(const Entity entity);
  void set_signature(const Entity entity, const Signature signature);
  [[nodiscard]] Signature get_signature(const Entity entity);

public:
  std::queue<Entity> available_entities;
  std::array<Signature, MAX_ENTITIES> signatures{};
  uint32_t living_entities = 0;
};

//
// Components...
// ( i.e. data, no functions )
//

class IComponentArray
{
public:
  virtual ~IComponentArray() = default;
  virtual void entity_destroyed(const Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
  void insert_data(const Entity entity, const T component)
  {
    assert(entity_to_index.find(entity) == entity_to_index.end() &&
           "Component added to the same enitty more than once.");

    // put new entry at end, and update the maps
    size_t new_index = valid_entries_in_array;
    entity_to_index[entity] = new_index;
    index_to_entity[new_index] = entity;
    components[new_index] = component;
    ++valid_entries_in_array;
  };

  void remove_data(const Entity entity)
  {
    assert(entity_to_index.find(entity) != entity_to_index.end() && "Removing non-existent component.");

    // copy element at end into deleted elements place
    size_t index_of_removed_entity = entity_to_index[entity];
    size_t index_of_last_element = valid_entries_in_array - 1;
    components[index_of_removed_entity] = components[index_of_last_element];

    // update map to point to moved spot
    Entity entity_of_last_element = index_to_entity[index_of_last_element];
    entity_to_index[entity_of_last_element] = index_of_removed_entity;
    index_to_entity[index_of_removed_entity] = entity_of_last_element;

    entity_to_index.erase(entity);
    index_to_entity.erase(index_of_last_element);

    --valid_entries_in_array;
  };

  T& get_data(const Entity entity)
  {
    assert(entity_to_index.find(entity) != entity_to_index.end() && "Retrieving non-existent component.");

    // return a reference to the component
    return components[entity_to_index[entity]];
  };

  void entity_destroyed(Entity entity) override
  {
    if (entity_to_index.find(entity) != entity_to_index.end()) {
      // Remove the entity's component if it existed
      remove_data(entity);
    }
  };

public:
  std::array<T, MAX_ENTITIES> components;
  std::unordered_map<Entity, size_t> entity_to_index;
  std::unordered_map<Entity, size_t> index_to_entity;
  size_t valid_entries_in_array = 0;
};

class ComponentManager
{
public:
  template<typename T>
  void register_component()
  {
    const char* type_name = typeid(T).name();

    assert(component_types.find(type_name) == component_types.end() && "Registering component type more than once.");

    // add this component to the componet type map
    component_types.insert({ type_name, next_component_type });

    // create component array pointer and add it to the component
    component_arrays.insert({ type_name, std::make_shared<ComponentArray<T>>() });

    // increment the value so that the next registered component value is different
    ++next_component_type;
  }

  template<typename T>
  [[nodiscard]] ComponentType get_component_type()
  {
    const char* type_name = typeid(T).name();

    assert(component_types.find(type_name) != component_types.end() && "Component not registered before use.");

    return component_types[type_name];
  }

  template<typename T>
  void add_component(const Entity entity, const T component)
  {
    // Add a component to the array for an entity
    get_component_array<T>()->insert_data(entity, component);
  }

  template<typename T>
  void remove_component(const Entity entity)
  {
    // Remove a component from the array for an entity
    get_component_array<T>()->remove_data(entity);
  }

  template<typename T>
  [[nodiscard]] T& get_component(const Entity entity)
  {
    // Get a reference to a component from the array for an entity
    return get_component_array<T>()->get_data(entity);
  }

  void entity_destroyed(const Entity entity)
  {
    // Notify each component array that an entity has been destroyed
    // If it has a component for that entity, it will remove it
    for (auto const& pair : component_arrays) {
      auto const& component = pair.second;
      component->entity_destroyed(entity);
    }
  }

private:
  // helper function to get the statically casted pointer to the ComponentArray of type T.
  template<typename T>
  std::shared_ptr<ComponentArray<T>> get_component_array()
  {
    const char* type_name = typeid(T).name();

    assert(component_types.find(type_name) != component_types.end() && "Component not registered before use.");

    return std::static_pointer_cast<ComponentArray<T>>(component_arrays[type_name]);
  };

private:
  // map from type string pointer to a component type
  std::unordered_map<const char*, ComponentType> component_types;
  // map from the type string pointer to a component array
  std::unordered_map<const char*, std::shared_ptr<IComponentArray>> component_arrays;
  // the component type to be assigned to hte next registered component
  ComponentType next_component_type{};
};

//
// System...
// ( i.e. functions, no data )
//

class System
{
public:
  std::set<Entity> entities;
};

class SystemManager
{
public:
  template<typename T>
  [[nodiscard]] std::shared_ptr<T> register_system()
  {
    const char* type_name = typeid(T).name();

    assert(systems.find(type_name) == systems.end() && "Registering system more than once.");

    // Create a pointer to the system and return it so it can be used externally
    std::shared_ptr<T> system = std::make_shared<T>();

    // note: if an error here, make sure inheritance is public
    systems.insert({ type_name, system });

    return system;
  }

  template<typename T>
  void set_signature(const Signature& signature)
  {
    const char* type_name = typeid(T).name();

    assert(systems.find(type_name) != systems.end() && "System used before registered.");

    // Set the signature for this system
    signatures.insert({ type_name, signature });
  }

  void entity_destroyed(const Entity entity)
  {
    // Erase a destroyed entity from all system lists
    // mEntities is a set so no check needed
    for (auto const& pair : systems) {
      auto const& system = pair.second;
      system->entities.erase(entity);
    }
  }

  void entity_signature_changed(Entity entity, Signature entitySignature)
  {
    // Notify each system that an entity's signature changed
    for (auto const& pair : systems) {
      auto const& type = pair.first;
      auto const& system = pair.second;
      auto const& system_signature = signatures[type];

      // Entity signature matches system signature - insert into set
      if ((entitySignature & system_signature) == system_signature) {
        system->entities.insert(entity);
      }
      // Entity signature does not match system signature - erase from set
      else {
        system->entities.erase(entity);
      }
    }
  }

private:
  // Map from system type string pointer to a signature
  std::unordered_map<const char*, Signature> signatures{};
  // Map from system type string pointer to a system pointer
  std::unordered_map<const char*, std::shared_ptr<System>> systems{};
};

//
// assemble ecs...
//

class Coordinator
{
public:
  Coordinator()
  {
    component_manager = std::make_unique<ComponentManager>();
    entity_manager = std::make_unique<EntityManager>();
    system_manager = std::make_unique<SystemManager>();
  };

  //
  // Entity methods
  //

  Entity create_entity() { return entity_manager->create_entity(); }

  void destroy_entity(const Entity entity)
  {
    entity_manager->destroy_entity(entity);
    component_manager->entity_destroyed(entity);
    system_manager->entity_destroyed(entity);
  };

  //
  // Component methods
  //

  template<typename T>
  void register_component()
  {
    component_manager->register_component<T>();
  }

  template<typename T>
  void add_component(const Entity entity, const T component)
  {
    component_manager->add_component<T>(entity, component);

    auto signature = entity_manager->get_signature(entity);
    signature.set(component_manager->get_component_type<T>(), true);
    entity_manager->set_signature(entity, signature);

    system_manager->entity_signature_changed(entity, signature);
  }

  template<typename T>
  void remove_component(const Entity entity)
  {
    component_manager->remove_component<T>(entity);

    auto signature = entity_manager->get_signature(entity);
    signature.set(component_manager->get_component_type<T>(), false);
    entity_manager->set_signature(entity, signature);

    system_manager->entity_signature_changed(entity, signature);
  }

  template<typename T>
  T& get_component(const Entity entity) const
  {
    return component_manager->get_component<T>(entity);
  }

  template<typename T>
  ComponentType get_component_type() const
  {
    return component_manager->get_component_type<T>();
  }

  //
  // System methods...
  //

  template<typename T>
  std::shared_ptr<T> register_system()
  {
    return system_manager->register_system<T>();
  }

  template<typename T>
  void set_system_signature(const Signature signature)
  {
    system_manager->set_signature<T>(signature);
  }

private:
  std::unique_ptr<ComponentManager> component_manager;
  std::unique_ptr<EntityManager> entity_manager;
  std::unique_ptr<SystemManager> system_manager;
};

} // namespace game2d