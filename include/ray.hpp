#pragma once

#include "vec3f.hpp"

namespace geometry {

class Ray {
public:
  Ray() = default;
  Ray(math::Vec3f origin, math::Vec3f direction);

  math::Vec3f origin;
  math::Vec3f direction;
};

math::Vec3f pointOnLne(math::Vec3f const &origin,    //
                       math::Vec3f const &direction, //
                       float t);

math::Vec3f evaluate(Ray const &ray, float t);

} // namespace
