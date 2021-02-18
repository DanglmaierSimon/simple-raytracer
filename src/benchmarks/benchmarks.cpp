#include <fstream>
#include <iostream>
#include <random>

#define USE_FIXED_RANDOM

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

ppm_image do_render(int const            width,
                    int const            height,
                    int const            samples,
                    hittable_list const& world,
                    camera const&        cam,
                    std::string const&   file_name)
{

    const int max_recursion_depth = 50;

    const int total_scan_lines = height - 1;

    ppm_image image(width, height, samples, file_name);

    for (int j = height - 1; j >= 0; --j) {

        for (int i = 0; i < width; i++) {
            vec3 color(0, 0, 0);

            for (int s = 0; s < samples; s++) {
                const auto u = (i + util::random_double()) / width;
                const auto v = (j + util::random_double()) / height;

                const ray r = cam.get_ray(u, v);

                color += ray_color(r, world, max_recursion_depth);
            }

            image.set_color(i, j, color);
        }
    }

    return image;
}

hittable_list create_scene_glass_spheres()
{
    using namespace std;

    hittable_list world;

    world.add(
        make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(vec3(0.5, 0.5, 0.5))));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            const vec3 center(a + 0.9 * util::random_double(),
                              0.2,
                              b + 0.9 * util::random_double());
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

hittable_list create_scene_three_spheres()
{
    using namespace std;

    hittable_list world;

    world.add(
        make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(vec3(0.5, 0.5, 0.5))));

    world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));

    world.add(
        make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));

    world.add(
        make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0)));

    return world;
}

hittable_list create_scene_glass_spheres_one_row()
{
    using namespace std;

    hittable_list world;

    world.add(
        make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(vec3(0.5, 0.5, 0.5))));

    world.add(make_shared<sphere>(vec3(-10, 1, 0), 1.0, make_shared<dielectric>(1.5)));

    world.add(make_shared<sphere>(vec3(-5, 1, 0), 1.0, make_shared<dielectric>(1.5)));

    world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));

    world.add(make_shared<sphere>(vec3(5, 1, 0), 1.0, make_shared<dielectric>(1.5)));

    world.add(make_shared<sphere>(vec3(10, 1, 0), 1.0, make_shared<dielectric>(1.5)));

    // Sphere in the back, colored brown
    world.add(
        make_shared<sphere>(vec3(20, 1, 0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));

    return world;
}

hittable_list create_scene_many_spheres()
{
    using namespace std;

    hittable_list world;

    world.add(
        make_shared<sphere>(vec3(0, -1000, 0), 1000, make_shared<lambertian>(vec3(0.5, 0.5, 0.5))));

    for (int a = -50; a < 31; a++) {
        for (int b = -25; b < 10; b++) {
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

    return world;
}

static void BM_GlassSpheres(benchmark::State& state)
{
    const int img_width         = 200;
    const int img_height        = 100;
    const int samples_per_pixel = 100;

    const double aspect_ratio = double(img_width) / img_height;

    const vec3   lookfrom(13, 2, 3);
    const vec3   lookat(0, 0, 0);
    const vec3   vup(0, 1, 0);
    const double dist_to_focus = 10.0;
    const double aperture      = 0.1;

    const camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    for (auto _ : state) {
        ppm_image image = do_render(img_width,
                                    img_height,
                                    samples_per_pixel,
                                    create_scene_glass_spheres(),
                                    cam,
                                    "image_glass_spheres_" + std::to_string(img_width) + "_"
                                        + std::to_string(samples_per_pixel) + ".ppm");

        state.PauseTiming();

        image.write_image();

        state.ResumeTiming();
    }
}

static void BM_ManySpheres(benchmark::State& state)
{
    const int img_width         = 200;
    const int img_height        = 100;
    const int samples_per_pixel = 100;

    const double aspect_ratio = double(img_width) / img_height;

    const vec3   lookfrom(13, 2, 3);
    const vec3   lookat(0, 0, 0);
    const vec3   vup(0, 1, 0);
    const double dist_to_focus = 10.0;
    const double aperture      = 0.1;

    const camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    for (auto _ : state) {
        ppm_image image = do_render(img_width,
                                    img_height,
                                    samples_per_pixel,
                                    create_scene_many_spheres(),
                                    cam,
                                    "image_many_spheres_" + std::to_string(img_width) + "_"
                                        + std::to_string(samples_per_pixel) + ".ppm");

        state.PauseTiming();

        image.write_image();

        state.ResumeTiming();
    }
}

static void BM_LargeImage(benchmark::State& state)
{
    const int img_width         = 1280;
    const int img_height        = 720;
    const int samples_per_pixel = 100;

    const double aspect_ratio = double(img_width) / img_height;

    const vec3   lookfrom(13, 2, 3);
    const vec3   lookat(0, 0, 0);
    const vec3   vup(0, 1, 0);
    const double dist_to_focus = 10.0;
    const double aperture      = 0.1;

    const camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    for (auto _ : state) {
        ppm_image image = do_render(img_width,
                                    img_height,
                                    samples_per_pixel,
                                    create_scene_three_spheres(),
                                    cam,
                                    "image_three_spheres_" + std::to_string(img_width) + "_"
                                        + std::to_string(samples_per_pixel) + ".ppm");

        state.PauseTiming();

        image.write_image();

        state.ResumeTiming();
    }
}

static void BM_LargeImageThreeSpheresOneRow(benchmark::State& state)
{
    const int img_width         = 1280;
    const int img_height        = 720;
    const int samples_per_pixel = 100;

    const double aspect_ratio = double(img_width) / img_height;

    const vec3   lookfrom(-20, 1, 0);
    const vec3   lookat(0, 1, 0);
    const vec3   vup(0, 1, 0);
    const double dist_to_focus = 10.0;
    const double aperture      = 0.1;

    const camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    for (auto _ : state) {
        ppm_image image = do_render(img_width,
                                    img_height,
                                    samples_per_pixel,
                                    create_scene_glass_spheres_one_row(),
                                    cam,
                                    "image_three_spheres_one_row" + std::to_string(img_width) + "_"
                                        + std::to_string(samples_per_pixel) + ".ppm");

        state.PauseTiming();

        image.write_image();

        state.ResumeTiming();
    }
}

BENCHMARK(BM_GlassSpheres)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_ManySpheres)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_LargeImage)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_LargeImageThreeSpheresOneRow)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();