#include <math/optimization.hpp>

#include <limits>
#include <vector>

namespace fcm {

// An implementation of Lemke's Complementary Pivot Method for solving the
// Linear Complementary problem. This is state of the art in 1988, so it's
// probably dated by now.
// TODO there are 30 years of improvements to make
// Given an n-dimensional vector q and an n x n matrix M, finds an n-dimensional
// vector z such that:
// z>=0
// q+Mz>=0
// z^T * (q+Mz)=0
SOLVER_CODES LCP_Lemke(Eigen::MatrixXf M, Eigen::VectorXf q, Eigen::VectorXf &w,
                       Eigen::VectorXf &z) {
  // not passing by const ref because M and q are modified
  // so user has option to move or copy them in.
  const int n = q.size();

  if (M.rows() != n and M.cols() != n)
    return INVALID_INPUT;

  int r, s = 2 * n;
  float min_coeff = q.minCoeff(&r);

  // trivial case
  if (min_coeff >= -lemke_tol) {
    w = q;
    z = Eigen::VectorXf::Zero(n);
    return SUCCESS;
  }

  Eigen::MatrixXf P = Eigen::MatrixXf::Identity(n, n);
  P.col(r) = Eigen::VectorXf::Constant(n, -1);

  // initialize tableau
  Eigen::MatrixXf tab(n, 2 * n + 2);
  tab.block(0, 0, n, n) = P;
  tab.block(0, n, n, n) = -P * M;
  tab.col(2 * n) = Eigen::VectorXf::Zero(n);
  tab(r, 2 * n) = 1;
  tab.col(2 * n + 1) = P * q;

  // We'll use these later but don't want to re-allocate
  Eigen::MatrixXf rank_one_update(n, 2 * n + 2);
  Eigen::MatrixXf r_row(1, 2 * n + 2);
  Eigen::MatrixXf s_col(n, 1);

  // So we can map between variables and basic variables
  std::vector<int> basic_idx_to_col(n), col_to_basic_idx(2 * n + 1, -1);
  for (int i = 0; i < n; ++i)
    if (i != r)
      basic_idx_to_col[i] = i;
  basic_idx_to_col[r] = 2 * n;
  for (int i = 0; i < 2 * n + 1; ++i) {
    if (i != r and i < n) {
      col_to_basic_idx[i] = i;
    }
  }
  col_to_basic_idx[2 * n] = r;

  // Indexes in tab of the columns of the inverse basis
  std::vector<int> inv_idxs(n);
  for (int i = 0; i < n; ++i) {
    if (i != r) {
      inv_idxs[i] = i;
    } else {
      inv_idxs[i] = 2 * n;
    }
  }

  // Will be used later for lexico min entering variable
  std::vector<int> lex(n);
  int lex_sz = 0, lex_idx = 0;

  int col_that_left = r;

  int iters = 0;
  for (; iters < lemke_max_iters; ++iters) {
    // Artificial variable left basis, we're done!
    if (col_that_left == 2 * n) {
      w = Eigen::VectorXf::Zero(n);
      z = Eigen::VectorXf::Zero(n);
      for (int i = 0; i < n; ++i) {
        w(i) = (col_to_basic_idx[i] == -1)
                   ? 0
                   : tab(col_to_basic_idx[i], 2 * n + 1);
        z(i) = col_to_basic_idx[i + n] == -1
                   ? 0
                   : tab(col_to_basic_idx[i + n], 2 * n + 1);
      }
      return SUCCESS;
    }

    // Complementary pivot rule
    s = (col_that_left < n) ? col_that_left + n : col_that_left - n;

    // Min ratio test
    float min_ratio = std::numeric_limits<float>::max();
    lex_sz = 0;
    for (int i = 0; i < n; ++i) {
      if (tab(i, s) > 0) {
        float tmp = tab(i, 2 * n + 1) / tab(i, s);
        if (tmp < min_ratio) {
          lex_sz = lex_idx = 1;
          ;
          lex[0] = i;
          min_ratio = tmp;
        } else if (tmp == min_ratio) {
          ++lex_sz;
          lex[lex_idx++] = i;
        }
      }
    }

    if (lex_sz > 0) {
      // Bland's rule on steroids
      int col = 0;
      int new_sz = 0, new_idx = 0;
      while (lex_sz != 1) {
        min_ratio = tab(lex[0], inv_idxs[col]) / tab(lex[0], s);
        for (int i = 1; i < lex_sz; ++i) {
          float tmp = tab(lex[i], inv_idxs[col]) / tab(lex[i], s);
          if (tmp < min_ratio) {
            min_ratio = tmp;
            new_sz = new_idx = 1;
            lex[0] = lex[i];
          } else if (tmp == min_ratio) {
            ++new_sz;
            lex[new_idx++] = lex[i];
          }
        }
        lex_sz = new_sz;
        ++col;
      }
      r = lex[0];
    } else {
      // Entering column is <= 0, we've found a feasible ray
      // For our purposes, this means infeasible
      return LEMKE_RAY_TERMINATION;
    }

    // Pivot about tab(r, s)
    r_row = tab.row(r) * (1 / tab(r, s));
    s_col = tab.col(s);
    rank_one_update.noalias() = s_col * r_row;
    tab -= rank_one_update;
    tab.col(s) = Eigen::VectorXf::Zero(n);
    tab.row(r) = r_row;

    col_that_left = basic_idx_to_col[r];

    // update basic / nonbasic vars
    col_to_basic_idx[s] = r;
    col_to_basic_idx[basic_idx_to_col[r]] = -1;
    basic_idx_to_col[r] = s;
  }
  return ITER_COUNT_EXCEEDED;
}

} // namespace fcm
