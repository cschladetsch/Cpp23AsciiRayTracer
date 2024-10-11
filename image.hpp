#pragma once
#include <rang.hpp>
#include <vector>
#include <iostream>
#include <format>

class Image {
    int width, height;
    std::vector<rang::fg> pixels;

public:
    Image(int width, int height) : width(width), height(height), pixels(width * height) {}

    void setPixel(int x, int y, rang::fg color) {
        pixels[y * width + x] = color;
    }

    void display() const {
        const char* shades = " .:;~=#OB@";
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                rang::fg color = pixels[y * width + x];
                int index = (color != rang::fg::reset) ? 8 : 0;
                std::cout << color << shades[index];
            }
            std::cout << rang::fg::reset << '\n';
        }
    }
};
