#pragma once

// used in stb for memory alloc
#define STBI_MALLOC(sz) malloc(sz)
#define STBI_REALLOC(p, newsz) realloc(p, newsz)
#define STBI_FREE(p) free(p)

#include "../../extern/stb/stb_image.h"

#include <memory>

namespace helper {
struct DestroySTBImage {
  void operator()(unsigned char *ptr) { stbi_image_free(ptr); }
};
} // namespace helper

using stbi_image_arr =
    std::unique_ptr<unsigned char[], helper::DestroySTBImage>;
