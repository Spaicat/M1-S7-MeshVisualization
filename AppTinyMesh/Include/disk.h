// Disk

#include <vector>
#include <iostream>

class Disk
{
    protected:
        double radius;
    public:
        explicit Disk(double);

        double Radius() const;
};

inline double Disk::Radius() const
{
  return radius;
}
