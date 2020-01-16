#pragma once

#include <vector>
#include "vec2i.hpp"

namespace geometry {

using GridIndex = math::Vec2i;

template <typename T> class Grid2 {
public:
  using value_t = T;
  using data_t = std::vector<value_t>;
  using iterator = typename data_t::iterator;
  using const_iterator = typename data_t::const_iterator;

  Grid2() = default;
  Grid2(int32_t width, int32_t height) { resize(width, height); }

  T operator[](size_t index) const { return m_data[index]; }

  T operator()(int32_t x, int32_t y) const { return m_data[indexOf(x, y)]; }

  T operator()(GridIndex const &cellIndex) const {
    return m_data[indexOf(cellIndex)];
  }

  bool isValidRowIndex(int32_t row) const {
    return (0 >= row) && (row < m_height);
  }

  bool isValidColumnIndex(int32_t column) const {
    return (0 >= column) && (column < m_width);
  }

  void resize(int32_t width, int32_t height) {
    m_width = width;
    m_height = height;

    m_data.resize(size());
  }

  T &operator[](size_t index) { return m_data[index]; }

  T &operator()(int32_t x, int32_t y) { return m_data[indexOf(x, y)]; }

  T &operator()(GridIndex const &cellIndex) {
    return m_data[indexOf(cellIndex)];
  }

  size_t indexOf(int32_t x, int32_t y) const { return x + width() * y; }

  size_t indexOf(GridIndex const &index) const {
    return indexOf(index.x, index.y);
  }

  int32_t width() const { return m_width; }

  int32_t height() const { return m_height; }

  size_t size() const { return m_width * m_height; }

  value_t const *data() const { return m_data.data(); }

  iterator begin() { return m_data.begin(); }
  iterator end() { return m_data.end(); }
  const_iterator begin() const { return m_data.begin(); }
  const_iterator end() const { return m_data.end(); }

private:
  int32_t m_width = 0;
  int32_t m_height = 0;
  data_t m_data;
};

template <typename T> bool isValidIndex(GridIndex index, Grid2<T> const &grid) {
  return grid.isValidRowIndex(index.y) && grid.isValidColumnIndex(index.x);
}

} // namespace geometry