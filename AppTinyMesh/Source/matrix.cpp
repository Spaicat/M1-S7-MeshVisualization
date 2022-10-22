#include "matrix.h"

/*!
\brief Create a 3x3 matrix with only 0.
*/
Matrix::Matrix()
{
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      this->matrix[i][j] = 0;
    }
  }
}

/*!
\brief Constructor of a 3x3 matrix from a 2d array.
\param m the 3x3 matrix to build the matrix
*/
Matrix::Matrix(std::array<std::array<double, 3>, 3> m) :matrix(m)
{
}

// --- Access specific element ---

std::array<double, 3> Matrix::operator[](int n) const
{
  return this->matrix[n];
}

std::array<double, 3>& Matrix::operator[](int n)
{
  return this->matrix[n];
}

// --- Unary operators ---

Matrix Matrix::operator+() const
{
  return *this;
}

Matrix Matrix::operator-() const
{
  return Matrix({{
    { -this->matrix[0][0], -this->matrix[0][1], -this->matrix[0][2] },
    { -this->matrix[1][0], -this->matrix[1][1], -this->matrix[1][2] },
    { -this->matrix[2][0], -this->matrix[2][1], -this->matrix[2][3] }
  }});
}

// --- Matrix|Matrix operations ---

Matrix& Matrix::operator=(const Matrix& other)
{
  if (*this == other)
    return *this;

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      this->matrix[i][j] = other[i][j];
    }
  }

  return *this;
}

Matrix Matrix::operator+(const Matrix& other) const
{
  Matrix newMatrix = Matrix();

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      newMatrix[i][j] = this->matrix[i][j] + other[i][j];
    }
  }
  
  return newMatrix;
}

Matrix& Matrix::operator+=(const Matrix& other)
{
  Matrix newMatrix = *this + other;
  *this = newMatrix;
  return *this;
}

Matrix Matrix::operator-(const Matrix& other) const
{
  Matrix newMatrix = Matrix();

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      newMatrix[i][j] = this->matrix[i][j] - other[i][j];
    }
  }
  
  return newMatrix;
}

Matrix& Matrix::operator-=(const Matrix& other)
{
  Matrix newMatrix = *this - other;
  *this = newMatrix;
  return *this;
}

Matrix Matrix::operator*(const Matrix& other) const
{
  Matrix newMatrix = Matrix();

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      for (int k = 0; k < 3; k++)
      {
        newMatrix[i][j] += this->matrix[i][k] * other[k][j];
      }
    }
  }

  return newMatrix;
}

Matrix& Matrix::operator*=(const Matrix& other)
{
  Matrix newMatrix = *this * other;
  *this = newMatrix;
  return *this;
}

Matrix Matrix::operator/(const Matrix& other) const
{
  return *this * other.inverse();
}

Matrix& Matrix::operator/=(const Matrix& other)
{
  Matrix newMatrix = *this / other;
  *this = newMatrix;
  return *this;
}

// --- Matrix|Scalar operations ---

Matrix Matrix::operator+(double x) const
{
  Matrix newMatrix = Matrix();

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      newMatrix[i][j] = this->matrix[i][j] + x;
    }
  }

  return newMatrix;
}

Matrix& Matrix::operator+=(double x)
{
  Matrix newMatrix = *this + x;
  *this = newMatrix;
  return *this;
}

Matrix operator+(double x, const Matrix& matrix)
{
  Matrix newMatrix = Matrix();

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      newMatrix[i][j] = x + matrix[i][j];
    }
  }

  return newMatrix;
}

Matrix Matrix::operator-(double x) const
{
  Matrix newMatrix = Matrix();

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      newMatrix[i][j] = this->matrix[i][j] - x;
    }
  }

  return newMatrix;
}

Matrix& Matrix::operator-=(double x)
{
  Matrix newMatrix = *this - x;
  *this = newMatrix;
  return *this;
}

Matrix operator-(double x, const Matrix& matrix)
{
  Matrix newMatrix = Matrix();

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      newMatrix[i][j] = x - matrix[i][j];
    }
  }

  return newMatrix;
}

Matrix Matrix::operator*(double x) const
{
  Matrix newMatrix = Matrix();

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      newMatrix[i][j] = this->matrix[i][j] * x;
    }
  }

  return newMatrix;
}

Matrix& Matrix::operator*=(double x)
{
  Matrix newMatrix = *this * x;
  *this = newMatrix;
  return *this;
}

Matrix operator*(double x, const Matrix& matrix)
{
  Matrix newMatrix = Matrix();

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      newMatrix[i][j] = x * matrix[i][j];
    }
  }

  return newMatrix;
}

