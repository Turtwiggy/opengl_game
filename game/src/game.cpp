// your header
#include "game.hpp"

// components
#include "modules/camera/components.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/ui_hierarchy/components.hpp"
#include "modules/ui_profiler/components.hpp"

// systems
#include "modules/camera/system.hpp"
#include "modules/physics/process_actor_actor.hpp"
#include "modules/physics/process_move_objects.hpp"
#include "modules/renderer/system.hpp"
#include "modules/sprites/system.hpp"
#include "modules/ui_game/system.hpp"
#include "modules/ui_hierarchy/system.hpp"
#include "modules/ui_physics/system.hpp"
#include "modules/ui_profiler/system.hpp"

// gameplay
#include "components/app.hpp"
#include "components/objectives.hpp"
#include "components/selectable.hpp"
#include "components/units.hpp"
#include "create_entities.hpp"
#include "modules/ui_hierarchy/components.hpp"
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
const static int demo_unit_offset = 100;

void
init_game_state(entt::registry& registry, engine::Application& app)
{
  registry.each([&registry](auto entity) { registry.destroy(entity); });
  registry.set<SINGLETON_HierarchyComponent>(SINGLETON_HierarchyComponent());
  registry.set<SINGLETON_PhysicsComponent>(SINGLETON_PhysicsComponent());
  registry.set<SINGLETON_ResourceComponent>(SINGLETON_ResourceComponent());
  registry.set<SINGLETON_GamePausedComponent>(SINGLETON_GamePausedComponent());
  registry.set<SINGLETON_ColoursComponent>(SINGLETON_ColoursComponent());

  // create hierarchy root node
  auto& hi = registry.ctx<SINGLETON_HierarchyComponent>();
  hi.root = registry.create();
  registry.emplace<TagComponent>(hi.root, "root-node");
  registry.emplace<EntityHierarchyComponent>(hi.root, hi.root);

  const auto& colours = registry.ctx<SINGLETON_ColoursComponent>();
  auto& r = registry.ctx<SINGLETON_ResourceComponent>();
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();

  const auto viewport_wh = ri.viewport_size_render_at;
  const int camera_x = viewport_wh.x / 2 - battlefield_xy.x / 2;
  const int camera_y = viewport_wh.y / 2 - battlefield_xy.y / 2;
  create_camera(registry, camera_x, camera_y);
  create_cursor(registry);
  // create_debug_square(registry);

  // input
  // PlayerInputComponent pic;
  // pic.use_keyboard = true;
  // registry.emplace<PlayerInputComponent>(r, pic);

  // army 0
  {
    std::string sprite{ "EMPTY" };
    std::string name;

    int x = 100, y = 200, sx = 50, sy = 200;
    name = { "UNIT GROUP 0" };
    auto e = create_unit_group(registry, x, y, sx, sy, name, sprite, colours.backdrop_red, colours.red);
    auto& u = registry.get<UnitGroupComponent>(e).units;
    u.push_back(create_unit(registry, e, "unit 1", colours.cyan));
    u.push_back(create_unit(registry, e, "unit 2", colours.cyan));
    u.push_back(create_unit(registry, e, "unit 3", colours.cyan));
    u.push_back(create_unit(registry, e, "unit 4", colours.cyan));
    u.push_back(create_unit(registry, e, "unit 5", colours.cyan));

    x = 200, y = 400, sx = 50, sy = 200;
    name = { "UNIT GROUP 1" };
    e = create_unit_group(registry, x, y, sx, sy, name, sprite, colours.cyan, colours.dblue);

    x = 100, y = 600, sx = 50, sy = 200;
    name = { "UNIT GROUP 2" };
    e = create_unit_group(registry, x, y, sx, sy, name, sprite, colours.cyan, colours.dblue);
  }

  // army 1
  {
    std::string sprite{ "EMPTY" };
    std::string name;

    int x = battlefield_xy.x - 100, y = 200, sx = 50, sy = 200;
    name = { "UNIT GROUP 3" };
    auto e = create_unit_group(registry, x, y, sx, sy, name, sprite, colours.desat_red, colours.red);

    x = battlefield_xy.x - 200, y = 400, sx = 50, sy = 200;
    name = { "UNIT GROUP 4" };
    e = create_unit_group(registry, x, y, sx, sy, name, sprite, colours.desat_red, colours.red);

    x = battlefield_xy.x - 100, y = 600, sx = 50, sy = 200;
    name = { "UNIT GROUP 5" };
    e = create_unit_group(registry, x, y, sx, sy, name, sprite, colours.desat_red, colours.red);
  }

  // objectives
  {
    std::string sprite{ "EMPTY" };

    // tl
    int x = objective_size / 2.0f, y = objective_size / 2.0f, sx = objective_size, sy = objective_size;
    create_objective(registry, x, y, sx, sy, sprite);

    // bl
    x = objective_size / 2.0f, y = battlefield_xy.y - objective_size / 2.0f, sx = objective_size, sy = objective_size;
    create_objective(registry, x, y, sx, sy, sprite);

    // tr
    x = battlefield_xy.x - objective_size / 2.0f, y = objective_size / 2.0f, sx = objective_size, sy = objective_size;
    create_objective(registry, x, y, sx, sy, sprite);

    // br
    x = battlefield_xy.x - objective_size / 2.0f, y = battlefield_xy.y - objective_size / 2.0f, sx = objective_size,
    sy = objective_size;
    create_objective(registry, x, y, sx, sy, sprite);
  }
};

} // namespace game2d

