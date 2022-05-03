// your header
#include "cycle_sprite.hpp"

// components
#include "components/components.hpp"
#include "modules/renderer/components.hpp"

#include <string>

static int temp_cycle = 0;

void
game2d::update_cycle_sprite_system(entt::registry& registry, engine::Application& app)
{
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();
  auto& input = app.get_input();

  int temp = temp_cycle;

  {
    const auto& view = registry.view<PlayerComponent>();
    view.each([&registry, &temp, &input](auto entity, const auto& player) {
      if (input.get_key_down(SDL_SCANCODE_RETURN)) {
        temp += 1;
        temp %= 7;

        std::string sprite = "SHIP_EXPLOSION_" + std::to_string(temp);
        std::cout << "cycling... " << sprite << std::endl;

        registry.emplace<SpriteTagComponent>(entity, sprite);
      }
    });
  }

  temp_cycle = temp;
};
