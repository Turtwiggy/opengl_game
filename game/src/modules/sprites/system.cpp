// header
#include "system.hpp"

// my libs
#include "helpers.hpp"
#include "modules/renderer/components.hpp"

// std libs
#include <iostream>

namespace game2d {

void
update_sprite_tag_system(entt::registry& registry, engine::Application& app)
{
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();

  const auto& view = registry.view<SpriteTagComponent>();
  view.each([&registry, &ri](auto entity, const auto& sprite_tag) {
    sprite sprite_info = find_sprite(ri.sprites, sprite_tag.tag);

    if (registry.all_of<SpriteComponent>(entity)) {
      // A sprite component already exists, and a tag component was found
      // update the sprite component, and delete the tag component.
      SpriteComponent& sc = registry.get<SpriteComponent>(entity);
      sc.x = sprite_info.x;
      sc.y = sprite_info.y;
      sc.offset = sprite_info.angle;
      std::cout << "updating sprite component..." << std::endl;
    } else {
      // create a new sprite component, and delete the tag component
      SpriteComponent sc;
      sc.x = sprite_info.x;
      sc.y = sprite_info.y;
      sc.offset = sprite_info.angle;
      registry.emplace<SpriteComponent>(entity, sc);
    }

    std::cout << "replacing sprite tag...";
    registry.erase<SpriteTagComponent>(entity);
  });
};

} // namespace game2d