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

#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "ray.h"
#include "utility.h"
#include "vec3.h"

class material
{
   public:
    virtual bool scatter(const ray &in, const hit_record &rec,
                         color &attenuation, ray &scattered) const = 0;
};

class lambertian : public material
{
   public:
    lambertian(const color &a) : albedo(a){};

    virtual bool scatter(const ray &in, const hit_record &rec,
                         color &attenuation, ray &scattered) const override
    {
        vec3 scatter_dir = rec.n + random_unit_vector();

        if (scatter_dir.near_zero())
            scatter_dir = rec.n;

        scattered = ray(rec.p, scatter_dir);
        attenuation = albedo;

        return true;
    }

    color albedo;
};

class metal : public material
{
   public:
    metal(const color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1){};

    virtual bool scatter(const ray &in, const hit_record &rec,
                         color &attenuation, ray &scattered) const override
    {
        vec3 reflected = reflect(unit_vector(in.direction()), rec.n);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;

        return dot(scattered.direction(), rec.n) > 0;
    };

    color albedo;
    double fuzz;
};

class dielectric : public material
{
   public:
    dielectric(double r) : refractive_index(r){};

    virtual bool scatter(const ray &in, const hit_record &rec,
                         color &attenuation, ray &scattered) const override
    {
        attenuation = color(1, 1, 1);
        double refraction_ratio =
            rec.front ? (1.0 / refractive_index) : refractive_index;

        vec3 u_dir = unit_vector(in.direction());
        double cos_theta = fmin(dot(-u_dir, rec.n), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;
        if (cannot_refract ||
            reflectance(cos_theta, refraction_ratio) > random_double())
            direction = reflect(u_dir, rec.n);
        else
            direction = refract(u_dir, rec.n, refraction_ratio);

        scattered = ray(rec.p, direction);

        return true;
    }

    double refractive_index;

   private:
    // Schlick's approximation for reflectance
    static double reflectance(double cosine, double ref)
    {
        double r0 = (1 - ref) / (1 + ref);
        r0 *= r0;

        return r0 + (1 - r0) * pow(1 - cosine, 5);
    }
};

#endif  // MATERIAL_H