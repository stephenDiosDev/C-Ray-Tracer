#pragma once

#include <iosfwd>
#include <vector>
#include <array>
#include <memory>
#include <cstddef>
#include "grid2.hpp"
#include "vec3f.hpp"

namespace raster {

struct RGB {
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

RGB convertToRGB(math::Vec3f const& color);

math::Vec3f quantizedErrorCorrection(math::Vec3f color, float error);

class Image {
public:
  using data_ptr = std::unique_ptr<unsigned char, void (*)(void *)>;

public:
  Image(data_ptr data, uint32_t width, uint32_t height, uint8_t channels);

  unsigned char const *data() const;
  unsigned char *data();

  uint32_t width() const;
  uint32_t height() const;
  uint8_t channels() const;
  bool isEmpty() const;

private:
  data_ptr m_data;
  uint32_t m_width;
  uint32_t m_height;
  uint8_t m_channels;
};

Image read_image_from_file(char const *filename);

Image read_image_from_file_and_flipVertically(char const *filename);

int write_image_to_png(char const *filename, Image const &image);

int write_screen_to_file(char const *filename,
                         geometry::Grid2<RGB> const &sceen);

} // namespace
