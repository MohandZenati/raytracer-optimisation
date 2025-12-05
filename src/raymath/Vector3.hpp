#pragma once

#include <iostream>
#include <cmath>

#define COMPARE_ERROR_CONSTANT 0.000001

class Vector3
{
private:
public:
  double x = 0;
  double y = 0;
  double z = 0;

  inline Vector3() : x(0), y(0), z(0) {}
  inline Vector3(double iX, double iY, double iZ) : x(iX), y(iY), z(iZ) {}
  inline ~Vector3() {}

  inline const Vector3 operator+(Vector3 const &vec) const
  {
    return Vector3(x + vec.x, y + vec.y, z + vec.z);
  }

  inline const Vector3 operator-(Vector3 const &vec) const
  {
    return Vector3(x - vec.x, y - vec.y, z - vec.z);
  }

  inline const Vector3 operator*(double const &f) const
  {
    return Vector3(x * f, y * f, z * f);
  }

  inline const Vector3 operator/(double const &f) const
  {
    return Vector3(x / f, y / f, z / f);
  }

  inline Vector3 &operator=(Vector3 const &vec)
  {
    x = vec.x;
    y = vec.y;
    z = vec.z;
    return *this;
  }

  inline double lengthSquared() const
  {
    return (x * x + y * y + z * z);
  }

  inline double length() const
  {
    return std::sqrt(this->lengthSquared());
  }

  inline const Vector3 normalize() const
  {
    double len = this->length();
    if (len == 0)
      return Vector3();
    return *this / len;
  }

  inline double dot(Vector3 const &vec) const
  {
    return (x * vec.x + y * vec.y + z * vec.z);
  }

  inline const Vector3 projectOn(Vector3 const &vec) const
  {
    return vec * this->dot(vec);
  }

  const Vector3 reflect(Vector3 const &normal) const;
  const Vector3 cross(Vector3 const &vec) const;
  const Vector3 inverse() const;

  friend std::ostream &operator<<(std::ostream &_stream, Vector3 const &vec);
};
