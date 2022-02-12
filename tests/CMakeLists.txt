# this cmake compiles the game's unit tests
cmake_minimum_required(VERSION 3.0.0)

project(tests VERSION 0.1.0)

message("tests: ${CMAKE_SYSTEM_NAME}")
message("tests: ${CMAKE_BUILD_TYPE}")
message("tests: C: ${CMAKE_C_COMPILER_ID}")
message("tests: CXX: ${CMAKE_CXX_COMPILER_ID}")
message("tests: source_dir: ${CMAKE_SOURCE_DIR}")

#Add Testing
enable_testing()
find_package(GTest CONFIG REQUIRED)

# add source files
file(GLOB_RECURSE SRC_FILES 
  # engine source
  "${CMAKE_SOURCE_DIR}/game/src/ecs.cpp"
  # tests
  "${CMAKE_SOURCE_DIR}/tests/src/*.cpp"
)

add_executable(tests ${SRC_FILES} )

#Includes
target_include_directories(tests PRIVATE 
  ${CMAKE_SOURCE_DIR}/game/src
)

#Link Libs
# foreach(library ${ENGINE_LINK_LIBS})
# message("linking library... ${library}")
# target_link_libraries(tests PRIVATE ${library})
# endforeach()

#Link Testing Libs
target_link_libraries(tests PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)