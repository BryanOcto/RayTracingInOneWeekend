// #include <iostream>

#include "./lib/rtweekend.h"
#include "./lib/hittable.h"
#include "./lib/hittable_list.h"
#include "./lib/sphere.h"
#include "./lib/material.h"
#include "./lib/camera.h"

int main() {
  hittable_list world;
  
  auto material_ground = make_shared<lambertian>(colour(0.7, 0.7, 0.7));
  world.add(make_shared<sphere>(point3(0,-1000,0), 1000, material_ground));

  // this for loop and the three spheres are taken from the website; some parameters modified though.
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = colour::random() * colour::random();
          sphere_material = make_shared<lambertian>(albedo);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = colour::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<dialectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dialectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(colour(0.2, 0.4, 0.8));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(colour(0.5, 0.6, 0.7), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  camera cam;
  cam.aspect_ratio = 16.0/9.0;
  cam.img_width = 1920;
  cam.max_depth = 50;
  cam.samples_per_pixel = 500;

  cam.vfov = 20;
  cam.lookfrom = point3(13, 3, -5);
  cam.lookat = point3(0, 0, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0.6;
  cam.focus_dist = 12;

  cam.render(world);
}