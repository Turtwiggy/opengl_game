// your header
#include "modules/renderer/system.hpp"

// components/systems
#include "components/app.hpp"
#include "modules/camera/components.hpp"
#include "modules/camera/helpers.hpp"
#include "modules/renderer/components.hpp"
#include "modules/renderer/helpers/batch_quad.hpp"
#include "modules/renderer/helpers/helpers.hpp"
#include "modules/sprites/components.hpp"
#include "modules/sprites/helpers.hpp"

// engine headers
#include "engine/opengl/framebuffer.hpp"
#include "engine/opengl/render_command.hpp"
#include "engine/opengl/shader.hpp"
#include "engine/opengl/texture.hpp"
#include "engine/opengl/util.hpp"
using namespace engine;

// other lib
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace game2d {

glm::mat4
calculate_projection(int x, int y)
{
  return glm::ortho(0.0f, static_cast<float>(x), static_cast<float>(y), 0.0f, -1.0f, 1.0f);
};

void
rebind(entt::registry& registry, const glm::ivec2& wh)
{
  const auto& si = registry.ctx<SINGLETON_SpriteTextures>();
  auto& ri = registry.ctx<SINGLETON_RendererInfo>();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, ri.tex_id_main_scene);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, ri.tex_id_lighting);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, si.tex_id_kenny);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, si.tex_id_custom);
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, si.tex_id_sprout);

  int textures[5] = { static_cast<int>(ri.tex_unit_main_scene),
                      static_cast<int>(ri.tex_unit_lighting),
                      static_cast<int>(si.tex_unit_kenny),
                      static_cast<int>(si.tex_unit_custom),
                      static_cast<int>(si.tex_unit_sprout) };
  ri.instanced.bind();
  ri.instanced.set_mat4("projection", calculate_projection(wh.x, wh.y));
  ri.instanced.set_int_array("textures", textures, 5);
};

}; // namespace game2d

void
game2d::init_render_system(entt::registry& registry, const glm::ivec2& screen_wh)
{
  Framebuffer::default_fbo();

  SINGLETON_RendererInfo ri;

  new_texture_to_fbo(ri.fbo_lighting, ri.tex_id_lighting, ri.tex_unit_lighting, screen_wh);
  new_texture_to_fbo(ri.fbo_main_scene, ri.tex_id_main_scene, ri.tex_unit_main_scene, screen_wh);
  ri.instanced = Shader("assets/shaders/2d_instanced.vert", "assets/shaders/2d_instanced.frag");
  ri.fan = Shader("assets/shaders/2d_basic_with_proj.vert", "assets/shaders/2d_colour.frag");
  ri.viewport_size_render_at = screen_wh;
  ri.viewport_size_current = screen_wh;

  // initialize renderer
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  print_gpu_info();
  quad_renderer::QuadRenderer::init();

  registry.set<SINGLETON_RendererInfo>(ri);

  game2d::rebind(registry, screen_wh);
};

void
game2d::update_render_system(entt::registry& registry)
{
  auto& ri = registry.ctx<SINGLETON_RendererInfo>();
  auto& colours = registry.ctx<SINGLETON_ColoursComponent>();
  const glm::vec4& background_colour = colours.background;
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
    game2d::rebind(registry, viewport_wh);
  }

  // MAIN FBO
  Framebuffer::bind_fbo(ri.fbo_main_scene);
  RenderCommand::set_viewport(0, 0, viewport_wh.x, viewport_wh.y);
  RenderCommand::set_clear_colour(background_colour);
  RenderCommand::clear();

  // Do quad stuff
  {
    quad_renderer::QuadRenderer::reset_quad_vert_count();
    quad_renderer::QuadRenderer::begin_batch();

    // TODO: work out z-index
    // registry.sort<ZIndex>([](const auto& lhs, const auto& rhs) { return lhs.index < rhs.index; });

    quad_renderer::RenderDescriptor desc;
    const auto& view = registry.view<const TransformComponent, const SpriteComponent>();

    // registry.sort<renderable>([](const auto& lhs, const auto& rhs) { return lhs.z < rhs.z; });

    const auto& camera = get_main_camera(registry);
    const auto& camera_transform = registry.get<TransformComponent>(camera);

    view.each([&registry, &ri, &desc, &camera_transform](auto eid, const auto& transform, const auto& sc) {
      desc.pos_tl = camera_transform.position + transform.position - transform.scale / 2;
      desc.size = transform.scale;
      desc.angle_radians = sc.angle + transform.rotation.z;
      desc.colour = sc.colour;
      desc.tex_unit = sc.tex_unit;
      desc.sprite_offset = { sc.x, sc.y };
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
