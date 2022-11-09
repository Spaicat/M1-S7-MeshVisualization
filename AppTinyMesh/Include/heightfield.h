#include <vector>
#include <iostream>
#include <QImage>
#include <algorithm>
#include "mesh.h"
#include "meshcolor.h"
#include "color.h"

class HeightField
{
    protected:
        int width;
        int length;
        std::vector<std::vector<double>> height; //!< 2D array with height value between 0 and 1 (include).

        void AddTriangle(int, int, int, int, std::vector<int>&, std::vector<int>&);
        Color getColorBetweenGradient(Color, Color, double);
    public:
        explicit HeightField();
        explicit HeightField(QImage image);
        explicit HeightField(int width, int length);

        std::vector<double> operator[](int n) const;
        std::vector<double>& operator[](int n);
        int getWidth();
        int getLength();

        MeshColor generateMesh(double, double, double);
        Color generateColor(int, int, double);
        void flatten(int, int, double, double, double);
};
