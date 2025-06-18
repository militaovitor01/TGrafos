#include "disjoint_set.h"

UnionFind::UnionFind(int size) {
    parent.resize(size);
    rank.resize(size, 0);
    for (int i = 0; i < size; i++) {
        parent[i] = i;
    }
}

// Encontra o representante do conjunto
int UnionFind::find(int x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]);  // Path compression
    }
    return parent[x];
}

// Une dois conjuntos 
void UnionFind::unite(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);
    if (rootX == rootY) return;

    // Union by rank
    if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY;
    } else if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX;
    } else {
        parent[rootY] = rootX;
        rank[rootX]++;
    }
}

// Verifica se dois elementos estão no mesmo conjunto
bool UnionFind::connected(int x, int y) {
    return find(x) == find(y);
}
