#include "vec2i.hpp"

#include <cmath>
#include <iostream>

namespace math {

Vec2i::Vec2i(int32_t x, int32_t y) : x(x), y(y) {}

Vec2i &Vec2i::operator+=(Vec2i const &rhs) {
  x += rhs.x;
  y += rhs.y;
  return *this;
}
Vec2i &Vec2i::operator-=(Vec2i const &rhs) {
  x -= rhs.x;
  y -= rhs.y;
  return *this;
}

Vec2i &Vec2i::operator*=(int32_t rhs) {
  x *= rhs;
  y *= rhs;
  return *this;
}

Vec2i &Vec2i::operator/=(int32_t rhs) {
  x /= rhs;
  y /= rhs;
  return *this;
}

void Vec2i::zero() {
  x = 0;
  y = 0;
}

// Free function
/*
 * Vector-Vector Addition/ Subtraction
 */
Vec2i operator+(Vec2i const &a, Vec2i const &b) {
  return Vec2i(a.x + b.x, a.y + b.y);
}
Vec2i operator-(Vec2i const &a, Vec2i const &b) {
  return Vec2i(a.x - b.x, a.y - b.y);
}

/*
 * Scalar-Vector Multiplication/Division
 */
Vec2i operator*(int32_t s, Vec2i v) {
  v.x *= s;
  v.y *= s;
  return v;
}
Vec2i operator*(Vec2i v, int32_t s) { return s * v; }

Vec2i operator/(Vec2i v, int32_t s) {
  v.x /= s;
  v.y /= s;
  return v;
}

/*
 * Negation of vector
 * -v = (-1.f) * v
 */
Vec2i operator-(Vec2i v) {
  v.x = -v.x;
  v.y = -v.y;
  return v;
}

/*
 * Vector-Vector (inner/dot) product
 */
float operator*(Vec2i const &a, Vec2i const &b) {
  return a.x * b.x + a.y * b.y;
}
float dot(Vec2i const &a, Vec2i const &b) { return a * b; }

/*
 * Vector norm (length)
 */
float norm(Vec2i const &v) { return std::sqrt(v.x * v.x + v.y * v.y); }
float normSquared(Vec2i const &v) { return v.x * v.x + v.y * v.y; }

float distance(Vec2i const &a, Vec2i const &b) { return norm(a - b); }

float distanceSquared(Vec2i const &a, Vec2i const &b) {
  return normSquared(a - b);
}

Vec2i componentMultiplication(const Vec2i &lhs, const Vec2i &rhs) {
  return {lhs.x * rhs.x, lhs.y * rhs.y};
}

std::ostream &operator<<(std::ostream &out, Vec2i const &v) {
  return out << v.x << " " << v.y;
}

std::istream &operator>>(std::istream &in, Vec2i &v) {
  return in >> v.x >> v.y;
}

} // namespace math
