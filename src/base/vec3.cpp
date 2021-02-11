#include "vec3.h"

void vec3::write_color(std::ostream& os, int samples_per_pixel)
{
    // Divide the color total by the number of samples and gamma-correct
    // for a gamma value of 2.0.
    auto scale = 1.0 / samples_per_pixel;

    auto r = sqrt(scale * x());
    auto g = sqrt(scale * y());
    auto b = sqrt(scale * z());

    os << static_cast<int>(256 * std::clamp(r, 0.0, 0.999)) << " "
       << static_cast<int>(256 * std::clamp(g, 0.0, 0.999)) << " "
       << static_cast<int>(256 * std::clamp(b, 0.0, 0.999)) << "\n";
}

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
