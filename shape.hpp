#pragma once
#include "ray.hpp"
#include <rang.hpp>

struct Shape {
    virtual bool intersect(const Ray& ray, double& t) const = 0;
    virtual Vec3 getNormal(const Vec3& point) const = 0;
    virtual rang::fg getColor() const = 0;
    virtual Vec3 getColorVec3() const = 0;
    virtual ~Shape() = default;
};
