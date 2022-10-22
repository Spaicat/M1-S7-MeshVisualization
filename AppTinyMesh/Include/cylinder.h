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

inline double Cylinder::Radius() const
{
  return radius;
}

inline double Cylinder::Height() const
{
  return height;
}