#pragma once

#include "ray.h"
#include "vec3.h"

#include <memory>

class material;

class hit_record {
  public:
    hit_record()  = default;
    ~hit_record() = default;

    constexpr void set_face_normal(ray const& r, vec3 const& outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal     = front_face ? outward_normal : -outward_normal;
    }

    vec3                      p;
    vec3                      normal;
    std::shared_ptr<material> mat_ptr{nullptr};
    double                    t{0.0};
    bool                      front_face{false};
};
