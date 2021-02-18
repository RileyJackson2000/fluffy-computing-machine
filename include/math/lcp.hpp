#pragma once

#include <math/constants.hpp>
#include <utils/eigen.hpp>

namespace fcm {

SolverCode LCP_Lemke(Eigen::MatrixXf M, Eigen::VectorXf q, Eigen::VectorXf &w,
                     Eigen::VectorXf &z);

} // namespace fcm
