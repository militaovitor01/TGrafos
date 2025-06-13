#include "graph.h"
#include <cmath>

/**
 * @brief Calcula a diferença entre dois pixels
 * 
 * A diferença é calculada como a distância euclidiana
 * entre os componentes RGB dos pixels.
 * 
 * @param p1 Primeiro pixel
 * @param p2 Segundo pixel
 * @return Diferença entre os pixels
 */
float Graph::diff(const Pixel& p1, const Pixel& p2) const {
    // Calcula a diferença para cada componente de cor
    float dr = p1.r - p2.r;
    float dg = p1.g - p2.g;
    float db = p1.b - p2.b;
    
    // Retorna a distância euclidiana
    return std::sqrt(dr*dr + dg*dg + db*db);
}

/**
 * @brief Construtor do grafo
 * 
 * Constrói o grafo a partir de uma imagem, criando arestas
 * entre pixels adjacentes e calculando seus pesos.
 * 
 * @param img Imagem de entrada
 */
Graph::Graph(const Image& img) {
    width = img.getWidth();
    height = img.getHeight();
    
    // Para cada pixel da imagem
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Obtém o pixel atual
            Pixel current = img.getPixel(x, y);
            
            // Verifica o pixel à direita
            if (x + 1 < width) {
                Pixel right = img.getPixel(x + 1, y);
                edges.push_back({
                    pixelToIndex(x, y),
                    pixelToIndex(x + 1, y),
                    diff(current, right)
                });
            }
            
            // Verifica o pixel abaixo
            if (y + 1 < height) {
                Pixel down = img.getPixel(x, y + 1);
                edges.push_back({
                    pixelToIndex(x, y),
                    pixelToIndex(x, y + 1),
                    diff(current, down)
                });
            }
        }
    }
}

/**
 * @brief Retorna a lista de arestas do grafo
 * @return Referência constante para a lista de arestas
 */
const std::vector<Edge>& Graph::getEdges() const {
    return edges;
}

/**
 * @brief Retorna a largura da imagem
 * @return Largura em pixels
 */
int Graph::getWidth() const {
    return width;
}

/**
 * @brief Retorna a altura da imagem
 * @return Altura em pixels
 */
int Graph::getHeight() const {
    return height;
}

/**
 * @brief Converte coordenadas (x,y) em índice linear
 * 
 * Converte as coordenadas 2D de um pixel em um índice
 * linear único, usado para identificar vértices no grafo.
 * 
 * @param x Coordenada x do pixel
 * @param y Coordenada y do pixel
 * @return Índice linear do pixel
 */
int Graph::pixelToIndex(int x, int y) const {
    return y * width + x;
}
