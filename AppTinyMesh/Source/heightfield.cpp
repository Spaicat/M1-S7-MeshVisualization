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
\brief Constructor which take the "value" of each pixel of an image to determine the height.
*/
HeightField::HeightField()
{
  this->width = 0;
  this->length = 0;
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
  int normalCount = -1;
  double offsetX = (this->width*squareSize) / 2;
  double offsetY = (this->length*squareSize) / 2;
  this->flatten(this->width/2, this->length/2, 100, 0.5, 0.3);
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

        /*double slopeAngle1 = (vertices[secondI][2] - vertices[fourthI][2]) / (vertices[secondI][0] - vertices[fourthI][0]);
        double slopeAngle2 = (vertices[secondI][2] - vertices[fourthI][2]) / (vertices[secondI][1] - vertices[fourthI][1]);
        qDebug().noquote() << slopeAngle1 << "-" << slopeAngle2;*/

        // Triangle
        AddTriangle(firstI, fourthI, secondI, normalCount-1, va, na);
        AddTriangle(firstI, fourthI, thirdI, normalCount, va, na);
      }
    }
  }

  Mesh plane = Mesh(vertices, normals, va, na);
  return plane;
}

MeshColor HeightField::generateMeshColor(double heightMax, double squareSize)
{
  Color gradColor1 = Color();
  Mesh plane = this->generateMesh(heightMax, squareSize);
  std::vector<Color> cols;
  cols.resize(plane.Vertexes());
  std::vector<int> na = plane.NormalIndexes();
  for (int i = 0; i < this->width; i++)
  {
    for (int j = 0; j < this->length; j++)
    {
      int iterator = i * this->length + j;

      Color color1;
      Color color2;
      if (this->height[i][j] == 0)
      {
        cols[iterator] = Color(0,127,159);
      }
      else if (this->height[i][j] < 0.04)
      {
        cols[iterator] = Color(245, 223, 152);
      }
      else
      {
        color1 = Color(82,204,168);
        color2 = Color(176,213,76);
        cols[iterator] = this->getColorBetweenGradient(color1, color2, this->height[i][j]*0.96);
      }
    }
  }

  return MeshColor(plane, cols, plane.VertexIndexes());
}

// https://iquilezles.org/articles/smin/
double HeightField::smoothMax(double a, double b, double k)
{
  k = -k;
  return smoothMin(a, b, k);
}

double HeightField::smoothMin(double a, double b, double k)
{
  double h = std::clamp((b - a + k) / (2 * k), 0.0, 1.0);
  return a * h + b * (1 - h) - k * h * (1 - h);
}

void HeightField::flatten(int x, int y, double radius, double min, double smoothness)
{
  double rad = pow(radius, 2);
  for (int i = 0; i < this->width; i++)
  {
    for (int j = 0; j < this->length; j++)
    {
      double distance = pow((x-i), 2) + pow((y-j), 2);
      if (distance < rad)
      {
        if (this->height[i][j] > min)
        {
          double res = smoothMax(this->height[i][j]/rad * distance, min, smoothness);
          if (res < this->height[i][j])
            this->height[i][j] = res;
        }
        else
        {
          double res = smoothMin(this->height[i][j]/distance * rad, min, smoothness);
          if (res > this->height[i][j])
            this->height[i][j] = res;
        }
      }
    }
  }
}
