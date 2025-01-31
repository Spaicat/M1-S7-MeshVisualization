#include "mesh.h"
#include <QDebug>
#include <string>

/*!
\class Mesh mesh.h

\brief Core triangle mesh class.
*/



/*!
\brief Initialize the mesh to empty.
*/
Mesh::Mesh()
{
}

/*!
\brief Initialize the mesh from a list of vertices and a list of triangles.

Indices must have a size multiple of three (three for triangle vertices and three for triangle normals).

\param vertices List of geometry vertices.
\param indices List of indices wich represent the geometry triangles.
*/
Mesh::Mesh(const std::vector<Vector>& vertices, const std::vector<int>& indices) :vertices(vertices), varray(indices)
{
  normals.resize(vertices.size(), Vector::Z);
}

/*!
\brief Create the mesh.

\param vertices Array of vertices.
\param normals Array of normals.
\param va, na Array of vertex and normal indexes.
*/
Mesh::Mesh(const std::vector<Vector>& vertices, const std::vector<Vector>& normals, const std::vector<int>& va, const std::vector<int>& na) :vertices(vertices), normals(normals), varray(va), narray(na)
{
}

/*!
\brief Reserve memory for arrays.
\param nv,nn,nvi,nvn Number of vertices, normals, vertex indexes and vertex normals.
*/
void Mesh::Reserve(int nv, int nn, int nvi, int nvn)
{
  vertices.reserve(nv);
  normals.reserve(nn);
  varray.reserve(nvi);
  narray.reserve(nvn);
}

/*!
\brief Empty
*/
Mesh::~Mesh()
{
}

/*!
\brief Smooth the normals of the mesh.

This function weights the normals of the faces by their corresponding area.
\sa Triangle::AreaNormal()
*/
void Mesh::SmoothNormals()
{
  // Initialize 
  normals.resize(vertices.size(), Vector::Null);

  narray = varray;

  // Accumulate normals
  for (int i = 0; i < varray.size(); i += 3)
  {
    Vector tn = Triangle(vertices[varray.at(i)], vertices[varray.at(i + 1)], vertices[varray.at(i + 2)]).AreaNormal();
    normals[narray[i + 0]] += tn;
    normals[narray[i + 1]] += tn;
    normals[narray[i + 2]] += tn;
  }

  // Normalize 
  for (int i = 0; i < normals.size(); i++)
  {
    Normalize(normals[i]);
  }
}

/*!
\brief Add a smooth triangle to the geometry.
\param a, b, c Index of the vertices.
\param na, nb, nc Index of the normals.
*/
void Mesh::AddSmoothTriangle(int a, int na, int b, int nb, int c, int nc)
{
  varray.push_back(a);
  narray.push_back(na);
  varray.push_back(b);
  narray.push_back(nb);
  varray.push_back(c);
  narray.push_back(nc);
}

/*!
\brief Add a triangle to the geometry.
\param a, b, c Index of the vertices.
\param n Index of the normal.
*/
void Mesh::AddTriangle(int a, int b, int c, int n)
{
  varray.push_back(a);
  narray.push_back(n);
  varray.push_back(b);
  narray.push_back(n);
  varray.push_back(c);
  narray.push_back(n);
}

/*!
\brief Add a smmoth quadrangle to the geometry.

Creates two smooth triangles abc and acd.

\param a, b, c, d  Index of the vertices.
\param na, nb, nc, nd Index of the normal for all vertices.
*/
void Mesh::AddSmoothQuadrangle(int a, int na, int b, int nb, int c, int nc, int d, int nd)
{
  // First triangle
  AddSmoothTriangle(a, na, b, nb, c, nc);

  // Second triangle
  AddSmoothTriangle(a, na, c, nc, d, nd);
}

/*!
\brief Add a quadrangle to the geometry.

\param a, b, c, d  Index of the vertices and normals.
*/
void Mesh::AddQuadrangle(int a, int b, int c, int d)
{
  AddSmoothQuadrangle(a, a, b, b, c, c, d, d);
}

/*!
\brief Compute the bounding box of the object.
*/
Box Mesh::GetBox() const
{
  if (vertices.size() == 0)
  {
    return Box::Null;
  }
  return Box(vertices);
}

