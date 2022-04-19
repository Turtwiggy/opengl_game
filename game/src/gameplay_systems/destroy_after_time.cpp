// your header
#include "destroy_after_time.hpp"

// components
#include "gameplay_components/components.hpp"

#include <imgui.h>

void
game2d::update_destroy_after_time_system(entt::registry& registry, engine::Application& app, float dt)
{
  const auto& view = registry.view<DestroyAfterTimeComponent>();
  view.each([&registry, &dt](const auto entity, auto& time) {
    time.time -= dt;
    if (time.time <= 0.0f)
      registry.destroy(entity);
  });
};
