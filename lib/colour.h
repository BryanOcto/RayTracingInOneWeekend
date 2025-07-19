// taken from the website
// except changing "color" to "colour" xD
#ifndef COLOUR_H
#define COLOUR_H

#include "interval.h"
#include "vec3.h"

using colour = vec3;

inline double linear_to_gamma(double linear_component) {
    // prevent errors with the square root of a negative number
    if (linear_component > 0) return std::sqrt(linear_component);
    return 0;
}

void write_colour(std::ostream& out, const colour& pixel_colour) {
    auto r = pixel_colour.x();
    auto g = pixel_colour.y();
    auto b = pixel_colour.z();

    // apply transformation to gamma space
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // translate the [0,1] component values to the byte range [0,255].
    static const interval intensity(0.000, 0.999);
    const int max_byte = 256;
    int rbyte = int(max_byte * intensity.clamp(r));
    int gbyte = int(max_byte * intensity.clamp(g));
    int bbyte = int(max_byte * intensity.clamp(b));

    // write out the pixel colour components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif