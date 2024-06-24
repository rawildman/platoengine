#include "plato/utilities/Triangle.hpp"

#include <cmath>

#include "plato/utilities/Vector3.hpp"

namespace plato::utilities
{
double Triangle::volume() const
{
    const Vector3 a = p1 - p0;
    const Vector3 b = p2 - p0;
    const Vector3 tBCrossA = cross(b, a);

    return std::sqrt(dot(tBCrossA, tBCrossA)) * 0.5;
}

Coordinate Triangle::centroid() const { return (p0 + p1 + p2) / 3; }

}  // namespace plato::utilities
