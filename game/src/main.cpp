
#if defined(__EMSCRIPTEN__)
#include <SDL.h>
#include <SDL_opengles2.h>
#include <emscripten.h>
#else  // end emscripten
// #include <SDL.h>
// #define IMGUI_IMPL_OPENGL_LOADER_GLEW 1
#endif // end windows

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl.h>
#include <imgui_internal.h>
#if defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>
#endif

// game headers
#include "components.hpp"
#include "ecs.hpp"
#include "modules/renderer/components.hpp"
#include "modules/renderer/system.hpp"
#include "modules/ui_profiler/components.hpp"
#include "modules/ui_profiler/system.hpp"
#include "systems/cursor.hpp"
using namespace game2d;

// engine headers
#include "engine/application.hpp"
#include "engine/util.hpp"
using namespace engine;

// other libs
#include "glm/glm.hpp"
#include "imgui.h"

// std lib
#include <chrono>
#include <iostream>

int
main(int argc, char* argv[])
{
  IM_UNUSED(argc);
  IM_UNUSED(argv);

  bool vsync = true;
  glm::ivec2 start_screen_wh = { 1366, 720 };
  Application app("2D Game [0.0.8]", start_screen_wh.x, start_screen_wh.y, vsync);
  game2d::Coordinator registry;

  std::cout << "Running main()" << std::endl;
  const auto app_start = std::chrono::high_resolution_clock::now();

  //
  // configure ecs...
  //

  // .. register components
  registry.register_component<TagComponent>();
  registry.register_component<PositionIntComponent>();
  registry.register_component<RenderSizeComponent>();
  registry.register_component<ColourComponent>();
  registry.register_component<SpriteComponent>();
  registry.register_component<CursorComponent>();

  // .. register systems
  auto renderer = registry.register_system<game2d::RenderSpriteSystem>();
  {
    game2d::Signature signature;
    signature.set(registry.get_component_type<TagComponent>());
    signature.set(registry.get_component_type<PositionIntComponent>());
    signature.set(registry.get_component_type<RenderSizeComponent>());
    signature.set(registry.get_component_type<ColourComponent>());
    signature.set(registry.get_component_type<SpriteComponent>());
    registry.set_system_signature<RenderSpriteSystem>(signature);
  }
  auto cursor = registry.register_system<game2d::CursorSystem>();
  {
    game2d::Signature signature;
    signature.set(registry.get_component_type<TagComponent>());
    signature.set(registry.get_component_type<PositionIntComponent>());
    signature.set(registry.get_component_type<RenderSizeComponent>());
    signature.set(registry.get_component_type<ColourComponent>());
    signature.set(registry.get_component_type<SpriteComponent>());
    signature.set(registry.get_component_type<CursorComponent>());
    registry.set_system_signature<CursorSystem>(signature);
  }

  // colours
  const glm::vec4 colour_red = glm::vec4(232 / 255.0f, 80 / 255.0f, 100 / 255.0f, 1.0f);
  const glm::vec4 colour_cyan = glm::vec4(8 / 255.0f, 177 / 255.0f, 190 / 255.0f, 1.0f);
  const glm::vec4 colour_dblue = glm::vec4(49 / 255.0f, 99 / 255.0f, 188 / 255.0f, 1.0f);
  const glm::vec4 colour_white = glm::vec4(1.0f);
  const glm::vec4 colour_green = glm::vec4(100 / 255.0f, 188 / 255.0f, 49 / 255.0f, 1.0f);

  // init
  const SINGLETON_GridSizeComponent grid = SINGLETON_GridSizeComponent();
  const int GRID_SIZE = grid.size_xy;
  // init once only
  RendererInfo ri = init_render_system(start_screen_wh);
  // init_audio_system(registry);
  // init_ui_profiler_system(registry);
  // init_ui_hierarchy_system(registry);
  // init_ui_map_editor_system(registry);
  // could be deleted and re-init at any time
  // init_game_state(registry);
  // temp
  app.get_input().open_controllers();

  // Add a cursor, made of 4 lines
  {
    for (int i = 0; i < 4; i++) {
      auto r = registry.create_entity();
      registry.add_component<TagComponent>(r, TagComponent{ std::string("cursor" + std::to_string(i)) });
      // rendering
      registry.add_component<ColourComponent>(r, ColourComponent(colour_red));
      registry.add_component<PositionIntComponent>(r, PositionIntComponent{});
      registry.add_component<RenderSizeComponent>(r, RenderSizeComponent{ GRID_SIZE, GRID_SIZE });
      registry.add_component<SpriteComponent>(r, SpriteComponent{ sprite::type::EMPTY });
      // gameplay
      registry.add_component<CursorComponent>(r, CursorComponent{ i });
    }
  }

  // Add some blocks
  {
    for (int i = 1; i < 30; i++) {
      auto r = registry.create_entity();
      registry.add_component<TagComponent>(r, TagComponent{ std::string("a-blocks" + std::to_string(i)) });
      // rendering
      registry.add_component<ColourComponent>(r, ColourComponent(colour_dblue));
      registry.add_component<PositionIntComponent>(r, PositionIntComponent{ (22 + i) * GRID_SIZE, 35 * GRID_SIZE });
      registry.add_component<RenderSizeComponent>(r, RenderSizeComponent{ GRID_SIZE, GRID_SIZE });
      registry.add_component<SpriteComponent>(r, SpriteComponent{ sprite::type::EMPTY });
    }
  }

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

  bool hide_windows_console = false;
  if (hide_windows_console)
    engine::hide_windows_console();

  log_time_since("(INFO) End Setup ", app_start);

  // #if defined(__EMSCRIPTEN__)
  //   // This function call won't return, and will engage in an infinite loop,
  //   // processing events from the browser, and dispatching them.
  //   emscripten_set_main_loop_arg(main_loop, NULL, 0, true);
  // #else
  //   while (app.is_running())
  //     main_loop(nullptr);
  // #endif

  while (app.is_running()) {
    Uint64 frame_start_time = SDL_GetPerformanceCounter();

    app.frame_begin(); // input events

    float delta_time_s = app.get_delta_time();
    if (delta_time_s >= 0.25f)
      delta_time_s = 0.25f;

    //   // Profiler& p = registry.ctx<Profiler>();

    //   // SINGLETON_GamePausedComponent& gp = registry.ctx<SINGLETON_GamePausedComponent>();
    //   // if (app.get_input().get_key_down(SDL_SCANCODE_P)) {
    //   //   gp.paused = !gp.paused;
    //   //   std::cout << "game paused: " << gp.paused << std::endl;
    //   // }

    // #if defined _DEBUG || defined DEBUG
    //   if (app.get_input().get_key_down(SDL_SCANCODE_R)) {
    //     init_game_state(registry);
    //   }
    //   if (app.get_input().get_key_down(SDL_SCANCODE_ESCAPE)) {
    //     app.shutdown();
    //   }
    // #endif

    //   // physics
    //   Uint64 start_physics = SDL_GetPerformanceCounter();
    //   {
    //     // if (!gp.paused) {
    //     //   // // move objects, checking collisions along way
    //     //   // update_move_objects_system(registry, app, dt);
    //     //   // // generate all collisions between actor-actor objects
    //     //   // update_physics_system(registry, app, dt);
    //     //   // // process actor-actor collisions
    //     //   // update_actor_actor_collision_system(registry, app, dt);
    //     // }
    //   }
    //   Uint64 end_physics = SDL_GetPerformanceCounter();
    //   // p.physics_elapsed_ms = (end_physics - start_physics) / float(SDL_GetPerformanceFrequency()) * 1000.0f;

    // game logic
    Uint64 start_game_tick = SDL_GetPerformanceCounter();
    {
      // if (!gp.paused) {
      cursor->update(registry, app, ri, grid);
      //   // update_animation_system(registry, app, dt);
      //   // // update_audio_system(registry, app, dt);
      //   // update_player_input_system(registry, app);
      //   // update_click_to_destroy_system(registry, app);
      //   // update_velocity_in_boundingbox_system(registry, app, dt);
      //   // update_parry_system(registry, app, dt);
      //   // // editor stuff
      //   // update_editor_camera_system(registry, app, dt);
      //   // update_map_editor_system(registry, app, dt);
      //   // // destroy any entities
      //   // update_destroy_after_time_system(registry, app, dt);
      // }
    }
    Uint64 end_game_tick = SDL_GetPerformanceCounter();
    //   // p.game_tick_elapsed_ms = (end_game_tick - start_game_tick) / float(SDL_GetPerformanceFrequency()) * 1000.0f;

    // rendering
    Uint64 start_render = SDL_GetPerformanceCounter();
    {
      renderer->update(registry, ri);
    }
    //   Uint64 end_render = SDL_GetPerformanceCounter();
    //   // p.render_elapsed_ms = (end_render - start_render) / float(SDL_GetPerformanceFrequency()) * 1000.0f;

    // ui
    {
      // update_ui_profiler_system(registry, app);
      // update_ui_physics_system(registry, app);
      // update_ui_hierarchy_system(registry, app);
      // // update_ui_gizmos_system(registry, app, dt); // update after hierarchy
      // update_ui_map_editor_system(registry, app, dt);

      const auto mouse_pos = app.get_input().get_mouse_pos();
      ImGui::Begin("Mouse");
      ImGui::Text("%i, %i", mouse_pos.x, mouse_pos.y);
      ImGui::End();
    };

    // end frame
    {
      end_frame_render_system();
    }

    app.frame_end(frame_start_time);
  }

  return 0;
}