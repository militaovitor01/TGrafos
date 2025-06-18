#ifndef SEGMENT_GRAPH_H
#define SEGMENT_GRAPH_H

#include <vector>

struct Edge {
    int a, b;
    float w;
    Edge(int a_, int b_, float w_);
    bool operator<(const Edge& other) const;
};

class Universe {
public:
    Universe(int elements);
    ~Universe();
    int find(int x) const;
    int find_and_compress(int x);
    void join(int x, int y);
    int size(int x) const;
    int num_sets() const;
private:
    struct UniElement;
    std::vector<UniElement> elements;
};

void segment_graph(int num_vertices, std::vector<Edge>& edges, float k, Universe& u);

#endif // SEGMENT_GRAPH_H 