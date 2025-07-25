// taken from the website
#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material {
  public:
    virtual ~material() = default;

    virtual bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const {
      return false;
    }
};

class lambertian : public material {
  public:
    lambertian(const colour& albedo) : albedo(albedo) {}

    bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const override {
      // find the scatter direction like before, but just in case near zero (cancelling the vectors out) we just set to be the normal vector.
      vec3 scatter_direction = rec.normal + random_unit_vector();
      if (scatter_direction.near_zero()) scatter_direction = rec.normal;
      scattered = ray(rec.p, scatter_direction); // set the outgoing ray
      attenuation = albedo; // set the colour?
      return true; // yes we scattered
    }

  private:
    colour albedo;
};

class metal : public material {
  public:
    metal(const colour& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const override {
      vec3 reflected = reflect(r_in.direction(), rec.normal);
      reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
      scattered = ray(rec.p, reflected);
      attenuation = albedo;
      return (dot(scattered.direction(), rec.normal) > 0); // if in surface then don't scatter; just absorb.
    }
  
  private:
    colour albedo;
    double fuzz;
};

class dialectric : public material {
  public:
    dialectric(double refraction_index) : refraction_index{refraction_index} {}

    bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const override {
      attenuation = colour(1.0, 1.0, 1.0); // fully let the light through

      // this makes sense if you look at the formula for R'_perp with eta/eta' being the scaling factor at the front, and how that's what we pass into our refract function.
      // since if front_face, then we're coming in from the air (refractive index approx. 1) / vacuum, into the material (refraction_index).
      double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

      // we work with unit vectors since our refraction function assumes that both R and R' are unit vectors, or at least of the same length (see personal notes for more details and diagrams from the book).
      vec3 unit_direction = unit_vector(r_in.direction());
      const double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0); // remember want signs to not affect so make them face same direction by taking - of unit_direction.
      const double sin_theta = std::sqrt(1 - cos_theta * cos_theta);

      // see the reflectance function below; for approximation for glass whose "reflectivity...varies with angles"
      bool cannot_refract = ri * sin_theta > 1.0;
      vec3 direction;
      if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
        direction = reflect(unit_direction, rec.normal);
      } else {
        direction = refract(unit_direction, rec.normal, ri);
      }

      scattered = ray(rec.p, direction);
      return true;
    }
  private:
    // Refractive index in vacuum or air.
    // Otherwise, the ratio of: (material's refractive index)/(refractive index of enclosing media)
    double refraction_index;

    // copy-pasted from website
    static double reflectance(double cosine, double refraction_index) {
      // Use Schlick's approximation for reflectance.
      auto r0 = (1 - refraction_index) / (1 + refraction_index);
      r0 = r0*r0;
      return r0 + (1-r0)*std::pow((1 - cosine),5);
    }
};

#endif