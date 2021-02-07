#include <model/material.hpp>

namespace fcm {

float _static_friction[num_mats][num_mats];
void init_static_friction() {
  // we don't need to fill in interactions between test materials and real ones
  _static_friction[TEST_MAT_1][TEST_MAT_1] = 0.8;

  _static_friction[STEEL][STEEL] = 0.6;
}

float _spring_constant[num_mats]; // TODO replace with young's modulus? - JL
void init_spring_constant() {
  _spring_constant[TEST_MAT_1] = 1000;

  _spring_constant[STEEL] = 3000; // I made this up - JL
}

float _density[num_mats];
void init_density() {
  _density[TEST_MAT_1] = 5;

  _density[STEEL] = 5000;
}

void init_materials() {
  init_static_friction();
  init_spring_constant();
  init_density();
}

float static_friction(Material mat1, Material mat2) {
  return _static_friction[mat1][mat2];
}

float spring_constant(Material mat) { return _spring_constant[mat]; }

float density(Material mat) { return _density[mat]; }

} // namespace fcm
