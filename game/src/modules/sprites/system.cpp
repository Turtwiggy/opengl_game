// header
#include "system.hpp"

// my libs
#include "modules/renderer/components.hpp"
#include "modules/sprites/helpers.hpp"

// engine
#include "engine/opengl/texture.hpp"

// std libs
#include <iostream>

namespace game2d {

void
init_sprite_system(entt::registry& registry)
{
  SINGLETON_SpriteTextures textures;

  // load textures
  std::vector<std::pair<int, std::string>> textures_to_load;
  textures_to_load.emplace_back(textures.tex_unit_kenny_nl, textures.sheet_kenny);
  textures_to_load.emplace_back(textures.tex_unit_custom_spaceships, textures.sheet_custom);
  textures_to_load.emplace_back(textures.tex_unit_sprout_lands, textures.sheet_sprout);

  auto tex_ids = engine::load_textures_threaded(textures_to_load);
  textures.tex_id_custom = tex_ids[0];
  textures.tex_id_kenny = tex_ids[1];
  textures.tex_id_sprout_lands = tex_ids[1];

  load_sprite_yml(textures.animations, textures.yml_kenny);
  load_sprite_yml(textures.animations, textures.yml_custom);
  load_sprite_yml(textures.animations, textures.yml_sprout);

  registry.set<SINGLETON_SpriteTextures>(textures);
}

void
update_sprite_system(entt::registry& registry)
{
  const auto& ri = registry.ctx<SINGLETON_RendererInfo>();

  // const auto& view = registry.view<SpriteTagComponent>();
  // view.each([&registry, &ri](auto entity, const auto& sprite_tag) {
  //   sprite sprite_info = find_sprite(textures.sprites, sprite_tag.tag);

  //   if (registry.all_of<SpriteSlotComponent>(entity)) {
  //     // A sprite component already exists, and a tag component was found
  //     // update the sprite component, and delete the tag component.
  //     SpriteSlotComponent& sc = registry.get<SpriteSlotComponent>(entity);
  //     sc.x = sprite_info.x;
  //     sc.y = sprite_info.y;
  //     sc.offset = sprite_info.angle;
  //     std::cout << "updating sprite component..." << std::endl;
  //   } else {
  //     // create a new sprite component, and delete the tag component
  //     SpriteSlotComponent sc;
  //     sc.x = sprite_info.x;
  //     sc.y = sprite_info.y;
  //     sc.offset = sprite_info.angle;
  //     registry.emplace<SpriteSlotComponent>(entity, sc);
  //   }

  //   // std::cout << "spritetag: " << sprite_info.name << ", ";
  //   registry.erase<SpriteTagComponent>(entity);
  // });
};

} // namespace game2d