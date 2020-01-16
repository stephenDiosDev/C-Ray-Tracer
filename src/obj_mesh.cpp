#include "obj_mesh.hpp"

#include <algorithm>
#include <cassert>

using namespace math;

namespace geometry {

std::vector<math::Vec3f>
calculateTriangleNormals(std::vector<IndicesTriangle> const &indexTriangles,
                         std::vector<math::Vec3f> const &vertices) {
  std::vector<Vec3f> tmp;

  return tmp;
}

std::vector<math::Vec3f>
calculateVertexNormals(std::vector<IndicesTriangle> const &indexTriangles,
                       std::vector<math::Vec3f> const &vertices) {
	std::vector<Vec3f> tmp;

	return tmp;
}

std::vector<math::Vec3f>
calculateVertexNormals(std::vector<IndicesTriangle> const &indexTriangles,
                       std::vector<math::Vec3f> const &vertices,
                       std::vector<math::Vec3f> const &triangleNormals) {
	std::vector<Vec3f> tmp;

	return tmp;
}

} // namespace geometry
