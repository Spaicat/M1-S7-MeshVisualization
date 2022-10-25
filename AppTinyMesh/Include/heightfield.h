#include <vector>
#include <iostream>
#include <QImage>
#include "mesh.h"

class HeightField
{
    protected:
        std::vector<std::vector<double>> height; //!< 2D array with height value between 0 and 1 (include).
        int width;
        int length;

        void AddTriangle(int, int, int, int, std::vector<int>&, std::vector<int>&);
    public:
        explicit HeightField();
        explicit HeightField(QImage image);
        //explicit HeightField(/* Noise */, double size);

        Mesh generateMesh(double, double);
        //static HeightField randomNoise();
};
