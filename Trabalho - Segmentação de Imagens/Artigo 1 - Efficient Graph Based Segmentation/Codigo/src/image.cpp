#include "image.h"
#include <fstream>
#include <sstream>
#include <iostream>

Image::Image() : width(0), height(0) {}

bool Image::loadPPM(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return false;

    std::string format;
    file >> format;
    if (format != "P6") return false;

    file >> width >> height;
    int maxVal;
    file >> maxVal;
    file.ignore(); // ignora o '\n' após o valor máximo

    data.resize(height, std::vector<Pixel>(width));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            char rgb[3];
            file.read(rgb, 3);
            data[i][j] = { static_cast<unsigned char>(rgb[0]),
                           static_cast<unsigned char>(rgb[1]),
                           static_cast<unsigned char>(rgb[2]) };
        }
    }

    return true;
}

bool Image::savePPM(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) return false;

    file << "P6\n" << width << " " << height << "\n255\n";

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            const Pixel& p = data[i][j];
            file.put(p.r).put(p.g).put(p.b);
        }
    }

    return true;
}

int Image::getWidth() const { return width; }
int Image::getHeight() const { return height; }

Pixel Image::getPixel(int x, int y) const {
    return data[y][x]; // linha, coluna
}

void Image::setPixel(int x, int y, const Pixel& pixel) {
    data[y][x] = pixel;
}

const std::vector<std::vector<Pixel>>& Image::getData() const {
    return data;
}