void
game2d::init(entt::registry& registry, engine::Application& app, glm::ivec2 screen_wh)
{
  // init once only
  init_render_system(registry, screen_wh);
  registry.set<Profiler>(Profiler());
  app.get_input().open_controllers(); // enable controllers

  init_game_state(registry, app);
};

void
game2d::fixed_update(entt::registry& registry, engine::Application& app, float fixed_dt)
{
  Profiler& p = registry.ctx<Profiler>();
  SINGLETON_GamePausedComponent& gp = registry.ctx<SINGLETON_GamePausedComponent>();

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

  if (ri.viewport_process_events) {
    if (app.get_input().get_key_down(SDL_SCANCODE_P))
      gp.paused = !gp.paused;
    if (app.get_input().get_key_down(SDL_SCANCODE_R))
      init_game_state(registry, app);
    if (app.get_input().get_key_down(SDL_SCANCODE_F))
      app.get_window().toggle_fullscreen();
  }
  if (app.get_input().get_key_down(SDL_SCANCODE_ESCAPE))
    app.shutdown();
  if (ri.viewport_size_render_at != ri.viewport_size_current) {
    // viewport was updated, recenter the camera on the battlefield
    const auto& cameras = registry.view<CameraComponent, PositionIntComponent>();
    const auto& main_camera = cameras.front();
    auto& main_camera_position = registry.get<PositionIntComponent>(main_camera);
    const int camera_x = ri.viewport_size_current.x / 2 - battlefield_xy.x / 2;
    const int camera_y = ri.viewport_size_current.y / 2 - battlefield_xy.y / 2;
    main_camera_position.x = camera_x;
    main_camera_position.y = camera_y;
  }

  // game logic
  Uint64 start_game_tick = SDL_GetPerformanceCounter();
  {
    if (!gp.paused) {
      // ... systems that always update
      {
        update_objectives_system(registry, app);
        update_cursor_system(registry, app);
        update_pathfinding_system(registry, app);
        update_select_objects_system(registry, app);
        update_select_objects_highlight_system(registry, app);
        update_select_objects_move_system(registry, app);
        update_unit_group_position_units_system(registry, app, dt);
      }

      // ... systems that update if viewport is focused
      {
        if (ri.viewport_process_events) {
          update_camera_system(registry, app);
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
    update_ui_game_system(registry, app);
    update_ui_physics_system(registry, app);
    update_ui_hierarchy_system(registry, app);
    update_ui_profiler_system(registry, app);
  };

  // end frame
  {
    end_frame_render_system(registry);
  }
};
