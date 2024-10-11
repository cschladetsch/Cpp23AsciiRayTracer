#pragma once
#include "shape.hpp"
#include "vec3.hpp"
#include <vector>
#include <memory>
#include <optional>
#include <cmath>
#include <rang.hpp>

struct PixelData {
    char character;
    rang::fg color;
};

class Scene {
    std::vector<std::unique_ptr<Shape>> objects;
    Vec3 lightDir = Vec3(1, 1, 1).normalize();
    Vec3 lightColor = Vec3(1, 1, 1);  // White light
    double ambientStrength = 0.1;
    double specularStrength = 0.5;
    int specularExponent = 32;

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

        // Check for ground plane intersection
        double t_ground = -ray.origin.y / ray.direction.y;
        if (t_ground > 0 && t_ground < closest) {
            return std::make_pair(nullptr, t_ground);  // nullptr indicates ground plane
        }

        if (hitObject)
            return std::make_pair(hitObject, closest);

        // If no intersection, it's the sky
        return std::make_pair(nullptr, std::numeric_limits<double>::infinity());
    }

    PixelData shade(const Ray& ray) const {
        auto hit = trace(ray);
        const auto& [object, t] = *hit;

        if (object == nullptr) {
            if (std::isinf(t)) {
                // Sky
                return {'.', rang::fg::blue};
            } else {
                // Ground plane
                Vec3 hitPoint = ray.origin + ray.direction * t;
                int checkX = static_cast<int>(std::floor(hitPoint.x)) % 2;
                int checkZ = static_cast<int>(std::floor(hitPoint.z)) % 2;
                return {(checkX ^ checkZ) ? '#' : '_', rang::fg::green};
            }
        }

        Vec3 point = ray.origin + ray.direction * t;
        Vec3 normal = object->getNormal(point);
        
        // Diffuse lighting (simplified)
        double diff = std::max(normal.dot(lightDir), 0.0);

        // Choose ASCII character based on lighting intensity
        const char shades[] = " .:-=+*#%@";
        int shadeIndex = static_cast<int>(diff * 9);
        char shadeChar = shades[shadeIndex];

        // Get object color
        rang::fg objectColor = object->getColor();

        return {shadeChar, objectColor};
    }
};
