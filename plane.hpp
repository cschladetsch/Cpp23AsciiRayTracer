#pragma once
#include "shape.hpp"
#include "vec3.hpp"
#include <string>

struct Plane : Shape {
    Vec3 point;
    Vec3 normal;
    std::string color;  // ANSI color code

    Plane(const Vec3& point, const Vec3& normal, const std::string& color)
        : point(point), normal(normal), color(color) {}

    bool intersect(const Ray& ray, double& t) const override {
        // Plane-ray intersection logic
        double denom = normal.dot(ray.direction);
        if (std::abs(denom) > 1e-6) {
            t = (point - ray.origin).dot(normal) / denom;
            return (t >= 0);
        }
        return false;
    }

    Vec3 getNormal(const Vec3& /*unused*/) const override {
        return normal;
    }

    // Updated getColor() method to return ANSI color string
    std::string getColor() const override {
        return color;
    }
};

