#include <fstream>
#include <iostream>

#include "base/hittable.h"
#include "base/hittable_list.h"
#include "base/ppm_image.h"
#include "base/ray.h"
#include "base/vec3.h"
#include "materials/dielectric.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "objects/camera.h"
#include "objects/sphere.h"
#include "util/elapsed_timer.h"
#include "util/profiler.h"
#include "util/util.h"

hittable_list random_scene()
{
    using namespace std;

    elapsed_timer timer;
    timer.start();

    hittable_list world;

    world.add(
        make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(vec3(0.5, 0.5, 0.5))));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            const auto choose_mat = util::random_double();
            const vec3 center(a + 0.9 * util::random_double(),
                              0.2,
                              b + 0.9 * util::random_double());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    const auto albedo = vec3::random() * vec3::random();
                    const auto lam    = make_shared<lambertian>(albedo);
                    const auto s      = make_shared<sphere>(center, 0.2, lam);

                    world.add(s);
                }
                else if (choose_mat < 0.95) {
                    // metal
                    const auto albedo = vec3::random(.5, 1);
                    const auto fuzz   = util::random_double(0, 0.5);
                    const auto m      = make_shared<metal>(albedo, fuzz);
                    const auto s      = make_shared<sphere>(center, 0.2, m);

                    world.add(s);
                }
                else {
                    // glass
                    const auto d = make_shared<dielectric>(1.5);
                    const auto s = make_shared<sphere>(center, 0.2, d);

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

    std::cout << "Creating the world took " << util::format_time(timer.elapsed()) << "."
              << std::endl;

    return world;
}

int main()
{
    elapsed_timer timer_total;
    timer_total.start();

    const int img_width  = 200;
    const int img_height = 100;

    const int samples_per_pixel = 100;

    const double aspect_ratio = double(img_width) / img_height;

    const int max_recursion_depth = 50;

    const int total_scan_lines = img_height - 1;

    ppm_image image{img_width, img_height, samples_per_pixel, "image.ppm"};

    int scan_lines_remaining = total_scan_lines;

    profiler scan_line_profiler(total_scan_lines);

    const auto world = random_scene();

    const vec3   lookfrom(13, 2, 3);
    const vec3   lookat(0, 0, 0);
    const vec3   vup(0, 1, 0);
    const double dist_to_focus = 10.0;
    const double aperture      = 0.1;

    const camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    for (int j = img_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << scan_lines_remaining << " " << std::flush;

        elapsed_timer timer;
        timer.start();

        for (int i = 0; i < img_width; i++) {
            vec3 color(0, 0, 0);

            for (int s = 0; s < samples_per_pixel; s++) {
                const auto u = (i + util::random_double()) / img_width;
                const auto v = (j + util::random_double()) / img_height;

                const ray r = cam.get_ray(u, v);

                color += ray_color(r, world, max_recursion_depth);
            }

            image.set_color(i, j, color);
        }

        scan_lines_remaining--;

        scan_line_profiler.add(timer.nsecs_elapsed());
    }

    auto const image_gen_time = timer_total.elapsed();

    std::cout << std::endl;

    std::cout << "Writing image file..." << std::endl;

    image.write_image();

    auto const total_time = timer_total.elapsed();

    scan_line_profiler.print_profiler_stats();

    std::cout << "Image generation time: " << util::format_time(image_gen_time) << std::endl;
    std::cout << "Total time: " << util::format_time(total_time) << std::endl;

    std::cout << "Done!\n";
    return 0;
}
