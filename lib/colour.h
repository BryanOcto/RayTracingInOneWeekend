// taken from the website
// except changing "color" to "colour" xD
#ifndef COLOUR_H
#define COLOUR_H

#include "vec3.h"

#include <iostream>

using colour = vec3;

void write_colour(std::ostream& out, const colour& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate the [0,1] component values to the byte range [0,255].
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    // Write out the pixel colour components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif