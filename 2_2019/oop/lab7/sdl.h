#ifndef D_SDL_H_
#define D_SDL_H_ 1

#include <string>

#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_video.h"

namespace sdl {

struct sdl {
  sdl();
  ~sdl();

};

struct renderer {
  renderer(const std::string& window_name);
  ~renderer();

  // set color for subsequent operations
  void set_color(uint8_t r, uint8_t g, uint8_t b) const;
  // fill screen with current color
  void clear() const;
  // draw segment with current color
  void draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2) const;

  // every command draws to a temporary buffer
  // this function swaps temporary buffer containing new frame with current frame
  void present() const;

private:
  sdl system;
  SDL_Window* window_;
  SDL_Renderer* renderer_;

};

struct quit_event {
  quit_event() = default;
  quit_event(const SDL_QuitEvent& e);

private:
  SDL_QuitEvent event_;

};

struct mouse_button_event {
  mouse_button_event() = default;
  mouse_button_event(const SDL_MouseButtonEvent& e);

  static constexpr uint32_t down = SDL_MOUSEBUTTONDOWN;
  static constexpr uint32_t up = SDL_MOUSEBUTTONUP;

  static constexpr uint8_t left = SDL_BUTTON_LEFT;
  static constexpr uint8_t right = SDL_BUTTON_RIGHT;

  // button up or down
  uint32_t type() const;
  // left or right button
  uint8_t button() const;
  // distance from left border in pixels
  int32_t x() const;
  // distance from top border in pixels
  int32_t y() const;

private:
  SDL_MouseButtonEvent event_;

};

struct event {
  // try to convert generic event to some specific event
  bool extract(quit_event& event) const;
  bool extract(mouse_button_event& event) const;

  // try to get next event
  static bool poll(event& e);

private:
  SDL_Event event_;

};

} // namespace sdl

#endif // D_SDL_H_
