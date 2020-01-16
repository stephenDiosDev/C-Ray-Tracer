#include "ray.hpp"

namespace geometry {

Ray::Ray(math::Vec3f origin, math::Vec3f direction)
    : origin(origin), direction(direction) {}

math::Vec3f pointOnLne(math::Vec3f const &origin, math::Vec3f const &direction,
                       float t) {
  return origin + direction * t;
}

math::Vec3f evaluate(Ray const &ray, float t) {
  return pointOnLne(ray.origin, ray.direction, t);
}

} // namespace geometry