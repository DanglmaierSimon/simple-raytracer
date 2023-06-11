#pragma once

#include <vector>

#include "hittable.h"

using std::shared_ptr;

class hittable_list : public hittable {

  public:
    hittable_list() = default;
    ~hittable_list() = default;

    explicit hittable_list(shared_ptr<hittable> const& obj);

    void clear();

    void add(shared_ptr<hittable> const& obj);

    bool hit(ray const& r, double t_min, double t_max, hit_record& rec) const override;

  private:
    std::vector<shared_ptr<hittable>> _objects;
};
