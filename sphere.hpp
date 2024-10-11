#pragma once
#include "shape.hpp"
#include <cmath>

struct Sphere : Shape {
    Vec3 center;
    double radius;
    rang::fg color;
    Vec3 colorVec3;

    Sphere(const Vec3& center, double radius, rang::fg color, const Vec3& colorVec3) 
        : center(center), radius(radius), color(color), colorVec3(colorVec3) {}

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

    rang::fg getColor() const override {
        return color;
    }

    Vec3 getColorVec3() const override {
        return colorVec3;
    }
};
