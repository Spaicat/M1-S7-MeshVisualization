// Torus

// Self include
#include "torus.h"

/*!
\brief Create a torus given a radius and its thickness.
\param radius The radius of the torus (toroidal).
\param thickness The thickness of the torus (poloidal).
*/
Torus::Torus(double radius, double thickness)
{
    Torus::radius = radius;
    Torus::thickness = thickness;
}
