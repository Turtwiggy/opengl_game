#pragma once

// helpers
#include "modules/sprites/components.hpp"

// engine headers
#include "engine/opengl/shader.hpp"

// c++ lib headers
#include <vector>

// other
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <nlohmann/json.hpp>

namespace game2d {

struct TagComponent
{
  std::string tag;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(TagComponent, tag)
};

struct TransformComponent
{
  glm::ivec3 position{ 0, 0, 0 };
  glm::vec3 position_dxdy{ 0.0f, 0.0f, 0.0f };

  glm::vec3 rotation = { 0, 0, 0 };

  glm::ivec3 scale{ 0, 0, 0 };
  glm::vec3 scale_dxdy{ 0.0f, 0.0f, 0.0f };

  // quat(glm::vec3(90, 45, 0))
  // gtx::quaternion::angleAxis(degrees(RotationAngle), RotationAxis);
  // glm::quaternion::toMat4(quaternion);
  // mat4 RotationMatrix = quaternion::toMat4(quaternion);
  // mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
};

struct ColourComponent
{
  glm::vec4 colour = { 1.0f, 1.0f, 1.0f, 1.0f };

  ColourComponent() = default;
  ColourComponent(float r, float g, float b, float a)
  {
    colour.x = r;
    colour.y = g;
    colour.z = b;
    colour.a = a;
  };
  ColourComponent(const ColourComponent& c)
  {
    colour.x = c.colour.x;
    colour.y = c.colour.y;
    colour.z = c.colour.z;
    colour.a = c.colour.a;
  };
  ColourComponent(const glm::vec4& c)
  {
    colour.x = c.x;
    colour.y = c.y;
    colour.z = c.z;
    colour.a = c.a;
  };

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ColourComponent, colour.x, colour.y, colour.z, colour.a)
};

struct TextureComponent
{
  int tex_unit = 100;

  // do not serialize
};

// Attributes only updated by renderer system, read by anything.
struct SINGLETON_RendererInfo
{
  // sprites
  std::vector<sprite> sprites;
  // fbo
  unsigned int fbo_main_scene = 0;
  unsigned int fbo_lighting = 0;
  // shaders
  engine::Shader instanced;
  engine::Shader fan;
  // texture unit (slot on gpu)
  const int tex_unit_main_scene = 0;
  const int tex_unit_lighting = 1;
  const int tex_unit_kenny_nl = 2;
  const int tex_unit_custom_spaceships = 3;
  // texture ids (id after bound)
  unsigned int tex_id_main_scene = 0;
  unsigned int tex_id_lighting = 0;
  unsigned int tex_id_kenny = 0;
  unsigned int tex_id_custom = 0;
  // viewport
  // note: values are updated in render
  glm::ivec2 viewport_size_render_at = { 0, 0 };
  glm::ivec2 viewport_size_current = { 0, 0 };
  glm::ivec2 viewport_pos = { 0, 0 };
  bool viewport_process_events = false;
};

} // namespace game2d