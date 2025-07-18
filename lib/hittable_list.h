#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>

// make this itself hittable
// so no need to write loop to go through each!
class hittable_list : public hittable {
  public:
    std::vector<shared_ptr<hittable>> objects;

    // constructors
    // default: leave objects vector empty
    // one arg.: initialize with that object
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void add(shared_ptr<hittable> object) {
      objects.push_back(object);
    }

    bool hit(const ray& r, interval ray_t_interval, hit_record& rec) const override {
      // more efficient solution: just store closest_t instead of the entire hit_record.
      hit_record curr_rec;
      double closest_so_far = infinity;
      bool has_hit = false;

      for (shared_ptr<hittable> object : objects) {
        // it's a pointer so use ->
        if (!object->hit(r, ray_t_interval, curr_rec)) continue;
        has_hit = true;
        if (curr_rec.t < closest_so_far) {
          // we got a closer object so replace
          // BLUNDER: forgot to set rec here
          closest_so_far = curr_rec.t;
          rec = curr_rec;
        }
      }

      return has_hit;
    }
};

#endif