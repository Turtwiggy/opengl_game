// header
#include "system.hpp"

// my libs
#include "modules/sprites/helpers.hpp"
#include "resources/textures.hpp"

// engine
#include "engine/opengl/texture.hpp"

namespace game2d {

void
init_sprite_system(entt::registry& registry)
{
  auto& tex = registry.ctx<SINGLETON_Textures>();
  auto& anim = registry.set<SINGLETON_Animations>();

  // load textures
  std::vector<std::pair<int, std::string>> textures_to_load;
  textures_to_load.emplace_back(tex.tex_unit_kenny, tex.sheet_kenny);
  textures_to_load.emplace_back(tex.tex_unit_custom, tex.sheet_custom);
  textures_to_load.emplace_back(tex.tex_unit_sprout, tex.sheet_sprout);

  auto tex_ids = engine::load_textures_threaded(textures_to_load);
  tex.tex_id_kenny = tex_ids[0];
  tex.tex_id_custom = tex_ids[1];
  tex.tex_id_sprout = tex_ids[2];

  // load animations

  load_sprite_yml(anim.animations, tex.yml_kenny);
  load_sprite_yml(anim.animations, tex.yml_custom);
  load_sprite_yml(anim.animations, tex.yml_sprout);
}

void
update_sprite_system(entt::registry& registry, float dt)
{
  const auto& anim = registry.ctx<SINGLETON_Animations>();

  const auto& view = registry.view<SpriteComponent, SpriteAnimationComponent>();
  view.each([&registry, &anim, &dt](auto& sprite, auto& animation) {
    //
    SpriteAnimation current_animation = find_animation(anim.animations, animation.playing_animation_name);

    if (!animation.playing)
      return;

    int frames = current_animation.animation_frames.size();

    animation.frame_dt += dt * animation.speed;
    if (animation.frame_dt >= current_animation.animation_frames_per_second) {
      // next frame!
      animation.frame_dt -= current_animation.animation_frames_per_second;
      animation.frame += 1;

      if (animation.frame >= frames && !animation.looping)
        animation.playing = false;

      animation.frame %= frames;
      sprite.x = current_animation.animation_frames[animation.frame].x;
      sprite.y = current_animation.animation_frames[animation.frame].y;
    }
  });
};

} // namespace game2d