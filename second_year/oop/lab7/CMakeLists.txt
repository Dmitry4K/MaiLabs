cmake_minimum_required(VERSION 3.0)

project(lab7x)

set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_STANDARD 17)

add_executable(lab7x
  main.cpp
  sdl.cpp
  canvas.cpp
  loader.cpp
  undo_commands/remove_command.cpp
)

add_subdirectory(lib/SDL2/)
target_link_libraries(lab7x SDL2-static)
target_include_directories(lab7x PRIVATE ${SDL2_INCLUDE_DIR})

add_subdirectory(lib/imgui/)
target_include_directories(imgui PRIVATE lib/SDL2/include/)
target_link_libraries(lab7x imgui)
