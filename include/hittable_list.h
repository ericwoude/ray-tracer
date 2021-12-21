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