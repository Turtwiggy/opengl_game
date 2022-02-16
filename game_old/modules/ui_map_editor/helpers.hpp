#pragma once

// other lib headers
#include "ecs.hpp"

// c++ lib headers
#include <iostream>

namespace game2d {
//

void
serialize_to_text(game2d::Coordinator& registry, std::string path);

void
deserialize_text_to_registry(game2d::Coordinator& registry, std::string path);

} // namespace game2d