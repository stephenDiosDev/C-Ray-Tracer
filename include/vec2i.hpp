#pragma once

#include <cstdint>
#include <iosfwd>

namespace math {

struct Vec2i {
  int32_t x = 0;
  int32_t y = 0;

  Vec2i() = default;

  Vec2i(int32_t x, int32_t y);

  Vec2i &operator+=(Vec2i const &rhs);

  Vec2i &operator-=(Vec2i const &rhs);

  Vec2i &operator*=(int32_t rhs);

  Vec2i &operator/=(int32_t rhs);

  void zero();
};

// Free function
/*
 * Vector-Vector Addition/ Subtraction
 */
Vec2i operator+(Vec2i const &a, Vec2i const &b);
Vec2i operator-(Vec2i const &a, Vec2i const &b);

/*
 * Scalar-Vector Multiplication/Division
 */
Vec2i operator*(int32_t s, Vec2i v);
Vec2i operator*(Vec2i v, int32_t s);

Vec2i operator/(Vec2i v, int32_t s);

/*
 * Negation of vector
 * -v = (-1) * v
 */
Vec2i operator-(Vec2i v);

/*
 * Vector-Vector (inner/dot) product
 */
float operator*(Vec2i const &a, Vec2i const &b);
float dot(Vec2i const &a, Vec2i const &b);

/*
 * Vector norm (length)
 */
float norm(Vec2i const &v);
float normSquared(Vec2i const &v);

/*
 * Linear int32_terpolation
 */
Vec2i lerp(Vec2i const &a, Vec2i const &b, float t);

float distance(Vec2i const &a, Vec2i const &b);

float distanceSquared(Vec2i const &a, Vec2i const &b);

Vec2i componentMultiplication(const Vec2i &lhs, const Vec2i &rhs);

std::ostream &operator<<(std::ostream &out, Vec2i const &v);

std::istream &operator>>(std::istream &in, Vec2i &v);

} // namespace math
