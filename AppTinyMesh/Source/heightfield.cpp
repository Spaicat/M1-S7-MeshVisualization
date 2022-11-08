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

//! Gets the i-th coordinate of the height field.
std::vector<double> HeightField::operator[](int i) const
{
  return this->height[i];
}

//! Returns the i-th coordinate of height field.
std::vector<double>& HeightField::operator[](int i)
{
  return this->height[i];
}

/*!
\brief Get the width.
*/
int HeightField::getWidth()
{
  return this->width;
}

/*!
\brief Get the length.
*/
int HeightField::getLength()
{
  return this->width;
}

/*!
\brief Generate a mesh with the object parameters.
\param heightMax Max height that the mesh can't exceed.
\param squareSize Size of one square (4 vertices) of the mesh.
\return The mesh generated.
*/
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
      vertices.push_back(Vector(i*squareSize - offsetX, (this->length-j)*squareSize - offsetY, height[i][j]*heightMax));

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
        Vector triangleNormal = v2 / v1;
        normals.push_back(triangleNormal);

        v1 = vertices[fourthI] - vertices[thirdI];
        v2 = vertices[firstI] - vertices[thirdI];
        triangleNormal = v2 / v1;
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

/*!
\brief Color an height field mesh based on the slope.
\param plane The height field mesh to be colored.
\param mult Coefficient to amplify the slope (thus the color).
\return The mesh color generated.
*/
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

      // Compute X and Y slope (Distance = 1 and height between 0 and 1)
      double slopeAngle1 = 0;
      double slopeAngle2 = 0;
      if (i > 0 && j > 0)
      {
        slopeAngle1 = height[i][j] - height[i-1][j];
        slopeAngle2 = height[i][j] - height[i][j-1];
      }
      else if (i > 0)
      {
        slopeAngle1 = height[i][j] - height[i-1][j];
        slopeAngle2 = height[i][j] - height[i][j+1];
      }
      else if (j > 0)
      {
        slopeAngle1 = height[i][j] - height[i+1][j];
        slopeAngle2 = height[i][j] - height[i][j-1];
      }
      else
      {
        slopeAngle1 = height[i][j] - height[i+1][j];
        slopeAngle2 = height[i][j] - height[i][j+1];
      }

      // Compute Norm of the vector obtained (pente between 0 and 1)
      double pente = Norm(Vector(slopeAngle1, slopeAngle2, 0));

      // Compute color gradient
      Color color1;
      Color color2;

      if (height[i][j] > 0.8) {
        color1 = Color(255, 255, 255);
        color2 = Color(35, 35, 35);
      }
      else
      {
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
      }
      cols[iterator] = this->getColorBetweenGradient(color1, color2, mult*pente);
    }
  }

  return MeshColor(plane, cols, plane.VertexIndexes());
}

/*!
\brief Flatten a part (in a radius) of the height field (with gradation to the side).
\param x The X position in the height field corresponding to the center of the flatten operation.
\param y The Y position in the height field corresponding to the center of the flatten operation.
\param radius The radius of the area that will be flatten.
\param floor The floor height to flatten to (The goal).
\param strength Coefficient to amplify the strength of the flattening.
\return The mesh color generated.
*/
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
        // Compute a gradient (Flattening strength is stronger as we get closer to the center)
        double strengthCalc = pow((1 - (distance / rad)), 2) * strength;
        height[i][j] = strengthCalc * floor + (1 - strengthCalc) * height[i][j];
      }
    }
  }
}
