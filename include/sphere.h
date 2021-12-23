#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable
{
   public:
    sphere() {}
    sphere(point3 c, double r, std::shared_ptr<material> m)
        : center(c), radius(r), mat_ptr(m){};

    virtual bool hit(const ray& r, double t_min, double t_max,
                     hit_record& rec) const override;

    point3 center;
    double radius;
    std::shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max,
                 hit_record& rec) const
{
    vec3 oc = r.origin() - center;

    double a = r.direction().length_squared();
    double h = dot(oc, r.direction());
    double c = oc.length_squared() - radius * radius;
    double discriminant = h * h - a * c;

    if (discriminant < 0)
        return false;

    double d_sqrt = sqrt(discriminant);
    double root = (-h - d_sqrt) / a;
    if (root < t_min || root > t_max)
    {
        root = (-h + d_sqrt) / a;
        if (root < t_min || root > t_max)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif  // SPHERE_H