#pragma once

// game2d headers
#include "modules/renderer/helpers/renderers/batch_triangle.hpp"

// other lib headers
#include "ecs.hpp"

// c++ lib headers
#include <vector>

namespace game2d {

struct RenderHexagons : public engine::triangle_renderer::ITriangleRenderable
{
  std::vector<engine::triangle_renderer::TriangleDescriptor> get_triangles(game2d::Coordinator& registry);
};

} // namespace game2d