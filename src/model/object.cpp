#include <utils/glm.hpp>

#include <model/mesh.hpp>
#include <model/object.hpp>

namespace fcm {

Object::Object(std::shared_ptr<MeshData> meshData)
    : glMeshData{meshData.get()}, meshData{std::move(meshData)} {}

Sphere::Sphere(float radius, bool faceNormals)
    : Object{genSphereMesh(radius, 10, 10, faceNormals)}, radius{radius} {}

} // namespace fcm
