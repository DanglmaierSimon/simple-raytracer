#pragma once

#include "ray.h"
#include "vec3.h"

class material;

struct hit_record {
  vec3 p;
  vec3 normal;
  shared_ptr<material> mat_ptr;
  double t;
  bool front_face;

  inline void set_face_normal(ray const &r, vec3 const &outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class hittable {
public:
  virtual bool hit(ray const &r, double t_min, double t_max,
                   hit_record &rec) const = 0;
};