#include "../include/segment_graph.h"
#include <vector>
#include <algorithm>
#include <cmath>

// Edge implementation
Edge::Edge(int a_, int b_, float w_) : a(a_), b(b_), w(w_) {}
bool Edge::operator<(const Edge& other) const {
    return w < other.w;
}

// Universe implementation
struct Universe::UniElement {
    int parent, rank, size;
};

Universe::Universe(int elements) : elements(elements) {
    for (int i = 0; i < elements; i++) {
        this->elements[i].rank = 0;
        this->elements[i].size = 1;
        this->elements[i].parent = i;
    }
}

Universe::~Universe() {}

int Universe::find(int x) const {
    int y = x;
    while (y != elements[y].parent)
        y = elements[y].parent;
    return y;
}

int Universe::find_and_compress(int x) {
    int y = find(x);
    elements[x].parent = y;
    return y;
}

void Universe::join(int x, int y) {
    x = find_and_compress(x);
    y = find_and_compress(y);
    if (elements[x].rank > elements[y].rank) {
        elements[y].parent = x;
        elements[x].size += elements[y].size;
    } else {
        elements[x].parent = y;
        elements[y].size += elements[x].size;
        if (elements[x].rank == elements[y].rank)
            elements[y].rank++;
    }
}

int Universe::size(int x) const {
    return elements[find(x)].size;
}

int Universe::num_sets() const {
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

static inline float threshold(int size, float c) {
    return c / size;
}

void segment_graph(int num_vertices, std::vector<Edge>& edges, float c, Universe& u) {
    std::sort(edges.begin(), edges.end());
    std::vector<float> threshold_values(num_vertices);
    for (int i = 0; i < num_vertices; i++)
        threshold_values[i] = threshold(1, c);
    for (const auto& edge : edges) {
        int a = u.find_and_compress(edge.a);
        int b = u.find_and_compress(edge.b);
        if (a != b) {
            if (edge.w <= threshold_values[a] && edge.w <= threshold_values[b]) {
                u.join(a, b);
                a = u.find_and_compress(a);
                threshold_values[a] = edge.w + threshold(u.size(a), c);
            }
        }
    }
} 