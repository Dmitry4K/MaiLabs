cmake_minimum_required(VERSION 3.0)

project(imgui)

add_library(imgui
  imgui.cpp
  imgui_sdl.cpp
  imgui_draw.cpp
  imgui_widgets.cpp
  imgui_impl_sdl.cpp
)

target_include_directories(imgui PUBLIC "./")
