// your header
#include "game.hpp"

// systems&components&helpers
#include "create_entities.hpp"
#include "modules/audio/system.hpp"
#include "modules/camera/components.hpp"
#include "modules/camera/helpers.hpp"
#include "modules/camera/system.hpp"
#include "modules/events/components.hpp"
#include "modules/events/helpers/keyboard.hpp"
#include "modules/events/system.hpp"
#include "modules/lifecycle/components.hpp"
#include "modules/lifecycle/system.hpp"
#include "modules/physics/components.hpp"
#include "modules/physics/process_actor_actor.hpp"
#include "modules/physics/process_move_objects.hpp"
#include "modules/renderer/components.hpp"
#include "modules/renderer/system.hpp"
#include "modules/sprites/components.hpp"
#include "modules/sprites/system.hpp"
#include "modules/ui_hierarchy/components.hpp"
#include "modules/ui_hierarchy/system.hpp"
#include "modules/ui_physics/system.hpp"
#include "modules/ui_profiler/components.hpp"
#include "modules/ui_profiler/system.hpp"

// resources
#include "resources/audio.hpp"
#include "resources/colour.hpp"
#include "resources/textures.hpp"

// game systems
#include "game_modules/components/game.hpp"
#include "game_modules/systems/asteroid.hpp"
#include "game_modules/systems/player.hpp"
#include "game_modules/systems/turret.hpp"
#include "game_modules/systems/ui_highscore.hpp"
#include "game_modules/systems/ui_place_entity.hpp"

// other lib
#include <glm/glm.hpp>

// std lib
#include <string>

namespace game2d {

void
init_splash_screen(entt::registry& registry)
{
  registry.each([&registry](auto entity) { registry.destroy(entity); });
  registry.set<SINGLETON_PhysicsComponent>();
  registry.set<SINGLETON_GamePausedComponent>();
  registry.set<SINGLETON_GameOverComponent>();
  registry.set<SINGLETON_HierarchyComponent>();
  registry.set<SINGLETON_EntityBinComponent>();
  create_hierarchy_root_node(registry);
  create_camera(registry);
};

void
init_game_state(entt::registry& registry)
{
  registry.each([&registry](auto entity) { registry.destroy(entity); });
  registry.set<SINGLETON_PhysicsComponent>();
  registry.set<SINGLETON_GamePausedComponent>();
  registry.set<SINGLETON_GameOverComponent>();
  registry.set<SINGLETON_HierarchyComponent>();
  registry.set<SINGLETON_EntityBinComponent>();
  create_hierarchy_root_node(registry);
  create_camera(registry);

  auto& gs = registry.set<SINGLETON_AsteroidGameStateComponent>();
  auto player = create_player(registry);

  auto& player_transform = registry.get<TransformComponent>(player);
  player_transform.position.x = 600;
  player_transform.position.y = 400;
  auto& player_speed = registry.get<PlayerComponent>(player);
  player_speed.speed = 250.0f;

  for (int i = 0; i < gs.initial_asteroids; i++)
    auto asteroid = create_asteroid(registry);
};

} // namespace game2d

void
game2d::init(entt::registry& registry, glm::ivec2 screen_wh)
{
  // init once only
  registry.set<Profiler>();
  registry.set<SINGLETON_Textures>();
  registry.set<SINGLETON_ResourceComponent>();
  registry.set<SINGLETON_ColoursComponent>();
  init_sprite_system(registry);
  init_render_system(registry, screen_wh);
  init_input_system(registry);
  init_audio_system(registry);

  init_game_state(registry);
};

void
game2d::fixed_update(entt::registry& registry, engine::Application& app, float fixed_dt)
{
  const auto& gp = registry.ctx<SINGLETON_GamePausedComponent>();
  auto& p = registry.ctx<Profiler>();

  // physics
  Uint64 start_physics = SDL_GetPerformanceCounter();
  {
    if (!gp.paused) {

      update_lifecycle_system(registry, fixed_dt);
      // TODO: update hroot.children if entity is removed

      // move objects, checking collisions along way
      update_move_objects_system(registry, app, fixed_dt);

      // generate all collisions between actor-actor objects
      update_actor_actor_system(registry, app);
    }
  }
  Uint64 end_physics = SDL_GetPerformanceCounter();
  p.physics_elapsed_ms = (end_physics - start_physics) / float(SDL_GetPerformanceFrequency()) * 1000.0f;
}

void
game2d::update(entt::registry& registry, engine::Application& app, float dt)
{
  auto& p = registry.ctx<Profiler>();
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();

  // game logic
  Uint64 start_game_tick = SDL_GetPerformanceCounter();
  {
    update_input_system(registry, app);

    const auto& input = registry.ctx<SINGLETON_InputComponent>();
    if (ri.viewport_process_events) {
      {
        auto& gp = registry.ctx<SINGLETON_GamePausedComponent>();
        if (get_key_down(input, SDL_SCANCODE_P))
          gp.paused = !gp.paused;
      }
      if (get_key_down(input, SDL_SCANCODE_T))
        init_splash_screen(registry);
      if (get_key_down(input, SDL_SCANCODE_R))
        init_game_state(registry);
      if (get_key_down(input, SDL_SCANCODE_F))
        app.window->toggle_fullscreen();
    }
    {
      auto& go = registry.ctx<SINGLETON_GameOverComponent>();
      if (go.over)
        init_game_state(registry);
    }

    if (get_key_down(input, SDL_SCANCODE_ESCAPE))
      app.shutdown();

    update_audio_system(registry);

    auto& gp = registry.ctx<SINGLETON_GamePausedComponent>();
    if (!gp.paused) {
      // ... systems that always update (when not paused)
      {
        // update_cursor_system(registry);
        update_asteroid_system(registry);
        update_player_system(registry);
        update_turret_system(registry);
      }

      // ... systems that update if viewport is focused
      {
        if (ri.viewport_process_events) {
          update_camera_system(registry);
        }
      }
    }
  };
  Uint64 end_game_tick = SDL_GetPerformanceCounter();
  p.game_tick_elapsed_ms = (end_game_tick - start_game_tick) / float(SDL_GetPerformanceFrequency()) * 1000.0f;

  // rendering
  Uint64 start_render = SDL_GetPerformanceCounter();
  {
    update_sprite_system(registry, dt);
    update_render_system(registry); // put rendering on thread?
  };
  Uint64 end_render = SDL_GetPerformanceCounter();
  p.render_elapsed_ms = (end_render - start_render) / float(SDL_GetPerformanceFrequency()) * 1000.0f;

  // ui
  {
    // TODO: fix this
    bool is_release = false;
    if (!is_release) {
      // editor
      update_ui_physics_system(registry);
      update_ui_hierarchy_system(registry);
      update_ui_profiler_system(registry);
      update_ui_place_entity_system(registry);
    }
    update_ui_highscore_system(registry);
  };

  // end frame
  {
    end_frame_render_system(registry);
  }
};
