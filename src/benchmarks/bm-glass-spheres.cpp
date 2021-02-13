#include <fstream>
#include <iostream>
#include <random>

#include "camera.h"
#include "dielectric.h"
#include "elapsed_timer.h"
#include "hittable.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "metal.h"
#include "ppm_image.h"
#include "profiler.h"
#include "ray.h"
#include "sphere.h"
#include "util.h"
#include "vec3.h"

#include <benchmark/benchmark.h>

double get_random()
{
    // Fixed seed to guarantee same random numbers each time
    static std::mt19937                           generator(0);
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);

    return distribution(generator);
}

hittable_list create_scene()
{
    using namespace std;

    hittable_list world;

    world.add(
        make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(vec3(0.5, 0.5, 0.5))));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            const vec3 center(a + 0.9 * get_random(), 0.2, b + 0.9 * get_random());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {

                // glass
                const auto d = make_shared<dielectric>(1.5);
                const auto s = make_shared<sphere>(center, 0.2, d);

                world.add(s);
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

static void BM_Render(benchmark::State& state)
{
    const int img_width         = 200;
    const int img_height        = 100;
    const int samples_per_pixel = 100;

    ppm_image image{img_width,
                    img_height,
                    samples_per_pixel,
                    "image" + std::to_string(img_width) + "_" + std::to_string(samples_per_pixel)
                        + ".ppm"};

    for (auto _ : state) {

        const double aspect_ratio = double(img_width) / img_height;

        const int max_recursion_depth = 50;

        const int total_scan_lines = img_height - 1;

        const auto world = create_scene();

        const vec3   lookfrom(13, 2, 3);
        const vec3   lookat(0, 0, 0);
        const vec3   vup(0, 1, 0);
        const double dist_to_focus = 10.0;
        const double aperture      = 0.1;

        const camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

        for (int j = img_height - 1; j >= 0; --j) {

            for (int i = 0; i < img_width; i++) {
                vec3 color(0, 0, 0);

                for (int s = 0; s < samples_per_pixel; s++) {
                    const auto u = (i + get_random()) / img_width;
                    const auto v = (j + get_random()) / img_height;

                    const ray r = cam.get_ray(u, v);

                    color += ray_color(r, world, max_recursion_depth);
                }

                image.set_color(i, j, color);
                benchmark::ClobberMemory();
            }
        }
    }

    image.write_image();
}

BENCHMARK(BM_Render)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();