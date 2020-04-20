#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// Usings

using std::make_shared;
using std::shared_ptr;

// Constants

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = M_PI;

// Helper functions

constexpr inline double degrees_to_radians(double degrees) {
  return degrees * pi / 180.0;
}

constexpr inline double ffmin(double a, double b) { return a <= b ? a : b; }
constexpr inline double ffmax(double a, double b) { return a >= b ? a : b; }

inline double random_double() { return rand() / (RAND_MAX + 1.0); }
inline double random_double(double min, double max) {
  return min + (max - min) * random_double();
}

constexpr inline double clamp(double x, double min, double max) {
  if (x < min) {
    return min;
  }

  if (x > max) {
    return max;
  }

  return x;
}