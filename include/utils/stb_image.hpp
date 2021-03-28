#pragma once

// used in stb for memory alloc
#define STBI_MALLOC(sz) malloc(sz)
#define STBI_REALLOC(p, newsz) realloc(p, newsz)
#define STBI_FREE(p) free(p)

#include "../../extern/stb/stb_image.h"
#include "../../extern/stb/stb_image_write.h"

// some macros for convenience that they don't define
// Y = monochromatic
#define STBI_COLOUR_Y 1
#define STBI_COLOUR_YA 2
#define STBI_COLOUR_RGB 3
#define STBI_COLOUR_RGBA 4

#include <memory>

namespace helper {
struct DestroySTBImage {
  void operator()(unsigned char *ptr) { stbi_image_free(ptr); }
};
} // namespace helper

using stbi_image_arr =
    std::unique_ptr<unsigned char[], helper::DestroySTBImage>;
