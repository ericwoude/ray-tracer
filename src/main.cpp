#include <cmath>
#include <cstdlib>
#include <functional>
#include <thread>

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "ray.h"
#include "sphere.h"
#include "triangle.h"
#include "utility.h"
#include "vec3.h"

color ray_color(const ray& r, const hittable& world, int depth)
{
    hit_record rec;

    if (depth <= 0)
        return color(0, 0, 0);

    if (world.hit(r, 0.001, infinity, rec))
    {
        point3 target = rec.p + rec.n + random_in_unit_sphere();
        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
    }

    vec3 u_dir = unit_vector(r.direction());
    double t = 0.5 * (u_dir.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
    // Screen
    const double aspect_ratio = 16.0 / 9.0;
    const int width = 400;
    const int height = static_cast<int>(width / aspect_ratio);
    const int sample_amount = 100;
    const int depth = 50;

    // World
    hittable_list world;
    // world.add(std::make_shared<triangle>(
    // point3(-0.5, 0, -1), point3(0.5, 0, -1), point3(0, 0.5, -1)));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));

    // Camera
    camera cam;

    // Render
    std::array<color, height * width> screen;
    std::cout << "P3\n" << width << " " << height << "\n255\n";

    auto partial_render =
        [cam, world](int s, int e, std::array<color, height * width>& screen) {
            int row = 0;
            int col = 0;
            div_t dv;

            for (int x = s; x < e; x++)
            {
                dv = std::div(x, width);
                col = dv.rem;
                row = (height - 1) - dv.quot;

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
    std::vector<std::thread> threads;
    const int thread_amount = 8;
    const double quotient = width * height / thread_amount;
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