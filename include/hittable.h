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

#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class material;

struct hit_record
{
    point3 p;
    vec3 n;
    std::shared_ptr<material> mat_ptr;
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