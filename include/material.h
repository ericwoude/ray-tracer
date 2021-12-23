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