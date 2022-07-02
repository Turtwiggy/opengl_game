// header
#include "system.hpp"

// my libs
#include "modules/sprites/helpers.hpp"

// engine
#include "engine/opengl/texture.hpp"

#include <imgui.h> // temp

namespace game2d {

void
init_sprite_system(entt::registry& registry)
{
  SINGLETON_SpriteTextures textures;

  // load textures
  std::vector<std::pair<int, std::string>> textures_to_load;
  textures_to_load.emplace_back(textures.tex_unit_kenny, textures.sheet_kenny);
  textures_to_load.emplace_back(textures.tex_unit_custom, textures.sheet_custom);
  textures_to_load.emplace_back(textures.tex_unit_sprout, textures.sheet_sprout);

  auto tex_ids = engine::load_textures_threaded(textures_to_load);
  textures.tex_id_kenny = tex_ids[0];
  textures.tex_id_custom = tex_ids[1];
  textures.tex_id_sprout = tex_ids[2];

  load_sprite_yml(textures.animations, textures.yml_kenny);
  load_sprite_yml(textures.animations, textures.yml_custom);
  load_sprite_yml(textures.animations, textures.yml_sprout);

  registry.set<SINGLETON_SpriteTextures>(textures);
}

void
update_sprite_system(entt::registry& registry, float dt)
{
  const auto& si = registry.ctx<SINGLETON_SpriteTextures>();

  const auto& view = registry.view<SpriteComponent, SpriteAnimationComponent>();
  view.each([&registry, &si, &dt](auto& sprite, auto& animation) {
    //
    SpriteAnimation current_animation = find_animation(si.animations, animation.playing_animation_name);

    int frames = current_animation.animation_frames.size();

    animation.frame_dt += dt;
    if (animation.frame_dt >= current_animation.animation_frames_per_second) {
      // next frame!
      animation.frame_dt -= current_animation.animation_frames_per_second;
      animation.frame += 1;
      animation.frame %= frames;

      sprite.x = current_animation.animation_frames[animation.frame].x;
      sprite.y = current_animation.animation_frames[animation.frame].y;
    }
  });
};

} // namespace game2d