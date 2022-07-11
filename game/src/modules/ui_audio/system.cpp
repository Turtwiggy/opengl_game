// header
#include "system.hpp"

#include "resources/audio.hpp"

// other lib headers
#include <imgui.h>

void
game2d::update_ui_audio_system(entt::registry& registry)
{
  const auto& audio = registry.ctx<SINGLETON_AudioComponent>();

  // Profiler
  ImGui::Begin("Audio ", NULL, ImGuiWindowFlags_NoFocusOnAppearing);
  {
    ImGui::Button("(todo) Play Sound A");
    ImGui::Button("(todo) Play Sound B");
    ImGui::Button("(todo) Play Music A");
    ImGui::Button("(todo) Play Music B");
  }
  ImGui::End();
};