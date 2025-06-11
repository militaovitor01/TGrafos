#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "image.h"

struct Edge {
    int a, b;       // índices dos pixels (linearizados)
    float weight;
};

class Graph {
private:
    int width, height;
    std::vector<Edge> edges;

    float diff(const Pixel& p1, const Pixel& p2) const;

public:
    Graph(const Image& img);
    const std::vector<Edge>& getEdges() const;
    int getWidth() const;
    int getHeight() const;
    int pixelToIndex(int x, int y) const;
};

#endif // GRAPH_H
