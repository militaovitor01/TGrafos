#ifndef FILTER_H
#define FILTER_H

#include <vector>

namespace Filter {
    std::vector<float> smooth(const std::vector<float>& channel, int width, int height, float sigma);
    float diff(const std::vector<float>& r, const std::vector<float>& g, const std::vector<float>& b, int width, int x1, int y1, int x2, int y2);
}

#endif // FILTER_H 