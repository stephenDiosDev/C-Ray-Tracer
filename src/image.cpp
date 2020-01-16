#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

namespace raster {

Image::Image(data_ptr data, uint32_t width, uint32_t height, uint8_t channels)
    : m_data(std::move(data)), m_width(width), m_height(height),
      m_channels(channels) {}

unsigned char const *Image::data() const { return m_data.get(); }

unsigned char *Image::data() { return m_data.get(); }

uint32_t Image::width() const { return m_width; }

uint32_t Image::height() const { return m_height; }

uint8_t Image::channels() const { return m_channels; }

bool Image::isEmpty() const { return m_data.get() == nullptr; }

void freeImage(void *ptr) { STBI_FREE(ptr); }

Image read_image_from_file(char const *filename) {
  int width = 0;
  int height = 0;
  int channels = 0;
  int requestedChannels = 0; // 0 : all

  stbi_set_flip_vertically_on_load(false); // sets static state

  Image::data_ptr data = Image::data_ptr( //
      stbi_load(filename,                 // name
                &width, &height,          // size
                &channels,                // channels out
                requestedChannels         // requested channels; 0 : all
                ),
      freeImage);

  if (data.get() == nullptr)
    std::cerr << "[Error] could not load image: "
              << std::string(filename).c_str() << '\n';

  return {std::move(data),  //
          uint32_t(width),  //
          uint32_t(height), //
          uint8_t(channels)};
}

Image read_image_from_file_and_flipVertically(char const *filename) {
  int width = 0;
  int height = 0;
  int channels = 0;
  int requestedChannels = 0; // 0 : all

  stbi_set_flip_vertically_on_load(true); // sets static state

  Image::data_ptr data = Image::data_ptr( //
      stbi_load(filename,                 // name
                &width, &height,          // size
                &channels,                // channels out
                requestedChannels         // requested channels; 0 : all
                ),
      freeImage);

  if (data.get() == nullptr)
    std::cerr << "[Error] could not load image: "
              << std::string(filename).c_str() << '\n';

  return {std::move(data),  //
          uint32_t(width),  //
          uint32_t(height), //
          uint8_t(channels)};
}

int write_image_to_png(char const *filename, Image const &image) {
  stbi_flip_vertically_on_write(true);
  return stbi_write_png(
      filename, image.width(), image.height(), image.channels(), image.data(),
      sizeof(unsigned char) * image.width() * image.channels());
}

int write_screen_to_file(char const *filename,
                         geometry::Grid2<RGB> const &screen) {

  stbi_flip_vertically_on_write(true);
  return stbi_write_png(filename, screen.width(), screen.height(), 3,
                        screen.data(),
                        sizeof(unsigned char) * screen.width() * 3);
}

RGB convertToRGB(math::Vec3f const &color) {
  auto clamp = [](float t) {
    if (t < 0.f)
      t = 0.f;
    if (t > 1.f)
      t = 1.f;
    return t;
  };
  RGB rgb;
  rgb.r = clamp(color.x) * 255;
  rgb.g = clamp(color.y) * 255;
  rgb.b = clamp(color.z) * 255;
  return rgb;
}

math::Vec3f quantizedErrorCorrection(math::Vec3f color, float error) {
  color += math::Vec3f{error, error, error};
  return color;
}

} // namespace image
