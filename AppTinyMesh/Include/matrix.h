#define _USE_MATH_DEFINES
#include <iostream>
#include <array>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "mathematics.h"

// Implement a 3x3 matrix
class Matrix
{
  protected:
    std::array<std::array<double, 3>, 3> matrix;
  public:
    Matrix();
    Matrix(std::array<std::array<double, 3>, 3>);
    ~Matrix() {}

    // Access specific element
    std::array<double, 3> operator[](int) const;
    std::array<double, 3>& operator[](int);

    // Unary operators
    Matrix operator+() const;
    Matrix operator-() const;

    // Matrix|Matrix operations
    Matrix& operator=(const Matrix&);
    Matrix operator+(const Matrix&) const;
    Matrix& operator+=(const Matrix&);
    Matrix operator-(const Matrix&) const;
    Matrix& operator-=(const Matrix&);
    Matrix operator*(const Matrix&) const;
    Matrix& operator*=(const Matrix&);
    Matrix operator/(const Matrix&) const;
    Matrix& operator/=(const Matrix&);

    // Matrix|Scalar operations
    Matrix operator+(double) const;
    Matrix& operator+=(double);
    friend Matrix operator+(double, const Matrix&);
    Matrix operator-(double) const;
    Matrix& operator-=(double);
    friend Matrix operator-(double, const Matrix&);
    Matrix operator*(double) const;
    Matrix& operator*=(double);
    friend Matrix operator*(double, const Matrix&);
    Matrix operator/(double) const;
    Matrix& operator/=(double);
    friend Matrix operator/(double, const Matrix&);

    // Matrix|Vector operation
    Vector operator*(const Vector&) const;
    
    // Boolean operations
    bool operator==(const Matrix&) const;
    bool operator!=(const Matrix&) const;
    bool operator>(const Matrix&) const;
    bool operator<(const Matrix&) const;
    bool operator>=(const Matrix&) const;
    bool operator<=(const Matrix&) const;

    // Debug
    friend std::ostream& operator<<(std::ostream&, const Matrix&);
    std::string toString() const;

    // Matrix specific operations
    double determinant() const;
    Matrix transpose() const;
    Matrix inverse() const;

    // Specific matrix
    static Matrix rotationX(double);
    static Matrix rotationY(double);
    static Matrix rotationZ(double);

    static Matrix scale(double);
};
