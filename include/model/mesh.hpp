#pragma once

#include <vector>

#include <utils/glm.hpp>


namespace fcm {

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 norm;
};

struct MeshData
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    void faceNormals();
};

MeshData genSphereMesh(float radius, unsigned int sectorCount, unsigned int stackCount, bool faceNormals);

}
