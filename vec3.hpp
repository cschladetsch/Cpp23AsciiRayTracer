

#pragma once
#include <cmath>

struct Vec3 {
    double x, y, z;
    Vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(double d) const { return Vec3(x * d, y * d, z * d); }
    Vec3 operator*(const Vec3& v) const { return Vec3(x * v.x, y * v.y, z * v.z); } // New element-wise multiplication
    Vec3& normalize() { return *this = *this * (1 / std::sqrt(x * x + y * y + z * z)); }
    double dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
};
