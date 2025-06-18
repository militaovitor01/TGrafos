#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <vector>

class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    // Construtor
    UnionFind(int size);

    // Encontra o representante do conjunto
    int find(int x);

    // Une dois conjuntos
    void unite(int x, int y);

    // Verifica se dois elementos estão no mesmo conjunto
    bool connected(int x, int y);
};

#endif // UNIONFIND_H
