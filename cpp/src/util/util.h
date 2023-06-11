#pragma once

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <string>

class vec3;
class ray;
class hittable;

namespace util {

// Constants
constexpr double PI = M_PI;

// Helper functions

namespace {
double constexpr sqrtNewtonRaphson(double x, double curr, double prev)
{
    return curr == prev ? curr : sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
}
} // namespace

/*
* Constexpr version of the square root
* Return value:
*   - For a finite and non-negative value of "x", returns an approximation for the square root of "x"
*   - Otherwise, returns NaN
*/
constexpr double sqrt(double x)
{
    assert(x >= 0);
    assert(x < std::numeric_limits<double>::infinity());

    if (x == 0) {
        return 0;
    }

    return sqrtNewtonRaphson(x, x, 0);
}

constexpr double degrees_to_radians(double degrees)
{
    return degrees * PI / 180.0;
}

double random_double();
double random_double(double min, double max);

std::string left_pad_string(std::string str, unsigned int length, char pad_char);
std::string format_time(std::chrono::nanoseconds time);
std::string format_time(long long time);

} // namespace util