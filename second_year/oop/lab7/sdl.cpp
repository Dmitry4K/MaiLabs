#include "sdl.h"

#include <SDL.h>

#include "imgui.h"
#include "imgui_sdl.h"
#include "imgui_impl_sdl.h"

namespace sdl {

sdl::sdl() {
  SDL_Init(SDL_INIT_VIDEO);
}

sdl::~sdl() {
  SDL_Quit();
}

renderer::renderer(const std::string& window_name):
    window_(SDL_CreateWindow(window_name.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, 0)),
    renderer_(SDL_CreateRenderer(window_, -1, SDL_RENDERER_SOFTWARE)) {
  ImGui::CreateContext();
  ImGui_ImplSDL2_Init(window_);
  ImGuiSDL::Initialize(renderer_, 800, 600);
  ImGui_ImplSDL2_NewFrame(window_);
  ImGui::NewFrame();
}

renderer::~renderer() {
  ImGuiSDL::Deinitialize();
  ImGui::DestroyContext();
  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
}

void renderer::set_color(uint8_t r, uint8_t g, uint8_t b) const {
  SDL_SetRenderDrawColor(renderer_, r, g, b, 255);
}

void renderer::clear() const {
  SDL_RenderClear(renderer_);
}

void renderer::draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2) const {
  SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
}

void renderer::present() const {
  ImGui::Render();
  ImGuiSDL::Render(ImGui::GetDrawData());
  SDL_RenderPresent(renderer_);
  ImGui_ImplSDL2_NewFrame(window_);
  ImGui::NewFrame();
}

quit_event::quit_event(const SDL_QuitEvent& e): event_(e) {}

mouse_button_event::mouse_button_event(const SDL_MouseButtonEvent& e): event_(e) {}

uint32_t mouse_button_event::type() const {
  return event_.type;
}

uint8_t mouse_button_event::button() const {
  return event_.button;
}

int32_t mouse_button_event::x() const {
  return event_.x;
}

int32_t mouse_button_event::y() const {
  return event_.y;
}

bool event::extract(quit_event& event) const {
  if(event_.type == SDL_QUIT){
    event = event_.quit;
    return true;
  }
  return false;
}

bool event::extract(mouse_button_event& event) const {
  if(event_.type == SDL_MOUSEBUTTONDOWN || event_.type == SDL_MOUSEBUTTONUP){
    event = event_.button;
    return true;
  }
  return false;
}

bool event::poll(event& e) {
  bool result = SDL_PollEvent(&e.event_);
  if(result){
    ImGui_ImplSDL2_ProcessEvent(&e.event_);
  }
  return result;
}

} // namespace sdl
