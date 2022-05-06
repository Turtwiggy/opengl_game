// your header
#include "game.hpp"

// components
// #include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/ui_hierarchy/components.hpp"
#include "modules/ui_profiler/components.hpp"

// systems
// #include "modules/physics/process_move_objects.hpp"
// #include "modules/physics/system.hpp"
// #include "modules/ui_physics/system.hpp"
#include "modules/renderer/system.hpp"
#include "modules/sprites/system.hpp"
#include "modules/ui_hierarchy/system.hpp"
#include "modules/ui_profiler/system.hpp"

// gameplay
#include "components/app.hpp"
#include "components/grid.hpp"
#include "helpers/create_entities.hpp"
#include "systems/cursor.hpp"
// #include "systems/collisions_actor_actor.hpp"
// #include "components/components.hpp"
// #include "systems/destroy_after_time.hpp"
// #include "systems/player_input.hpp"

// engine headers
#include "engine/app/io.hpp"
#include "engine/maths/maths.hpp"

namespace game2d {

void
init_game_state(entt::registry& registry, engine::Application& app)
{
  registry.each([&registry](auto entity) { registry.destroy(entity); });
  registry.set<SINGLETON_HierarchyComponent>(SINGLETON_HierarchyComponent());
  registry.set<SINGLETON_ResourceComponent>(SINGLETON_ResourceComponent());
  registry.set<SINGLETON_GamePausedComponent>(SINGLETON_GamePausedComponent());
  registry.set<SINGLETON_GridSizeComponent>(SINGLETON_GridSizeComponent());
  registry.set<SINGLETON_ColoursComponent>(SINGLETON_ColoursComponent());
  const auto colours = registry.ctx<SINGLETON_ColoursComponent>();

  create_cursor(registry); // Add a cursor, made of 4 lines
  // create_player(registry, 5, 10, colours.red);
  create_player(registry, 40, 15, colours.cyan);
};

} // namespace game2d

void
game2d::init(entt::registry& registry, engine::Application& app, glm::ivec2 screen_wh)
{
  // init once only
  init_render_system(registry, screen_wh);
  registry.set<Profiler>(Profiler());
  app.get_input().open_controllers(); // enable controllers

  init_game_state(registry, app); // could be deleted and re-init at any time
};

void
game2d::fixed_update(entt::registry& registry, engine::Application& app, float fixed_dt)
{
  Profiler& p = registry.ctx<Profiler>();
  SINGLETON_GamePausedComponent& gp = registry.ctx<SINGLETON_GamePausedComponent>();

  // physics
  Uint64 start_physics = SDL_GetPerformanceCounter();
  {
    // if (!gp.paused) {
    // move objects, checking collisions along way
    // update_move_objects_system(registry, app, fixed_dt);
    // generate all collisions between actor-actor objects
    // update_physics_system(registry, app, fixed_dt);
    // process actor-actor collisions
    // update_actor_actor_collision_system(registry, app, fixed_dt);
    // }
  }
  Uint64 end_physics = SDL_GetPerformanceCounter();
  p.physics_elapsed_ms = (end_physics - start_physics) / float(SDL_GetPerformanceFrequency()) * 1000.0f;
}

void
game2d::update(entt::registry& registry, engine::Application& app, float dt)
{
  Profiler& p = registry.ctx<Profiler>();
  SINGLETON_GamePausedComponent& gp = registry.ctx<SINGLETON_GamePausedComponent>();

  if (app.get_input().get_key_down(SDL_SCANCODE_P))
    gp.paused = !gp.paused;
  if (app.get_input().get_key_down(SDL_SCANCODE_R))
    init_game_state(registry, app);
  if (app.get_input().get_key_down(SDL_SCANCODE_F))
    app.get_window().toggle_fullscreen();
  if (app.get_input().get_key_down(SDL_SCANCODE_ESCAPE))
    app.shutdown();

  // game logic
  Uint64 start_game_tick = SDL_GetPerformanceCounter();
  {
    if (!gp.paused) {
      // ... systems that always update
      {
        update_cursor_system(registry, app);
      }

      // ... now systems that only update if viewport is focused
      {
        const auto& ri = registry.ctx<SINGLETON_RendererInfo>();
        if (ri.viewport_process_events) {
          // update_player_input_system(registry, app, dt);

          // last thing to do for gametick: destroy any entities
          // update_destroy_after_time_system(registry, app, dt);
        }
      }
    }
  };
  Uint64 end_game_tick = SDL_GetPerformanceCounter();
  p.game_tick_elapsed_ms = (end_game_tick - start_game_tick) / float(SDL_GetPerformanceFrequency()) * 1000.0f;

  // rendering
  Uint64 start_render = SDL_GetPerformanceCounter();
  {
    update_sprite_tag_system(registry, app);
    update_render_system(registry, app);
  };
  Uint64 end_render = SDL_GetPerformanceCounter();
  p.render_elapsed_ms = (end_render - start_render) / float(SDL_GetPerformanceFrequency()) * 1000.0f;

  // ui
  {
    // update_ui_physics_system(registry, app);
    update_ui_hierarchy_system(registry, app);
    update_ui_profiler_system(registry, app);
  };

  // end frame
  {
    end_frame_render_system(registry);
  }
};
