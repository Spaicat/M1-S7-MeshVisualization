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

Color HeightField::getColorBetweenGradient(Color color1, Color color2, double percent)
{
  double resRed = color1[0] + percent * (color2[0] - color1[0]);
  double resGreen = color1[1] + percent * (color2[1] - color1[1]);
  double resBlue = color1[2] + percent * (color2[2] - color1[2]);
  return Color(resRed, resGreen, resBlue);
}

/*!
\brief Constructor empty.
*/
HeightField::HeightField()
{
  this->width = 0;
  this->length = 0;
}

/*!
\brief Constructor empty, but define width and length.
*/
HeightField::HeightField(int width, int length)
{
  this->width = width;
  this->length = length;
  height.resize(width);
  for (int i = 0; i < width; i++)
  {
    height[i].resize(length);
  }
}

/*!
\brief Constructor which take the "value" of each pixel of an image to determine the height.
*/
HeightField::HeightField(QImage image)
{
  this->width = image.width();
  this->length = image.height();
  height.resize(image.width());
  for(int i = 0; i < image.width(); i++)
  {
    height[i].resize(image.height());
    for(int j = 0; j < image.height(); j++)
    {
      QColor color = image.pixelColor(i,j);
      height[i][j] = (double)color.value() / 255; // Max=255 for the value
    }
  }
}

std::vector<double> HeightField::operator[](int n) const
{
  return this->height[n];
}

std::vector<double>& HeightField::operator[](int n)
{
  return this->height[n];
}

int HeightField::getWidth()
{
  return this->width;
}

int HeightField::getLength()
{
  return this->width;
}

Mesh HeightField::generateMesh(double heightMax, double squareSize)
{
  std::vector<Vector> vertices;
  std::vector<Vector> normals;
  std::vector<int> va;
  std::vector<int> na;
  int normalCount = -1;
  double offsetX = (this->width*squareSize) / 2;
  double offsetY = (this->length*squareSize) / 2;
  for (int i = 0; i < this->width; i++)
  {
    for (int j = 0; j < this->length; j++)
    {
      vertices.push_back(Vector(i*squareSize - offsetX, j*squareSize - offsetY, height[i][j]*heightMax));

      if (i > 0 && j > 0)
      {
        // Index
        int firstI = j + this->length * (i-1) - 1;
        int secondI = j + this->length * (i-1);
        int thirdI = j + this->length * i - 1;
        int fourthI = j + this->length * i;

        // Normals of the two triangles
        Vector v1 = vertices[firstI] - vertices[secondI];
        Vector v2 = vertices[fourthI] - vertices[secondI];
        Vector triangleNormal = v1 / v2;
        normals.push_back(triangleNormal);

        v1 = vertices[fourthI] - vertices[thirdI];
        v2 = vertices[firstI] - vertices[thirdI];
        triangleNormal = v1 / v2;
        normals.push_back(triangleNormal);

        normalCount += 2;

        // Triangle
        AddTriangle(firstI, fourthI, secondI, normalCount-1, va, na);
        AddTriangle(firstI, fourthI, thirdI, normalCount, va, na);
      }
    }
  }

  Mesh plane = Mesh(vertices, normals, va, na);
  return plane;
}

MeshColor HeightField::generateMeshColor(Mesh& plane, double mult)
{
  Color gradColor1 = Color();
  std::vector<Color> cols;
  cols.resize(plane.Vertexes());
  std::vector<int> na = plane.NormalIndexes();
  for (int i = 0; i < this->width; i++)
  {
    for (int j = 0; j < this->length; j++)
    {
      int iterator = i * this->length + j;

      if (i > 0 && j > 0)
      {
        // Compute X and Y slope (Distance = 1 and height between 0 and 1)
        double slopeAngle1 = height[i][j] - height[i-1][j];
        double slopeAngle2 = height[i][j] - height[i][j-1];

        // Compute Norm of the vector obtained (pente between 0 and 1)
        double pente = Norm(Vector(slopeAngle1, slopeAngle2, 0));

        // Choice of colors
        Color color1;
        Color color2;
        if (mult*pente < 0.5)
        {
          color1 = Color(89, 189, 64);
          color2 = Color(0, 0, 0);
        }
        else
        {
          color1 = Color(110, 110, 110);
          color2 = Color(0, 0, 0);
        }

        if (height[i][j] > 0.8) {
          color1 = Color(255, 255, 255);
          color2 = Color(35, 35, 35);
        }
        cols[iterator] = this->getColorBetweenGradient(color1, color2, mult*pente);
      }
    }
  }

  return MeshColor(plane, cols, plane.VertexIndexes());
}

void HeightField::flatten(int x, int y, double radius, double floor, double strength)
{
  double rad = pow(radius, 2);
  for (int i = 0; i < this->width; i++)
  {
    for (int j = 0; j < this->length; j++)
    {
      double distance = pow((i-x), 2) + pow((j-y), 2);
      if (distance < rad)
      {
        double strengthCalc = (1 - (distance / rad)) * (1 - (distance / rad)) * strength;
        height[i][j] = strengthCalc * floor + (1 - strengthCalc) * height[i][j];
      }
    }
  }
}
