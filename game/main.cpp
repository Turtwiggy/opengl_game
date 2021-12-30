
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

#include <stdio.h>

int
main()
{
  printf("Hello, world!");

  return 0;
}