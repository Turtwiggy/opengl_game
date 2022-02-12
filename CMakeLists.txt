cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_VERBOSE_MAKEFILE on)

if(EMSCRIPTEN)
  message("Compiling for emscripten")
  # WEB_DIR = build-web
  # EXE = $(WEB_DIR)/index.html
  # EMS += -s USE_SDL=2
  # EMS += -s DISABLE_EXCEPTION_CATCHING=1
  # LDFLAGS += -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=0 -s ASSERTIONS=1
  # CXXFLAGS += -Os $(EMS)
  # LDFLAGS += --shell-file $(ASSETS_HTML_TEMPLATE) $(EMS)
  # set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $(CMAKE_CURRENT_SOURCE_DIR)/build.emscripten)
  # set (CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/thirdparty/gameengine/thirdparty/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)
  # set_target_properties(client PROPERTIES LINK_FLAGS "-s DEMANGLE_SUPPORT=1 --preload-file ${CMAKE_SOURCE_DIR}/assets --bind")
else()
  message("Not compiling for emscripten - adding vcpkg ${CMAKE_SOURCE_DIR}")
  set (CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/thirdparty/gameengine/thirdparty/vcpkg/scripts/buildsystems/vcpkg.cmake)
endif()

project(subprojects VERSION 0.1.0)

# projects
add_subdirectory(game)
add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)