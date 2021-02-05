#pragma once

#include <model/object.hpp>
#include <utils/glm.hpp>

#include <string>
#include <vector>
#include <memory>


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
};

static MeshData genSphereMesh(float radius, unsigned int sectorCount, unsigned int stackCount)
{
    MeshData md;

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            Vertex v;

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            v.pos = glm::vec3{x, y, z};

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            v.norm = glm::vec3{nx, ny, nz};

            md.vertices.push_back(v);
        }
    }

    int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0)
            {
                md.indices.push_back(k1);
                md.indices.push_back(k2);
                md.indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if(i != (stackCount-1))
            {
                md.indices.push_back(k1 + 1);
                md.indices.push_back(k2);
                md.indices.push_back(k2 + 1);
            }
        }
    }
    
    return md;
}

}
