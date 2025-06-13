#include "disjoint_set.h"

/**
 * @brief Construtor da estrutura Union-Find
 * 
 * Inicializa a estrutura com 'size' elementos, onde cada elemento
 * forma seu próprio conjunto (é seu próprio pai) e tem rank 0.
 * 
 * @param size Número de elementos inicial
 */
UnionFind::UnionFind(int size) {
    parent.resize(size);
    rank.resize(size, 0);
    
    // Cada elemento é seu próprio pai inicialmente
    for (int i = 0; i < size; ++i) {
        parent[i] = i;
    }
}

/**
 * @brief Encontra o representante do conjunto que contém o elemento x
 * 
 * Utiliza compressão de caminho para otimizar futuras buscas:
 * - Durante a busca, todos os elementos visitados são conectados
 * diretamente ao representante do conjunto
 * 
 * @param x Elemento cujo representante será encontrado
 * @return Representante do conjunto
 */
int UnionFind::find(int x) {
    // Se x não é seu próprio pai, encontra o representante do pai
    if (parent[x] != x) {
        parent[x] = find(parent[x]);  // Compressão de caminho
    }
    return parent[x];
}

/**
 * @brief Une os conjuntos que contêm os elementos x e y
 * 
 * Utiliza união por rank para manter a árvore balanceada:
 * - O conjunto com menor rank é anexado ao conjunto com maior rank
 * - Se os ranks são iguais, um é incrementado
 * 
 * @param x Primeiro elemento
 * @param y Segundo elemento
 */
void UnionFind::unite(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);
    
    // Se já estão no mesmo conjunto, não faz nada
    if (rootX == rootY) return;
    
    // Une o conjunto de menor rank ao de maior rank
    if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY;
    } else if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX;
    } else {
        // Se os ranks são iguais, une e incrementa o rank
        parent[rootY] = rootX;
        rank[rootX]++;
    }
}

/**
 * @brief Verifica se dois elementos estão no mesmo conjunto
 * 
 * Dois elementos estão no mesmo conjunto se têm o mesmo representante
 * 
 * @param x Primeiro elemento
 * @param y Segundo elemento
 * @return true se os elementos estão no mesmo conjunto, false caso contrário
 */
bool UnionFind::connected(int x, int y) {
    return find(x) == find(y);
}

/**
 * @brief Retorna o tamanho do conjunto ao qual o elemento x pertence
 */
int UnionFind::size(int x) {
    return componentSize[find(x)];
}