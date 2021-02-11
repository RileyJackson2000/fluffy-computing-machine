#pragma once

#include <math/constants.hpp>
#include <utils/eigen.hpp>

namespace fcm {

SOLVER_CODES LP_Lemke(Eigen::VectorXf c, Eigen::MatrixXf A, Eigen::VectorXf b,
                      Eigen::VectorXf &x, Eigen::VectorXf &y);

}
