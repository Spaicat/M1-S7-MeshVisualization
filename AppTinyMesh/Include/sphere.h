#include <vector>
#include <iostream>

#include "mathematics.h"

class Sphere
{
    protected:
        double radius;
    public:
        explicit Sphere(double);

        double Radius() const;
};

//! Returns the radius of the sphere.
inline double Sphere::Radius() const
{
  return radius;
}
