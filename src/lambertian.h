#pragma once

#include "material.h"
#include "vec3.h"

class lambertian : public material {
  public:
    explicit constexpr lambertian(vec3 const& a)
        : _albedo{a}
    {
    }

    bool scatter(ray const&        r_in,
                 hit_record const& rec,
                 vec3&             attenuation,
                 ray&              scattered) const override
    {
        vec3 scatter_direction = rec.normal + random_unit_vector();

        scattered   = ray(rec.p, scatter_direction);
        attenuation = _albedo;
        return true;
    }

  private:
    vec3 _albedo;
};