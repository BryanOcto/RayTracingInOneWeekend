#include <iostream>

#include "./lib/rtweekend.h"
#include "./lib/hittable.h"
#include "./lib/hittable_list.h"
#include "./lib/sphere.h"

colour ray_colour(const ray& r, const hittable& world) {
  hit_record rec;
  if (world.hit(r, 0, infinity, rec)) {
    const vec3 normal_transformed = (rec.normal + vec3(1, 1, 1))/2; // like below, force to 0<=n<=1
    return colour(normal_transformed.x(), normal_transformed.y(), normal_transformed.z());
  }

  // we don't have a hit so draw the background
  const vec3 unit_direction = unit_vector(r.direction());
  // at this point, unit_direction has -1 <= y <= 1
  // since can be pointing up or down
  // so need to move to 0 <= y <= 1 by +1 then /2
  const double a = (unit_direction.y() + 1)/2;
  return (1.0 - a)*colour(1, 1, 1) + a*colour(0.3, 0.6, 0.9);
}

int main() {
  // PART 1: NUMBERS AND CAMERA
  // setup the image
  const double aspect_ratio = 16.0/9.0;
  const int img_width = 500;
  int img_height = std::max(int(img_width/aspect_ratio), 1);

  // setup the viewport and camera
  const double focal_length = 1.0;
  const double viewport_width = 2.0; // arbitrary size
  const double viewport_height = viewport_width * (double(img_height)/img_width); // calculate from actual aspect ratio from number of pixel rounding.
  const point3 camera_centre = point3(0, 0, 0);

  // calculate V_u (horizontal) and V_v (vertical) across viewport
  const vec3 viewport_u = vec3(viewport_width, 0, 0);
  const vec3 viewport_v = vec3(0, -viewport_height, 0);

  // calculate delta_u and delta_v (distance between pixel centres)
  const vec3 pixel_delta_u = viewport_u/img_width;
  const vec3 pixel_delta_v = viewport_v/img_height;

  // calculate location of top-left pixel and viewport corner
  // BLUNDER: it's -1 on z not on x
  const point3 viewport_centre = focal_length*vec3(0, 0, -1);
  const point3 viewport_upper_left = viewport_centre - viewport_u/2 - viewport_v/2;
  const point3 pixel_00_loc = viewport_upper_left + pixel_delta_u/2 + pixel_delta_v/2;

  // world
  hittable_list world;
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.3));
  world.add(make_shared<sphere>(point3(0,-100.5, -1), 100));

  // PART 2: RENDERING
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
      // get the ray to the current pixel
      const point3 pixel_centre = pixel_00_loc + j*pixel_delta_u + i*pixel_delta_v;
      const vec3 ray_direction = pixel_centre - camera_centre;
      const ray r(camera_centre, ray_direction);

      // get the colour the pixel intersects
      const colour pixel_colour = ray_colour(r, world);
      write_colour(std::cout, pixel_colour);
    }
  }
  std::clog << "\rDone!                                    \n";
}