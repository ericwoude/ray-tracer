#include <cmath>

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

    if (world.hit(r, 0, infinity, rec))
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
    std::cout << "P3\n" << width << " " << height << "\n255\n";

    for (int j = height - 1; j >= 0; --j)
    {
        for (int i = 0; i < width; ++i)
        {
            color pixel_color(0, 0, 0);

            for (int x = 0; x < sample_amount; x++)
            {
                double u = (i + random_double()) / (width - 1);
                double v = (j + random_double()) / (height - 1);
                ray r = cam.get_ray(u, v);

                pixel_color += ray_color(r, world, depth);
            }

            write_color(std::cout, pixel_color, sample_amount);
        }
    }

    return 0;
}