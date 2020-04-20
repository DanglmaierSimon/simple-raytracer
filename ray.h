#pragma once

#include "util.h"
#include "vec3.h"

class ray {
  public:
    constexpr ray() = default;
    constexpr ray(vec3 const& origin, vec3 const& direction)
        : _origin{origin}
        , _dir{direction} {};

    constexpr vec3 origin() const
    {
        return _origin;
    }
    constexpr vec3 direction() const
    {
        return _dir;
    }

    constexpr vec3 at(double t) const
    {
        return origin() + t * direction();
    }

  private:
    vec3 _origin;
    vec3 _dir;
};

double hit_sphere(vec3 const& center, double const radius, ray const& r)
{
    vec3 oc = r.origin() - center;

    auto a      = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c      = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;

    if (discriminant < 0) {
        return -1.0;
    }

    return (-half_b - sqrt(discriminant)) / a;
}
