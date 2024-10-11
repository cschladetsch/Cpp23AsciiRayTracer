#pragma once
#include "shape.hpp"
#include "vec3.hpp"
#include <string>

struct Cube : Shape {
    Vec3 center;
    double size;
    std::string color;  // ANSI color code

    Cube(const Vec3& center, double size, const std::string& color)
        : center(center), size(size), color(color) {}

    bool intersect(const Ray& ray, double& t) const override {
        // Implement cube-ray intersection logic here
        return false;
    }

    Vec3 getNormal(const Vec3& point) const override {
        // Return the normal at the given point for a cube
        return Vec3(0, 1, 0);  // Placeholder normal
    }

    // Updated getColor() method to return ANSI color string
    std::string getColor() const override {
        return color;
    }
};

