#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <iomanip>
#include "include/image.h"
#include "include/converter.h"
#include "include/segmenter.h"

void printUsage(const char* programName) {
    std::cout << "Uso: " << programName << " [imagem] [sigma] [k] [min]" << std::endl;
    std::cout << "  imagem: nome do arquivo PNG em data/ (padrão: horse.png)" << std::endl;
    std::cout << "  sigma:  parâmetro de suavização (padrão: 0.5)" << std::endl;
    std::cout << "  k:      threshold de segmentação (padrão: 500)" << std::endl;
    std::cout << "  min:    tamanho mínimo do segmento (padrão: 50)" << std::endl;
    std::cout << std::endl;
    std::cout << "Exemplos:" << std::endl;
    std::cout << "  " << programName << "                    # usa valores padrão" << std::endl;
    std::cout << "  " << programName << " homer.png          # processa homer.png" << std::endl;
    std::cout << "  " << programName << " bigTree.png 1.0 300 100  # com parâmetros customizados" << std::endl;
}

int main(int argc, char* argv[]) {
    // Iniciar cronômetro
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Configurações dos diretórios
    const std::string input_dir = "data/";
    const std::string output_dir = "result/";
    
    // Parâmetros padrão
    std::string input_file = "horse.png";
    float sigma = 0.5;
    float k = 500;
    int min = 50;
    
    // Processar argumentos de linha de comando
    if (argc > 1) {
        if (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h") {
            printUsage(argv[0]);
            return 0;
        }
        input_file = argv[1];
    }
    if (argc > 2) sigma = std::stof(argv[2]);
    if (argc > 3) k = std::stof(argv[3]);
    if (argc > 4) min = std::stoi(argv[4]);

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

    std::cout << "=== Segmentação de Imagens por Grafo ===" << std::endl;
    std::cout << "Arquivo de entrada: " << input_file << std::endl;
    std::cout << "Parâmetros: sigma=" << sigma << ", k=" << k << ", min=" << min << std::endl;
    std::cout << "Iniciando processamento..." << std::endl << std::endl;

    try {
        // Converter PNG para PPM
        std::cout << "1. Convertendo PNG para PPM..." << std::endl;
        auto step1_start = std::chrono::high_resolution_clock::now();
        
        if (!convert_png_to_ppm(input_path, temp_input_ppm)) {
            throw std::runtime_error("Failed to convert input PNG to PPM");
        }
        
        auto step1_end = std::chrono::high_resolution_clock::now();
        auto step1_duration = std::chrono::duration_cast<std::chrono::milliseconds>(step1_end - step1_start);
        std::cout << "   ✓ Conversão PNG→PPM concluída em " << step1_duration.count() << " ms" << std::endl;

        // Ler e processar a imagem
        std::cout << "2. Lendo imagem PPM..." << std::endl;
        auto step2_start = std::chrono::high_resolution_clock::now();
        
        Image input_image = read_ppm(temp_input_ppm);
        
        auto step2_end = std::chrono::high_resolution_clock::now();
        auto step2_duration = std::chrono::duration_cast<std::chrono::milliseconds>(step2_end - step2_start);
        std::cout << "   ✓ Imagem lida em " << step2_duration.count() << " ms" << std::endl;
        std::cout << "   Dimensões: " << input_image.width << "x" << input_image.height << " pixels" << std::endl;
        std::cout << "   Total de pixels: " << (input_image.width * input_image.height) << std::endl;

        // Segmentação da imagem
        std::cout << "3. Executando segmentação..." << std::endl;
        auto step3_start = std::chrono::high_resolution_clock::now();
        
        segment(input_image, sigma, k, min, temp_output_ppm);
        
        auto step3_end = std::chrono::high_resolution_clock::now();
        auto step3_duration = std::chrono::duration_cast<std::chrono::milliseconds>(step3_end - step3_start);
        std::cout << "   ✓ Segmentação concluída em " << step3_duration.count() << " ms" << std::endl;

        // Converter resultado de PPM para PNG
        std::cout << "4. Convertendo PPM para PNG..." << std::endl;
        auto step4_start = std::chrono::high_resolution_clock::now();
        
        if (!convert_ppm_to_png(temp_output_ppm, output_path)) {
            throw std::runtime_error("Failed to convert output PPM to PNG");
        }
        
        auto step4_end = std::chrono::high_resolution_clock::now();
        auto step4_duration = std::chrono::duration_cast<std::chrono::milliseconds>(step4_end - step4_start);
        std::cout << "   ✓ Conversão PPM→PNG concluída em " << step4_duration.count() << " ms" << std::endl;

        // Limpar arquivos temporários
        std::remove(temp_input_ppm.c_str());
        std::remove(temp_output_ppm.c_str());

        // Calcular tempo total
        auto end_time = std::chrono::high_resolution_clock::now();
        auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        
        // Converter para segundos e milissegundos
        auto total_seconds = total_duration.count() / 1000;
        auto total_milliseconds = total_duration.count() % 1000;

        std::cout << std::endl << "=== Resumo da Execução ===" << std::endl;
        std::cout << "✓ Processamento concluído com sucesso!" << std::endl;
        std::cout << "✓ Resultado salvo em: " << output_path << std::endl;
        std::cout << std::endl << "=== Tempos de Execução ===" << std::endl;
        std::cout << "• Conversão PNG→PPM: " << step1_duration.count() << " ms" << std::endl;
        std::cout << "• Leitura da imagem:  " << step2_duration.count() << " ms" << std::endl;
        std::cout << "• Segmentação:        " << step3_duration.count() << " ms" << std::endl;
        std::cout << "• Conversão PPM→PNG: " << step4_duration.count() << " ms" << std::endl;
        std::cout << "• Tempo total:        " << total_seconds << "." << std::setfill('0') << std::setw(3) << total_milliseconds << " segundos" << std::endl;
        
        // Calcular porcentagens
        double total_ms = total_duration.count();
        std::cout << std::endl << "=== Distribuição do Tempo ===" << std::endl;
        std::cout << "• Conversão PNG→PPM: " << std::fixed << std::setprecision(1) << (step1_duration.count() / total_ms * 100) << "%" << std::endl;
        std::cout << "• Leitura da imagem:  " << std::fixed << std::setprecision(1) << (step2_duration.count() / total_ms * 100) << "%" << std::endl;
        std::cout << "• Segmentação:        " << std::fixed << std::setprecision(1) << (step3_duration.count() / total_ms * 100) << "%" << std::endl;
        std::cout << "• Conversão PPM→PNG: " << std::fixed << std::setprecision(1) << (step4_duration.count() / total_ms * 100) << "%" << std::endl;
        
        // Performance por pixel
        int total_pixels = input_image.width * input_image.height;
        double ms_per_pixel = total_ms / total_pixels;
        std::cout << std::endl << "=== Performance ===" << std::endl;
        std::cout << "• Pixels processados: " << total_pixels << std::endl;
        std::cout << "• Tempo por pixel:    " << std::fixed << std::setprecision(3) << ms_per_pixel << " ms/pixel" << std::endl;
        std::cout << "• Pixels por segundo: " << std::fixed << std::setprecision(0) << (total_pixels / (total_ms / 1000.0)) << " pixels/s" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "❌ Erro: " << e.what() << std::endl;
        // Tentar limpar arquivos temporários em caso de erro
        std::remove(temp_input_ppm.c_str());
        std::remove(temp_output_ppm.c_str());
        return 1;
    }

    return 0;
} 