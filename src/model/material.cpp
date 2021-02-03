#include <model/material.hpp>

namespace fcm {

double _static_friction[num_mats][num_mats];
void init_static_friction() { _static_friction[STEEL][STEEL] = 0.6; }

double _spring_constant[num_mats]; // TODO replace with young's modulus - JL
void init_spring_constant() {
  _spring_constant[STEEL] = 1000; // I made this up - JL
}

void init_materials() {
  init_static_friction();
  init_spring_constant();
}

double static_friction(Material mat1, Material mat2) {
  return _static_friction[mat1][mat2];
}

double spring_constant(Material mat) { return _spring_constant[mat]; }

} // namespace fcm
