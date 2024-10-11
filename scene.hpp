#pragma once
#include "shape.hpp"
#include "vec3.hpp"
#include <vector>
#include <memory>
#include <optional>
#include <cmath>
#include <rang.hpp>

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
        
        // Ambient
        Vec3 ambient = lightColor * ambientStrength;

        // Diffuse
        double diff = std::max(normal.dot(lightDir), 0.0);
        Vec3 diffuse = lightColor * diff;

        // Specular
        Vec3 viewDir = (ray.origin - point).normalize();
        Vec3 reflectDir = (lightDir - normal * 2.0 * lightDir.dot(normal)).normalize();
        double spec = std::pow(std::max(viewDir.dot(reflectDir), 0.0), specularExponent);
        Vec3 specular = lightColor * specularStrength * spec;

        // Combine lighting
        Vec3 lighting = ambient + diffuse + specular;
        
        // Apply lighting to object color
        Vec3 objColor = object->getColorVec3();
        Vec3 result = objColor * lighting;

        // Convert to rang::fg
        int r = std::min(static_cast<int>(result.x * 255), 255);
        int g = std::min(static_cast<int>(result.y * 255), 255);
        int b = std::min(static_cast<int>(result.z * 255), 255);

        if (r > 128 && g > 128 && b > 128) return rang::fg::gray;
        if (r > 128 && g > 128) return rang::fg::yellow;
        if (r > 128 && b > 128) return rang::fg::magenta;
        if (g > 128 && b > 128) return rang::fg::cyan;
        if (r > 128) return rang::fg::red;
        if (g > 128) return rang::fg::green;
        if (b > 128) return rang::fg::blue;
        return rang::fg::reset;
    }
};
