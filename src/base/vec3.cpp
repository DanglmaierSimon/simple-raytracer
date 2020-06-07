#include "vec3.h"

#include "util.h"

double vec3::length() const
{
    return sqrt(length_squared());
}

void vec3::write_color(std::ostream& os, int samples_per_pixel)
{
    // Divide the color total by the number of samples and gamma-correct
    // for a gamma value of 2.0.
    auto scale = 1.0 / samples_per_pixel;

    auto r = sqrt(scale * _e[0]);
    auto g = sqrt(scale * _e[1]);
    auto b = sqrt(scale * _e[2]);

    os << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << " "
       << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << " "
       << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << "\n";
}

vec3 vec3::random()
{
    return vec3(random_double(), random_double(), random_double());
}

vec3 vec3::random(double min, double max)
{
    return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

vec3 unit_vector(const vec3& v)
{
    return v / v.length();
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
    auto a = random_double(0, 2 * pi);
    auto z = random_double(-1, 1);

    auto r = sqrt(1 - z * z);

    return vec3(r * cos(a), r * sin(a), z);
}

vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat)
{
    auto cos_theta      = dot(-uv, n);
    vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_perp     = -sqrt(1.0 - r_out_parallel.length_squared()) * n;

    return r_out_parallel + r_out_perp;
}

vec3 random_in_unit_disk()
{
    while (true) {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() < 1) {
            return p;
        }
    }
}
