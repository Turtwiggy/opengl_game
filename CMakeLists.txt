#this cmake lists compiles the game_2d with the engine

cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_VERBOSE_MAKEFILE on)
project(game_2d VERSION 0.1.0)

message("game_2d: ${CMAKE_SYSTEM_NAME} ${CMAKE_BUILD_TYPE}")
message("game_2d: C: ${CMAKE_C_COMPILER_ID}")
message("game_2d: CXX: ${CMAKE_CXX_COMPILER_ID}")

# build the engine
include("${CMAKE_SOURCE_DIR}/thirdparty/gameengine/build_engine.cmake")

# Add VCPKG packages
# foreach(package ${ENGINE_PACKAGES_CONFIG})
#   message("${PROJECT_NAME} finding package... ${package}")
#   find_package(${package} CONFIG REQUIRED)
# endforeach()
# foreach(package ${ENGINE_PACKAGES})
#   message("${PROJECT_NAME} finding package...  ${package}")
#   find_package(${package} REQUIRED)
# endforeach()

# Add source files
file(GLOB_RECURSE SRC_FILES 
  "${CMAKE_SOURCE_DIR}/game/main.cpp"
)

add_executable(game_2d
  ${SRC_FILES}
  # add icon to executable
  # ${CMAKE_CURRENT_BINARY_DIR}/assets/2d_game/textures/resource.rc
)

# includes
target_include_directories(game_2d PRIVATE 
  ${ENGINE_INCLUDES} 
  ${CMAKE_SOURCE_DIR}/examples/game_2d/src
  ${CMAKE_SOURCE_DIR}/thirdparty/entt/single_include/entt
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # add_definitions("-Wall")
    # add_definitions("-Wextra")
    # add_definitions("-Wpedantic")
    # add_definitions("-Werror")
    # add_definitions("-Wformat")
    add_definitions("-std=c++17") 
endif()
# elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

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
  message("Not compiling for emscripten")
endif()


# link libs
foreach(library ${ENGINE_LINK_LIBS})
  message("${PROJECT_NAME} linking library... ${library}")
  target_link_libraries(game_2d PRIVATE ${library})
endforeach()

# Windows things
# IF(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
#   # create symlink assets for build
#   set (source "${CMAKE_SOURCE_DIR}/assets")
#   set (destination "${CMAKE_CURRENT_BINARY_DIR}/assets")
#   add_custom_command(
#     TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination} 
#     DEPENDS ${destination}
#     COMMENT "symbolic link resources folder from ${source} => ${destination}"
#   )
# endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)