#include "ray_intersect.hpp"
#include "common_matrices.hpp"
#include <math.h>
#include <cmath>
#include <algorithm>


namespace geometry {

Hit::operator bool() const { return didIntersect; }

Hit intersect(Ray const &ray, Triangle const &triangle) {
  Hit hit;

  //construct list of values in a 3x3 matrix
    math::Mat3f A = math::Mat3f{triangle.a().x - triangle.b().x, triangle.a().x - triangle.c().x, ray.direction.x,
                              triangle.a().y - triangle.b().y, triangle.a().y - triangle.c().y, ray.direction.y,
                              triangle.a().z - triangle.b().z, triangle.a().z - triangle.c().z, ray.direction.z};

    float determinantA = math::determinant(A);

    math::Mat3f gammaMatrix = math::Mat3f{triangle.a().x - triangle.b().x, triangle.a().x - ray.origin.x, ray.direction.x,
                                          triangle.a().y - triangle.b().y, triangle.a().y - ray.origin.y, ray.direction.y,
                                          triangle.a().z - triangle.b().z, triangle.a().z - ray.origin.z, ray.direction.z};
    float gamma = math::determinant(gammaMatrix) / determinantA;

    math::Mat3f betaMatrix = math::Mat3f{triangle.a().x - ray.origin.x, triangle.a().x - triangle.c().x, ray.direction.x,
                                   triangle.a().y - ray.origin.y, triangle.a().y - triangle.c().y, ray.direction.y,
                                   triangle.a().z - ray.origin.z, triangle.a().z - triangle.c().z, ray.direction.z};
    float beta = math::determinant(betaMatrix) / determinantA;



    math::Mat3f tMatrix = math::Mat3f{triangle.a().x - triangle.b().x, triangle.a().x - triangle.c().x, triangle.a().x - ray.origin.x,
                                      triangle.a().y - triangle.b().y, triangle.a().y - triangle.c().y, triangle.a().y - ray.origin.y,
                                      triangle.a().z - triangle.b().z, triangle.a().z - triangle.c().z, triangle.a().z - ray.origin.z};
    float tResult = math::determinant(tMatrix) / determinantA;


    if(tResult < 0 || tResult > 10000) {
      hit.didIntersect = false;
      return hit;
    }

    if(gamma < 0 || gamma > 1) {
        hit.didIntersect = false;
        return hit;
    }

    if(beta < 0 || beta > (1 - gamma)) {
        hit.didIntersect = false;
        return hit;
    }

    hit.didIntersect = true;
    hit.rayDepth = tResult;

  return hit;
}

Hit intersect(Ray const &ray, Sphere const &sphere) {
  Hit hit;


  math::Vec3f d(ray.direction);
  math::Vec3f e(ray.origin);
  math::Vec3f c(sphere.origin);
  float R = sphere.radius;

    float discriminant = pow((d * (e - c)), 2) - ((d * d)*((e - c) * (e - c) - (R * R)));

    if(discriminant < 0) {
        hit.didIntersect = false;
        return hit;
    }

    //calculate t
    float t0 = ((-d) * (e - c) + std::sqrt(discriminant)) / (d * d);
    float t1 = ((-d) * (e - c) - std::sqrt(discriminant)) / (d * d);

    //we want the smaller of the 2
    float t = std::min(t0, t1);


    hit.rayDepth = t;
    hit.didIntersect = true;
    return hit;
}

Hit intersect(Ray const &r, Plane const &p) {
  Hit hit;

  auto denom = r.direction * p.normal;
  if (std::abs(denom) < 1e-5f)
    return hit;

  auto t = ((p.origin - r.origin) * p.normal) / denom;

  if (t < 0.f)
    return hit;

  hit.didIntersect = true;
  hit.rayDepth = t;

  return hit;
}

} // namespace geometry
