#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {

  public:
    sphere() = default;
    sphere(vec3 center, double radius, shared_ptr<material> const& m)
        : _center{center}
        , _radius{radius}
        , _mat_ptr{m}
    {
    }

  public:
    virtual bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override;

    vec3 center() const
    {
        return _center;
    }
    double radius() const
    {
        return _radius;
    }

  private:
    vec3                 _center;
    double               _radius;
    shared_ptr<material> _mat_ptr;
};

bool sphere::hit(ray const& r, double t_min, double t_max, hit_record& rec) const
{
    vec3 oc = r.origin() - center();

    auto a            = r.direction().length_squared();
    auto half_b       = dot(oc, r.direction());
    auto c            = oc.length_squared() - radius() * radius();
    auto discriminant = half_b * half_b - a * c;

    if (discriminant > 0) {
        auto root = sqrt(discriminant);
        auto temp = (-half_b - root) / a;

        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);

            vec3 outward_normal = (rec.p - center()) / radius();

            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = _mat_ptr;
            return true;
        }

        temp = (-half_b + root) / a;

        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);

            vec3 outward_normal = (rec.p - center()) / radius();

            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = _mat_ptr;
            return true;
        }
    }

    return false;
}