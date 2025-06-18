#include "../include/segmenter.h"
#include "../include/image.h"
#include "../include/filter.h"
#include "../include/segment_graph.h"
#include <iostream>
#include <chrono>
#include <vector>

void segment(const Image& in_image, float sigma, float k, int min_size, const std::string& output_ppm) {
    auto start_time = std::chrono::steady_clock::now();

    int height = in_image.height;
    int width = in_image.width;
    std::cout << "Height: " << height << std::endl;
    std::cout << "Width: " << width << std::endl;

    // Separar canais e suavizar
    std::vector<float> red_channel(width * height);
    std::vector<float> green_channel(width * height);
    std::vector<float> blue_channel(width * height);

    for (int i = 0; i < width * height; i++) {
        red_channel[i] = in_image.pixels[i].r;
        green_channel[i] = in_image.pixels[i].g;
        blue_channel[i] = in_image.pixels[i].b;
    }

    auto smooth_red = Filter::smooth(red_channel, width, height, sigma);
    auto smooth_green = Filter::smooth(green_channel, width, height, sigma);
    auto smooth_blue = Filter::smooth(blue_channel, width, height, sigma);

    // Construir grafo
    std::vector<Edge> edges;
    edges.reserve(width * height * 4);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x < width - 1) {
                edges.push_back(Edge(
                    y * width + x,
                    y * width + (x + 1),
                    Filter::diff(smooth_red, smooth_green, smooth_blue, width, x, y, x + 1, y)
                ));
            }
            if (y < height - 1) {
                edges.push_back(Edge(
                    y * width + x,
                    (y + 1) * width + x,
                    Filter::diff(smooth_red, smooth_green, smooth_blue, width, x, y, x + 1, y)
                ));
            }
            if (x < width - 1 && y < height - 1) {
                edges.push_back(Edge(
                    y * width + x,
                    (y + 1) * width + (x + 1),
                    Filter::diff(smooth_red, smooth_green, smooth_blue, width, x, y, x + 1, y)
                ));
            }
            if (x < width - 1 && y > 0) {
                edges.push_back(Edge(
                    y * width + x,
                    (y - 1) * width + (x + 1),
                    Filter::diff(smooth_red, smooth_green, smooth_blue, width, x, y, x + 1, y)
                ));
            }
        }
    }

    // Segmentar
    Universe u(width * height);
    segment_graph(width * height, edges, k, u);

    // Pós-processamento de componentes pequenos
    for (const auto& edge : edges) {
        int a = u.find(edge.a);
        int b = u.find(edge.b);
        if (a != b && (u.size(a) < min_size || u.size(b) < min_size)) {
            u.join(a, b);
        }
    }

    // Criar imagem de saída
    Image output;
    output.width = width;
    output.height = height;
    output.pixels.resize(width * height);

    std::vector<RGB> colors(width * height);
    for (int i = 0; i < width * height; i++) {
        colors[i] = random_rgb();
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int comp = u.find(y * width + x);
            output.pixels[y * width + x] = colors[comp];
        }
    }

    auto end_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
    std::cout << "Execution time: " << elapsed.count() / 60 << " minute(s) and "
              << elapsed.count() % 60 << " seconds" << std::endl;

    // Salvar resultado em PPM
    write_ppm(output_ppm, output);
} 