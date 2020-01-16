#include "plane.hpp"

namespace geometry {
Plane::Plane(math::Vec3f normal, math::Vec3f origin)
    : normal(normal), origin(origin) {}
}