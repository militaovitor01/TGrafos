#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <vector>

/**
 * @brief Implementação da estrutura de dados Union-Find (Disjoint Set)
 * 
 * Esta estrutura de dados é fundamental para o algoritmo de segmentação,
 * permitindo gerenciar conjuntos disjuntos de forma eficiente. Ela suporta
 * duas operações principais:
 * - find: encontrar o representante de um conjunto
 * - unite: unir dois conjuntos
 * 
 * A implementação utiliza compressão de caminho e união por rank para
 * garantir complexidade quase constante nas operações.
 */
class UnionFind {
private:
    std::vector<int> parent;  // Array que armazena o pai de cada elemento
    std::vector<int> rank;    // Array que armazena o rank de cada conjunto
    std::vector<int> componentSize; // Tamanho de cada componente

public:
    /**
     * @brief Construtor da estrutura Union-Find
     * @param size Número de elementos inicial
     */
    UnionFind(int size);

    /**
     * @brief Encontra o representante do conjunto que contém o elemento x
     * 
     * Utiliza compressão de caminho para otimizar futuras buscas
     * 
     * @param x Elemento cujo representante será encontrado
     * @return Representante do conjunto
     */
    int find(int x);

    /**
     * @brief Une os conjuntos que contêm os elementos x e y
     * 
     * Utiliza união por rank para manter a árvore balanceada
     * 
     * @param x Primeiro elemento
     * @param y Segundo elemento
     */
    void unite(int x, int y);

    /**
     * @brief Verifica se dois elementos estão no mesmo conjunto
     * @param x Primeiro elemento
     * @param y Segundo elemento
     * @return true se os elementos estão no mesmo conjunto, false caso contrário
     */
    bool connected(int x, int y);

    /**
     * @brief Retorna o tamanho do conjunto ao qual o elemento x pertence
     * @param x Elemento
     * @return Tamanho do conjunto
     */
    int size(int x);
};

#endif // UNIONFIND_H
