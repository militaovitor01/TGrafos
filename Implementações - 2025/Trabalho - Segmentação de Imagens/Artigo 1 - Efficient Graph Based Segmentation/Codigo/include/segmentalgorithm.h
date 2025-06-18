#ifndef SEGMENT_ALGORITHM_H
#define SEGMENT_ALGORITHM_H

#include <vector>
#include "image.h"

struct Edge;

/**
 * @brief Realiza a segmentação de um grafo com base nos pesos das arestas.
 * 
 * @param numVertices Número total de vértices (pixels) no grafo.
 * @param edges Vetor de arestas do grafo com pesos.
 * @param k Parâmetro de controle de sensibilidade da segmentação.
 * @param minSize Tamanho mínimo de um componente. Componentes menores serão fundidos.
 * @return Vetor onde o índice representa o vértice e o valor representa o componente ao qual ele pertence.
 */
std::vector<int> segmentGraph(
    int numVertices,
    std::vector<Edge>& edges,
    float k,
    int minSize
);

Image segmentImage(const Image& image, float k, int minSize);

#endif // SEGMENT_ALGORITHM_H
