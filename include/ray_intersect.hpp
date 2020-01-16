#pragma once

#include <limits>
#include <cmath>

#include "ray.hpp"
#include "plane.hpp"
#include "sphere.hpp"
#include "triangle.hpp"

namespace geometry {
struct Hit {
  // just so we can use it as a bool in if statements, syntactic-sugar
  explicit operator bool() const;

  bool didIntersect = false;
  float rayDepth = std::numeric_limits<float>::max();
};

Hit intersect(Ray const &ray, Sphere const &sphere);

Hit intersect(Ray const &ray, Triangle const &triangle);

Hit intersect(Ray const &ray, Plane const &plane);

} // namespace geometry