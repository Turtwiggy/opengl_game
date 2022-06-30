#pragma once

// engine headers
#include "engine/opengl/shader.hpp"

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

// Attributes only updated by renderer system, read by anything.
struct SINGLETON_RendererInfo
{
  // fbo
  unsigned int fbo_main_scene = 0;
  unsigned int fbo_lighting = 0;
  // shaders
  engine::Shader instanced;
  engine::Shader fan;
  // viewport
  // note: values are updated in render
  glm::ivec2 viewport_size_render_at = { 0, 0 };
  glm::ivec2 viewport_size_current = { 0, 0 };
  glm::ivec2 viewport_pos = { 0, 0 };
  bool viewport_process_events = false;

  // texture unit (slot on gpu)
  const int tex_unit_main_scene = 0;
  const int tex_unit_lighting = 1;
  // texture ids (id after bound)
  unsigned int tex_id_main_scene = 0;
  unsigned int tex_id_lighting = 0;
};

} // namespace game2d