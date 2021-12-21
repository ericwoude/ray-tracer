#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "vec3.h"

class triangle : public hittable
{
   public:
    triangle() {}
    triangle(point3 t0, point3 t1, point3 t2) : v0(t0), v1(t1), v2(t2){};

    virtual bool hit(const ray& r, double t_min, double t_max,
                     hit_record& rec) const override;

    point3 v0;
    point3 v1;
    point3 v2;
};

// Möller–Trumbore intersection algorithm
bool triangle::hit(const ray& r, double t_min, double t_max,
                   hit_record& rec) const
{
    vec3 A = (r.origin() - v0) - (r.origin() - v2);
    vec3 B = (r.origin() - v0) - (r.origin() - v1);

    // Ray and triangle are parallel the determinant nears zero
    vec3 p = (cross(r.direction(), B));
    double determinant = dot(A, p);
    if (fabs(determinant) < 0.0001)
        return false;

    vec3 t = r.origin() - v0;
    double u = dot(t, p) / determinant;
    if (u < 0 || u > 1)
        return false;

    vec3 q = cross(t, A);
    double v = dot(r.direction(), q) / determinant;
    if (v < 0 || u + v > 1)
        return false;

    rec.t = dot(B, q) / determinant;
    rec.p = p;
    vec3 n = unit_vector(cross(A, B));
    rec.set_face_normal(r, n);

    return true;
}

#endif  // TRIANGLE_H