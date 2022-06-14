#pragma once

// other libs
#include <entt/entt.hpp>
#include <glm/glm.hpp>

// std lib
#include <string>

namespace game2d {

void
create_camera(entt::registry& r, int x, int y);

void
create_cursor(entt::registry& registry);

void
create_objective(entt::registry& r, int x, int y, int size_x, int size_y, const std::string& sprite);

void
create_unit(entt::registry& registry,
            int x,
            int y,
            int size_x,
            int size_y,
            const std::string& name,
            const std::string& sprite,
            const glm::vec4& start_colour,
            const glm::vec4& highlight_colour);

void
create_debug_square(entt::registry& registry);

} // namespace game2d