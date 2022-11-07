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

//! Returns the radius of the disk.
inline double Disk::Radius() const
{
  return radius;
}
