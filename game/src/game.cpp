
// header
#include "game.hpp"

// components
#include "components.hpp"
#include "modules/renderer/components.hpp"
#include "modules/ui_profiler/components.hpp"

namespace game2d {

Game::Game(const glm::vec2& start_screen_wh)
{
  renderer_info = init_render_system(start_screen_wh);

  // .. register components
  registry.register_component<TagComponent>();
  registry.register_component<PositionIntComponent>();
  registry.register_component<RenderSizeComponent>();
  registry.register_component<ColourComponent>();
  registry.register_component<SpriteComponent>();
  registry.register_component<CursorComponent>();
  registry.register_component<ProfilerComponent>();

  // .. register systems
  renderer = registry.register_system<RenderSpriteSystem>();
  {
    Signature signature;
    signature.set(registry.get_component_type<TagComponent>());
    signature.set(registry.get_component_type<PositionIntComponent>());
    signature.set(registry.get_component_type<RenderSizeComponent>());
    signature.set(registry.get_component_type<ColourComponent>());
    signature.set(registry.get_component_type<SpriteComponent>());
    registry.set_system_signature<RenderSpriteSystem>(signature);
  }
  profiler = registry.register_system<ProfilerSystem>();
  {
    Signature signature;
    signature.set(registry.get_component_type<ProfilerComponent>());
    registry.set_system_signature<ProfilerSystem>(signature);
  }
  cursor = registry.register_system<CursorSystem>();
  {
    Signature signature;
    signature.set(registry.get_component_type<TagComponent>());
    signature.set(registry.get_component_type<PositionIntComponent>());
    signature.set(registry.get_component_type<RenderSizeComponent>());
    signature.set(registry.get_component_type<ColourComponent>());
    signature.set(registry.get_component_type<SpriteComponent>());
    signature.set(registry.get_component_type<CursorComponent>());
    registry.set_system_signature<CursorSystem>(signature);
  }
};

void
Game::init()
{
  const SINGLETON_GridSizeComponent grid = SINGLETON_GridSizeComponent();
  const int GRID_SIZE = grid.size_xy;

  ProfilerSystem::init(registry);

  // colours
  const glm::vec4 colour_red = glm::vec4(232 / 255.0f, 80 / 255.0f, 100 / 255.0f, 1.0f);
  const glm::vec4 colour_cyan = glm::vec4(8 / 255.0f, 177 / 255.0f, 190 / 255.0f, 1.0f);
  const glm::vec4 colour_dblue = glm::vec4(49 / 255.0f, 99 / 255.0f, 188 / 255.0f, 1.0f);
  const glm::vec4 colour_white = glm::vec4(1.0f);
  const glm::vec4 colour_green = glm::vec4(100 / 255.0f, 188 / 255.0f, 49 / 255.0f, 1.0f);

  // Add a cursor, made of 4 lines
  {
    for (int i = 0; i < 4; i++) {
      auto r = registry.create_entity();
      registry.add_component<TagComponent>(r, TagComponent{ std::string("cursor" + std::to_string(i)) });
      // rendering
      registry.add_component<ColourComponent>(r, ColourComponent(colour_red));
      registry.add_component<PositionIntComponent>(r, PositionIntComponent{});
      registry.add_component<RenderSizeComponent>(r, RenderSizeComponent{ GRID_SIZE, GRID_SIZE });
      registry.add_component<SpriteComponent>(r, SpriteComponent{ sprite::type::EMPTY });
      // gameplay
      registry.add_component<CursorComponent>(r, CursorComponent{ i });
    }
  }

  // Add some blocks
  {
    for (int i = 1; i < 30; i++) {
      auto r = registry.create_entity();
      registry.add_component<TagComponent>(r, TagComponent{ std::string("a-blocks" + std::to_string(i)) });
      // rendering
      registry.add_component<ColourComponent>(r, ColourComponent(colour_dblue));
      registry.add_component<PositionIntComponent>(r, PositionIntComponent{ (22 + i) * GRID_SIZE, 35 * GRID_SIZE });
      registry.add_component<RenderSizeComponent>(r, RenderSizeComponent{ GRID_SIZE, GRID_SIZE });
      registry.add_component<SpriteComponent>(r, SpriteComponent{ sprite::type::EMPTY });
    }
  }

  // // Add a player
  // {
  //   entt::entity r = registry.create();
  //   registry.emplace<TagComponent>(r, "player0");
  //   // rendering
  //   registry.emplace<ColourComponent>(r, colour_cyan);
  //   registry.emplace<PositionIntComponent>(r, 25 * GRID_SIZE, 25 * GRID_SIZE);
  //   registry.emplace<RenderSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  //   registry.emplace<SpriteComponent>(r, sprite::type::PERSON_0);
  //   // physics
  //   registry.emplace<CollidableComponent>(r, static_cast<uint32_t>(GameCollisionLayer::ACTOR_PLAYER));
  //   registry.emplace<PhysicsSizeComponent>(r, GRID_SIZE, GRID_SIZE);
  //   registry.emplace<VelocityComponent>(r, 0.0f, 0.0f);
  //   // gameplay
  //   registry.emplace<AnimationBounce>(r);
  //   registry.emplace<Player>(r);
  //   registry.emplace<HealthComponent>(r);
  //   registry.emplace<DoubleJumpComponent>(r);
  // }

  // init once only
  // init_audio_system(registry);
  // init_ui_profiler_system(registry);
  // init_ui_hierarchy_system(registry);
  // init_ui_map_editor_system(registry);
  // could be deleted and re-init at any time
  // init_game_state(registry);
  // temp
  // app.get_input().open_controllers();
};

void
Game::update(Application& app, float dt)
{
  SINGLETON_GridSizeComponent grid;

  //   // Profiler& p = registry.ctx<Profiler>();

  //   // SINGLETON_GamePausedComponent& gp = registry.ctx<SINGLETON_GamePausedComponent>();
  //   // if (app.get_input().get_key_down(SDL_SCANCODE_P)) {
  //   //   gp.paused = !gp.paused;
  //   //   std::cout << "game paused: " << gp.paused << std::endl;
  //   // }

#if defined _DEBUG || defined DEBUG
  if (app.get_input().get_key_down(SDL_SCANCODE_R)) {
    // init_game_state(registry);
  }
  if (app.get_input().get_key_down(SDL_SCANCODE_ESCAPE)) {
    app.shutdown();
  }
#endif

  //   // physics
  //   Uint64 start_physics = SDL_GetPerformanceCounter();
  //   {
  //     // if (!gp.paused) {
  //     //   // // move objects, checking collisions along way
  //     //   // update_move_objects_system(registry, app, dt);
  //     //   // // generate all collisions between actor-actor objects
  //     //   // update_physics_system(registry, app, dt);
  //     //   // // process actor-actor collisions
  //     //   // update_actor_actor_collision_system(registry, app, dt);
  //     // }
  //   }
  //   Uint64 end_physics = SDL_GetPerformanceCounter();
  //   // p.physics_elapsed_ms = (end_physics - start_physics) / float(SDL_GetPerformanceFrequency()) * 1000.0f;

  // game logic
  Uint64 start_game_tick = SDL_GetPerformanceCounter();
  {
    // if (!gp.paused) {
    cursor->update(registry, app, renderer_info, grid);
    //   // update_animation_system(registry, app, dt);
    //   // // update_audio_system(registry, app, dt);
    //   // update_player_input_system(registry, app);
    //   // update_click_to_destroy_system(registry, app);
    //   // update_velocity_in_boundingbox_system(registry, app, dt);
    //   // update_parry_system(registry, app, dt);
    //   // // editor stuff
    //   // update_editor_camera_system(registry, app, dt);
    //   // update_map_editor_system(registry, app, dt);
    //   // // destroy any entities
    //   // update_destroy_after_time_system(registry, app, dt);
    // }
  }
  Uint64 end_game_tick = SDL_GetPerformanceCounter();
  //   // p.game_tick_elapsed_ms = (end_game_tick - start_game_tick) / float(SDL_GetPerformanceFrequency()) * 1000.0f;

  // rendering
  Uint64 start_render = SDL_GetPerformanceCounter();
  {
    renderer->update(registry, renderer_info);
  }
  Uint64 end_render = SDL_GetPerformanceCounter();
  //   // p.render_elapsed_ms = (end_render - start_render) / float(SDL_GetPerformanceFrequency()) * 1000.0f;

  // ui
  {
    profiler->update(registry, app);
    // update_ui_physics_system(registry, app);
    // update_ui_hierarchy_system(registry, app);
    // // update_ui_gizmos_system(registry, app, dt); // update after hierarchy
    // update_ui_map_editor_system(registry, app, dt);
  };

  // end frame
  {
    end_frame_render_system();
  }
};

} // namespace game2d