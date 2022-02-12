// your header
#include "systems/destroy_after_time.hpp"

// components
#include "game_components.hpp"

#include <imgui.h>

void
game2d::update_destroy_after_time_system(game2d::Coordinator& registry, engine::Application& app, float dt)
{
  const auto& view = registry.view<DestroyAfterTimeComponent>();
  view.each([&registry, &dt](const auto entity, auto& time) {
    time.time -= dt;
    if (time.time <= 0.0f)
      registry.destroy(entity);
  });
};
