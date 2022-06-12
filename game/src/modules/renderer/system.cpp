// your header
#include "modules/renderer/system.hpp"

// components/systems
#include "modules/renderer/components.hpp"
#include "modules/renderer/helpers/batch_quad.hpp"
#include "modules/renderer/helpers/helpers.hpp"
#include "modules/sprites/components.hpp"
#include "modules/sprites/helpers.hpp"
#include "modules/camera/components.hpp"

// engine headers
#include "engine/app/io.hpp"
#include "engine/opengl/framebuffer.hpp"
#include "engine/opengl/render_command.hpp"
#include "engine/opengl/shader.hpp"
#include "engine/opengl/texture.hpp"
#include "engine/opengl/util.hpp"
using namespace engine;

// other lib
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// c++ lib
#include <vector>

namespace game2d {

glm::mat4
calculate_projection(int x, int y)
{
  return glm::ortho(0.0f, static_cast<float>(x), static_cast<float>(y), 0.0f, -1.0f, 1.0f);
};

void
rebind(engine::Shader& shader, const glm::ivec2& wh, const SINGLETON_RendererInfo& ri)
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, ri.tex_id_main_scene);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, ri.tex_id_lighting);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, ri.tex_id_kenny);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, ri.tex_id_custom);

  int textures[4] = { static_cast<int>(tex_unit_main_scene),
                      static_cast<int>(tex_unit_lighting),
                      static_cast<int>(tex_unit_kenny_nl),
                      static_cast<int>(tex_unit_custom_spaceships) };
  shader.bind();
  shader.set_mat4("projection", calculate_projection(wh.x, wh.y));
  shader.set_int_array("textures", textures, 4);
};

}; // namespace game2d

void
game2d::init_render_system(entt::registry& registry, const glm::ivec2& screen_wh)
{
  Framebuffer::default_fbo();

  // load textures
  std::vector<std::pair<int, std::string>> textures_to_load;
  textures_to_load.clear();
  textures_to_load.emplace_back(tex_unit_kenny_nl,
                                "assets/textures/kennynl_1bit_pack/monochrome_transparent_packed.png");
  auto tex_kenny = load_textures_threaded(textures_to_load);

  textures_to_load.clear();
  textures_to_load.emplace_back(tex_unit_custom_spaceships, "assets/textures/custom_spaceships.png");
  auto tex_custom = load_textures_threaded(textures_to_load);

  SINGLETON_RendererInfo ri;
  ri.tex_id_custom = tex_custom[0];
  ri.tex_id_kenny = tex_kenny[0];

  new_texture_to_fbo(ri.fbo_lighting, ri.tex_id_lighting, tex_unit_lighting, screen_wh);
  new_texture_to_fbo(ri.fbo_main_scene, ri.tex_id_main_scene, tex_unit_main_scene, screen_wh);
  ri.instanced = Shader("assets/shaders/2d_instanced.vert", "assets/shaders/2d_instanced.frag");
  ri.fan = Shader("assets/shaders/2d_basic_with_proj.vert", "assets/shaders/2d_colour.frag");
  ri.viewport_size_render_at = screen_wh;
  ri.viewport_size_current = screen_wh;

  // load sprite configs
  std::string config_path = "assets/config/";
  std::string tex_0_sprite_config = config_path + std::string("spritemap_kennynl.yml");
  std::string tex_1_sprite_config = config_path + std::string("spritemap_custom.yml");
  load_sprite_yml(ri.sprites, tex_0_sprite_config);
  load_sprite_yml(ri.sprites, tex_1_sprite_config);

  // set shader info
  rebind(ri.instanced, screen_wh, ri);

  // initialize renderers
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  print_gpu_info();
  quad_renderer::QuadRenderer::init();

  // textures
  game2d::rebind(ri.instanced, screen_wh, ri);

  registry.set<SINGLETON_RendererInfo>(ri);
};

