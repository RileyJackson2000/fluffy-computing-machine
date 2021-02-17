#pragma once

#include <memory>
#include <vector>

namespace fcm {

using RenderObjectKey = size_t;
using MeshKey = size_t;
struct MeshData;
using MeshCache = std::vector<std::unique_ptr<MeshData>>;
struct RenderObject;
using RenderObjectCache = std::vector<std::unique_ptr<RenderObject>>;

} // namespace fcm
