#pragma once
#include "shape.hpp"
#include <vector>
#include <memory>
#include <optional>
#include <cmath>

class Scene {
    std::vector<std::unique_ptr<Shape>> objects;
    Vec3 lightDir = Vec3(1, 1, 1).normalize();

public:
    void addObject(std::unique_ptr<Shape> object) {
        objects.push_back(std::move(object));
    }

    std::optional<std::pair<const Shape*, double>> trace(const Ray& ray) const {
        const Shape* hitObject = nullptr;
        double closest = std::numeric_limits<double>::infinity();

        for (const auto& object : objects) {
            double t;
            if (object->intersect(ray, t) && t > 0 && t < closest) {
                hitObject = object.get();
                closest = t;
            }
        }

        if (hitObject)
            return std::make_pair(hitObject, closest);
        return std::nullopt;
    }

    rang::fg shade(const Ray& ray) const {
        auto hit = trace(ray);
        if (!hit) return rang::fg::reset; // Background color

        const auto& [object, t] = *hit;
        Vec3 point = ray.origin + ray.direction * t;
        Vec3 normal = object->getNormal(point);
        double diffuse = std::max(normal.dot(lightDir), 0.0);
        
        // Improved distance-based shading
        double intensity = 1.0 / (1.0 + t * 0.1);  // Adjust the 0.1 to change fall-off rate
        intensity = std::pow(intensity, 0.5);  // Soften the intensity falloff
        
        // Combine object color with lighting
        rang::fg color = object->getColor();
        return (diffuse * intensity > 0.2) ? color : rang::fg::reset;
    }
};
