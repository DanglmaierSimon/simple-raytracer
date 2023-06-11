#include "hittable_list.h"

#include <memory>

using std::make_shared;

hittable_list::hittable_list(const shared_ptr<hittable>& obj)
{
    add(obj);
}

void hittable_list::clear()
{
    _objects.clear();
}

void hittable_list::add(const shared_ptr<hittable>& obj)
{
    _objects.push_back(obj);
}

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    hit_record temp_rec;

    bool   hit_anything   = false;
    double closest_so_far = t_max;

    for (auto const& obj : _objects) {
        if (obj->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything   = true;
            closest_so_far = temp_rec.t;
            rec            = temp_rec;
        }
    }

    return hit_anything;
}
