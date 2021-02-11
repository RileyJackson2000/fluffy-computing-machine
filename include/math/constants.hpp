#pragma once

namespace fcm {

/* solver code */
enum SOLVER_CODES {
  SUCCESS = 0,
  FAILED = -1,
  INVALID_INPUT = -2,
  LEMKE_RAY_TERMINATION = -3, // For our purposes, this is equivalent to
                              // infeasibility (pos def matrices)
  INFEASIBLE = -4,
  ITER_COUNT_EXCEEDED = -5,
  UNBOUNDED = -6
};

/* lcp solver options */
constexpr inline int lemke_max_iters = 1 << 20;
constexpr inline float lemke_tol = 5e-5;

} // namespace fcm
