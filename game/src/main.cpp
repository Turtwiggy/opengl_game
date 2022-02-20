
// game headers
#include "game.hpp"
using namespace game2d;

// engine headers
#include "engine/application.hpp"
#include "engine/util.hpp"
using namespace engine;

// other libs
#include "entt/entt.hpp"
#include "imgui.h"

// std lib
#include <chrono>
#include <iostream>

static bool vsync = true;
static glm::ivec2 start_screen_wh = { 1366, 720 };
static Application app("2D Game [0.0.8]", start_screen_wh.x, start_screen_wh.y, vsync);
static entt::registry registry;

void
main_loop(void* arg)
{
  IM_UNUSED(arg); // do nothing with it

  Uint64 frame_start_time = SDL_GetPerformanceCounter();

  app.frame_begin(); // input events

  float delta_time_s = app.get_delta_time();
  if (delta_time_s >= 0.25f)
    delta_time_s = 0.25f;

  game2d::update(registry, app, delta_time_s);

  app.frame_end(frame_start_time);
}

int
main(int argc, char* argv[])
{
  IM_UNUSED(argc);
  IM_UNUSED(argv);

  std::cout << "Running main()" << std::endl;
  const auto app_start = std::chrono::high_resolution_clock::now();

  // configure application
  app.limit_fps = true;
  app.fps_if_limited = 60.0f;

  game2d::init(registry, app, start_screen_wh);

  bool hide_windows_console = false;
  if (hide_windows_console)
    engine::hide_windows_console();

  log_time_since("(INFO) End Setup ", app_start);

#if defined(__EMSCRIPTEN__)
  printf("Hello, Emscripten!");

  // This function call won't return, and will engage in an infinite loop,
  // processing events from the browser, and dispatching them.
  emscripten_set_main_loop_arg(main_loop, NULL, 0, true);
#else
  while (app.is_running())
    main_loop(nullptr);
#endif

  return 0;
}