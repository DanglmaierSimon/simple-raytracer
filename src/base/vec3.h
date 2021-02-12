#pragma once

#include <array>
#include <cmath>
#include <iostream>

#include "util.h"

class vec3 {
  public:
    constexpr vec3()
        : vec3(0.0, 0.0, 0.0)
    {
    }

    constexpr vec3(double e0, double e1, double e2)
        : d{e0, e1, e2}
    {
    }

    constexpr double x() const
    {
        return d[0];
    }
    constexpr double y() const
    {
        return d[1];
    }
    constexpr double z() const
    {
        return d[2];
    }

    constexpr vec3 operator-() const
    {
        return vec3(-x(), -y(), -z());
    }

    constexpr double operator[](int i) const
    {
        return d[i];
    }
    constexpr double& operator[](int i)
    {
        return d[i];
    }

    constexpr vec3& operator+=(vec3 const& v)
    {
        d[0] += v.x();
        d[1] += v.y();
        d[2] += v.z();

        return *this;
    }

    constexpr vec3& operator*=(double t)
    {
        d[0] *= t;
        d[1] *= t;
        d[2] *= t;
        return *this;
    }

    constexpr vec3 operator/=(double t)
    {
        return *this *= 1 / t;
    }

    constexpr double length() const
    {
        return util::sqrt(length_squared());
    }

    constexpr double length_squared() const
    {
        return x() * x() + y() * y() + z() * z();
    }

    static vec3 random();
    static vec3 random(double min, double max);

  private:
    double d[3];
};

inline std::ostream& operator<<(std::ostream& os, vec3 const& v)
{
    return os << v[0] << " " << v[1] << " " << v[2];
}

constexpr vec3 operator+(vec3 const& lhs, vec3 const& rhs)
{
    return vec3(lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2]);
}

constexpr vec3 operator-(vec3 const& lhs, vec3 const& rhs)
{
    return vec3(lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2]);
}

constexpr vec3 operator*(vec3 const& lhs, vec3 const& rhs)
{
    return vec3(lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2]);
}

constexpr vec3 operator*(double const t, vec3 const& v)
{
    return vec3(v[0] * t, v[1] * t, v[2] * t);
}

constexpr vec3 operator*(vec3 const& v, double const t)
{
    return t * v;
}

constexpr vec3 operator/(vec3 const& v, double const t)
{
    if (t == 0.0) {
        return vec3(0, 0, 0);
    }

    return (1 / t) * v;
}

constexpr double dot(vec3 const& lhs, vec3 const& rhs)
{
    return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}

constexpr vec3 cross(vec3 const& u, vec3 const& v)
{
    return vec3{u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]};
}

constexpr vec3 unit_vector(vec3 const& v)
{
    return v / v.length();
}

vec3 random_in_unit_sphere();
vec3 random_unit_vector();
vec3 random_in_unit_disk();

constexpr vec3 refract(vec3 const& uv, vec3 const& n, double etai_over_etat)
{
    auto cos_theta      = dot(-uv, n);
    vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_perp     = -util::sqrt(1.0 - r_out_parallel.length_squared()) * n;

    return r_out_parallel + r_out_perp;
}

constexpr vec3 reflect(vec3 const& v, vec3 const& n)
{
    return v - 2 * dot(v, n) * n;
}
