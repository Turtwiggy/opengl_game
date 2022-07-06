// header
#include "system.hpp"

// my lib
#include "modules/events/components.hpp"

// other lib
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_events.h>

// std lib
#include <iostream>

namespace game2d {

void
init_audio_system(entt::registry& registry){};

// https://wiki.libsdl.org/CategoryAudio
// https://wiki.libsdl.org/SDL_AudioSpec
void
update_audio_system(entt::registry& registry)
{
  const auto& events = registry.ctx<SINGLETON_InputComponent>();

  auto audio_added = std::find_if(
    events.sdl_events.begin(), events.sdl_events.end(), [](const auto& e) { return e.type == SDL_AUDIODEVICEADDED; });
  auto audio_removed = std::find_if(
    events.sdl_events.begin(), events.sdl_events.end(), [](const auto& e) { return e.type == SDL_AUDIODEVICEREMOVED; });

  // audio
  if (audio_added != events.sdl_events.end()) {
    const int count = SDL_GetNumAudioDevices(0);
    std::cout << "(audio device added) Audio Devices: " << count << std::endl;
  }
  if (audio_removed != events.sdl_events.end()) {
    const int count = SDL_GetNumAudioDevices(0);
    std::cout << "(audio device removed). Audio devices: " << count << std::endl;
  }
};

} // namespace game2d