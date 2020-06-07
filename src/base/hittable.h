#pragma once

#include "hit_record.h"
#include "ray.h"
#include "vec3.h"

#include <memory>

class material;

class hittable {
  public:
    virtual ~hittable() = default;

    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const = 0;
};
