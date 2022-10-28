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
        int width;
        int length;
        std::vector<std::vector<double>> height; //!< 2D array with height value between 0 and 1 (include).

        void AddTriangle(int, int, int, int, std::vector<int>&, std::vector<int>&);
        Color getColorBetweenGradient(Color, Color, double);
        double smoothMin(double, double, double);
        double smoothMax(double, double, double);
    public:
        explicit HeightField();
        explicit HeightField(QImage image);
        //explicit HeightField(/* Noise */, double size);

        Mesh generateMesh(double, double);
        MeshColor generateMeshColor(double, double);
        void flatten(int x, int y, double radius, double, double);
        //static HeightField randomNoise();
};
