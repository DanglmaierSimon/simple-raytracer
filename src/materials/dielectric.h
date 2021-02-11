#pragma once

#include "material.h"

#include "ray.h"
#include "util.h"
#include "vec3.h"

class dielectric : public material {
  public:
    explicit constexpr dielectric(double ri)
        : _ref_idx{ri} {};

    bool scatter(ray const&        r_in,
                 hit_record const& rec,
                 vec3&             attenuation,
                 ray&              scattered) const override
    {
        attenuation = vec3{1.0, 1.0, 1.0};
        double etai_over_etat;

        if (rec.front_face) {
            etai_over_etat = 1.0 / _ref_idx;
        }
        else {
            etai_over_etat = _ref_idx;
        }

        vec3 unit_direction = unit_vector(r_in.direction());

        double cos_theta = std::min(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        if (etai_over_etat * sin_theta > 1.0) {
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered      = ray(rec.p, reflected);
            return true;
        }

        double reflect_prob = schlick(cos_theta, etai_over_etat);

        if (util::random_double() < reflect_prob) {
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered      = ray(rec.p, reflected);
            return true;
        }

        vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
        scattered      = ray(rec.p, refracted);
        return true;
    }

  private:
    double _ref_idx;
};