/*!
\brief Creates an axis aligned box.

The object has 8 vertices, 6 normals and 12 triangles.
\param box The box.
*/
Mesh::Mesh(const Box& box)
{
  // Vertices
  vertices.resize(8);

  for (int i = 0; i < 8; i++)
  {
    vertices[i] = box.Vertex(i);
  }

  // Normals
  normals.push_back(Vector(-1, 0, 0));
  normals.push_back(Vector(1, 0, 0));
  normals.push_back(Vector(0, -1, 0));
  normals.push_back(Vector(0, 1, 0));
  normals.push_back(Vector(0, 0, -1));
  normals.push_back(Vector(0, 0, 1));

  // Reserve space for the triangle array
  varray.reserve(12 * 3);
  narray.reserve(12 * 3);

  AddTriangle(0, 2, 1, 4);
  AddTriangle(1, 2, 3, 4);

  AddTriangle(4, 5, 6, 5);
  AddTriangle(5, 7, 6, 5);

  AddTriangle(0, 4, 2, 0);
  AddTriangle(4, 6, 2, 0);

  AddTriangle(1, 3, 5, 1);
  AddTriangle(3, 7, 5, 1);

  AddTriangle(0, 1, 5, 2);
  AddTriangle(0, 5, 4, 2);

  AddTriangle(3, 2, 7, 3);
  AddTriangle(6, 7, 2, 3);
}

/*!
\brief Creates an axis aligned disk.

The object has n+1 vertices, 1 normal and n triangles.
\param disk The disk.
\param n The resolution (number of subdivision).
*/
Mesh::Mesh(const Disk& disk, int n)
{
  // Vertices
  // n+1 with the center
  vertices.resize(n+1);

  vertices[0] = Vector(0, 0, 0);
  for (int i = 1; i < n+1; i++)
  {
    double theta = 2*M_PI*i/n;
    double x = std::cos(theta) * disk.Radius();
    double y = std::sin(theta) * disk.Radius();
    vertices[i] = Vector(x, y, 0);
  }

  // Normal
  normals.push_back(Vector(0, 0, 1));

  // Reserve space for the triangle array
  varray.reserve(n * 3);
  narray.reserve(n * 3);

  for (int i = 0; i < n; i++)
  {
    int firstI = i+1;
    int secondI = (i+1) % n + 1;
    AddTriangle(0, firstI, secondI, 0);
  }
}

/*!
\brief Creates an axis aligned cylinder.

The object has 2*n+2 vertices, n+2 normals and 3*n triangles.
\param cylinder The cylinder.
\param n The resolution (number of subdivision).
*/
Mesh::Mesh(const Cylinder& cylinder, int n)
{
  // Vertices
  // 2*n+2 -> 2*n for bottom and top vertices and +2 for the two disk centers
  vertices.resize(2*n+2);

  vertices[0] = Vector(0, 0, cylinder.Height() / 2);
  vertices[1] = Vector(0, 0, -cylinder.Height() / 2);
  normals.push_back(Vector(0, 0, 1));
  normals.push_back(Vector(0, 0, -1));
  for (int i = 2; i < 2*n+2; i+=2)
  {
    double theta = 2*M_PI*i/2/n;
    double x = std::cos(theta) * cylinder.Radius();
    double y = std::sin(theta) * cylinder.Radius();
    double z = cylinder.Height() / 2;
    vertices[i] = Vector(x, y, z); // Top
    vertices[i+1] = Vector(x, y, -z); // Bottom
  }

  // Reserve space for the triangle array
  varray.reserve((3*n) * 3);
  narray.reserve((3*n) * 3);

  for (int i = 2; i <= n*2; i+=2)
  {
    int firstI = i;
    int secondI = i+1;
    int thirdI = i % (n*2) + 2;
    int fourthI = i % (n*2) + 3;

    // Normal - Cross product
    Vector v1 = vertices[secondI] - vertices[firstI];
    Vector v2 = vertices[thirdI] - vertices[firstI];
    Vector triangleNormal = v1 / v2;
    normals.push_back(triangleNormal);

    // Triangles
    AddTriangle(0, firstI, thirdI, 0); // Top disk triangle
    AddTriangle(1, secondI, fourthI, 1); // Bottom disk triangle
    AddTriangle(firstI, secondI, fourthI, 1 + i/2); // First triangle of cylinder
    AddTriangle(fourthI, thirdI, firstI, 1 + i/2); // Second triangle of cylinder
  }
}

