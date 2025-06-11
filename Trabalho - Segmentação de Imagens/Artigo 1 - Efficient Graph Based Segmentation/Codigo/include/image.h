#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>

struct Pixel {
    int r, g, b;
};

class Image {
private:
    int width, height;
    std::vector<std::vector<Pixel>> data;

public:
    Image();
    bool loadPPM(const std::string& filename);
    bool savePPM(const std::string& filename) const;
    
    int getWidth() const;
    int getHeight() const;
    Pixel getPixel(int x, int y) const;
    void setPixel(int x, int y, const Pixel& pixel);
    
    const std::vector<std::vector<Pixel>>& getData() const;
};

#endif // IMAGE_H
