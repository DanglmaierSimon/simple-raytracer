#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <memory>
#include <string>

class vec3;
class ray;
class hittable;

// Usings
using std::make_shared;
using std::shared_ptr;

// Constants
constexpr double pi = M_PI;

// Helper functions

constexpr double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

constexpr double ffmin(double a, double b)
{
    return std::min(a, b);
}
constexpr double ffmax(double a, double b)
{
    return std::max(a, b);
}

constexpr double clamp(double x, double min, double max)
{
    return std::clamp(x, min, max);
}

double random_double();
double random_double(double min, double max);

std::string left_pad_string(std::string str, unsigned int length, char pad_char);
std::string format_time(std::chrono::nanoseconds time);
std::string format_time(long long time);
