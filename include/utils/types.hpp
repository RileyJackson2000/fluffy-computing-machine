#pragma once

#include <memory>
#include <vector>

namespace fcm {

using ModelKey = size_t;
using MeshKey = size_t;
struct MeshData;
using MeshCache = std::vector<std::unique_ptr<MeshData>>;
struct Model;
using ModelCache = std::vector<std::unique_ptr<Model>>;

} // namespace fcm
