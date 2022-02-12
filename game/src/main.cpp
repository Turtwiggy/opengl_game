
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
#include "ecs.hpp"
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

static bool vsync = true;
static glm::ivec2 start_screen_wh = { 1366, 720 };
static Application app("2D Game [0.0.8]", start_screen_wh.x, start_screen_wh.y, vsync);
static game2d::Coordinator registry;

void
main_loop(void* arg)
{
  IM_UNUSED(arg); // do nothing with it

  Uint64 frame_start_time = SDL_GetPerformanceCounter();

  app.frame_begin(); // input events

  float delta_time_s = app.get_delta_time();
  if (delta_time_s >= 0.25f)
    delta_time_s = 0.25f;

  // game2d::update(registry, app, delta_time_s);

  app.frame_end(frame_start_time);
}

int
main(int argc, char* argv[])
{
  IM_UNUSED(argc);
  IM_UNUSED(argv);

  std::cout << "Running main()" << std::endl;
  const auto app_start = std::chrono::high_resolution_clock::now();

  // game2d::init(registry, app, start_screen_wh);

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

  return 0;
}