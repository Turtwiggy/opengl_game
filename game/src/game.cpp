// your header
#include "game.hpp"

// components
#include "game_components.hpp"
#include "modules/animation/components.hpp"
#include "modules/audio/components.hpp"
#include "modules/editor_camera/components.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/ui_gizmos/components.hpp"
#include "modules/ui_hierarchy/components.hpp"
#include "modules/ui_map/components.hpp"
#include "modules/ui_profiler/components.hpp"

// helpers
#include "helpers/physics_layers.hpp"

// systems
#include "modules/animation/system.hpp"
#include "modules/audio/system.hpp"
#include "modules/editor_camera/system.hpp"
#include "modules/physics/process_move_objects.hpp"
#include "modules/physics/system.hpp"
#include "modules/renderer/system.hpp"
#include "modules/ui_gizmos/system.hpp"
#include "modules/ui_hierarchy/system.hpp"
#include "modules/ui_map/system.hpp"
#include "modules/ui_physics/system.hpp"
#include "modules/ui_profiler/system.hpp"
#include "systems/click_to_destroy.hpp"
#include "systems/collisions_actor_actor.hpp"
#include "systems/cursor.hpp"
#include "systems/destroy_after_time.hpp"
#include "systems/player_input.hpp"

// engine headers
#include "engine/maths.hpp"
#include "engine/util.hpp"

// other project headers
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <imgui.h>

// c++ lib headers
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace game2d {

void
init_game_state(entt::registry& registry, engine::Application& app)
{
  registry.each([&registry](auto entity) { registry.destroy(entity); });
  registry.set<SINGLETON_PhysicsComponent>(SINGLETON_PhysicsComponent());
  registry.set<SINGLETON_GizmosComponent>(SINGLETON_GizmosComponent());
  registry.set<SINGLETON_ResourceComponent>(SINGLETON_ResourceComponent());
  registry.set<SINGLETON_GamePausedComponent>(SINGLETON_GamePausedComponent());
  registry.set<SINGLETON_GridSizeComponent>(SINGLETON_GridSizeComponent());
  registry.set<SINGLETON_ColoursComponent>(SINGLETON_ColoursComponent());

  const auto colours = registry.ctx<SINGLETON_ColoursComponent>();
  const int GRID_SIZE = registry.ctx<SINGLETON_GridSizeComponent>().size_xy;

  // Add a cursor, made of 4 lines
  {
    for (int i = 0; i < 4; i++) {
      entt::entity r = registry.create();
      registry.emplace<TagComponent>(r, std::string("cursor" + std::to_string(i)));
      // rendering
      registry.emplace<ColourComponent>(r, 1.0f, 0.0f, 0.0f, 0.5f);
      registry.emplace<PositionIntComponent>(r);
      registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
      registry.emplace<SpriteComponent>(r, sprite::type::EMPTY);
      // gameplay
      registry.emplace<CursorComponent>(r, i);
    }
  }

  // Add a player
  {
    entt::entity r = registry.create();
    registry.emplace<TagComponent>(r, "player0");

    // rendering
    registry.emplace<ColourComponent>(r, colours.cyan);
    registry.emplace<PositionIntComponent>(r, 40 * GRID_SIZE, 25 * GRID_SIZE);
    registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
    registry.emplace<SpriteComponent>(r, sprite::type::PERSON_6);
    // physics
    registry.emplace<CollidableComponent>(r, static_cast<uint32_t>(GameCollisionLayer::ACTOR_PLAYER));
    registry.emplace<PhysicsSizeComponent>(r, GRID_SIZE, GRID_SIZE);
    registry.emplace<VelocityComponent>(r, 0.0f, 0.0f);
    // input
    registry.emplace<PlayerComponent>(r, 0);
    PlayerInputComponent pic;
    pic.use_keyboard = true;
    registry.emplace<PlayerInputComponent>(r, pic);
    // gameplay
    registry.emplace<AnimationBounceComponent>(r);
    registry.emplace<HealthComponent>(r);
    registry.emplace<ClickToDestroyComponent>(r);
    registry.emplace<DoubleJumpComponent>(r);
  }

  // Add a second player
  {
    entt::entity r = registry.create();
    registry.emplace<TagComponent>(r, "player1");
    // rendering
    registry.emplace<ColourComponent>(r, colours.red);
    registry.emplace<PositionIntComponent>(r, 10 * GRID_SIZE, 25 * GRID_SIZE);
    registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
    registry.emplace<SpriteComponent>(r, sprite::type::PERSON_4);
    // physics
    registry.emplace<CollidableComponent>(r, static_cast<uint32_t>(GameCollisionLayer::ACTOR_PLAYER));
    registry.emplace<PhysicsSizeComponent>(r, GRID_SIZE, GRID_SIZE);
    registry.emplace<VelocityComponent>(r, 0.0f, 0.0f);
    // input
    registry.emplace<PlayerComponent>(r, 1);
    PlayerInputComponent pic;
    pic.use_controller = true;
    registry.emplace<PlayerInputComponent>(r, pic);
    // gameplay
    registry.emplace<AnimationBounceComponent>(r);
    registry.emplace<HealthComponent>(r);
    registry.emplace<ClickToDestroyComponent>(r);
    registry.emplace<DoubleJumpComponent>(r);
  }

  init_ui_map_system(registry, app);
};

} // namespace game2d

void
game2d::init(entt::registry& registry, engine::Application& app, glm::ivec2 screen_wh)
{
  // init once only
  // init_audio_system(registry);
  init_render_system(registry, screen_wh);
  registry.set<Profiler>(Profiler());
  registry.set<SINGLETON_HierarchyComponent>(SINGLETON_HierarchyComponent());
  app.get_input().open_controllers(); // enable controllers

  // could be deleted and re-init at any time
  init_game_state(registry, app);
};

void
game2d::update(entt::registry& registry, engine::Application& app, float dt)
{
  Profiler& p = registry.ctx<Profiler>();

  SINGLETON_GamePausedComponent& gp = registry.ctx<SINGLETON_GamePausedComponent>();
  if (app.get_input().get_key_down(SDL_SCANCODE_P)) {
    gp.paused = !gp.paused;
    std::cout << "game paused: " << gp.paused << std::endl;
  }
  if (app.get_input().get_key_down(SDL_SCANCODE_R)) {
    init_game_state(registry, app);
  }
  if (app.get_input().get_key_down(SDL_SCANCODE_ESCAPE)) {
    app.shutdown();
  }

  // physics
  Uint64 start_physics = SDL_GetPerformanceCounter();
  {
    if (!gp.paused) {
      // move objects, checking collisions along way
      update_move_objects_system(registry, app, dt);
      // generate all collisions between actor-actor objects
      update_physics_system(registry, app, dt);
      // process actor-actor collisions
      update_actor_actor_collision_system(registry, app, dt);
    }
  }
  Uint64 end_physics = SDL_GetPerformanceCounter();
  p.physics_elapsed_ms = (end_physics - start_physics) / float(SDL_GetPerformanceFrequency()) * 1000.0f;

  // game logic
  Uint64 start_game_tick = SDL_GetPerformanceCounter();
  {
    if (!gp.paused) {

      // ... systems that always update
      {
        update_player_input_system(registry, app);
        update_animation_system(registry, app, dt);
        // update_audio_system(registry, app, dt);
        update_cursor_system(registry, app);
        // editor stuff
        update_editor_camera_system(registry, app, dt);
      }

      // ... now systems that only update if viewport is focused
      {
        const auto& ri = registry.ctx<SINGLETON_RendererInfo>();
        if (ri.viewport_process_events) {
          update_click_to_destroy_system(registry, app);
          // last thing to do for gametick: destroy any entities
          update_destroy_after_time_system(registry, app, dt);
        }
      }
    }
  };
  Uint64 end_game_tick = SDL_GetPerformanceCounter();
  p.game_tick_elapsed_ms = (end_game_tick - start_game_tick) / float(SDL_GetPerformanceFrequency()) * 1000.0f;

  // rendering
  Uint64 start_render = SDL_GetPerformanceCounter();
  {
    update_render_system(registry, app);
  };
  Uint64 end_render = SDL_GetPerformanceCounter();
  p.render_elapsed_ms = (end_render - start_render) / float(SDL_GetPerformanceFrequency()) * 1000.0f;

  // ui
  {
    update_ui_profiler_system(registry, app);
    update_ui_map_system(registry, app, dt);
    update_ui_physics_system(registry, app);
    update_ui_hierarchy_system(registry, app);
    // update_ui_gizmos_system(registry, app, dt); // update after hierarchy
  };

  // end frame
  {
    end_frame_render_system(registry);
  }
};
