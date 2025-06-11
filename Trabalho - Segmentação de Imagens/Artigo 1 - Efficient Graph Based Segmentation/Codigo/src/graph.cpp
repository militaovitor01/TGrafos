#include "graph.h"
#include <cmath>

Graph::Graph(const Image& img) {
    width = img.getWidth();
    height = img.getHeight();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx1 = pixelToIndex(x, y);
            Pixel p1 = img.getPixel(x, y);

            // vizinho da direita
            if (x < width - 1) {
                Pixel p2 = img.getPixel(x + 1, y);
                int idx2 = pixelToIndex(x + 1, y);
                edges.push_back({ idx1, idx2, diff(p1, p2) });
            }

            // vizinho abaixo
            if (y < height - 1) {
                Pixel p2 = img.getPixel(x, y + 1);
                int idx2 = pixelToIndex(x, y + 1);
                edges.push_back({ idx1, idx2, diff(p1, p2) });
            }

            // diagonais (opcional)
            if (x < width - 1 && y < height - 1) {
                Pixel p2 = img.getPixel(x + 1, y + 1);
                int idx2 = pixelToIndex(x + 1, y + 1);
                edges.push_back({ idx1, idx2, diff(p1, p2) });
            }

            if (x < width - 1 && y > 0) {
                Pixel p2 = img.getPixel(x + 1, y - 1);
                int idx2 = pixelToIndex(x + 1, y - 1);
                edges.push_back({ idx1, idx2, diff(p1, p2) });
            }
        }
    }
}

float Graph::diff(const Pixel& p1, const Pixel& p2) const {
    return std::sqrt(
        (p1.r - p2.r) * (p1.r - p2.r) +
        (p1.g - p2.g) * (p1.g - p2.g) +
        (p1.b - p2.b) * (p1.b - p2.b)
    );
}

const std::vector<Edge>& Graph::getEdges() const {
    return edges;
}

int Graph::getWidth() const { return width; }
int Graph::getHeight() const { return height; }

int Graph::pixelToIndex(int x, int y) const {
    return y * width + x;
}
