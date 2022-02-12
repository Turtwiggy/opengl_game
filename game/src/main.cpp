
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
#include "renderer/components.hpp"
#include "renderer/system.hpp"
#include "ui_profiler/components.hpp"
#include "ui_profiler/system.hpp"
// #include "game.hpp"
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

  // .. register systems
  auto sprite_sys = registry.register_system<game2d::RenderSpriteSystem>();
  {
    game2d::Signature signature;
    signature.set(registry.get_component_type<TagComponent>());
    signature.set(registry.get_component_type<PositionIntComponent>());
    signature.set(registry.get_component_type<RenderSizeComponent>());
    signature.set(registry.get_component_type<ColourComponent>());
    signature.set(registry.get_component_type<SpriteComponent>());
    registry.set_system_signature<RenderSpriteSystem>(signature);
  }

  RendererInfo ri = init_render_system(start_screen_wh);

  // colours
  const glm::vec4 colour_red = glm::vec4(232 / 255.0f, 80 / 255.0f, 100 / 255.0f, 1.0f);
  const glm::vec4 colour_cyan = glm::vec4(8 / 255.0f, 177 / 255.0f, 190 / 255.0f, 1.0f);
  const glm::vec4 colour_dblue = glm::vec4(49 / 255.0f, 99 / 255.0f, 188 / 255.0f, 1.0f);
  const glm::vec4 colour_white = glm::vec4(1.0f);
  const glm::vec4 colour_green = glm::vec4(100 / 255.0f, 188 / 255.0f, 49 / 255.0f, 1.0f);
  const int GRID_SIZE = 16;

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

    {
      //
      game2d::prep_for_render(ri);
      // game2d::update(registry, app, delta_time_s);
    }

    app.frame_end(frame_start_time);
  }

  return 0;
}