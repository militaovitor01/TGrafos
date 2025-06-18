#ifndef SEGMENT_GRAPH_CPP
#define SEGMENT_GRAPH_CPP

#include <vector>
#include <algorithm>
#include <cmath>

// Classe Edge para representar as arestas do grafo
class Edge {
public:
int a, b;
float w;

Edge(int a, int b, float w) : a(a), b(b), w(w) {}

bool operator<(const Edge& other) const {
    return w < other.w;
}
};

// Implementação do Disjoint-Set
class Universe {
private:
struct Element {
    int rank;
    int p;
    int size;
};
std::vector<Element> elements;

public:
Universe(int elements_size) : elements(elements_size) {
    for (int i = 0; i < elements_size; i++) {
        elements[i].rank = 0;
        elements[i].size = 1;
        elements[i].p = i;
    }
}

// Adicionado const ao método find
int find(int x) const {
    int y = x;
    while (y != elements[y].p)
        y = elements[y].p;
    // Como é const, não podemos modificar elements[x].p
    // Removida a otimização de caminho
    return y;
}

// Versão não-const de find que permite otimização de caminho
int find_and_compress(int x) {
    int y = find(x);
    elements[x].p = y;
    return y;
}

void join(int x, int y) {
    x = find_and_compress(x);
    y = find_and_compress(y);
    
    if (elements[x].rank > elements[y].rank) {
        elements[y].p = x;
        elements[x].size += elements[y].size;
    } else {
        elements[x].p = y;
        elements[y].size += elements[x].size;
        if (elements[x].rank == elements[y].rank)
            elements[y].rank++;
    }
}

int size(int x) const { 
    return elements[find(x)].size; 
}

int num_sets() const {
    std::vector<bool> counted(elements.size(), false);
    int count = 0;
    
    for (size_t i = 0; i < elements.size(); i++) {
        int set_id = find(i);
        if (!counted[set_id]) {
            counted[set_id] = true;
            count++;
        }
    }
    return count;
}
};

// Função auxiliar para calcular o limiar
static inline float threshold(int size, float c) {
return c / size;
}

// Função principal de segmentação
void segment_graph(int num_vertices, std::vector<Edge>& edges, float c, Universe& u) {
// Ordena as arestas por peso
std::sort(edges.begin(), edges.end());

// Inicializa os thresholds
std::vector<float> threshold_values(num_vertices);
for (int i = 0; i < num_vertices; i++)
    threshold_values[i] = threshold(1, c);

// Percorre todas as arestas em ordem crescente de peso
for (const auto& edge : edges) {
    // Encontra os conjuntos de cada vértice
    int a = u.find_and_compress(edge.a);
    int b = u.find_and_compress(edge.b);

    if (a != b) {
        // Verifica se deve unir os componentes
        if (edge.w <= threshold_values[a] && edge.w <= threshold_values[b]) {
            u.join(a, b);
            a = u.find_and_compress(a);
            threshold_values[a] = edge.w + threshold(u.size(a), c);
        }
    }
}
}

#endif 