#include <QDebug>
#include "heightfield.h"

/*!
\brief Add a triangle to the geometry.
\param a, b, c Index of the vertices.
\param n Index of the normal.
*/
void HeightField::AddTriangle(int a, int b, int c, int n, std::vector<int>& va, std::vector<int>& na)
{
  va.push_back(a);
  na.push_back(n);
  va.push_back(b);
  na.push_back(n);
  va.push_back(c);
  na.push_back(n);
}

/*!
\brief Constructor which take the "value" of each pixel of an image to determine the height.
*/
HeightField::HeightField()
{
}

/*!
\brief Constructor which take the "value" of each pixel of an image to determine the height.
*/
HeightField::HeightField(QImage image)
{
  this->width = image.width();
  this->length = image.height();
  height.resize(image.width());
  for(int i = 0; i < image.width(); i++){
    height[i].resize(image.height());
    for(int j = 0; j < image.height(); j++){
      QColor color = image.pixelColor(i,j);
      height[i][j] = (double)color.value() / 255; // Max=255 for the value
    }
  }
}

Mesh HeightField::generateMesh(double heightMax, double squareSize)
{
  std::vector<Vector> vertices;
  std::vector<Vector> normals;
  std::vector<int> va;
  std::vector<int> na;
  for (int i = 0; i < this->width; i++)
  {
    for (int j = 0; j < this->length; j++)
    {
      vertices.push_back(Vector(i*squareSize, j*squareSize, height[i][j]*heightMax));
      normals.push_back(Vector(i*squareSize, j*squareSize, 1));

      if (i < this->width-1 && j < this->length-1)
      {
        int firstI = j + this->length * i;
        int secondI = j + this->length * i + 1;
        int thirdI = j + this->length * (i+1);
        int fourthI = j + this->length * (i+1) + 1;
        AddTriangle(firstI, fourthI, secondI, firstI, va, na);
        AddTriangle(firstI, fourthI, thirdI, firstI, va, na);
      }
    }
  }

  Mesh plane = Mesh(vertices, normals, va, na);
  return plane;
}
