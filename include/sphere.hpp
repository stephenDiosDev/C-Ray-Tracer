#pragma once

#include "vec3f.hpp"

namespace geometry {

class Sphere {
public:
  Sphere() = default;
  Sphere(math::Vec3f origin, float radius);
  math::Vec3f colour = {1.f, 0.f, 0.f};

  math::Vec3f origin;
  float radius = 1.f;
};

} // namespace geometry
