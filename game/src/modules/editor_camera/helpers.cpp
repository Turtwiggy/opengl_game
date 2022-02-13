// header
#include "modules/editor_camera/helpers.hpp"

namespace game2d {

glm::mat4
calculate_projection(int x, int y)
{
  return glm::ortho(0.0f, static_cast<float>(x), static_cast<float>(y), 0.0f, -1.0f, 1.0f);
};

// void
// update_view(CameraComponent& c)
// {
//   c.view = glm::lookAt(c.position, c.position + c.axis_forward, c.axis_up);
// };

} // namespace game2d