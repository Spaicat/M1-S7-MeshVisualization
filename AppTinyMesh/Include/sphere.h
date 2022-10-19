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

inline double Sphere::Radius() const
{
  return radius;
}