void
game2d::update_render_system(entt::registry& registry, engine::Application& app)
{
  SINGLETON_RendererInfo& ri = registry.ctx<SINGLETON_RendererInfo>();
  const glm::vec4 background_colour = glm::vec4(12.0f / 255.0f, 15.0f / 255.0f, 22.0f / 255.0f, 1.0f);
  auto viewport_wh = ri.viewport_size_render_at;

#ifdef _DEBUG
  // CHECK_OPENGL_ERROR(0);
  // DEBUG: hot reload shader
  // if (app.get_input().get_key_down(SDL_SCANCODE_T)) {
  //   ri.instanced.reload();
  //   rebind(ri.instanced, viewport_wh, ri);
  // }
#endif

  // Resize
  if (ri.viewport_size_current.x > 0.0f && ri.viewport_size_current.y > 0.0f &&
      (viewport_wh.x != ri.viewport_size_current.x || viewport_wh.y != ri.viewport_size_current.y)) {
    ri.viewport_size_render_at = ri.viewport_size_current;
    viewport_wh = ri.viewport_size_render_at;

    // update texture
    bind_tex(ri.tex_id_main_scene);
    update_bound_texture_size(viewport_wh);
    unbind_tex();
    RenderCommand::set_viewport(0, 0, viewport_wh.x, viewport_wh.y);
    rebind(ri.instanced, viewport_wh, ri);
  }

  // MAIN FBO
  Framebuffer::bind_fbo(ri.fbo_main_scene);
  RenderCommand::set_viewport(0, 0, viewport_wh.x, viewport_wh.y);
  RenderCommand::set_clear_colour(background_colour);
  RenderCommand::clear();

  rebind(ri.instanced, viewport_wh, ri);

  // Do quad stuff
  {
    quad_renderer::QuadRenderer::reset_quad_vert_count();
    quad_renderer::QuadRenderer::begin_batch();

    // TODO: work out z-index
    // registry.sort<ZIndex>([](const auto& lhs, const auto& rhs) { return lhs.index < rhs.index; });

    quad_renderer::RenderDescriptor desc;
    const auto& view = registry.view<const PositionIntComponent,
                                     const RenderSizeComponent,
                                     const ColourComponent,
                                     const SpriteComponent,
                                     const TextureComponent>();

    const auto& cameras = registry.view<const CameraComponent, const PositionIntComponent>();
    const auto& main_camera = cameras.front();
    const auto& main_camera_position = registry.get<PositionIntComponent>(main_camera);

    view.each([&registry, &ri, &desc, &main_camera_position](
                auto eid, const auto& p, const auto& s, const auto& c, const auto& sc, const auto& tex) {
      float angle_radians = sc.offset;
      if (registry.all_of<RenderAngleComponent>(eid))
        angle_radians += registry.get<RenderAngleComponent>(eid).angle_radians;
      desc.pos_tl = { main_camera_position.x + p.x - int(s.w / 2.0f), main_camera_position.y + p.y - int(s.h / 2.0f) };
      desc.colour = c.colour;
      desc.size = { s.w, s.h };
      desc.tex_unit = tex.tex_unit;
      desc.sprite_offset = { sc.x, sc.y };
      desc.angle_radians = angle_radians;
      quad_renderer::QuadRenderer::draw_sprite(desc, ri.instanced);
    });

    quad_renderer::QuadRenderer::end_batch();
    quad_renderer::QuadRenderer::flush(ri.instanced);
  }

  // default fbo
  Framebuffer::default_fbo();
  RenderCommand::set_viewport(0, 0, viewport_wh.x, viewport_wh.y);
  RenderCommand::set_clear_colour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  RenderCommand::clear();

  // Note: ImGui::Image takes in TexID not TexUnit
  ViewportInfo vi = render_texture_to_imgui_viewport(ri.tex_id_main_scene);
  // If the viewport moves - viewport position will be a frame behind.
  // This would mainly affect an editor, a game viewport probably(?) wouldn't move that much
  // (or if a user is moving the viewport, they likely dont need that one frame?)
  ri.viewport_pos = glm::vec2(vi.pos.x, vi.pos.y);
  ri.viewport_size_current = { vi.size.x, vi.size.y };
  ri.viewport_process_events = vi.focused && vi.hovered;
};

void
game2d::end_frame_render_system(entt::registry& registry)
{
  quad_renderer::QuadRenderer::end_frame();
};
