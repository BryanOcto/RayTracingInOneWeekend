// #include <iostream>

#include "./lib/rtweekend.h"
#include "./lib/hittable.h"
#include "./lib/hittable_list.h"
#include "./lib/sphere.h"
#include "./lib/material.h"
#include "./lib/camera.h"

int main() {
  hittable_list world;
  
  auto material_ground = make_shared<lambertian>(colour(0.3, 0.4, 0.1));
  auto material_centre = make_shared<lambertian>(colour(0.2, 0.4, 0.8));
  // auto material_left = make_shared<metal>(colour(0.8, 0.8, 0.8), 0.3);
  auto material_left = make_shared<dialectric>(1.5);
  auto material_bubble = make_shared<dialectric>(1.00/1.50);
  auto material_right = make_shared<metal>(colour(0.9, 0.8, 0.5), 1.0);

  world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground));
  world.add(make_shared<sphere>(point3(0, 0, -1.3), 0.5, material_centre));
  world.add(make_shared<sphere>(point3(-1, 0, -1.3), 0.5, material_left));
  world.add(make_shared<sphere>(point3(-1, 0, -1.3), 0.4, material_bubble));
  world.add(make_shared<sphere>(point3(1, 0, -1.3), 0.5, material_right));

  camera cam;
  cam.aspect_ratio = 16.0/9.0;
  cam.img_width = 500;
  cam.max_depth = 50;
  cam.samples_per_pixel = 100;

  cam.vfov = 20;
  cam.lookfrom = point3(-2, 2, 1);
  cam.lookat = point3(0, 0, -1);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 10;
  cam.focus_dist = 3.4;

  cam.render(world);
}