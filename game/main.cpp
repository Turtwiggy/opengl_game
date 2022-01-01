
#if defined(__EMSCRIPTEN__)
#include <SDL.h>
#include <SDL_opengles2.h>
#include <emscripten.h>
#else  // end emscripten
// #include <SDL.h>
// #define IMGUI_IMPL_OPENGL_LOADER_GLEW 1
#endif // end windows

// #include <backends/imgui_impl_opengl3.h>
// #include <backends/imgui_impl_sdl.h>
// #include <imgui_internal.h>
// #if defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
// #include <GL/glew.h>
// #endif

#include <imgui.h>
#include <stdio.h>

void
main_loop(void* arg)
{
  IM_UNUSED(arg); // do nothing with it
}

int
main(int argc, char* argv[])
{
  IM_UNUSED(argc);
  IM_UNUSED(argv);

#if defined(__EMSCRIPTEN__)
  printf("Hello, Emscripten!");

  // This function call won't return, and will engage in an infinite loop,
  // processing events from the browser, and dispatching them.
  emscripten_set_main_loop_arg(main_loop, NULL, 0, true);
#else
  printf("Hello, not-emscripten!!");

  while (1)
    main_loop(nullptr);
#endif

  return 0;
}