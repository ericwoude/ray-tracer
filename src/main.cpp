#include <cmath>
#include <cstdlib>
#include <functional>
#include <thread>

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "ray.h"
#include "sphere.h"
#include "triangle.h"
#include "utility.h"
#include "vec3.h"

using std::make_shared;

color ray_color(const ray& r, const hittable& world, int depth)
{
    hit_record rec;

    if (depth <= 0)
        return color(0, 0, 0);

    if (world.hit(r, 0.001, infinity, rec))
    {
        ray scattered;
        color attenuation;

        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);

        return color(0, 0, 0);
    }

    vec3 u_dir = unit_vector(r.direction());
    double t = 0.5 * (u_dir.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

hittable_list generate_world()
{
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2,
                          b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9)
            {
                std::shared_ptr<material> sphere_material;

                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(
                        make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(
                        make_shared<sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(
                        make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int main()
{
    // Screen
    const auto aspect_ratio = 16.0 / 9.0;
    const int width = 400;
    const int height = static_cast<int>(width / aspect_ratio);
    const int sample_amount = 500;
    const int depth = 30;

    // Camera
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture,
               dist_to_focus);

    // Render
    std::cout << "P3\n" << width << " " << height << "\n255\n";
    auto world = generate_world();

    auto partial_render = [cam, world](int s, int e,
                                       std::vector<color>& screen) {
        div_t dv;
        int row = 0;
        int col = 0;

        for (int x = s; x < e; x++)
        {
            dv = std::div(x, width);
            row = (height - 1) - dv.quot;
            col = dv.rem;

            color pixel_color(0, 0, 0);

            for (int k = 0; k < sample_amount; k++)
            {
                double u = (col + random_double()) / (width - 1);
                double v = (row + random_double()) / (height - 1);
                ray r = cam.get_ray(u, v);

                pixel_color += ray_color(r, world, depth);
            }

            screen[x] = pixel_color;
        }
    };

    /*
     *  Divide work between the workers; each worker gets to compute
     *  (width * height) / thead_amount --- amount of pixels.
     */
    std::vector<color> screen(height * width);
    std::vector<std::thread> threads;
    const int thread_amount = 100;
    constexpr double quotient = width * height / thread_amount;
    int start = 0;
    int end = 0;
    for (int i = 0; i < thread_amount; i++)
    {
        start = i * quotient;
        end = (i + 1) * quotient;

        threads.push_back(
            std::thread(partial_render, start, end, std::ref(screen)));
    }

    for (auto& t : threads)
    {
        if (t.joinable())
            t.join();
    }

    for (const auto& c : screen)
    {
        write_color(std::cout, c, sample_amount);
    }

    return 0;
}