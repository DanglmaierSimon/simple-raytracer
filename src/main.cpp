#include <fstream>
#include <iostream>

#include "camera.h"
#include "dielectric.h"
#include "hittable.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "metal.h"
#include "ray.h"
#include "sphere.h"
#include "util.h"
#include "vec3.h"

inline vec3 ray_color(ray const& r, hittable const& world, int depth)
{
    hit_record rec;

    if (depth <= 0) {
        return vec3(0, 0, 0);
    }

    if (world.hit(r, 0.001, infinity, rec)) {
        ray  scattered;
        vec3 attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }

        return vec3(0, 0, 0);
    }

    vec3 unit_dir = unit_vector(r.direction());

    auto t = 0.5 * (unit_dir.y() + 1.0);

    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

hittable_list random_scene()
{
    hittable_list world;

    world.add(
        make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(vec3(0.5, 0.5, 0.5))));

    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = vec3::random() * vec3::random();
                    auto lam    = make_shared<lambertian>(albedo);
                    auto s      = make_shared<sphere>(center, 0.2, lam);

                    world.add(s);
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = vec3::random(.5, 1);
                    auto fuzz   = random_double(0, 0.5);
                    auto m      = make_shared<metal>(albedo, fuzz);
                    auto s      = make_shared<sphere>(center, 0.2, m);

                    world.add(s);
                }
                else {
                    // glass
                    auto d = make_shared<dielectric>(1.5);
                    auto s = make_shared<sphere>(center, 0.2, d);

                    world.add(s);
                }
            }
        }
    }

    world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));

    world.add(
        make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));

    world.add(
        make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0)));

    return world;
}

int main()
{
    std::ofstream out("image.ppm", std::ios::trunc | std::ios::out);

    const int img_width  = 200;
    const int img_height = 100;

    const int samples_per_pixel = 100;

    const double aspect_ratio = double(img_width) / img_height;

    const int max_recursion_depth = 50;

    const int total_scan_lines = img_height - 1;

    int scan_lines_remaining = total_scan_lines;

    auto world = random_scene();

    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture      = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    out << "P3\n" << img_width << " " << img_height << "\n255\n";

    for (int j = img_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << scan_lines_remaining << " " << std::flush;

        for (int i = 0; i < img_width; i++) {
            vec3 color(0, 0, 0);

            for (int s = 0; s < samples_per_pixel; s++) {
                auto u = (i + random_double()) / img_width;
                auto v = (j + random_double()) / img_height;

                ray r = cam.get_ray(u, v);

                color += ray_color(r, world, max_recursion_depth);
            }
            color.write_color(out, samples_per_pixel);
        }

        scan_lines_remaining--;
    }

    std::cerr << "\nDone!\n";
    return 0;
}