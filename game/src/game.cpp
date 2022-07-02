// your header
#include "game.hpp"

// components
#include "modules/camera/components.hpp"
#include "modules/events/components.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/ui_hierarchy/components.hpp"
#include "modules/ui_profiler/components.hpp"

// system modules
#include "modules/camera/system.hpp"
#include "modules/events/system.hpp"
#include "modules/physics/process_actor_actor.hpp"
#include "modules/physics/process_move_objects.hpp"
#include "modules/renderer/system.hpp"
#include "modules/sprites/system.hpp"
#include "modules/ui_game/system.hpp"
#include "modules/ui_hierarchy/system.hpp"
#include "modules/ui_physics/system.hpp"
#include "modules/ui_profiler/system.hpp"

// game helpers
#include "create_entities.hpp"
#include "modules/camera/helpers.hpp"
#include "modules/events/helpers/keyboard.hpp"

// game modules
#include "components/app.hpp"
#include "components/objectives.hpp"
#include "components/selectable.hpp"
#include "components/units.hpp"
#include "game_modules/turn_system/components.hpp"
#include "game_modules/turn_system/system.hpp"
#include "modules/ui_hierarchy/components.hpp"
#include "systems/animation_set_by_velocity.hpp"
#include "systems/cursor.hpp"
#include "systems/objectives.hpp"
#include "systems/pathfinding.hpp"
#include "systems/select_objects.hpp"
#include "systems/select_objects_highlight.hpp"
#include "systems/select_objects_move.hpp"
#include "systems/unit_group_position_units.hpp"

// engine headers
#include "engine/app/io.hpp"
#include "engine/maths/maths.hpp"

// std lib
#include <glm/glm.hpp>
#include <string>

namespace game2d {

// game config
const static glm::ivec2 battlefield_xy{ 800, 800 };
const static int objective_size = 250;

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
  init_input_system(registry);
  init_ui_hierarchy_system(registry);

  const auto& colours = registry.ctx<SINGLETON_ColoursComponent>();
  auto& r = registry.ctx<SINGLETON_ResourceComponent>();
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();

  const auto viewport_wh = ri.viewport_size_render_at;
  const int camera_x = viewport_wh.x / 2 - battlefield_xy.x / 2;
  const int camera_y = viewport_wh.y / 2 - battlefield_xy.y / 2;
  create_camera(registry, camera_x, camera_y);
  create_cursor(registry);
  // create_debug_square(registry);

  // army 0
  {
    std::string name;

    int x = 100, y = 200, sx = 100, sy = 100;
    name = { "UNIT GROUP 0" };

    auto u1 = create_unit(registry, "unit 1", colours.player_unit);
    // auto u2 = create_unit(registry, "unit 2", colours.player_unit);
    // auto u3 = create_unit(registry, "unit 3", colours.player_unit);
    // auto u4 = create_unit(registry, "unit 4", colours.player_unit);
    // auto u5 = create_unit(registry, "unit 5", colours.player_unit);
    auto e = create_unit_group(registry, x, y, sx, sy, name, colours.cyan, colours.dblue);
    auto& u = registry.get<UnitGroupComponent>(e).units;
    u.push_back(u1);
    // u.push_back(u2);
    // u.push_back(u3);
    // u.push_back(u4);
    // u.push_back(u5);
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
  init_sprite_system(registry);
  init_render_system(registry, screen_wh);
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
    auto camera_position = ri.viewport_size_current / 2 - battlefield_xy / 2;
    transform.position.x = camera_position.x;
    transform.position.y = camera_position.y;
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

    if (!gp.paused) {
      // ... systems that always update
      {
        update_cursor_system(registry);
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
