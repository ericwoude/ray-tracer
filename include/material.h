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
    metal(const color &a) : albedo(a){};

    virtual bool scatter(const ray &in, const hit_record &rec,
                         color &attenuation, ray &scattered) const override
    {
        vec3 reflected = reflect(unit_vector(in.direction()), rec.n);
        scattered = ray(rec.p, reflected);
        attenuation = albedo;

        return dot(scattered.direction(), rec.n) > 0;
    };

    color albedo;
};

#endif  // MATERIAL_H