#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <memory>
#include <omp.h>
#include "vec3.hpp"
#include "ray.hpp"
#include "shape.hpp"
#include "sphere.hpp"
#include "scene.hpp"
#include <rang.hpp>

int main() {
    const int width = 120;
    const int height = 40;
    const double aspectRatio = static_cast<double>(width) / height;
    const double fov = 60.0 * M_PI / 180.0;
    const Vec3 cameraPos(0, 2, -5);

    Scene scene;
    scene.addObject(std::make_unique<Sphere>(Vec3(-2, 1, 0), 1.0, rang::fg::red));
    scene.addObject(std::make_unique<Sphere>(Vec3(0, 1, 0), 1.0, rang::fg::yellow));
    scene.addObject(std::make_unique<Sphere>(Vec3(2, 1, 0), 1.0, rang::fg::cyan));

    std::vector<std::vector<PixelData>> frameBuffer(height, std::vector<PixelData>(width));

    #pragma omp parallel for collapse(2)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double xx = (2 * ((x + 0.5) / width) - 1) * tan(fov / 2) * aspectRatio;
            double yy = (1 - 2 * ((y + 0.5) / height)) * tan(fov / 2);
            Vec3 direction = Vec3(xx, yy, 1).normalize();
            Ray ray(cameraPos, direction);
            frameBuffer[y][x] = scene.shade(ray);
        }
    }

    // Render the frame buffer
    for (const auto& row : frameBuffer) {
        for (const auto& pixel : row) {
            std::cout << pixel.color << pixel.character << rang::fg::reset;
        }
        std::cout << '\n';
    }

    return 0;
}
