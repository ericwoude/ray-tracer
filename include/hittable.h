#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct hit_record
{
    point3 p;
    vec3 n;
    double t;
    bool front;

    inline void set_face_normal(const ray& r, const vec3& outward_normal)
    {
        front = dot(r.direction(), outward_normal) < 0;
        n = front ? outward_normal : -outward_normal;
    }
};

class hittable
{
   public:
    virtual bool hit(const ray& r, double t_min, double t_max,
                     hit_record& rec) const = 0;
};
#endif  // HITTABLE_H