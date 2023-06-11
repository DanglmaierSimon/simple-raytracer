#include "vec3.h"

vec3 vec3::random()
{
    return vec3(util::random_double(), util::random_double(), util::random_double());
}

vec3 vec3::random(double min, double max)
{
    return vec3(util::random_double(min, max),
                util::random_double(min, max),
                util::random_double(min, max));
}

vec3 random_in_unit_sphere()
{
    while (true) {
        auto p = vec3::random(-1, 1);

        if (p.length_squared() >= 1) {
            continue;
        }
        return p;
    }
}

vec3 random_unit_vector()
{
    const auto a = util::random_double(0, 2 * util::PI);
    const auto z = util::random_double(-1, 1);

    const auto r = util::sqrt(1 - z * z);

    return vec3(r * cos(a), r * sin(a), z);
}

vec3 random_in_unit_disk()
{
    while (true) {
        const auto p = vec3(util::random_double(-1, 1), util::random_double(-1, 1), 0);
        if (p.length_squared() < 1) {
            return p;
        }
    }
}
