// #include <iostream>

#include "./lib/rtweekend.h"
#include "./lib/hittable.h"
#include "./lib/hittable_list.h"
#include "./lib/sphere.h"
#include "./lib/material.h"
#include "./lib/camera.h"

int main() {
  hittable_list world;
  
  // purpose of this is to make it fill the vertical height of the screen I think; this would be half the height of the vfov.
  const double R = std::cos(pi/4);

  const auto material_left = make_shared<lambertian>(colour(0,0,1));
  const auto material_right = make_shared<lambertian>(colour(1,0,0));

  world.add(make_shared<sphere>(point3(-R,0,-1), R, material_left));
  world.add(make_shared<sphere>(point3(R,0,-1), R, material_right));
  
  camera cam;
  cam.aspect_ratio = 16.0/9.0;
  cam.img_width = 500;
  cam.max_depth = 50;
  cam.samples_per_pixel = 100;
  cam.vfov = 90;
  cam.render(world);
}