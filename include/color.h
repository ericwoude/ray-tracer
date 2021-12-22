#ifndef COLOR_H
#define COLOR_H

#include <iostream>

#include "utility.h"
#include "vec3.h"

void write_color(std::ostream &out, color pixel_color, int sample_amount)
{
    // Divide the color by the sample amount and correct using gamma 2
    double r = sqrt(pixel_color.x() / sample_amount);
    double g = sqrt(pixel_color.y() / sample_amount);
    double b = sqrt(pixel_color.z() / sample_amount);

    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << " "
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << " "
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << "\n";
}

#endif  // COLOR_H