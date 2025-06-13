#include "image.h"
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * @brief Construtor padrão da classe Image
 * Inicializa uma imagem vazia com dimensões 0x0
 */
Image::Image() : width(0), height(0) {}

/**
 * @brief Carrega uma imagem do formato PPM
 * 
 * O formato P6 do PPM é um formato binário que armazena:
 * - Uma linha de cabeçalho com "P6"
 * - Dimensões da imagem (largura e altura)
 * - Valor máximo de cor (255)
 * - Dados binários dos pixels em RGB
 * 
 * @param filename Caminho do arquivo PPM
 * @return true se a imagem foi carregada com sucesso, false caso contrário
 */
bool Image::loadPPM(const std::string& filename) {
    // Abre o arquivo em modo binário
    std::ifstream file(filename, std::ios::binary);
    if (!file) return false;

    // Verifica o formato do arquivo (deve ser P6)
    std::string format;
    file >> format;
    if (format != "P6") return false;

    // Lê as dimensões da imagem
    file >> width >> height;
    int maxVal;
    file >> maxVal;
    file.ignore(); // ignora o '\n' após o valor máximo

    // Redimensiona a matriz de pixels
    data.resize(height, std::vector<Pixel>(width));

    // Lê os dados dos pixels
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            char rgb[3];
            file.read(rgb, 3);
            data[i][j] = { static_cast<unsigned char>(rgb[0]),
                           static_cast<unsigned char>(rgb[1]),
                           static_cast<unsigned char>(rgb[2]) };
        }
    }

    return true;
}

/**
 * @brief Salva a imagem no formato PPM
 * 
 * Salva a imagem no formato P6 do PPM, que inclui:
 * - Cabeçalho com "P6"
 * - Dimensões da imagem
 * - Valor máximo de cor (255)
 * - Dados binários dos pixels em RGB
 * 
 * @param filename Caminho onde a imagem será salva
 * @return true se a imagem foi salva com sucesso, false caso contrário
 */
bool Image::savePPM(const std::string& filename) const {
    // Abre o arquivo em modo binário
    std::ofstream file(filename, std::ios::binary);
    if (!file) return false;

    // Escreve o cabeçalho do arquivo
    file << "P6\n" << width << " " << height << "\n255\n";

    // Escreve os dados dos pixels
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            const Pixel& p = data[i][j];
            file.put(p.r).put(p.g).put(p.b);
        }
    }

    return true;
}

/**
 * @brief Retorna a largura da imagem
 * @return Largura da imagem em pixels
 */
int Image::getWidth() const { return width; }

/**
 * @brief Retorna a altura da imagem
 * @return Altura da imagem em pixels
 */
int Image::getHeight() const { return height; }

/**
 * @brief Obtém o pixel em uma posição específica
 * @param x Coordenada x do pixel (coluna)
 * @param y Coordenada y do pixel (linha)
 * @return Pixel na posição (x,y)
 */
Pixel Image::getPixel(int x, int y) const {
    return data[y][x]; // linha, coluna
}

/**
 * @brief Define o valor de um pixel em uma posição específica
 * @param x Coordenada x do pixel (coluna)
 * @param y Coordenada y do pixel (linha)
 * @param pixel Novo valor do pixel
 */
void Image::setPixel(int x, int y, const Pixel& pixel) {
    data[y][x] = pixel;
}

/**
 * @brief Retorna a matriz de pixels da imagem
 * @return Referência constante para a matriz de pixels
 */
const std::vector<std::vector<Pixel>>& Image::getData() const {
    return data;
}
