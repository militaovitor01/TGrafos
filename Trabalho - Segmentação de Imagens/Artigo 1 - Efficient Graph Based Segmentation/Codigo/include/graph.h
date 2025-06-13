#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "image.h"

/**
 * @brief Estrutura que representa uma aresta do grafo
 * 
 * Cada aresta conecta dois pixels da imagem e possui um peso
 * que representa a diferença entre eles.
 */
struct Edge {
    int a, b;       // índices dos pixels (linearizados)
    float weight;   // peso da aresta (diferença entre os pixels)
};

/**
 * @brief Classe que representa o grafo da imagem
 * 
 * Esta classe constrói um grafo a partir de uma imagem, onde:
 * - Cada pixel é um vértice
 * - Arestas conectam pixels adjacentes
 * - O peso de cada aresta é a diferença entre os pixels
 * 
 * O grafo é usado pelo algoritmo de segmentação para
 * determinar quais regiões da imagem devem ser unidas.
 */
class Graph {
private:
    int width, height;              // Dimensões da imagem
    std::vector<Edge> edges;        // Lista de arestas do grafo

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
    float diff(const Pixel& p1, const Pixel& p2) const;

public:
    /**
     * @brief Construtor do grafo
     * 
     * Constrói o grafo a partir de uma imagem, criando arestas
     * entre pixels adjacentes e calculando seus pesos.
     * 
     * @param img Imagem de entrada
     */
    Graph(const Image& img);

    /**
     * @brief Retorna a lista de arestas do grafo
     * @return Referência constante para a lista de arestas
     */
    const std::vector<Edge>& getEdges() const;

    /**
     * @brief Retorna a largura da imagem
     * @return Largura em pixels
     */
    int getWidth() const;

    /**
     * @brief Retorna a altura da imagem
     * @return Altura em pixels
     */
    int getHeight() const;

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
    int pixelToIndex(int x, int y) const;
};

#endif // GRAPH_H
