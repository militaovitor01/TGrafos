#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <memory>

class Filter {
private:
 static constexpr float WIDTH = 4.0f;

 class GaussianMask {
 private:
     std::vector<std::vector<float>> mask;
     int length;

 public:
     explicit GaussianMask(float sigma) {
         sigma = std::max(sigma, 0.01f);
         length = static_cast<int>(std::ceil(sigma * WIDTH)) + 1;
         mask.resize(length, std::vector<float>(length));

         for (int i = 0; i < length; i++) {
             for (int j = 0; j < length; j++) {
                 mask[i][j] = std::exp(-0.5f * ((i * i) / (sigma * sigma) + (j * j) / (sigma * sigma)));
             }
         }
         normalize();
     }

     void normalize() {
         float sum = 0;
         
         // Soma todos os elementos
         for (int i = 0; i < length; i++) {
             for (int j = 0; j < length; j++) {
                 sum += std::abs(mask[i][j]);
             }
         }
         sum *= 4;

         // Subtrai os elementos extras que foram contados múltiplas vezes
         sum -= 3 * std::abs(mask[0][0]);
         for (int i = 0; i < length; i++) {
             sum -= 2 * std::abs(mask[0][i]); // primeira linha
             sum -= 2 * std::abs(mask[i][0]); // primeira coluna
         }

         // Normaliza a máscara
         for (auto& row : mask) {
             for (auto& val : row) {
                 val /= sum;
             }
         }
     }

     float operator()(int i, int j) const {
         return mask[i][j];
     }

     int size() const {
         return length;
     }
 };

 class Image {
 private:
     std::vector<float> data;
     int width;
     int height;

 public:
     Image(const std::vector<float>& src, int w, int h)
         : data(src), width(w), height(h) {}

     float at(int x, int y) const {
         return data[y * width + x];
     }

     int getWidth() const { return width; }
     int getHeight() const { return height; }
 };

public:
 static std::vector<float> smooth(const std::vector<float>& src, int width, int height, float sigma) {
     Image img(src, width, height);
     GaussianMask mask(sigma);
     std::vector<float> output(src.size());

     for (int y = 0; y < height; y++) {
         for (int x = 0; x < width; x++) {
             float sum = mask(0, 0) * img.at(x, y);

             for (int i = 0; i < mask.size(); i++) {
                 for (int j = 0; j < mask.size(); j++) {
                     if (i != 0 || j != 0) {
                         int y1 = std::max(y - j, 0);
                         int y2 = std::min(y + j, height - 1);
                         int x1 = std::max(x - i, 0);
                         int x2 = std::min(x + i, width - 1);

                         sum += mask(i, j) * (
                             img.at(x1, y1) +
                             img.at(x2, y1) +
                             img.at(x2, y2) +
                             img.at(x1, y2)
                         );
                     }
                 }
             }
             output[y * width + x] = sum;
         }
     }

     return output;
 }

 static float diff(const std::vector<float>& r, const std::vector<float>& g, const std::vector<float>& b,
                  int width, int x1, int y1, int x2, int y2) {
     float dr = r[y1 * width + x1] - r[y2 * width + x2];
     float dg = g[y1 * width + x1] - g[y2 * width + x2];
     float db = b[y1 * width + x1] - b[y2 * width + x2];
     return std::sqrt(dr * dr + dg * dg + db * db);
 }
};

#endif 