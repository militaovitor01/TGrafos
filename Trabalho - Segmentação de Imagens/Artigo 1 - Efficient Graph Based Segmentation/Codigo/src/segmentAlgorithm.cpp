#include <vector>
#include <algorithm>
#include "segmentalgorithm.h"
#include "Graph.h"
#include "disjoint_set.h"
#include "Image.h"

struct Edge {
    int a, b;       // índices dos nós (pixels ou regiões)
    float weight;   // diferença entre os pixels
};

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
                threshold[newParent] = e.weight + thresholdFunction(uf.size(newParent), k);
            }
        }
    }

    // Pós-processamento para juntar componentes menores que minSize
    for (const Edge& e : edges) {
        int a = uf.find(e.a);
        int b = uf.find(e.b);
        if (a != b && (uf.size(a) < minSize || uf.size(b) < minSize)) {
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
