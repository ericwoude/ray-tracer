#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "material.h"
#include "utility.h"
#include "vec3.h"

class triangle : public hittable
{
   public:
    triangle() {}
    triangle(point3 t0, point3 t1, point3 t2, std::shared_ptr<material> m)
        : v0(t0), v1(t1), v2(t2), mat_ptr(m){};

    virtual bool hit(const ray& r, double t_min, double t_max,
                     hit_record& rec) const override;

    point3 v0;
    point3 v1;
    point3 v2;
    std::shared_ptr<material> mat_ptr;
};

// Möller–Trumbore intersection algorithm
bool triangle::hit(const ray& r, double t_min, double t_max,
                   hit_record& rec) const
{
    double determinant, f, u, v;
    vec3 h, s, q;

    vec3 A = (r.origin() - v0) - (r.origin() - v2);
    vec3 B = (r.origin() - v0) - (r.origin() - v1);

    // Ray and triangle are parallel if the determinant nears zero
    h = cross(r.direction(), B);
    determinant = dot(A, h);
    if (fabs(determinant) < epsilon)
        return false;

    f = 1.0 / determinant;
    s = r.origin() - v0;
    u = f * dot(s, h);
    if (u < 0.0 || u > 1.0)
        return false;

    q = cross(s, A);
    v = f * dot(r.direction(), q);
    if (v < 0.0 || u + v > 1.0)
        return false;

    // line intersection but not ray intersection
    double t = f * dot(B, q);
    if (t <= epsilon)
        return false;

    rec.t = t;
    rec.p = r.at(rec.t);
    rec.set_face_normal(r, rec.p);
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif  // TRIANGLE_H