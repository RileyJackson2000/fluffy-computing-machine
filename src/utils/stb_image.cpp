#include <utils/stb_image.hpp>

// save state
#pragma GCC diagnostic push

// turn off warnings for now
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

#define STB_IMAGE_IMPLEMENTATION
#include "../../extern/stb/stb_image.h"

// turn warnings back on
#pragma GCC diagnostic pop

stbi_image_arr stbi_new_image_arr(int width, int height, int numChannels) {
  auto data =
      static_cast<unsigned char *>(STBI_MALLOC(width * height * numChannels));
  return stbi_image_arr{data};
}