Matrix Matrix::operator/(double x) const
{
  return *this * (1/x);
}

Matrix& Matrix::operator/=(double x)
{
  Matrix newMatrix = *this / x;
  *this = newMatrix;
  return *this;
}

Matrix operator/(double x, const Matrix& matrix)
{
  Matrix newMatrix = Matrix();

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      newMatrix[i][j] = x / matrix[i][j];
    }
  }

  return newMatrix;
}

// --- Matrix|Vector operation ---

Vector Matrix::operator*(const Vector& v) const
{
  Vector newVector = Vector(0, 0, 0);

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      newVector[i] += this->matrix[i][j] * v[j];
    }
  }

  return newVector;
}

// --- Boolean operations ---

bool Matrix::operator==(const Matrix& other) const
{
  bool res = true;

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      res = res && this->matrix[i][j] == other[i][j];
    }
  }
  
  return res;
}

bool Matrix::operator!=(const Matrix& other) const
{
  return !(*this == other);
}

bool Matrix::operator>(const Matrix& other) const
{
  bool res = true;

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      res = res && this->matrix[i][j] > other[i][j];
    }
  }
  
  return res;
}

bool Matrix::operator<(const Matrix& other) const
{
  return !(*this > other) && (*this != other);
}

bool Matrix::operator>=(const Matrix& other) const
{
  return !(*this < other);
}

bool Matrix::operator<=(const Matrix& other) const
{
  return !(*this > other);
}

// --- Debug ---

std::ostream& operator<<(std::ostream& s, const Matrix& matrix)
{
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      s << std::setw(3) << matrix[i][j] << " ";
    }
    s << std::endl;
  }

  return s;
}

std::string Matrix::toString() const
{
  std::ostringstream sstream;
  sstream << this->matrix;
  return sstream.str();
}

// --- Matrix specific operations ---

Matrix Matrix::transpose() const
{
  Matrix newMatrix = Matrix();

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      newMatrix[i][j] = this->matrix[j][i];
    }
  }

  return newMatrix;
}

double Matrix::determinant() const
{
  double res = 0;

  int sign = 1;
  for (int i = 0; i < 3; i++)
  {
    // Determinant of 2x2 inner matrix
    std::vector<double> subMatrix;
    for (int j = 1; j < 3; j++)
    {
      for (int k = 0; k < 3; k++)
      {
        if (k != i)
        {
          subMatrix.push_back(this->matrix[j][k]);
        }
      }
    }
    res += sign * this->matrix[0][i] * (subMatrix[0]*subMatrix[3] - subMatrix[1]*subMatrix[2]);
    sign *= -1;
  }
  
  return res;
}

Matrix Matrix::inverse() const
{
  double det = this->determinant();
  if (det == 0)
  {
    throw std::invalid_argument("Can't find the inverse of a Matrix with a zero determinant");
  }

  Matrix newMatrix = Matrix();

  int sign = 1;
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      // Determinant of 2x2 inner matrix
      std::vector<double> subMatrix;
      for (int k = 0; k < 3; k++)
      {
        for (int m = 0; m < 3; m++)
        {
          if (k != j && m != i)
          {
            subMatrix.push_back(this->matrix[m][k]);
          }
        }
      }
      newMatrix[i][j] += sign * (subMatrix[0]*subMatrix[3] - subMatrix[1]*subMatrix[2]);
      sign *= -1;
    }
  }

  newMatrix = (1/det) * newMatrix.transpose();

  return newMatrix;
}

// --- Specific matrix ---

Matrix Matrix::rotationX(double angle)
{
  double rad = angle * M_PI / 180;
  return Matrix({{
    {1, 0, 0},
    {0, std::cos(rad), -std::sin(rad)},
    {0, std::sin(rad), std::cos(rad)}
  }});
}

Matrix Matrix::rotationY(double angle)
{
  double rad = angle * M_PI / 180;
  return Matrix({{
    {std::cos(rad), 0, std::sin(rad)},
    {0, 1, 0},
    {-std::sin(rad), 0, std::cos(rad)}
  }});
}

Matrix Matrix::rotationZ(double angle)
{
  double rad = angle * M_PI / 180;
  return Matrix({{
    {std::cos(rad), -std::sin(rad), 0},
    {std::sin(rad), std::cos(rad), 0},
    {0, 0, 1},
  }});
}

Matrix Matrix::homothety(double x)
{
  return Matrix({{
    {x, 0, 0},
    {0, x, 0},
    {0, 0, x},
  }});
}
