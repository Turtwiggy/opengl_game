// your header
#include "game.hpp"

// components
#include "modules/camera/components.hpp"
#include "modules/events/components.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/ui_hierarchy/components.hpp"
#include "modules/ui_profiler/components.hpp"

// systems
#include "modules/audio/system.hpp"
#include "modules/camera/system.hpp"
#include "modules/events/system.hpp"
#include "modules/renderer/system.hpp"
#include "modules/sprites/system.hpp"
#include "modules/ui_audio/system.hpp"
#include "modules/ui_game/system.hpp"
#include "modules/ui_hierarchy/system.hpp"
#include "modules/ui_physics/system.hpp"
#include "modules/ui_profiler/system.hpp"

// helpers
#include "create_entities.hpp"
#include "modules/camera/helpers.hpp"
#include "modules/events/helpers/keyboard.hpp"
#include "modules/physics/process_actor_actor.hpp"
#include "modules/physics/process_move_objects.hpp"

// resources?
#include "resources/colour.hpp"
#include "resources/textures.hpp"

// game modules
#include "game_modules/components/app.hpp"
#include "game_modules/components/objectives.hpp"
#include "game_modules/components/selectable.hpp"
#include "game_modules/components/units.hpp"
#include "game_modules/systems/animated_cursor_click.hpp"
#include "game_modules/systems/animation_set_by_velocity.hpp"
#include "game_modules/systems/cursor.hpp"
#include "game_modules/systems/objectives.hpp"
#include "game_modules/systems/pathfinding.hpp"
#include "game_modules/systems/select_objects.hpp"
#include "game_modules/systems/select_objects_highlight.hpp"
#include "game_modules/systems/select_objects_move.hpp"
#include "game_modules/systems/unit_group_position_units.hpp"
#include "game_modules/turn_system/components.hpp"
#include "game_modules/turn_system/system.hpp"

// engine headers
// #include "engine/app/io.hpp"
// #include "engine/maths/maths.hpp"

// other lib
#include <glm/glm.hpp>

// std lib
#include <string>

namespace game2d {

void
init_game_state(entt::registry& registry, engine::Application& app)
{
  registry.each([&registry](auto entity) { registry.destroy(entity); });
  registry.set<SINGLETON_HierarchyComponent>(SINGLETON_HierarchyComponent());
  registry.set<SINGLETON_PhysicsComponent>(SINGLETON_PhysicsComponent());
  registry.set<SINGLETON_ResourceComponent>(SINGLETON_ResourceComponent());
  registry.set<SINGLETON_GamePausedComponent>(SINGLETON_GamePausedComponent());
  registry.set<SINGLETON_ColoursComponent>(SINGLETON_ColoursComponent());
  registry.set<SINGLETON_TurnComponent>(SINGLETON_TurnComponent());
  init_ui_hierarchy_system(registry);

  const auto& colours = registry.ctx<SINGLETON_ColoursComponent>();
  auto& r = registry.ctx<SINGLETON_ResourceComponent>();
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();

  create_camera(registry, 0, 0);
  create_cursor(registry);
  // create_debug_square(registry);

  // army 0
  {
    std::string name;

    int x = 100, y = 200, sx = 100, sy = 100;
    name = { "UNIT GROUP 0" };

    auto u1 = create_unit(registry, "unit 1", colours.player_unit);
    auto u2 = create_unit(registry, "unit 2", colours.player_unit);
    auto u3 = create_unit(registry, "unit 3", colours.player_unit);
    auto u4 = create_unit(registry, "unit 4", colours.player_unit);
    auto u5 = create_unit(registry, "unit 5", colours.player_unit);
    auto e = create_unit_group(registry, x, y, sx, sy, name, colours.cyan, colours.dblue);
    auto& u = registry.get<UnitGroupComponent>(e).units;
    u.insert(u.end(), { u1, u2, u3, u4, u5 });
  }

  // // objectives
  // {
  //   std::string sprite{ "EMPTY" };
  //   // tl
  //   int x = objective_size / 2.0f, y = objective_size / 2.0f, sx = objective_size, sy = objective_size;
  //   create_objective(registry, x, y, sx, sy, sprite)
  // }
};

} // namespace game2d

void
game2d::init(entt::registry& registry, engine::Application& app, glm::ivec2 screen_wh)
{
  // init once only
  registry.set<SINGLETON_Textures>();
  init_sprite_system(registry);
  init_render_system(registry, screen_wh);
  init_input_system(registry);
  init_audio_system(registry);
  registry.set<Profiler>(Profiler());
  // open_controllers(); // enable controllers

  init_game_state(registry, app);
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
      // move objects, checking collisions along way
      update_move_objects_system(registry, app, fixed_dt);
      // generate all collisions between actor-actor objects
      update_actor_actor_system(registry, app);
      // process actor-actor collisions
      // update_actor_actor_collision_system(registry, app, fixed_dt);
    }
  }
  Uint64 end_physics = SDL_GetPerformanceCounter();
  p.physics_elapsed_ms = (end_physics - start_physics) / float(SDL_GetPerformanceFrequency()) * 1000.0f;
}

void
game2d::update(entt::registry& registry, engine::Application& app, float dt)
{
  auto& p = registry.ctx<Profiler>();
  auto& gp = registry.ctx<SINGLETON_GamePausedComponent>();
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();

  if (ri.viewport_size_render_at != ri.viewport_size_current) {
    // viewport was updated, recenter the camera on the battlefield
    const auto& main_camera = get_main_camera(registry);
    auto& transform = registry.get<TransformComponent>(main_camera);
    transform.position = { 0, 0, 0 };
  }

  // game logic
  Uint64 start_game_tick = SDL_GetPerformanceCounter();
  {
    update_input_system(registry, app);

    const auto& input = registry.ctx<SINGLETON_InputComponent>();
    if (ri.viewport_process_events) {
      if (get_key_down(input, SDL_SCANCODE_P))
        gp.paused = !gp.paused;
      if (get_key_down(input, SDL_SCANCODE_R))
        init_game_state(registry, app);
      if (get_key_down(input, SDL_SCANCODE_F))
        app.window->toggle_fullscreen();
    }
    if (get_key_down(input, SDL_SCANCODE_ESCAPE))
      app.shutdown();

    update_audio_system(registry);

    if (!gp.paused) {
      // ... systems that always update (when not paused)
      {
        update_cursor_system(registry);
        update_animated_cursor_click_system(registry);
        update_objectives_system(registry);
        update_pathfinding_system(registry);
        update_animation_set_by_velocity_system(registry);
        update_select_objects_system(registry);
        update_select_objects_highlight_system(registry);
        update_select_objects_move_system(registry);
        update_unit_group_position_units_system(registry);
        update_turn_system(registry);
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
    update_ui_audio_system(registry);
    update_ui_game_system(registry);
    update_ui_physics_system(registry);
    update_ui_hierarchy_system(registry);
    update_ui_profiler_system(registry);
  };

  // end frame
  {
    end_frame_render_system(registry);
  }
};