/*!
\brief Creates an axis aligned sphere.

The object has n*(n-1) + 2 vertices, n*n normals and n*n triangles.
\param sphere The sphere.
\param n The resolution (number of subdivision).
*/
Mesh::Mesh(const Sphere& sphere, int n)
{
  // Vertices
  vertices.resize(n*(n-1) + 2);

  for (int i = 0; i < n-1; i++)
  {
    double phi = M_PI*(i+1)/n;
    for (int j = 0; j < n; j++)
    {
      double theta = 2*M_PI*j/n;
      double x = std::sin(phi) * std::cos(theta) * sphere.Radius();
      double y = std::sin(phi) * std::sin(theta) * sphere.Radius();
      double z = std::cos(phi) * sphere.Radius();
      vertices[j + n * i + 1] = Vector(x, y, z);
    }
  }

  // Reserve space for the triangle array
  varray.reserve((n * n) * 3);
  narray.reserve((n * n) * 3);

  // Top and Bottom vertices and triangles
  int lastElement = n*(n-1) + 1;
  vertices[0] = Vector(0, 0, sphere.Radius());
  normals.push_back(Vector(0, 0, -1));
  vertices[lastElement] = Vector(0, 0, -sphere.Radius());
  normals.push_back(Vector(0, 0, 1));

  for (int i = 0; i < n; i++)
  {
    int firstI = i+1;
    int secondI = (i+1) % n + 1;
    AddTriangle(0, firstI, secondI, 1);
    firstI += n*(n-2);
    secondI += n*(n-2);
    AddTriangle(lastElement, firstI, secondI, 0);
  }

  // All the remaining sphere
  for (int i = 0; i < n-2; i++) // n-2 to remove top and bottom vertices
  {
    int i_next = i+1;
    for (int j = 0; j < n; j++)
    {
      int j_next = (j+1) % n;
      int firstI = i * n + j + 1;
      int secondI = i * n + j_next + 1;
      int thirdI = i_next * n + j + 1;
      int fourthI = i_next * n + j_next + 1;

      // Normal - Cross product
      Vector v1 = vertices[secondI] - vertices[firstI];
      Vector v2 = vertices[thirdI] - vertices[firstI];
      Vector triangleNormal = v2 / v1;
      normals.push_back(triangleNormal);

      // Triangles
      AddTriangle(firstI, secondI, thirdI, firstI+1);
      AddTriangle(secondI, thirdI, fourthI, firstI+1);
    }
  }
}

/*!
\brief Creates an axis aligned torus.

The object has n_toroidal*n_poloidal vertices, n_toroidal*n_poloidal normals and n_toroidal*n_poloidal triangles.
\param torus The torus.
\param n_toroidal The resolution of toroidal direction.
\param n_poloidal The resolution of poloidal direction.
*/
Mesh::Mesh(const Torus& torus, int n_toroidal, int n_poloidal)
{
  // Vertices
  vertices.resize(n_toroidal * n_poloidal);

  for (int i = 0; i < n_toroidal; i++)
  {
    for (int j = 0; j < n_poloidal; j++)
    {
      double theta_toroidal = 2*M_PI*i/n_toroidal;
      double theta_poloidal = 2*M_PI*j/n_poloidal;
      double x = std::cos(theta_poloidal) * ((std::cos(theta_toroidal) * torus.Thickness()) + torus.Radius());
      double y = std::sin(theta_poloidal) * ((std::cos(theta_toroidal) * torus.Thickness()) + torus.Radius());
      double z = std::sin(theta_toroidal) * torus.Thickness();
      vertices[j + n_poloidal * i] = Vector(x, y, z);
    }
  }

  // Reserve space for the triangle array
  varray.reserve((n_toroidal * n_poloidal) * 3);
  narray.reserve((n_toroidal * n_poloidal) * 3);

  for (int i = 0; i < n_toroidal; i++)
  {
    int i_next = (i+1) % n_toroidal;
    for (int j = 0; j < n_poloidal; j++)
    {
      int j_next = (j+1) % n_poloidal;
      int firstI = i * n_poloidal + j;
      int secondI = i * n_poloidal + j_next;
      int thirdI = i_next * n_poloidal + j;
      int fourthI = i_next * n_poloidal + j_next;

      // Normal - Cross product
      Vector v1 = vertices[secondI] - vertices[firstI];
      Vector v2 = vertices[thirdI] - vertices[firstI];
      Vector triangleNormal = v1 / v2;
      normals.push_back(triangleNormal);

      // Trianles
      AddSmoothTriangle(firstI, firstI, secondI, firstI, thirdI, firstI);
      AddSmoothTriangle(secondI, firstI, thirdI, firstI, fourthI, firstI);
    }
  }
}

