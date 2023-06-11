#pragma once

#include "material.h"
#include "vec3.h"

class metal : public material {
  public:
    constexpr metal(vec3 const& a, double f) noexcept
        : _albedo{a}
        , _fuzz{f < 1 ? f : 1}
    {
    }

    bool scatter(ray const&        r_in,
                 hit_record const& rec,
                 vec3&             attenuation,
                 ray&              scattered) const override
    {

        const vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered            = ray(rec.p, reflected + _fuzz * random_in_unit_sphere());
        attenuation          = _albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

  private:
    vec3   _albedo;
    double _fuzz;
};