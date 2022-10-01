/*
 * This file is part of Simple Ray Tracer.
 * (https://github.com/ericwoude/ray-tracer)
 *
 * The MIT License (MIT)
 *
 * Copyright © 2022 Eric van der Woude
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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