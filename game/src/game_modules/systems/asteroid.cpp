#include "asteroid.hpp"

#include "create_entities.hpp"
#include "game_modules/components/components.hpp"
#include "glm/glm.hpp"
#include "modules/lifecycle/components.hpp"
#include "modules/physics/components.hpp"
#include "modules/renderer/components.hpp"

#include "engine/maths/maths.hpp"

#include <imgui.h>

static float max_asteroids = 45;
static float time_between_asteroids = 0.25f;
static float time_since_last_asteroid = 0.25f;

void
game2d::update_asteroid_system(entt::registry& r)
{
  auto& rnd = r.ctx<SINGLETON_ResourceComponent>();
  auto& eb = r.ctx<SINGLETON_EntityBinComponent>();
  const auto& ri = r.ctx<SINGLETON_RendererInfo>();
  const float dt = ImGui::GetIO().DeltaTime;
  const auto& view = r.view<AsteroidComponent, VelocityComponent, TransformComponent>();

  view.each([&eb, &rnd, &dt, &ri](auto entity, auto& asteroid, auto& vel, auto& transform) {
    if (!asteroid.initialized) {
      asteroid.initialized = true;

      // Start()

      // .. random starting velocity
      const float rnd_x_vel = engine::rand_det_s(rnd.rnd.rng, -75.0f, 75.0f);
      const float rnd_y_vel = engine::rand_det_s(rnd.rnd.rng, -75.0f, 75.0f);
      vel.x = rnd_x_vel;
      vel.y = rnd_y_vel;

      // .. random starting position
      const float rnd_x_pos = engine::rand_det_s(rnd.rnd.rng, 0.0f, ri.viewport_size_render_at.x);
      const float rnd_y_pos = engine::rand_det_s(rnd.rnd.rng, 0.0f, ri.viewport_size_render_at.y);
      transform.position.x = rnd_x_pos;
      transform.position.y = rnd_y_pos;

      // .. random time alive
      const float rnd_time_alive = engine::rand_det_s(rnd.rnd.rng, 5.0f, 20.0f);
      asteroid.max_time_alive = rnd_time_alive;

      // .. random spin amount
      const float rnd_spin_amount = engine::rand_det_s(rnd.rnd.rng, 0.0f, engine::HALF_PI);
      asteroid.spin_amount = rnd_spin_amount;
    }

    // Update()

    // .. spin the asteroid
    transform.rotation.z += asteroid.spin_amount * dt;

    // .. tick the asteroid's life
    asteroid.time_alive += dt;
    if (asteroid.time_alive > asteroid.max_time_alive) {
      eb.dead.push_back(entity);
    }
  });

  // Asteroid Spawner

  size_t live_asteroids = r.view<AsteroidComponent>().size();
  time_since_last_asteroid -= dt;
  if (time_since_last_asteroid <= 0.0f && live_asteroids <= max_asteroids) {
    time_since_last_asteroid = time_between_asteroids;
    create_asteroid(r);

    // Make game more difficult
    time_between_asteroids -= 0.01f;
    time_between_asteroids = glm::max(0.0f, time_between_asteroids);

    // Make game even more difficult
    max_asteroids += 1;
  }
};