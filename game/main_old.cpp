// game headers
#include "game.hpp"
using namespace game2d;

// engine headers
#include "engine/application.hpp"
#include "engine/util.hpp"
#include "engine/util_windows.hpp"
using namespace engine;

// other lib headers
#include <entt/entt.hpp>

// std lib headers
#include <iostream>

#if defined(__EMSCRIPTEN__)
// Emscripten wants to run the mainloop because of the way the browser is single threaded.
// For this, it requires a void() function. In order to avoid breaking the flow of the
// readability of this example, we're going to use a C++11 lambda to keep the mainloop code
// within the rest of the main function. This hack isn't needed in production, and one may
// simply have an actual main loop function to give to emscripten instead.
#include <functional>
static std::function<void()> loop;
static void
main_loop()
{
  loop();
}
#endif

int
main(int argc, char* argv[])
{
  std::cout << "Running main()" << std::endl;
  const auto app_start = std::chrono::high_resolution_clock::now();

  bool hide_windows_console = false;
  if (hide_windows_console)
    engine::hide_windows_console();

  bool vsync = true;
  glm::ivec2 start_screen_wh = { 1366, 720 };
  Application app("2D Game [0.0.8]", start_screen_wh.x, start_screen_wh.y, vsync);

  entt::registry registry;
  game2d::init(registry, app, start_screen_wh);
  log_time_since("(INFO) End Setup ", app_start);

#if defined(__EMSCRIPTEN__)
  // See comments around line 30.
  loop = [&]()
  // Note that this doesn't process the 'done' boolean anymore. The function emscripten_set_main_loop will
  // in effect not return, and will set an infinite loop, that is going to be impossible to break. The
  // application can then only stop when the brower's tab is closed, and you will NOT get any notification
  // about exitting. The browser will then cleanup all resources on its own.
#else
  while (app.is_running())
#endif
  {
    Uint64 frame_start_time = SDL_GetPerformanceCounter();

    app.frame_begin(); // input events

    float delta_time_s = app.get_delta_time();
    if (delta_time_s >= 0.25f)
      delta_time_s = 0.25f;

    game2d::update(registry, app, delta_time_s);

    app.frame_end(frame_start_time);
  };

#if defined(__EMSCRIPTEN__)
  // This function call will not return.
  // emscripten_set_main_loop(main_loop, 0, true);
  // A fully portable version of this code that doesn't use the lambda hack might have an #else that does:
  //    while (!done) main_loop();
#endif

  return 0;
}