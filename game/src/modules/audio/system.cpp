// header
#include "system.hpp"

// my lib
#include "modules/audio/components.hpp"
#include "modules/events/components.hpp"

// other lib
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_events.h>

// std lib
#include <iostream>

namespace game2d {

void
init_audio_system(entt::registry& registry)
{
  SINGLETON_AudioComponent audio;

  // todo: some fun things

  registry.set<SINGLETON_AudioComponent>(audio);
};

// https://wiki.libsdl.org/CategoryAudio
// https://wiki.libsdl.org/SDL_AudioSpec
void
update_audio_system(entt::registry& registry)
{
  const auto& events = registry.ctx<SINGLETON_InputComponent>();
  auto& audio = registry.ctx<SINGLETON_AudioComponent>();

  auto audio_added = std::find_if(
    events.sdl_events.begin(), events.sdl_events.end(), [](const auto& e) { return e.type == SDL_AUDIODEVICEADDED; });
  auto audio_removed = std::find_if(
    events.sdl_events.begin(), events.sdl_events.end(), [](const auto& e) { return e.type == SDL_AUDIODEVICEREMOVED; });

  if (audio_added != events.sdl_events.end()) {
    const int count = SDL_GetNumAudioDevices(0);
    std::cout << "(audio device added) Audio Devices: " << count << std::endl;
  }

  // Note: this event is only fired if the device was in use by sdl.
  if (audio_removed != events.sdl_events.end()) {
    const int count = SDL_GetNumAudioDevices(0);
    std::cout << "(audio device removed). Audio devices: " << count << std::endl;
  }
};

} // namespace game2d