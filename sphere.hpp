#pragma once
#include "shape.hpp"
#include "vec3.hpp"
#include <rang.hpp>

struct Sphere : Shape {
    Vec3 center;
    double radius;
    rang::fg color;  // Use rang::fg for colors

    Sphere(const Vec3& center, double radius, rang::fg color)
        : center(center), radius(radius), color(color) {}

    bool intersect(const Ray& ray, double& t) const override {
        Vec3 oc = ray.origin - center;
        double b = oc.dot(ray.direction);
        double c = oc.dot(oc) - radius * radius;
        double discriminant = b * b - c;
        if (discriminant < 0) return false;
        t = -b - std::sqrt(discriminant);
        return t > 0;
    }

    Vec3 getNormal(const Vec3& point) const override {
        return (point - center).normalize();
    }

    // Return rang::fg for color
    rang::fg getColor() const override {
        return color;
    }
};

