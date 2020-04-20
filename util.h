#pragma once

#include <cmath>
#include <cstdlib>
#include <functional>
#include <limits>
#include <memory>
#include <random>

// Usings
using std::make_shared;
using std::shared_ptr;

// Constants
constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi       = M_PI;

// Helper functions

constexpr inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

constexpr inline double ffmin(double a, double b)
{
    return a <= b ? a : b;
}
constexpr inline double ffmax(double a, double b)
{
    return a >= b ? a : b;
}

inline double random_double()
{
    static std::random_device                     rand_dev;
    static std::mt19937                           generator(rand_dev());
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);

    return distribution(generator);
}

inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

constexpr inline double clamp(double x, double min, double max)
{
    if (x < min) {
        return min;
    }

    if (x > max) {
        return max;
    }

    return x;
}