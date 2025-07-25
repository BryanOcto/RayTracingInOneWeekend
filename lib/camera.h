#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {
  public:
    double aspect_ratio = 1.0; // ratio of image width on height
    int img_width = 100; // image width in pixels
    int samples_per_pixel = 10; // for antialiasing
    int max_depth = 10; // maximum number of times ray bounces in scene

    double vfov = 90; // vertical field of view (FOV) angle
    point3 lookfrom = point3(0, 0, 0);
    point3 lookat = point3(0, 0, -1);
    vec3 vup = vec3(0, 1, 0);

    double defocus_angle = 0; // angle of the cone from a point on the plane of perfect focus to the disk at `lookfrom`. Similar to vfov in how it's measured if you just cut a cross-section through the cone (it's symmetrical so doesn't matter where you do that).
    double focus_dist = 10; // distance from `lookfrom` to plane of perfect focus

    void render(const hittable_list& world) {
      initialise();

      // image information
      const int max_colour = 255;
      std::cout << "P3\n";
      std::cout << img_width << " " << img_height << "\n";
      std::cout << max_colour << "\n";

      // actual image
      // BUG: switched up i and j; i is row number, so that's vertical.
      for (int i = 0; i < img_height; i++) {
        std::clog << "\rScanlines remaining: " << (img_height - i) << " " << std::flush;
        for (int j = 0; j < img_width; j++) {
          // aggregate colour of samples
          colour pixel_colour(0, 0, 0);
          for (int sample_index = 0; sample_index < samples_per_pixel; sample_index++) {
            // get the random ray, and then the colour we get from that
            const ray r = get_ray(i, j);
            pixel_colour += ray_colour(r, max_depth, world);
          }

          // find the average of the colours
          pixel_colour *= pixel_samples_scale;
          write_colour(std::cout, pixel_colour);
        }
      }
      std::clog << "\rDone!                                    \n";
    }
  private:
    int img_height; // calculated image height in pixels based on aspect_ratio and img_width
    point3 camera_centre;
    point3 pixel_00_loc; // location of the centre of the top-left corner pixel
    vec3 pixel_delta_u; // horizontal distance between pixel centres
    vec3 pixel_delta_v; // vertical distance between pixel centres
    double pixel_samples_scale; // scaling factor 
    vec3 u, v, w; // basis vectors defining the orthonormal camera basis; u, v for horizontal, vertical on the viewing plane, -w for direction camera is looking in.
    vec3 defocus_disk_u; // defocus disk horizontal radius
    vec3 defocus_disk_v; // defocus disk vertical radius

    void initialise() {
      // setup the image
      img_height = std::max(int(img_width/aspect_ratio), 1);
      
      // setup the viewport and camera
      // viewport_height calculated based on FOV angle
      // see personal notes for more details on conventions.
      const double theta = degrees_to_radians(vfov);
      const double h = std::tan(theta/2);
      const double viewport_height = 2.0 * h * focus_dist;
      const double viewport_width = viewport_height * double(img_width)/img_height; // calculate from actual aspect ratio from number of pixel rounding.
      camera_centre = lookfrom;

      // calculate the u, v, w unit vectors
      w = unit_vector(lookfrom - lookat); // remember we're looking in the direction of -w. This is also critical in deducing the other vector directions.
      u = unit_vector(cross(vup, w));
      v = cross(w, u);

      // calculate V_u (horizontal) and V_v (vertical) across viewport
      const vec3 viewport_u = viewport_width * u;
      const vec3 viewport_v = viewport_height * -v; // remember this should be facing down.

      // calculate delta_u and delta_v (distance between pixel centres)
      pixel_delta_u = viewport_u/img_width;
      pixel_delta_v = viewport_v/img_height;

      // calculate location of top-left pixel and viewport corner
      // BLUNDER: it's -1 on z not on x
      const point3 viewport_centre = camera_centre - focus_dist*w; // looking from the camera and travelling along the focal_length to the plane 
      const point3 viewport_upper_left = viewport_centre - viewport_u/2 - viewport_v/2;
      pixel_00_loc = viewport_upper_left + pixel_delta_u/2 + pixel_delta_v/2;

      // calculate the camera defocus disk basis vectors.
      const double defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle)/2);
      defocus_disk_u = u * defocus_radius;
      defocus_disk_v = v * defocus_radius;

      // calculate pixel sample scaling factor
      // MISTAKE: need to force a cast to double with 1.0;
      // can't just write 1 since that does integer division.
      pixel_samples_scale = 1.0 / samples_per_pixel;
    }

    colour ray_colour(const ray& r, int depth, const hittable& world) const {
      // If we've reached the bounce limit,
      // no more light is gathered, thus return black.
      if (depth <= 0) {
        return colour(0, 0, 0);
      }

      hit_record rec;
      if (world.hit(r, interval(0.001, infinity), rec)) {
        ray scattered;
        colour attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered)) {
          return attenuation * ray_colour(scattered, depth-1, world);
        }
        // MISTAKE: forgot to return no colour (black) for being absorbed.
        return colour(0, 0, 0);

        // vec3 bounce_direction = rec.normal + random_unit_vector();
        // return 0.5*ray_colour(ray(rec.p, bounce_direction), depth-1, world);
      }

      // we don't have a hit so draw the background
      const vec3 unit_direction = unit_vector(r.direction());
      // at this point, unit_direction has -1 <= y <= 1
      // since can be pointing up or down
      // so need to move to 0 <= y <= 1 by +1 then /2
      const double a = (unit_direction.y() + 1)/2;
      return (1.0 - a)*colour(1, 1, 1) + a*colour(0.3, 0.6, 0.9);
    }

    ray get_ray(int i, int j) const {
      // get a ray from a random point on the defocus disk towards a random point within the pixel i, j
      // MISTAKE: offset must be scaled by the delta.
      const vec3 offset = sample_square();
      const point3 random_loc_in_pixel = pixel_00_loc 
        + (j+offset.x())*pixel_delta_u
        + (i+offset.y())*pixel_delta_v;
      
      const vec3 ray_origin = (defocus_angle <= 0) ? camera_centre : defocus_disk_sample();
      const vec3 ray_direction = random_loc_in_pixel - ray_origin;

      return ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const {
      // returns the vector to a random point within the unit square centred at (0, 0, 0)
      // keeping z fixed at 0
      // namely x in [-0.5, 0.5], y in [-0.5, 0.5]
      return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    point3 defocus_disk_sample() const {
      // returns a point within the defocus disk at the camera
      // logic: get a random point in this unit disk, then use the defocus_disk_u and defocus_disk_v that spans across the whole disk to scale the components appropriately.
      const point3 p = random_in_unit_disk();
      return camera_centre + (defocus_disk_u * p[0]) + (defocus_disk_v * p[1]);
    }
};

#endif