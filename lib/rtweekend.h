// Common utilities for this book;
// assume it's included in all code files
#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>


// C++ Std Usings

using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// MISTAKE: we have a *static* distribution and generator,
// thus we can't just pass in the min and max as arguments
// rather must make separate function.
inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator; // this is a good random number generator
    return distribution(generator);
}
// MISTAKE: first *scale up* to (max-min) width; we're not normalising here so NOT division
// then we shift it to min.
inline double random_double(double min, double max) {
    return min + (max - min)*random_double();
}

// Common Headers

#include "colour.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif