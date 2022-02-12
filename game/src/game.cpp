// your header
#include "game.hpp"

// components
#include "components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/ui_profiler/components.hpp"

// helpers
#include "helpers/physics_layers.hpp"

// systems
#include "modules/renderer/system.hpp"
#include "modules/ui_profiler/system.hpp"
// #include "systems/click_to_destroy.hpp"
// #include "systems/collisions_actor_actor.hpp"
// #include "systems/cursor.hpp"
// #include "systems/destroy_after_time.hpp"
// #include "systems/parry.hpp"
// #include "systems/player_input.hpp"
// #include "systems/velocity_in_boundingbox.hpp"

// engine headers
#include "engine/maths.hpp"
#include "engine/util.hpp"

// other project headers
#include <glm/glm.hpp>

// c++ lib headers
#include <string>

namespace game2d {

void
init_game_state(game2d::Coordinator& registry)
{
  // registry.each([&registry](auto entity) { registry.destroy(entity); });
  // init_physics_system(registry);
  // init_ui_gizmos_system(registry);
  // registry.set<SINGLETON_ResourceComponent>(SINGLETON_ResourceComponent());
  // registry.set<SINGLETON_GamePausedComponent>(SINGLETON_GamePausedComponent());
  // registry.set<SINGLETON_GridSizeComponent>(SINGLETON_GridSizeComponent());

  // colours
  const glm::vec4 colour_red = glm::vec4(232 / 255.0f, 80 / 255.0f, 100 / 255.0f, 1.0f);
  const glm::vec4 colour_cyan = glm::vec4(8 / 255.0f, 177 / 255.0f, 190 / 255.0f, 1.0f);
  const glm::vec4 colour_dblue = glm::vec4(49 / 255.0f, 99 / 255.0f, 188 / 255.0f, 1.0f);
  const glm::vec4 colour_white = glm::vec4(1.0f);
  const glm::vec4 colour_green = glm::vec4(100 / 255.0f, 188 / 255.0f, 49 / 255.0f, 1.0f);

  // access singleton data
  // const auto& res = registry.ctx<SINGLETON_ResourceComponent>();
  // const auto& gs = registry.ctx<SINGLETON_GridSizeComponent>();
  // const int GRID_SIZE = gs.size_xy;

  // Editor Camera
  // {
  //   entt::entity r = registry.create();
  //   registry.emplace<TagComponent>(r, "Editor Camera");
  //   registry.emplace<EditorCameraComponent>(r);
  //   registry.emplace<CameraComponent>(r);
  // }

  // // Add a cursor, made of 4 lines
  // {
  //   for (int i = 0; i < 4; i++) {
  //     entt::entity r = registry.create();
  //     registry.emplace<TagComponent>(r, std::string("cursor" + std::to_string(i)));
  //     // rendering
  //     registry.emplace<ColourComponent>(r, 1.0f, 0.0f, 0.0f, 0.5f);
  //     registry.emplace<PositionIntComponent>(r);
  //     registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  //     registry.emplace<SpriteComponent>(r, sprite::type::EMPTY);
  //     // gameplay
  //     registry.emplace<CursorComponent>(r, i);
  //   }
  // }

  // // Add some blocks
  // {
  //   for (int i = 1; i < 30; i++) {
  //     entt::entity r = registry.create();
  //     registry.emplace<TagComponent>(r, std::string("a-blocks" + std::to_string(i)));
  //     // rendering
  //     registry.emplace<ColourComponent>(r, colour_dblue);
  //     registry.emplace<PositionIntComponent>(r, (22 + i) * GRID_SIZE, 35 * GRID_SIZE);
  //     registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  //     registry.emplace<SpriteComponent>(r, sprite::type::EMPTY);
  //     // physics
  //     registry.emplace<CollidableComponent>(
  //       r, static_cast<uint32_t>(GameCollisionLayer::SOLID_WALL), PhysicsType::SOLID);
  //     registry.emplace<PhysicsSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  //     // gameplay
  //     FlashColourComponent f;
  //     f.start_colour = colour_dblue;
  //     f.flash_colour = colour_green;
  //     registry.emplace<FlashColourComponent>(r, f);
  //     registry.emplace<HealthComponent>(r, 3.0f);
  //     registry.emplace<ClickToDestroyComponent>(r);
  //     // registry.emplace<ParryComponent>(r);
  //   }
  // }

  // // Add some blocks
  // {
  //   for (int i = 1; i < 10; i++) {
  //     entt::entity r = registry.create();
  //     registry.emplace<TagComponent>(r, std::string("b-blocks" + std::to_string(i)));
  //     // rendering
  //     registry.emplace<ColourComponent>(r, colour_green);
  //     registry.emplace<PositionIntComponent>(r, (10 + i) * GRID_SIZE, 30 * GRID_SIZE);
  //     registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  //     registry.emplace<SpriteComponent>(r, sprite::type::EMPTY);
  //     // physics
  //     registry.emplace<CollidableComponent>(
  //       r, static_cast<uint32_t>(GameCollisionLayer::SOLID_WALL), PhysicsType::SOLID);
  //     registry.emplace<PhysicsSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  //     // gameplay
  //     FlashColourComponent f;
  //     f.start_colour = colour_dblue;
  //     f.flash_colour = colour_green;
  //     registry.emplace<FlashColourComponent>(r, f);
  //     registry.emplace<HealthComponent>(r, 3.0f);
  //     registry.emplace<ClickToDestroyComponent>(r);
  //     // registry.emplace<ParryComponent>(r);
  //   }
  // }

  // // Add some blocks
  // {
  //   for (int i = 1; i < 25; i++) {

  //     entt::entity r = registry.create();
  //     registry.emplace<TagComponent>(r, std::string("c-blocks" + std::to_string(i)));
  //     // rendering
  //     registry.emplace<ColourComponent>(r, colour_cyan);
  //     registry.emplace<PositionIntComponent>(r, (24 + i) * GRID_SIZE, 24 * GRID_SIZE);
  //     registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  //     registry.emplace<SpriteComponent>(r, sprite::type::EMPTY);
  //     // physics
  //     registry.emplace<CollidableComponent>(
  //       r, static_cast<uint32_t>(GameCollisionLayer::SOLID_WALL), PhysicsType::SOLID);
  //     registry.emplace<PhysicsSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  //     // gameplay
  //     FlashColourComponent f;
  //     f.start_colour = colour_dblue;
  //     f.flash_colour = colour_green;
  //     registry.emplace<FlashColourComponent>(r, f);
  //     registry.emplace<HealthComponent>(r, 3.0f);
  //     registry.emplace<ClickToDestroyComponent>(r);
  //     // registry.emplace<ParryComponent>(r);
  //   }
  // }

  // // Add a player
  // {
  //   entt::entity r = registry.create();
  //   registry.emplace<TagComponent>(r, "player0");
  //   // rendering
  //   registry.emplace<ColourComponent>(r, colour_cyan);
  //   registry.emplace<PositionIntComponent>(r, 25 * GRID_SIZE, 25 * GRID_SIZE);
  //   registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  //   registry.emplace<SpriteComponent>(r, sprite::type::PERSON_0);
  //   // physics
  //   registry.emplace<CollidableComponent>(r, static_cast<uint32_t>(GameCollisionLayer::ACTOR_PLAYER));
  //   registry.emplace<PhysicsSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  //   registry.emplace<VelocityComponent>(r, 0.0f, 0.0f);
  //   // gameplay
  //   registry.emplace<AnimationBounce>(r);
  //   registry.emplace<Player>(r);
  //   registry.emplace<HealthComponent>(r);
  //   registry.emplace<DoubleJumpComponent>(r);
  // }
};

} // namespace game2d

void
game2d::init(game2d::Coordinator& registry, engine::Application& app, glm::ivec2 screen_wh)
{
  // init once only
  // init_audio_system(registry);
  init_render_system(registry, screen_wh);
  init_ui_profiler_system(registry);
  // init_ui_hierarchy_system(registry);
  // init_ui_map_editor_system(registry);

  // could be deleted and re-init at any time
  init_game_state(registry);

  // temp
  app.get_input().open_controllers();
};

void
game2d::update(game2d::Coordinator& registry, engine::Application& app, float dt)
{
  // Profiler& p = registry.ctx<Profiler>();

  // SINGLETON_GamePausedComponent& gp = registry.ctx<SINGLETON_GamePausedComponent>();
  // if (app.get_input().get_key_down(SDL_SCANCODE_P)) {
  //   gp.paused = !gp.paused;
  //   std::cout << "game paused: " << gp.paused << std::endl;
  // }

#if defined _DEBUG || defined DEBUG
  if (app.get_input().get_key_down(SDL_SCANCODE_R)) {
    init_game_state(registry);
  }
  if (app.get_input().get_key_down(SDL_SCANCODE_ESCAPE)) {
    app.shutdown();
  }
#endif

  // physics
  Uint64 start_physics = SDL_GetPerformanceCounter();
  {
    // if (!gp.paused) {
    //   // // move objects, checking collisions along way
    //   // update_move_objects_system(registry, app, dt);
    //   // // generate all collisions between actor-actor objects
    //   // update_physics_system(registry, app, dt);
    //   // // process actor-actor collisions
    //   // update_actor_actor_collision_system(registry, app, dt);
    // }
  }
  Uint64 end_physics = SDL_GetPerformanceCounter();
  // p.physics_elapsed_ms = (end_physics - start_physics) / float(SDL_GetPerformanceFrequency()) * 1000.0f;

  // game logic
  Uint64 start_game_tick = SDL_GetPerformanceCounter();
  {
    // if (!gp.paused) {
    //   // update_animation_system(registry, app, dt);
    //   // // update_audio_system(registry, app, dt);
    //   // update_player_input_system(registry, app);
    //   // update_cursor_system(registry, app);
    //   // update_click_to_destroy_system(registry, app);
    //   // update_velocity_in_boundingbox_system(registry, app, dt);
    //   // update_parry_system(registry, app, dt);
    //   // // editor stuff
    //   // update_editor_camera_system(registry, app, dt);
    //   // update_map_editor_system(registry, app, dt);
    //   // // destroy any entities
    //   // update_destroy_after_time_system(registry, app, dt);
    // }
  };
  Uint64 end_game_tick = SDL_GetPerformanceCounter();
  // p.game_tick_elapsed_ms = (end_game_tick - start_game_tick) / float(SDL_GetPerformanceFrequency()) * 1000.0f;

  // rendering
  Uint64 start_render = SDL_GetPerformanceCounter();
  {
    update_render_system(registry, app);
  };
  Uint64 end_render = SDL_GetPerformanceCounter();
  // p.render_elapsed_ms = (end_render - start_render) / float(SDL_GetPerformanceFrequency()) * 1000.0f;

  // ui
  {
    update_ui_profiler_system(registry, app);
    // update_ui_physics_system(registry, app);
    // update_ui_hierarchy_system(registry, app);
    // // update_ui_gizmos_system(registry, app, dt); // update after hierarchy
    // update_ui_map_editor_system(registry, app, dt);
  };

  // end frame
  {
    end_frame_render_system(registry);
  }
};