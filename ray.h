#pragma once

#include "util.h"
#include "vec3.h"

class ray {
  public:
    constexpr ray() = default;
    constexpr ray(vec3 const& origin, vec3 const& direction)
        : _origin{origin}
        , _dir{direction} {};

    [[nodiscard]] constexpr vec3 origin() const
    {
        return _origin;
    }
    [[nodiscard]] constexpr vec3 direction() const
    {
        return _dir;
    }

    [[nodiscard]] constexpr vec3 at(double t) const
    {
        return origin() + t * direction();
    }

  private:
    vec3 _origin;
    vec3 _dir;
};
