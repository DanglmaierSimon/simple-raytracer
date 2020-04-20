#pragma once

#include "material.h"
#include "vec3.h"

class lambertian : public material {
  public:
    lambertian(vec3 const& a)
        : albedo{a}
    {
    }

    bool scatter(const ray&        r_in,
                 const hit_record& rec,
                 vec3&             attenuation,
                 ray&              scattered) const override
    {

        vec3 scatter_direction = rec.normal + random_unit_vector();
        scattered              = ray(rec.p, scatter_direction);
        attenuation            = albedo;
        return true;
    }

  private:
    vec3 albedo;
};