#pragma once

#include "vec3f.hpp"

namespace geometry {

class Plane {
public:
  Plane() = default;
  Plane(math::Vec3f normal, math::Vec3f origin);

  math::Vec3f normal = {0.f, 1.f, 0.f};
  math::Vec3f origin = {0.f, 0.f, 0.f};
  math::Vec3f colour = {0.f, 0.f, 0.f};
};

} // namespace geometry
