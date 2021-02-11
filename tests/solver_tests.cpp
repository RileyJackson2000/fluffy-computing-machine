#include "math/constants.hpp"
#include "math/lp.hpp"
#include "math/qcp.hpp"
#include <cmath>
#include <tuple>

#include <gtest/gtest.h>

#include <math/optimization.hpp>

TEST(SolverSuite, LemkeFeasible1) {
  const int n = 4;
  Eigen::MatrixXf M(n, n);
  M << -1, 1, 1, 1, 1, -1, 1, 1, -1, -1, -2, 0, -1, -1, 0, -2;
  M *= -1;
  Eigen::VectorXf q(n);
  q << 3, 5, -9, -5;
  Eigen::VectorXf w, z;
  EXPECT_EQ(fcm::LCP_Lemke(M, q, w, z), fcm::SUCCESS);
  EXPECT_GE(z.minCoeff(), -fcm::lemke_tol);
  Eigen::VectorXf w2 = q + M * z;
  EXPECT_GE(w2.minCoeff(), -fcm::lemke_tol);
  for (int i = 0; i < n; ++i) {
    EXPECT_TRUE(abs(w(i) - w2(i)) < fcm::lemke_tol);
  }
  for (int i = 0; i < n; ++i) {
    EXPECT_TRUE(abs(z(i) * w(i)) < fcm::lemke_tol / n);
  }
}

TEST(SolverSuite, LemkeFeasible2) {
  const int n = 3;
  Eigen::MatrixXf M(n, n);
  M << 1, 0, 0, 2, 1, 0, 2, 2, 1;
  Eigen::VectorXf q(n);
  q << -8, -12, -14;
  Eigen::VectorXf w, z;
  EXPECT_EQ(fcm::LCP_Lemke(M, q, w, z), fcm::SUCCESS);
  EXPECT_GE(z.minCoeff(), -fcm::lemke_tol);
  Eigen::VectorXf w2 = q + M * z;
  EXPECT_GE(w2.minCoeff(), -fcm::lemke_tol);
  for (int i = 0; i < n; ++i) {
    EXPECT_TRUE(abs(w(i) - w2(i)) < fcm::lemke_tol);
  }
  for (int i = 0; i < n; ++i) {
    EXPECT_TRUE(abs(z(i) * w(i)) < fcm::lemke_tol / n);
  }
}

TEST(SolverSuite, LemkeRay) {
  const int n = 3;
  Eigen::MatrixXf M(n, n);
  M << -1, 0, -3, 1, -2, -5, -2, -1, -2;
  Eigen::VectorXf q(n);
  q << -3, -2, -1;
  Eigen::VectorXf w, z;
  EXPECT_EQ(fcm::LCP_Lemke(M, q, w, z), fcm::LEMKE_RAY_TERMINATION);
}

TEST(SolverSuite, LemkeDegenerate) {
  // this is an example of a test which will cycle using the analog of Bland's
  // rule
  const int n = 3;
  Eigen::MatrixXf M(n, n);
  M << 1, 2, 0, 0, 1, 2, 2, 0, 1;
  Eigen::VectorXf q(n);
  q << -1, -1, -1;
  Eigen::VectorXf w, z;
  EXPECT_EQ(fcm::LCP_Lemke(M, q, w, z), fcm::SUCCESS);
  EXPECT_GE(z.minCoeff(), -fcm::lemke_tol);
  Eigen::VectorXf w2 = q + M * z;
  EXPECT_GE(w2.minCoeff(), -fcm::lemke_tol);
  for (int i = 0; i < n; ++i) {
    EXPECT_TRUE(abs(w(i) - w2(i)) < fcm::lemke_tol);
  }
  for (int i = 0; i < n; ++i) {
    EXPECT_TRUE(abs(z(i) * w(i)) < fcm::lemke_tol / n);
  }
}

TEST(SolverSuite, LPFeasible) {
  const int n = 2;
  const int m = 3;
  Eigen::MatrixXf A(m, n);
  A << 4, 8, 2, 1, 3, 2;
  Eigen::VectorXf c(n);
  c << 2, 3;
  Eigen::VectorXf b(m);
  b << 12, 3, 4;
  Eigen::VectorXf x, y;
  EXPECT_EQ(fcm::LP_Lemke(c, A, b, x, y), fcm::SUCCESS);
  EXPECT_GE(x.minCoeff(), -fcm::lemke_tol);
  Eigen::VectorXf tmp = b - A * x;
  EXPECT_GE(tmp.minCoeff(), -fcm::lemke_tol);
  tmp = A.transpose() * y - c;
  EXPECT_GE(tmp.minCoeff(), -fcm::lemke_tol);
  EXPECT_LE(abs((c.transpose() * x - b.transpose() * y)(0)), fcm::lemke_tol);
}

TEST(SolverSuite, LPInfeasible) {
  const int n = 2;
  const int m = 2;
  Eigen::MatrixXf A = Eigen::MatrixXf::Zero(n, m);
  Eigen::VectorXf b = Eigen::VectorXf::Constant(m, -1);
  Eigen::VectorXf c = Eigen::VectorXf::Zero(n);
  Eigen::VectorXf x, y;
  EXPECT_EQ(fcm::LP_Lemke(c, A, b, x, y), fcm::INFEASIBLE);
}

TEST(SolverSuite, QCPFeasible) {
  const int n = 3;
  const int m = 1;
  Eigen::VectorXf c(n);
  c << -66, -54, -20;
  Eigen::MatrixXf A(m, n);
  A << 1, 1, 1;
  Eigen::VectorXf b(m);
  b << 1;
  Eigen::MatrixXf Q(n, n);
  Q << 34, 16, 4, 16, 34, 16, 4, 16, 8;
  Eigen::VectorXf x, y;
  EXPECT_EQ(fcm::QCP_Lemke(c, Q, A, b, x, y), fcm::SUCCESS);
  EXPECT_GE(x.minCoeff(), -fcm::lemke_tol);
  EXPECT_GE(y.minCoeff(), -fcm::lemke_tol);
  Eigen::MatrixXf g(n, m + n);
  g.block(0, 0, n, m) = A.transpose();
  g.block(0, m, n, n) = -Eigen::MatrixXf::Identity(n, n);
  auto tmp = c + Q * x + g * y;
  for (int i = 0; i < n; ++i) {
    EXPECT_LE(abs(tmp(i)), fcm::lemke_tol);
  }
  Eigen::VectorXf tmp2(m + n);
  tmp2.head(m) = A * x - b;
  tmp2.tail(n) = x;
  for (int i = 0; i < n + m; ++i) {
    EXPECT_LE(abs(y(i) * tmp2(i)), fcm::lemke_tol);
  }
}

TEST(SolverSuite, QCPInfeasible) {
  const int n = 2;
  const int m = 2;
  Eigen::MatrixXf A = Eigen::MatrixXf::Zero(n, m);
  Eigen::VectorXf b = Eigen::VectorXf::Constant(m, -1);
  Eigen::VectorXf c = Eigen::VectorXf::Zero(n);
  Eigen::MatrixXf Q = Eigen::MatrixXf::Identity(n, n);
  Eigen::VectorXf x, y;
  EXPECT_EQ(fcm::QCP_Lemke(c, Q, A, b, x, y), fcm::INFEASIBLE);
}
