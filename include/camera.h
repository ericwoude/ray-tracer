#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vec3.h"

class camera
{
   public:
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov,
           double aspect_ratio)
    {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        double viewport_height = 2.0 * h;
        double viewport_width = viewport_height * aspect_ratio;

        vec3 w = unit_vector(lookfrom - lookat);
        vec3 u = unit_vector(cross(vup, w));
        vec3 v = cross(w, u);

        origin = lookfrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left = origin - horizontal / 2 - vertical / 2 - w;
    }

    ray get_ray(double s, double t) const
    {
        return ray(origin, lower_left + s * horizontal + t * vertical - origin);
    }

   private:
    point3 origin;
    point3 lower_left;
    vec3 horizontal;
    vec3 vertical;
};

#endif  // CAMERA_H