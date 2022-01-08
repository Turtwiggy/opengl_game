#this cmake lists compiles the game_2d with the engine

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
  message("Not compiling for emscripten - adding vcpkg")
  set (CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/thirdparty/gameengine/thirdparty/vcpkg/scripts/buildsystems/vcpkg.cmake)
endif()

#if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_definitions("-std=c++17") 
endif()

project(game_2d VERSION 0.1.0)
message("game_2d: ${CMAKE_SYSTEM_NAME}")
message("game_2d: ${CMAKE_BUILD_TYPE}")
message("game_2d: C: ${CMAKE_C_COMPILER_ID}")
message("game_2d: CXX: ${CMAKE_CXX_COMPILER_ID}")

# build the engine
include("${CMAKE_SOURCE_DIR}/thirdparty/gameengine/build_engine.cmake")

# Add VCPKG packages
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-mixer CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")

# Add source files
file(GLOB_RECURSE SRC_FILES 
  ${ENGINE_SOURCE},
  "${CMAKE_SOURCE_DIR}/game/*.cpp"
)

add_executable(game_2d
  ${SRC_FILES}
  # add icon to executable
  # ${CMAKE_CURRENT_BINARY_DIR}/assets/2d_game/textures/resource.rc
)

# includes
target_include_directories(game_2d PRIVATE 
  ${ENGINE_INCLUDES} 
  ${CMAKE_SOURCE_DIR}/thirdparty/entt/single_include
  ${CMAKE_SOURCE_DIR}/game
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

  # link windows libs
  target_link_libraries(game_2d PRIVATE SDL2::SDL2 SDL2::SDL2main)
  target_link_libraries(game_2d PRIVATE SDL2::SDL2_mixer)
  target_link_libraries(game_2d PRIVATE opengl32)
  target_link_libraries(game_2d PRIVATE GLEW::GLEW)
  target_link_libraries(game_2d PRIVATE glm::glm)
  # game networking sockets libs (windows only)
  # OpenSSL::SSL OpenSSL::Crypto
  # protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite
  # GameNetworkingSockets::shared GameNetworkingSockets::static GameNetworkingSockets::GameNetworkingSockets GameNetworkingSockets::GameNetworkingSockets_s

  # create symlink assets for build
  set (source "${CMAKE_SOURCE_DIR}/assets")
  set (destination "${CMAKE_CURRENT_BINARY_DIR}/assets")
  add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination} 
    DEPENDS ${destination}
    COMMENT "symbolic link resources folder from ${source} => ${destination}"
  )
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)