#include <vector>
#include <iostream>

#include "mathematics.h"

class Torus
{
    protected:
        double radius, thickness;
    public:
        explicit Torus(double, double);

        double Radius() const;
        double Thickness() const;
};

//! Returns the radius of the torus.
inline double Torus::Radius() const
{
  return radius;
}

//! Returns the thickness of the torus.
inline double Torus::Thickness() const
{
  return thickness;
}
