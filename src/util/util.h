#pragma once

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <limits>
#include <memory>
#include <random>
#include <string>

// Usings
using std::make_shared;
using std::shared_ptr;

// Constants
constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi       = M_PI;

// Helper functions

constexpr double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

constexpr double ffmin(double a, double b)
{
    return a <= b ? a : b;
}
constexpr double ffmax(double a, double b)
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

constexpr double clamp(double x, double min, double max)
{
    if (x < min) {
        return min;
    }

    if (x > max) {
        return max;
    }

    return x;
}

std::string left_pad_string(std::string str, unsigned int length, char pad_char)
{
    if (length < str.size()) {
        return str;
    }

    return str.insert(0, length - str.size(), pad_char);
}

std::string format_time(std::chrono::nanoseconds time)
{
    using namespace std::chrono;

    auto ms_str =
        left_pad_string(std::to_string(duration_cast<milliseconds>(time).count() % 1000), 3, '0');

    auto mcs_str =
        left_pad_string(std::to_string(duration_cast<microseconds>(time).count() % 1000), 3, '0');

    auto ns_str =
        left_pad_string(std::to_string(duration_cast<nanoseconds>(time).count() % 1000), 3, '0');

    return ms_str + "ms " + mcs_str + "µs " + ns_str + "ns";
}

std::string format_time(long long time)
{
    return format_time(static_cast<std::chrono::nanoseconds>(time));
}
