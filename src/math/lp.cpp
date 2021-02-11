#include <math/lcp.hpp>
#include <math/lp.hpp>
#include <utils/eigen.hpp>

namespace fcm {

// Binding for solving Linear Programs of the form
// min c^T * x : A * x <= b, x >= 0
// using Lemke's algorithm. The form of the matrix obtained when transforming
// this problem into an LCP is copositive, hence Lemke's algorithm returns an
// optimal solution if and only if it exists. Here x is a primal optimal
// solution, y is a dual optimal solution.
SOLVER_CODES LP_Lemke(Eigen::VectorXf c, Eigen::MatrixXf A, Eigen::VectorXf b,
                      Eigen::VectorXf &x, Eigen::VectorXf &y) {
  const int m = A.rows(), n = A.cols();
  if (c.size() != n or b.size() != m)
    return INVALID_INPUT;
  Eigen::MatrixXf M = Eigen::MatrixXf::Zero(n + m, n + m);
  M.block(0, n, n, m) = A.transpose();
  M.block(n, 0, m, n) = -A;
  Eigen::VectorXf q(n + m);
  q.head(n) = -c;
  q.tail(m) = b;
  Eigen::VectorXf w, z;
  auto code = LCP_Lemke(M, q, w, z);
  if (code == SUCCESS) {
    x = z.head(n);
    y = z.tail(m);
  }
  return (code == LEMKE_RAY_TERMINATION) ? INFEASIBLE : code;
}

} // namespace fcm
