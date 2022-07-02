#pragma once

// engine
#include "engine/colour.hpp"

// other libs
#include <entt/entt.hpp>
#include <glm/glm.hpp>

// std lib
#include <string>

namespace game2d {

void
create_empty(entt::registry& r);

void
create_camera(entt::registry& r, int x, int y);

entt::entity
create_renderable(entt::registry& r, const std::string& name, const engine::SRGBColour& colour);

void
create_cursor(entt::registry& registry);

entt::entity
create_unit_group(entt::registry& registry,
                  int x,
                  int y,
                  int size_x,
                  int size_y,
                  const std::string& name,
                  const engine::SRGBColour& start_colour,
                  const engine::SRGBColour& highlight_colour);

entt::entity
create_unit(entt::registry& registry, const std::string& name, const engine::SRGBColour& colour);

void
create_debug_square(entt::registry& registry);

} // namespace game2d