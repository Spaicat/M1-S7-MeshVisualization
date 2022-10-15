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

inline double Torus::Radius() const
{
  return radius;
}

inline double Torus::Thickness() const
{
  return thickness;
}
