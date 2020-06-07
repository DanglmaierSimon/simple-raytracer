#pragma once

#include <cmath>
#include <iostream>

class vec3 {
  public:
    constexpr vec3()
        : vec3(0.0, 0.0, 0.0)
    {
    }
    constexpr vec3(double e0, double e1, double e2)
        : _e{e0, e1, e2}
    {
    }

    constexpr double x() const
    {
        return _e[0];
    }
    constexpr double y() const
    {
        return _e[1];
    }
    constexpr double z() const
    {
        return _e[2];
    }

    constexpr vec3 operator-() const
    {
        return vec3(-_e[0], -_e[1], -_e[2]);
    }

    constexpr double operator[](int i) const
    {
        return _e[i];
    }
    constexpr double& operator[](int i)
    {
        return _e[i];
    }

    constexpr vec3& operator+=(vec3 const& v)
    {
        _e[0] += v._e[0];
        _e[1] += v._e[1];
        _e[2] += v._e[2];
        return *this;
    }

    constexpr vec3& operator*=(double t)
    {
        _e[0] *= t;
        _e[1] *= t;
        _e[2] *= t;
        return *this;
    }

    constexpr vec3 operator/=(double t)
    {
        return *this *= 1 / t;
    }

    double           length() const;
    constexpr double length_squared() const
    {
        return _e[0] * _e[0] + _e[1] * _e[1] + _e[2] * _e[2];
    }

    void write_color(std::ostream& os, int samples_per_pixel);

    static vec3 random();
    static vec3 random(double min, double max);

  private:
    double _e[3];
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

vec3 unit_vector(vec3 const& v);

vec3 random_in_unit_sphere();
vec3 random_unit_vector();
vec3 random_in_unit_disk();

vec3           refract(vec3 const& uv, vec3 const& n, double etai_over_etat);
constexpr vec3 reflect(vec3 const& v, vec3 const& n)
{
    return v - 2 * dot(v, n) * n;
}
