#include <vector>
#include <iostream>

class Cylinder
{
    protected:
        double radius, height;
	public:
        explicit Cylinder(double, double);

        double Radius() const;
        double Height() const;
};

//! Returns the radius of the cylinder.
inline double Cylinder::Radius() const
{
  return radius;
}

//! Returns the height of the cylinder.
inline double Cylinder::Height() const
{
  return height;
}
