#include <iostream>
#include <format>
#include <ranges>
#include <algorithm>
#include <atomic>
#include <thread>
#include <chrono>
#include <numbers>
#include <vector>
#include <utility>
#include <cmath>
#include <omp.h>

#include "vec3.hpp"
#include "ray.hpp"
#include "shape.hpp"
#include "sphere.hpp"
#include "scene.hpp"
#include "image.hpp"

int main() {
    const int width = 80;
    const int height = 40;
    const double fov = 60.0 * std::numbers::pi / 180.0;
    const Vec3 cameraPos(0, 0, 0);

    Scene scene;
    scene.addObject(std::make_unique<Sphere>(Vec3(0, 0, -5), 1, rang::fg::red, Vec3(1, 0, 0)));
    scene.addObject(std::make_unique<Sphere>(Vec3(2, 0, -6), 0.5, rang::fg::green, Vec3(0, 1, 0)));
    scene.addObject(std::make_unique<Sphere>(Vec3(-2, 0, -4), 0.7, rang::fg::blue, Vec3(0, 0, 1)));
    scene.addObject(std::make_unique<Sphere>(Vec3(0, -1, -3), 0.3, rang::fg::yellow, Vec3(1, 1, 0)));
    scene.addObject(std::make_unique<Sphere>(Vec3(-1, 1, -4), 0.4, rang::fg::cyan, Vec3(0, 1, 1)));

    Image image(width, height);

    std::atomic<int> progress = 0;
    int totalPixels = width * height;

    auto renderPixel = [&](int x, int y) {
        double xx = (2 * ((x + 0.5) / width) - 1) * std::tan(fov / 2.0) * width / height;
        double yy = (1 - 2 * ((y + 0.5) / height)) * std::tan(fov / 2.0);
        Vec3 direction = Vec3(xx, yy, -1).normalize();
        Ray ray(cameraPos, direction);
        rang::fg color = scene.shade(ray);
        image.setPixel(x, y, color);
        ++progress;
    };

    std::vector<std::pair<int, int>> pixels(width * height);
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            pixels[y * width + x] = std::make_pair(x, y);
        }
    }

    // Render in parallel using OpenMP
    #pragma omp parallel for
    for (size_t i = 0; i < pixels.size(); ++i) {
        auto [x, y] = pixels[i];
        renderPixel(x, y);
    }

    // Display progress
    bool rendering = true;
    std::thread progressThread([&]() {
        while (rendering) {
            std::cout << rang::fg::yellow 
                      << std::format("\rRendering progress: {}%", progress * 100 / totalPixels) 
                      << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    // Wait for rendering to complete
    #pragma omp taskwait

    rendering = false;
    progressThread.join();

    std::cout << rang::fg::green << "\rRendering complete!   " << rang::fg::reset << std::endl;

    // Display the rendered image
    image.display();

    return 0;
}
