#pragma once

#include <vector>
namespace fcm {

// make sure to change this if you add a new material
constexpr inline size_t num_mats = 1;

enum Material { STEEL };

void init_materials();

float density(Material);
float spring_constant(Material);
float static_friction(Material, Material);

} // namespace fcm
