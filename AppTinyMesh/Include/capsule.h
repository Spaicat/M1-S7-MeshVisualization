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

inline double Capsule::Radius() const
{
  return radius;
}

inline double Capsule::Height() const
{
  return height;
}
