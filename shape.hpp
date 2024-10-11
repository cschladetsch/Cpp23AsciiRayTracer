#pragma once
#include "vec3.hpp"
#include <rang.hpp>

struct Shape {
    virtual bool intersect(const Ray& ray, double& t) const = 0;
    virtual Vec3 getNormal(const Vec3& point) const = 0;

    // Ensure consistent return type for color (rang::fg)
    virtual rang::fg getColor() const = 0;

    virtual ~Shape() = default;
};

