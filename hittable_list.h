#pragma once

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

#include "hittable.h"

class hittable_list : public hittable {

public:
  hittable_list() = default;
  hittable_list(shared_ptr<hittable> const &obj) { add(obj); }

  void clear() { _objects.clear(); }
  void add(shared_ptr<hittable> const &obj) { _objects.push_back(obj); }

  virtual bool hit(ray const &r, double t_min, double t_max,
                   hit_record &rec) const override;

private:
  std::vector<shared_ptr<hittable>> _objects;
};

bool hittable_list::hit(ray const &r, double t_min, double t_max,
                        hit_record &rec) const {
  hit_record temp_rec;

  bool hit_anything = false;
  auto closest_so_far = t_max;

  for (auto const &obj : _objects) {
    if (obj->hit(r, t_min, closest_so_far, temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }

  return hit_anything;
}