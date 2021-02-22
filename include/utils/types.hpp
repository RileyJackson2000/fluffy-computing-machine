#pragma once

#include <memory>
#include <vector>

namespace fcm {

using RenderObjectKey = size_t;
struct RenderObject;
using RenderObjectCache = std::vector<std::unique_ptr<RenderObject>>;

} // namespace fcm
