#include <iostream>

#include "./lib/rtweekend.h"
#include "./lib/hittable.h"
#include "./lib/hittable_list.h"
#include "./lib/sphere.h"
#include "./lib/camera.h"

int main() {
  hittable_list world;
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.3));
  world.add(make_shared<sphere>(point3(0,-100.3, -1), 100));
  
  camera cam;
  cam.aspect_ratio = 16.0/9.0;
  cam.img_width = 500;
  cam.max_depth = 50;
  cam.render(world);
}