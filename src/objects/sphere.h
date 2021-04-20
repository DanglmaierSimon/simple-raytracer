#pragma once

#include <memory>
#include <utility>

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {

  public:
    sphere(vec3 center, double radius, std::shared_ptr<material> m) noexcept
        : _center{center}
        , _radius{radius}
        , _mat_ptr{std::move(m)}
    {
    }

  public:
    bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override;

    constexpr vec3 center() const
    {
        return _center;
    }
    constexpr double radius() const
    {
        return _radius;
    }

  private:
    vec3                      _center;
    double                    _radius{0.0};
    std::shared_ptr<material> _mat_ptr;
};

bool sphere::hit(ray const& r, double t_min, double t_max, hit_record& rec) const
{
    vec3 oc = r.origin() - center();

    const auto a            = r.direction().length_squared();
    const auto half_b       = dot(oc, r.direction());
    const auto c            = oc.length_squared() - radius() * radius();
    const auto discriminant = half_b * half_b - a * c;

    if (discriminant > 0) {
        const auto root = sqrt(discriminant);
        auto       temp = (-half_b - root) / a;

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

            const vec3 outward_normal = (rec.p - center()) / radius();

            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = _mat_ptr;
            return true;
        }
    }

    return false;
}
