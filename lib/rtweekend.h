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

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0, 1.0);
    static std::mt19937 generator; // this is a good random number generator
    return distribution(generator);
}

// Common Headers

#include "colour.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif