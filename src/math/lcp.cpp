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
SolverCode LCP_Lemke(Eigen::MatrixXf M, Eigen::VectorXf q, Eigen::VectorXf &w,
                     Eigen::VectorXf &z) {
  // not passing by const ref because M and q are modified
  // so user has option to move or copy them in.
  const int n = q.size();

  if (M.rows() != n and M.cols() != n)
    return SolverCode::INVALID_INPUT;

  int r, s = 2 * n;
  float minCoeff = q.minCoeff(&r);

  // trivial case
  if (minCoeff >= -lemkeTol) {
    w = q;
    z = Eigen::VectorXf::Zero(n);
    return SolverCode::SUCCESS;
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
  Eigen::MatrixXf rankOneUpdate(n, 2 * n + 2);
  Eigen::MatrixXf rRow(1, 2 * n + 2);
  Eigen::MatrixXf sCol(n, 1);

  // So we can map between variables and basic variables
  std::vector<int> basicIdxToCol(n), colToBasicIdx(2 * n + 1, -1);
  for (int i = 0; i < n; ++i)
    if (i != r)
      basicIdxToCol[i] = i;
  basicIdxToCol[r] = 2 * n;
  for (int i = 0; i < 2 * n + 1; ++i) {
    if (i != r and i < n) {
      colToBasicIdx[i] = i;
    }
  }
  colToBasicIdx[2 * n] = r;

  // Indexes in tab of the columns of the inverse basis
  std::vector<int> invIdxs(n);
  for (int i = 0; i < n; ++i) {
    if (i != r) {
      invIdxs[i] = i;
    } else {
      invIdxs[i] = 2 * n;
    }
  }

  // Will be used later for lexico min entering variable
  std::vector<int> lex(n);
  int lexSZ = 0, lexIdx = 0;

  int colThatLeft = r;

  int iters = 0;
  for (; iters < lemkeMaxIters; ++iters) {
    // Artificial variable left basis, we're done!
    if (colThatLeft == 2 * n) {
      w = Eigen::VectorXf::Zero(n);
      z = Eigen::VectorXf::Zero(n);
      for (int i = 0; i < n; ++i) {
        w(i) = (colToBasicIdx[i] == -1) ? 0 : tab(colToBasicIdx[i], 2 * n + 1);
        z(i) = colToBasicIdx[i + n] == -1
                   ? 0
                   : tab(colToBasicIdx[i + n], 2 * n + 1);
      }
      return SolverCode::SUCCESS;
    }

    // Complementary pivot rule
    s = (colThatLeft < n) ? colThatLeft + n : colThatLeft - n;

    // Min ratio test
    float minRatio = std::numeric_limits<float>::max();
    lexSZ = 0;
    for (int i = 0; i < n; ++i) {
      if (tab(i, s) > 0) {
        float tmp = tab(i, 2 * n + 1) / tab(i, s);
        if (tmp < minRatio) {
          lexSZ = lexIdx = 1;
          ;
          lex[0] = i;
          minRatio = tmp;
        } else if (tmp == minRatio) {
          ++lexSZ;
          lex[lexIdx++] = i;
        }
      }
    }

    if (lexSZ > 0) {
      // Bland's rule on steroids
      int col = 0;
      int newSZ = 0, newIdx = 0;
      while (lexSZ != 1) {
        minRatio = tab(lex[0], invIdxs[col]) / tab(lex[0], s);
        for (int i = 1; i < lexSZ; ++i) {
          float tmp = tab(lex[i], invIdxs[col]) / tab(lex[i], s);
          if (tmp < minRatio) {
            minRatio = tmp;
            newSZ = newIdx = 1;
            lex[0] = lex[i];
          } else if (tmp == minRatio) {
            ++newSZ;
            lex[newIdx++] = lex[i];
          }
        }
        lexSZ = newSZ;
        ++col;
      }
      r = lex[0];
    } else {
      // Entering column is <= 0, we've found a feasible ray
      // For our purposes, this means infeasible
      return SolverCode::LEMKE_RAY_TERMINATION;
    }

    // Pivot about tab(r, s)
    rRow = tab.row(r) * (1 / tab(r, s));
    sCol = tab.col(s);
    rankOneUpdate.noalias() = sCol * rRow;
    tab -= rankOneUpdate;
    tab.col(s) = Eigen::VectorXf::Zero(n);
    tab.row(r) = rRow;

    colThatLeft = basicIdxToCol[r];

    // update basic / nonbasic vars
    colToBasicIdx[s] = r;
    colToBasicIdx[basicIdxToCol[r]] = -1;
    basicIdxToCol[r] = s;
  }
  return SolverCode::ITER_COUNT_EXCEEDED;
}

} // namespace fcm
