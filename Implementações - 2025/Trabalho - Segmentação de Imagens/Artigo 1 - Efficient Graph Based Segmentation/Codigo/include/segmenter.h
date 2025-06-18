#ifndef SEGMENTER_H
#define SEGMENTER_H

#include "image.h"
#include <string>

// Função principal de segmentação
void segment(const Image& in_image, float sigma, float k, int min_size, const std::string& output_ppm);

#endif // SEGMENTER_H 