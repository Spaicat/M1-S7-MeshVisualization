#include <vector>
#include <iostream>
#include <QImage>
#include "mesh.h"
#include "meshcolor.h"
#include "color.h"

#include "mathematics.h"

class HeightField
{
    protected:
        std::vector<std::vector<double>> height; //!< 2D array with height value between 0 and 1 (include).
        int width;
        int length;

        // Function that are not accesible from HeightField
        void AddTriangle(int, int, int, int, std::vector<int>&, std::vector<int>&);
        Vector SubstractVector(const Vector& u, const Vector& v);
    public:
        explicit HeightField();
        explicit HeightField(QImage image);
        //explicit HeightField(/* Noise */, double size);

        Mesh generateMesh(double, double);
        MeshColor generateMeshColor(double, double);
        //static HeightField randomNoise();
};
