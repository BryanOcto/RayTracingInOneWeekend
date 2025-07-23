// taken from the website
#ifndef HITTABLE_H
#define HITTABLE_H

class material;

class hit_record {
  public:
    point3 p;
    vec3 normal;
    shared_ptr<material> mat;
    double t;
    bool front_face;

    // we use convention of normal pointing opposite to the ray
    void set_face_normal(const ray& r, const vec3& outward_normal) {
      // pointing in opposite directions
      // therefore coming in from the outside 
      front_face = dot(r.direction(), outward_normal) < 0; // FORGOT TO ADD < 0
      normal = front_face ? outward_normal : -outward_normal;
      // NOTE: we also assume that outward_normal is a UNIT VECTOR, thus we don't normalize again.
      // this is because it's often easier to normalize when we know the geometry, e.g. with sphere using the radius instead of the whole formula.
    }
};

class hittable {
  public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval ray_t_interval, hit_record& rec) const = 0;
};

#endif