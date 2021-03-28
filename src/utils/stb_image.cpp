// save state
#pragma GCC diagnostic push

// turn off warnings for now
#pragma GCC diagnostic ignored "-Wsign-compare"
#ifdef __GNUC__
// clang doesn't have this warning
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif // __GNUC__

#define STB_IMAGE_IMPLEMENTATION
#include "../../extern/stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../extern/stb/stb_image_write.h"

// turn warnings back on
#pragma GCC diagnostic pop
