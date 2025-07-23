#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable {
  public:
    sphere(const point3& centre, double radius, shared_ptr<material> mat) : centre{centre}, radius{std::fmax(0, radius)}, mat{mat} {
      // must initialise the material mat here.
    }
    bool hit(const ray& r, interval ray_t_interval, hit_record& rec) const override {
      const vec3 cq = centre - r.origin();
      const double a = r.direction().length_squared();
      const double h = dot(r.direction(), cq);
      const double c = cq.length_squared() - radius*radius;
      const double discriminant = h*h - a*c;

      // no solution
      if (discriminant < 0) return false;

      // try the two solutions, with preference to closer one
      // store the sqrt_discriminant because sqrt is an expensive operation
      const double sqrt_discriminant = std::sqrt(discriminant);
      double root = (h - sqrt_discriminant)/a;
      if (!ray_t_interval.surrounds(root)) {
        // return false; <-- MISTAKE!!! should only fail if the 2nd one fails too!!!
        root = (h + sqrt_discriminant)/a;
        if (!ray_t_interval.surrounds(root)) {
          return false;
        }
      }

      // now we should have a valid root and we populate the hit_record
      // note we can make the normal a unit vector without using the formula since we know its length is radius based on the geometry of the sphere.
      rec.p = r.at(root);
      vec3 outward_normal = (rec.p - centre) / radius;
      rec.set_face_normal(r, outward_normal);
      rec.t = root;
      rec.mat = mat;

      return true;
    }
  private:
    point3 centre;
    double radius;
    shared_ptr<material> mat;
};

#endif