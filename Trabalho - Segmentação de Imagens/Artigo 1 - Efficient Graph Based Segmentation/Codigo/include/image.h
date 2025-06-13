#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>

/**
 * @brief Estrutura que representa um pixel RGB
 * 
 * Cada pixel é composto por três componentes de cor:
 * - r: componente vermelho (0-255)
 * - g: componente verde (0-255)
 * - b: componente azul (0-255)
 */
struct Pixel {
    int r, g, b;
};

/**
 * @brief Classe responsável pelo gerenciamento de imagens
 * 
 * Esta classe implementa operações básicas para manipulação de imagens
 * no formato PPM (Portable Pixmap Format). Ela permite carregar imagens
 * do disco, salvá-las e manipular seus pixels individualmente.
 */
class Image {
private:
    int width, height;  // Dimensões da imagem
    std::vector<std::vector<Pixel>> data;  // Matriz de pixels da imagem

public:
    /**
     * @brief Construtor padrão
     * Inicializa uma imagem vazia com dimensões 0x0
     */
    Image();

    /**
     * @brief Carrega uma imagem do formato PPM
     * @param filename Caminho do arquivo PPM
     * @return true se a imagem foi carregada com sucesso, false caso contrário
     */
    bool loadPPM(const std::string& filename);

    /**
     * @brief Salva a imagem no formato PPM
     * @param filename Caminho onde a imagem será salva
     * @return true se a imagem foi salva com sucesso, false caso contrário
     */
    bool savePPM(const std::string& filename) const;
    
    /**
     * @brief Retorna a largura da imagem
     */
    int getWidth() const;

    /**
     * @brief Retorna a altura da imagem
     */
    int getHeight() const;

    /**
     * @brief Obtém o pixel em uma posição específica
     * @param x Coordenada x do pixel
     * @param y Coordenada y do pixel
     * @return Pixel na posição (x,y)
     */
    Pixel getPixel(int x, int y) const;

    /**
     * @brief Define o valor de um pixel em uma posição específica
     * @param x Coordenada x do pixel
     * @param y Coordenada y do pixel
     * @param pixel Novo valor do pixel
     */
    void setPixel(int x, int y, const Pixel& pixel);
    
    /**
     * @brief Retorna a matriz de pixels da imagem
     * @return Referência constante para a matriz de pixels
     */
    const std::vector<std::vector<Pixel>>& getData() const;
};

#endif // IMAGE_H
