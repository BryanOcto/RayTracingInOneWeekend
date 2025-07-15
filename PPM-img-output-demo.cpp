#include <iostream>
#include "./lib/vec3.h"
#include "./lib/colour.h"

int main() {
  const int img_width = 500;
  const int img_height = 250;
  const int max_colour = 255;

  // image information
  std::cout << "P3\n";
  std::cout << img_width << " " << img_height << "\n";
  std::cout << max_colour << "\n";

  // actual image
  // - rightwards (increasing j): more red
  // - downwards (increasing i): more green
  // therefore bottom right corner should be yellow
  for (int i = 0; i < img_height; i++) {
    std::clog << "\rScanlines remaining: " << (img_height - i) << " " << std::flush;
    for (int j = 0; j < img_width; j++) {
      const colour pixel_colour = colour(j/(img_width - 1.0), i/(img_height - 1.0), 0);
      write_colour(std::cout, pixel_colour);
    }
  }
  std::clog << "\rDone!                                    \n";
}

// OLD for loop inside
// // first make it implicitly a double to do division and multiplication
// // then implicitly cast to an integer
// const int r = (j/(img_width - 1.0))*max_colour;
// const int g = (i/(img_height - 1.0))*max_colour;
// const int b = 0;
// std::cout << r << " " << g << " " << b << "\n";

// OLD CODE
// abandoned using FMT because it slowed down compile time too much
// #include <iostream>

// #define FMT_HEADER_ONLY
// #include <fmt/format.h> // from https://stackoverflow.com/questions/63017719/cant-include-stdformat and https://stackoverflow.com/questions/73156022/c-fmt-library-undefined-reference and https://fmt.dev/latest/get-started/ and https://github.com/fmtlib/fmt/issues/2157

// int main() {
//   const int img_width = 500;
//   const int img_height = 500;
//   const int max_colour = 255;

//   std::cout << "P3\n";
//   std::cout << fmt::format("{} {}\n", img_width, img_height);
//   std::cout << max_colour << "\n";

//   for (int i = 0; i < img_height; i++) {
//     for (int j = 0; j < img_width; j++) {
//       const int r = std::min(j, max_colour);
//       const int g = std::min(i, max_colour);
//       const int b = 0;

//       std::cout << fmt::format("{} {} {}\n", r, g, b);
//     }
//   }
// }