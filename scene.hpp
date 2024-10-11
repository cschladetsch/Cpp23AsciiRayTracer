#pragma once
#include "shape.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include <vector>
#include <memory>
#include <optional>
#include <cmath>
#include <rang.hpp>

struct PixelData {
    char character;
    rang::fg color;  // Using rang::fg for colors
};

struct Light {
    Vec3 position;
    Vec3 color;  // RGB representation of the light color
};

class Scene {
    std::vector<std::unique_ptr<Shape>> objects;
    std::vector<Light> lights;  // Multiple light sources
    double ambientStrength = 0.05;
    double specularStrength = 0.3;
    int specularExponent = 16;
    double groundLevel = 0.0;  // Y-level of the ground plane

public:
    // Add objects to the scene
    void addObject(std::unique_ptr<Shape> object) {
        objects.push_back(std::move(object));
    }

    // Add lights to the scene
    void addLight(const Vec3& position, const Vec3& color) {
        lights.push_back({position, color});
    }

    // Trace function to find the closest object hit by a ray
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

        // Ground plane intersection
        double t_ground = (groundLevel - ray.origin.y) / ray.direction.y;
        if (t_ground > 0 && t_ground < closest) {
            return std::make_pair(nullptr, t_ground);  // nullptr indicates the ground plane
        }

        if (hitObject) {
            return std::make_pair(hitObject, closest);
        } else {
            return std::nullopt;
        }
    }

    // Checkerboard ground plane with varied characters
    PixelData getGroundPixel(const Vec3& hitPoint) const {
        int checkX = static_cast<int>(std::floor(hitPoint.x)) % 2;
        int checkZ = static_cast<int>(std::floor(hitPoint.z)) % 2;
        bool isCheckerDark = (checkX ^ checkZ);  // XOR to create the checkerboard pattern

        // Use different characters for checkerboard pattern
        const char darkChecker[] = "#▓";
        const char lightChecker[] = "-░";
        char shadeChar = isCheckerDark ? darkChecker[checkX] : lightChecker[checkZ];

        rang::fg color = isCheckerDark ? rang::fg::green : rang::fg::gray;  // Use gray instead of white
        return {shadeChar, color};
    }

    // Calculate shadows
    bool inShadow(const Vec3& point, const Vec3& lightPos) const {
        Vec3 shadowDir = (lightPos - point).normalize();
        Ray shadowRay(point + shadowDir * 0.001, shadowDir);  // Avoid self-shadowing
        auto shadowHit = trace(shadowRay);
        return shadowHit.has_value();
    }

    // Shading function using skyline and checkerboard ground
    PixelData shade(const Ray& ray) const {
        auto hit = trace(ray);
        if (!hit.has_value()) {
            // Sky background with gradient
            double t = 0.5 * (ray.direction.y + 1.0);
            if (t > 0.7) {
                return {' ', rang::fg::cyan};  // Bright sky
            } else if (t > 0.4) {
                return {' ', rang::fg::blue};  // Mid-level sky
            } else {
                return {' ', rang::fg::black};  // Darker skyline
            }
        }

        const auto& [object, t] = *hit;
        Vec3 point = ray.origin + ray.direction * t;

        // Handle the ground plane
        if (object == nullptr) {
            return getGroundPixel(point);
        }

        // Handle object shading
        Vec3 normal = object->getNormal(point);
        double ambient = ambientStrength;
        double totalDiffuse = 0.0;
        double totalSpecular = 0.0;

        for (const auto& light : lights) {
            Vec3 lightDir = (light.position - point).normalize();
            if (inShadow(point, light.position)) {
                continue;
            }

            double diffuse = std::max(normal.dot(lightDir), 0.0);
            totalDiffuse += diffuse;

            Vec3 viewDir = (ray.origin - point).normalize();
            Vec3 reflectDir = (normal * (2.0 * normal.dot(lightDir)) - lightDir).normalize();
            double specular = std::pow(std::max(viewDir.dot(reflectDir), 0.0), specularExponent);
            totalSpecular += specular * specularStrength;
        }

        double intensity = ambient + totalDiffuse + totalSpecular;

        // Expanded shading set
        const char shades[] = " .:░▒▓█@";
        int shadeIndex = static_cast<int>(intensity * 7);
        shadeIndex = std::clamp(shadeIndex, 0, 7);
        char shadeChar = shades[shadeIndex];

        // Use rang::fg colors, with valid colors like gray instead of white
        rang::fg objectColor = (intensity < 0.6) ? rang::fg::yellow : rang::fg::gray;  // Replaced white with gray

        return {shadeChar, objectColor};
    }
};

