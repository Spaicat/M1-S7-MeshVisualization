// Cylinder

// Self include
#include "cylinder.h"

/*!
\brief Create a cylinder given a radius and its height.
\param height The height of the cylinder.
\param radius The radius of the cylinder (Top and Bottom disk).
*/
Cylinder::Cylinder(double height, double radius)
{
    Cylinder::height = height;
    Cylinder::radius = radius;
}
