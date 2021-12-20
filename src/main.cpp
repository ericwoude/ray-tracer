#include <math.h>

#include "color.h"
#include "ray.h"
#include "vec3.h"

double hit_sphere(const vec3& center, double radius, const ray& r)
{
    vec3 oc = r.origin() - center;

    double a = dot(r.direction(), r.direction());
    double b = 2.0 * dot(oc, r.direction());
    double c = dot(oc, oc) - radius * radius;
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
        return -1.0;
    else
        return (-b - std::sqrt(discriminant)) / (2.0 * a);
}

color ray_color(const ray& r)
{
    double t = hit_sphere(vec3(0, 0, -1), 0.5, r);
    if (t > 0.0)
    {
        vec3 n = unit_vector(r.at(t) - vec3(0, 0, -1));
        return 0.5 * color(n.x() + 1, n.y() + 1, n.z() + 1);
    }

    vec3 u_dir = unit_vector(r.direction());
    t = 0.5 * (u_dir.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
    const double aspect_ratio = 16.0 / 9.0;
    const int width = 400;
    const int height = static_cast<int>(width / aspect_ratio);

    std::cout << "P3\n" << width << " " << height << "\n255\n";

    // Camera
    double viewport_height = 2.0;
    double viewport_width = viewport_height * aspect_ratio;
    double focal_length = 1.0;

    vec3 origin = vec3(0, 0, 0);
    vec3 horizontal = vec3(viewport_width, 0, 0);
    vec3 vertical = vec3(0, viewport_height, 0);
    vec3 lower_left =
        origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

    // Render
    for (int j = height - 1; j >= 0; --j)
    {
        for (int i = 0; i < width; ++i)
        {
            double u = double(i) / (width - 1);
            double v = double(j) / (height - 1);
            ray r(origin, lower_left + u * horizontal + v * vertical - origin);
            color pixel_color = ray_color(r);

            write_color(std::cout, pixel_color);
        }
    }

    return 0;
}