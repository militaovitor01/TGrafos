#include <vector>
#include <algorithm>
#include "segmentalgorithm.h"
#include "Graph.h"
#include "disjoint_set.h"
#include "image.h"
#include <map>
#include <random>
#include <cmath>

bool compareEdges(const Edge& e1, const Edge& e2) {
    return e1.weight < e2.weight;
}

float thresholdFunction(int size, float k) {
    return k / size;
}

std::vector<int> segmentGraph(
    int numVertices,
    std::vector<Edge>& edges,
    float k,
    int minSize
) {
    std::sort(edges.begin(), edges.end(), compareEdges);

    UnionFind uf(numVertices);
    std::vector<float> threshold(numVertices, thresholdFunction(1, k));

    for (const Edge& e : edges) {
        int a = uf.find(e.a);
        int b = uf.find(e.b);
        if (a != b) {
            if (e.weight <= threshold[a] && e.weight <= threshold[b]) {
                uf.unite(a, b);
                int newParent = uf.find(a);
                threshold[newParent] = e.weight + thresholdFunction(uf.getSize(newParent), k);
            }
        }
    }

    // Pós-processamento para juntar componentes menores que minSize
    for (const Edge& e : edges) {
        int a = uf.find(e.a);
        int b = uf.find(e.b);
        if (a != b && (uf.getSize(a) < minSize || uf.getSize(b) < minSize)) {
            uf.unite(a, b);
        }
    }

    // Resultado: vetor onde índice i indica a qual componente o pixel i pertence
    std::vector<int> components(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        components[i] = uf.find(i);
    }

    return components;
}

Image segmentImage(const Image& image, float k, int minSize) {
    int width = image.getWidth();
    int height = image.getHeight();
    int numVertices = width * height;
    std::vector<Edge> edges;

    // Cria arestas entre pixels vizinhos (4-conexão)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = y * width + x;
            Pixel p = image.getPixel(x, y);

            // Direita
            if (x + 1 < width) {
                Pixel q = image.getPixel(x + 1, y);
                float w = std::sqrt(
                    (p.r - q.r) * (p.r - q.r) +
                    (p.g - q.g) * (p.g - q.g) +
                    (p.b - q.b) * (p.b - q.b)
                );
                edges.push_back({idx, y * width + (x + 1), w});
            }
            // Baixo
            if (y + 1 < height) {
                Pixel q = image.getPixel(x, y + 1);
                float w = std::sqrt(
                    (p.r - q.r) * (p.r - q.r) +
                    (p.g - q.g) * (p.g - q.g) +
                    (p.b - q.b) * (p.b - q.b)
                );
                edges.push_back({idx, (y + 1) * width + x, w});
            }
        }
    }

    // Segmenta
    std::vector<int> components = segmentGraph(numVertices, edges, k, minSize);

    // Gera cores aleatórias para cada componente
    std::map<int, Pixel> colors;
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, 255);

    // Inicializa imagem de saída
    Image result = image;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = y * width + x;
            int comp = components[idx];
            if (colors.find(comp) == colors.end()) {
                colors[comp] = {dist(rng), dist(rng), dist(rng)};
            }
            result.setPixel(x, y, colors[comp]);
        }
    }
    return result;
}
