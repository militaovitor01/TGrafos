#include <iostream>
#include <string>
#include <cstdlib>
#include "include/image.h"
#include "include/converter.h"
#include "include/segmenter.h"

int main(int argc, char* argv[]) {
    // Configurações dos diretórios
    const std::string input_dir = "data/";
    const std::string output_dir = "result/";
    const std::string input_file = "homer.png";  // arquivo de entrada padrão

    // Construir caminhos completos
    std::string input_path = input_dir + input_file;

    // Extrair nome do arquivo sem extensão
    size_t lastdot = input_file.find_last_of(".");
    std::string filename_without_ext = input_file.substr(0, lastdot);

    // Construir caminho de saída
    std::string output_path = output_dir + filename_without_ext + ".png";

    // Arquivos temporários
    std::string temp_input_ppm = "temp_input.ppm";
    std::string temp_output_ppm = "temp_output.ppm";

    float sigma = 0.5;  // Parâmetro de suavização
    float k = 500;      // Define o limiar para determinar se dois segmentos devem ser unidos
    int min = 50;       // Tamanho mínimo do segmento em pixels

    try {
        // Converter PNG para PPM
        std::cout << "Converting input PNG to PPM..." << std::endl;
        if (!convert_png_to_ppm(input_path, temp_input_ppm)) {
            throw std::runtime_error("Failed to convert input PNG to PPM");
        }

        // Ler e processar a imagem
        Image input_image = read_ppm(temp_input_ppm);
        std::cout << "Processing..." << std::endl;
        segment(input_image, sigma, k, min, temp_output_ppm);

        // Converter resultado de PPM para PNG
        std::cout << "Converting output PPM to PNG..." << std::endl;
        if (!convert_ppm_to_png(temp_output_ppm, output_path)) {
            throw std::runtime_error("Failed to convert output PPM to PNG");
        }

        // Limpar arquivos temporários
        std::remove(temp_input_ppm.c_str());
        std::remove(temp_output_ppm.c_str());

        std::cout << "Successfully completed segmentation" << std::endl;
        std::cout << "Result saved to: " << output_path << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        // Tentar limpar arquivos temporários em caso de erro
        std::remove(temp_input_ppm.c_str());
        std::remove(temp_output_ppm.c_str());
        return 1;
    }

    return 0;
} 