/*!
\brief Creates an axis aligned capsule.

The object has n*(n-1) + 2 vertices, n*n normals and n*n triangles.
\param capsule The capsule.
\param n The resolution (number of subdivision).
*/
Mesh::Mesh(const Capsule& capsule, int n)
{
  // Vertices
  vertices.resize(n*(n-1) + 2);

  for (int i = 0; i < n-1; i++)
  {
    double phi = M_PI*(i+1)/n;
    for (int j = 0; j < n; j++)
    {
      double theta = 2*M_PI*j/n;
      double x = std::sin(phi) * std::cos(theta) * capsule.Radius();
      double y = std::sin(phi) * std::sin(theta) * capsule.Radius();
      double z = std::cos(phi) * capsule.Radius();
      if (i < n / 2)
      {
        vertices[j + n * i + 1] = Vector(x, y, z + capsule.Height() / 2);
      }
      else
      {
        vertices[j + n * i + 1] = Vector(x, y, z - capsule.Height() / 2);
      }
    }
  }

  // Reserve space for the triangle array
  varray.reserve((n * n) * 3);
  narray.reserve((n * n) * 3);

  // Top and Bottom vertices and triangles
  int lastElement = n*(n-1) + 1;
  vertices[0] = Vector(0, 0, (capsule.Height() / 2) + capsule.Radius());
  normals.push_back(Vector(0, 0, -1));
  vertices[lastElement] = Vector(0, 0, - (capsule.Height() / 2) - capsule.Radius());
  normals.push_back(Vector(0, 0, 1));

  for (int i = 0; i < n; i++)
  {
    int firstI = i+1;
    int secondI = (i+1) % n + 1;
    AddTriangle(0, firstI, secondI, 1);
    firstI += n*(n-2);
    secondI += n*(n-2);
    AddTriangle(lastElement, firstI, secondI, 0);
  }

  // All the remaining sphere
  for (int i = 0; i < n-2; i++) // n-2 to remove top and bottom vertices
  {
    int i_next = i+1;
    for (int j = 0; j < n; j++)
    {
      int j_next = (j+1) % n;
      int firstI = i * n + j + 1;
      int secondI = i * n + j_next + 1;
      int thirdI = i_next * n + j + 1;
      int fourthI = i_next * n + j_next + 1;

      // Normal - Cross product
      Vector v1 = vertices[secondI] - vertices[firstI];
      Vector v2 = vertices[thirdI] - vertices[firstI];
      Vector triangleNormal = v2 / v1;
      normals.push_back(triangleNormal);

      // Triangles
      AddTriangle(firstI, secondI, thirdI, firstI+1);
      AddTriangle(secondI, thirdI, fourthI, firstI+1);
    }
  }
}

/*!
\brief Operation to transform the mesh (Rotation, Scale).

Apply the transformation matrix to each vertices (and normals) of the mesh.
\param m The matrix.
*/
void Mesh::Transform(const Matrix& m)
{
  for (int i = 0; i < vertices.size(); i++)
  {
    vertices[i] = m * vertices[i];
  }
  for (int i = 0; i < normals.size(); i++)
  {
    normals[i] = m * normals[i];
  }
}

/*!
\brief Operation to translate the mesh.

Apply the a translation to each vertices (and normals) of the mesh.
\param v The vector tranlating each vertices.
*/
void Mesh::Translate(const Vector& v)
{
  for (int i = 0; i < vertices.size(); i++)
  {
    vertices[i] += v;
  }
  for (int i = 0; i < normals.size(); i++)
  {
    normals[i] += normals[i];
  }
}

/*!
\brief Operation to merge the mesh with another one.

Add vertices and normals to the current mesh.
\param m The mesh to merge to the current mesh.
*/
void Mesh::Merge(const Mesh& m)
{
  int thisVertices = this->Vertexes();
  int thisNormals = this->Normals();
  for (int i = 0; i < m.Vertexes(); i++)
  {
    vertices.push_back(m.Vertex(i));
  }
  for (int i = 0; i < m.Normals(); i++)
  {
    normals.push_back(m.Normal(i));
  }

  int mTriangles = m.Triangles();
  for (int i = 0; i < mTriangles; i++)
  {
    AddTriangle(
      thisVertices + m.VertexIndex(i, 0),
      thisVertices + m.VertexIndex(i, 1),
      thisVertices + m.VertexIndex(i, 2),
      thisNormals + m.NormalIndex(i, 0)
    );
  }
}

