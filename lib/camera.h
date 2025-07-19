#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera {
  public:
    double aspect_ratio = 1.0; // ratio of image width on height
    int img_width = 100; // image width in pixels
    int samples_per_pixel = 10; // for antialiasing
    int max_depth = 10; // maximum number of times ray bounces in scene

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

    void initialise() {
      // setup the image
      img_height = std::max(int(img_width/aspect_ratio), 1);
      
      // setup the viewport and camera
      const double focal_length = 1.0;
      const double viewport_width = 2.0; // arbitrary size
      const double viewport_height = viewport_width * (double(img_height)/img_width); // calculate from actual aspect ratio from number of pixel rounding.
      camera_centre = point3(0, 0, 0);

      // calculate V_u (horizontal) and V_v (vertical) across viewport
      const vec3 viewport_u = vec3(viewport_width, 0, 0);
      const vec3 viewport_v = vec3(0, -viewport_height, 0);

      // calculate delta_u and delta_v (distance between pixel centres)
      pixel_delta_u = viewport_u/img_width;
      pixel_delta_v = viewport_v/img_height;

      // calculate location of top-left pixel and viewport corner
      // BLUNDER: it's -1 on z not on x
      const point3 viewport_centre = focal_length*vec3(0, 0, -1);
      const point3 viewport_upper_left = viewport_centre - viewport_u/2 - viewport_v/2;
      pixel_00_loc = viewport_upper_left + pixel_delta_u/2 + pixel_delta_v/2;

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
        vec3 bounce_direction = random_on_hemisphere(rec.normal);
        return 0.5*ray_colour(ray(rec.p, bounce_direction), depth-1, world);
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
      // get a ray from the camera centre towards a random point within the pixel i, j
      // MISTAKE: offset must be scaled by the delta.
      const vec3 offset = sample_square();
      const point3 random_loc_in_pixel = pixel_00_loc 
        + (j+offset.x())*pixel_delta_u
        + (i+offset.y())*pixel_delta_v;
      
      const vec3 ray_direction = random_loc_in_pixel - camera_centre;

      return ray(camera_centre, ray_direction);
    }

    vec3 sample_square() const {
      // returns the vector to a random point within the unit square centred at (0, 0, 0)
      // keeping z fixed at 0
      // namely x in [-0.5, 0.5], y in [-0.5, 0.5]
      return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }
};

#endif