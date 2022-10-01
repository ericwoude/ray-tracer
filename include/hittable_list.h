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

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>

#include "hittable.h"

class hittable_list : public hittable
{
   public:
    hittable_list() {}
    hittable_list(std::shared_ptr<hittable> o) { add(o); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<hittable> o) { objects.push_back(o); }

    virtual bool hit(const ray& r, double t_min, double t_max,
                     hit_record& rec) const override;

    std::vector<std::shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max,
                        hit_record& rec) const
{
    hit_record tmp;
    bool hit = false;
    double closest = t_max;

    for (const auto& o : objects)
    {
        if (o->hit(r, t_min, closest, tmp))
        {
            hit = true;
            closest = tmp.t;
            rec = tmp;
        }
    }

    return hit;
}

#endif  // HITTABLE_LIST_H