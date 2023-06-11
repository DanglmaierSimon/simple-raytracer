#include "ray.h"

#include "hittable.h"
#include "material.h"

#include <numeric>

vec3 ray_color(const ray& r, const hittable& world, int depth)
{
    hit_record rec;

    if (depth <= 0) {
        return vec3(0, 0, 0);
    }

    if (world.hit(r, 0.001, std::numeric_limits<double>::infinity(), rec)) {
        ray  scattered;
        vec3 attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }

        return vec3(0, 0, 0);
    }

    const vec3 unit_dir = unit_vector(r.direction());

    const double t = 0.5 * (unit_dir.y() + 1.0);

    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}