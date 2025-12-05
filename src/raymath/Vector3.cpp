#include <iostream>
#include <cmath>
#include "Vector3.hpp"

const Vector3 Vector3::reflect(Vector3 const &normal) const
{
  Vector3 proj = this->projectOn(normal) * -2;
  Vector3 reflectDir = proj + *this;
  return reflectDir;
}

const Vector3 Vector3::cross(Vector3 const &b) const
{
  Vector3 c(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
  return c;
}

const Vector3 Vector3::inverse() const
{
  Vector3 c(1.0 / x, 1.0 / y, 1.0 / z);
  return c;
}

std::ostream &operator<<(std::ostream &_stream, Vector3 const &vec)
{
  return _stream << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
}
