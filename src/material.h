#pragma once

#include "hittable.h"
#include "ray.h"
#include "vec3.h"

class material {
  public:
    virtual bool scatter(const ray&        r_in,
                         const hit_record& rec,
                         vec3&             attenuation,
                         ray&              scattered) const = 0;
};

inline constexpr double schlick(double cosine, double ref_idx)
{
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0      = r0 * r0;

    return r0 + (1 - r0) * pow((1 - cosine), 5);
}