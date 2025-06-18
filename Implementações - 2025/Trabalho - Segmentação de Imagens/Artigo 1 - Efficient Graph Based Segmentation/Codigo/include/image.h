#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>

struct RGB {
    unsigned char r, g, b;
};

struct Image {
    int width;
    int height;
    std::vector<RGB> pixels;
};

// Função para gerar cores RGB aleatórias
RGB random_rgb();

// Função para ler arquivo PPM
Image read_ppm(const std::string& filename);

// Função para salvar arquivo PPM
void write_ppm(const std::string& filename, const Image& img);

#endif // IMAGE_H 