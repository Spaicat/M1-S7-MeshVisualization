#include <vector>
#include <iostream>

class Capsule
{
    protected:
        double radius, height;
    public:
        explicit Capsule(double, double);

        double Radius() const;
        double Height() const;
};

//! Returns the radius of the capsule.
inline double Capsule::Radius() const
{
  return radius;
}

//! Returns the height of the capsule.
inline double Capsule::Height() const
{
  return height;
}
