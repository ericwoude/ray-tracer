#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <limits>
#include <random>

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double epsilon = std::numeric_limits<double>::epsilon();
const double pi = 3.1415926535897932385;

// Functions
inline double degrees_to_radians(double degrees)
{
    return (degrees * pi) * 180.0;
}

inline double random_double()
{
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    static std::mt19937 gen;
    return dist(gen);
}

inline double random_double(double min, double max)
{
    static std::uniform_real_distribution<double> dist(min, max);
    static std::mt19937 gen;
    return dist(gen);
}

inline double clamp(double x, double min, double max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;

    return x;
}

#endif