/*!
\brief Operation to translate vertices located in a sphere.

Each vertices in the sphere (given in params) is translated in one direction.
The closer to the center of the sphere is the vertex, the stronger is the translation.
\param center The center of the sphere.
\param radius The radius of the sphere.
\param direction The vector direction that indicate where to translate to and the "strength".
*/
void Mesh::SphereWarp(const Vector& center, double radius, const Vector& direction)
{
  double rad = pow(radius, 2);
  for (int i = 0; i < this->Vertexes(); i++)
  {
    Vector curr = this->Vertex(i);
    double distance = pow((center[0]-curr[0]), 2) + pow((center[1]-curr[1]), 2) + pow((center[2]-curr[2]), 2);
    if (distance < rad)
    {
        vertices[i] += (radius-sqrt(distance)) * direction;
    }
  }
}



#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QRegularExpression>
#include <QtCore/qstring.h>

/*!
\brief Import a mesh from an .obj file.
\param filename File name.
*/
void Mesh::Load(const QString& filename)
{
  vertices.clear();
  normals.clear();
  varray.clear();
  narray.clear();

  QFile data(filename);

  if (!data.open(QFile::ReadOnly))
    return;
  QTextStream in(&data);

  // Set of regular expressions : Vertex, Normal, Triangle
  QRegularExpression rexv("v\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)");
  QRegularExpression rexn("vn\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)");
  QRegularExpression rext("f\\s*(\\d*)/\\d*/(\\d*)\\s*(\\d*)/\\d*/(\\d*)\\s*(\\d*)/\\d*/(\\d*)");
  while (!in.atEnd())
  {
    QString line = in.readLine();
    QRegularExpressionMatch match = rexv.match(line);
    QRegularExpressionMatch matchN = rexn.match(line);
    QRegularExpressionMatch matchT = rext.match(line);
    if (match.hasMatch())//rexv.indexIn(line, 0) > -1)
    {
      Vector q = Vector(match.captured(1).toDouble(), match.captured(2).toDouble(), match.captured(3).toDouble()); vertices.push_back(q);
    }
    else if (matchN.hasMatch())//rexn.indexIn(line, 0) > -1)
    {
      Vector q = Vector(matchN.captured(1).toDouble(), matchN.captured(2).toDouble(), matchN.captured(3).toDouble());  normals.push_back(q);
    }
    else if (matchT.hasMatch())//rext.indexIn(line, 0) > -1)
    {
      varray.push_back(matchT.captured(1).toInt() - 1);
      varray.push_back(matchT.captured(3).toInt() - 1);
      varray.push_back(matchT.captured(5).toInt() - 1);
      narray.push_back(matchT.captured(2).toInt() - 1);
      narray.push_back(matchT.captured(4).toInt() - 1);
      narray.push_back(matchT.captured(6).toInt() - 1);
    }
  }
  data.close();
}

/*!
\brief Save the mesh in .obj format, with vertices and normals.
\param url Filename.
\param meshName %Mesh name in .obj file.
*/
void Mesh::SaveObj(const QString& url, const QString& meshName) const
{
  QFile data(url);
  if (!data.open(QFile::WriteOnly))
    return;
  QTextStream out(&data);
  out << "g " << meshName << Qt::endl;
  for (int i = 0; i < vertices.size(); i++)
    out << "v " << vertices.at(i)[0] << " " << vertices.at(i)[1] << " " << vertices.at(i)[2] << QString('\n');
  for (int i = 0; i < normals.size(); i++)
    out << "vn " << normals.at(i)[0] << " " << normals.at(i)[1] << " " << normals.at(i)[2] << QString('\n');
  for (int i = 0; i < varray.size(); i += 3)
  {
    out << "f " << varray.at(i) + 1 << "//" << narray.at(i) + 1 << " "
      << varray.at(i + 1) + 1 << "//" << narray.at(i + 1) + 1 << " "
      << varray.at(i + 2) + 1 << "//" << narray.at(i + 2) + 1 << " "
      << "\n";
  }
  out.flush();
  data.close();
}

