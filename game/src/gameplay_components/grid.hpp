#pragma once

namespace game2d {

struct GridPositionComponent
{
  int x = 0;
  int y = 0;
};

struct SINGLETON_GridSizeComponent
{
  const int size_xy = 16;
};

} // namespace game2d
