#include <iostream>
#include <math/lcp.hpp>
#include <math/qcp.hpp>
#include <utils/eigen.hpp>

namespace fcm {

// Binding for solving Convex Quadratic Programs of the form
// min c^T * x + x^T * Q * x: Ax <= b, x >= 0
// using Lemke's algorithm (so we must have Q positive semidefinite).
// The form of the matrix obtained when transforming
// this problem into an LCP is copositive, hence Lemke's algorithm returns an
// optimal solution if and only if it exists. Here x is a primal optimal
// solution, y is the certifying Lagrange multiplier.
SOLVER_CODES QCP_Lemke(Eigen::VectorXf c, Eigen::MatrixXf Q, Eigen::MatrixXf A,
                       Eigen::VectorXf b, Eigen::VectorXf &x,
                       Eigen::VectorXf &y) {
  const int m = A.rows(), n = A.cols();
  if (c.size() != n or b.size() != m or Q.rows() != n or Q.cols() != n)
    return INVALID_INPUT;
  Eigen::MatrixXf M = Eigen::MatrixXf::Zero(n + m, n + m);
  M.block(0, 0, n, n) = Q;
  M.block(0, n, n, m) = A.transpose();
  M.block(n, 0, m, n) = -A;
  Eigen::VectorXf q(n + m);
  q.head(n) = c;
  q.tail(m) = b;
  Eigen::VectorXf w, z;
  auto code = LCP_Lemke(M, q, w, z);
  if (code == SUCCESS) {
    x = z.head(n);
    y.resize(n + m);
    y.head(m) = z.tail(m);
    y.tail(n) = w.head(n);
  }
  return (code == LEMKE_RAY_TERMINATION) ? INFEASIBLE : code;
}

} // namespace fcm
