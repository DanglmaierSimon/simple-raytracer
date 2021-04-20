#pragma once

#include "ray.h"
#include "util.h"
#include "vec3.h"

class camera {
  public:
    constexpr camera(vec3 const& lookfrom,
                     vec3 const& lookat,
                     vec3 const& vup,
                     double      vfov, // vertical field of view
                     double      aspect,
                     double      aperture,
                     double      focus_dist // for blur/depth of field simulation
                     )
        : _origin{lookfrom}
        , _lens_radius{aperture / 2}
        , _w{unit_vector(lookfrom - lookat)}
        , _u{unit_vector(cross(vup, _w))}
        , _v{cross(_w, _u)}
    {
        const auto theta       = util::degrees_to_radians(vfov);
        const auto half_height = tan(theta / 2);
        const auto half_width  = aspect * half_height;

        _lower_left_corner = _origin - half_width * focus_dist * _u - half_height * focus_dist * _v
                             - focus_dist * _w;

        _horicontal = 2 * half_width * focus_dist * _u;
        _vertical   = 2 * half_height * focus_dist * _v;
    }

    ray get_ray(double s, double t) const;

  private:
    const vec3   _origin;
    const double _lens_radius;
    vec3         _lower_left_corner;
    vec3         _horicontal;
    vec3         _vertical;
    vec3         _w;
    vec3         _u;
    vec3         _v;
};
