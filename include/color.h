/*
 * This file is part of Simple Ray Tracer.
 * (https://github.com/ericwoude/ray-tracer)
 *
 * The MIT License (MIT)
 *
 * Copyright © 2022 Eric van der Woude
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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