#include "animation_set_by_velocity.hpp"

#include "components/animation.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/sprites/components.hpp"
#include "modules/sprites/helpers.hpp"

#include <imgui.h>

namespace game2d {

void
update_animation_set_by_velocity_system(entt::registry& registry)
{
  const auto& si = registry.ctx<SINGLETON_SpriteTextures>();
  const auto& view =
    registry.view<SpriteAnimationComponent, SpriteComponent, AnimationSetByVelocityComponent, VelocityComponent>();

  view.each([&si](auto& anim, auto& sprite, const auto& asbv, const auto& vel) {
    std::string animation = "down_idle";

    float EPSILON = 0.05f;
    if (vel.x > EPSILON && vel.x > vel.y) {
      animation = "right_walk_cycle";
    } else if (vel.y > EPSILON && vel.y > vel.x) {
      animation = "down_walk_cycle";
    } else if (vel.x < -EPSILON && vel.x < vel.y) {
      animation = "left_walk_cycle";
    } else if (vel.y < -EPSILON && vel.y < vel.x) {
      animation = "up_walk_cycle";
    }

    if (animation != anim.playing_animation_name) {
      // immediately play new anim
      anim.frame = 0;
      anim.frame_dt = 0.0f;
      anim.playing_animation_name = animation;

      // set starting frame
      const auto& anim_data = find_animation(si.animations, anim.playing_animation_name);
      sprite.x = anim_data.animation_frames[0].x;
      sprite.y = anim_data.animation_frames[0].y;
    }

    ImGui::Begin("Anim");
    ImGui::Text("Velocity: %f %f", vel.x, vel.y);
    ImGui::Text("Anim: %s", animation.c_str());
    ImGui::Text("Sprite: %i %i", sprite.x, sprite.y);
    ImGui::End();
  });
}

} // namespace game2d