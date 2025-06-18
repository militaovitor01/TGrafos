#include "../include/image.h"
#include <iostream>
#include <fstream>
#include <random>
#include <stdexcept>

// Função para gerar cores RGB aleatórias
RGB random_rgb() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0, 1);
    return RGB{
        static_cast<unsigned char>(dis(gen) * 255),
        static_cast<unsigned char>(dis(gen) * 255),
        static_cast<unsigned char>(dis(gen) * 255)
    };
}

// Função para ler arquivo PPM
Image read_ppm(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string format;
    file >> format;
    if (format != "P6") {
        throw std::runtime_error("Invalid PPM format. Only P6 is supported.");
    }

    Image img;
    file >> img.width >> img.height;

    int maxval;
    file >> maxval;
    if (maxval != 255) {
        throw std::runtime_error("Only 8-bit per channel PPM files are supported.");
    }

    // Consume newline
    file.get();

    img.pixels.resize(img.width * img.height);
    file.read(reinterpret_cast<char*>(img.pixels.data()), img.pixels.size() * sizeof(RGB));

    return img;
}

// Função para salvar arquivo PPM
void write_ppm(const std::string& filename, const Image& img) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot create file: " + filename);
    }

    file << "P6\n" << img.width << " " << img.height << "\n255\n";
    file.write(reinterpret_cast<const char*>(img.pixels.data()), img.pixels.size() * sizeof(RGB));
} 