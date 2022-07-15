#pragma once

#include "resources/audio.hpp"

#include <SDL2/SDL_mixer.h>

#include <map>
#include <string>

namespace game2d {

struct SINGLETON_AudioComponent
{
  // audio device

  SDL_AudioDeviceID dev;

  // sounds

  const std::string sound_path = "assets/audio/MAGIC_SPELL_Attacking_Climbing_Bells_stereo_editedf.wav";
  Mix_Chunk* sound;
};

} // namespace game2d