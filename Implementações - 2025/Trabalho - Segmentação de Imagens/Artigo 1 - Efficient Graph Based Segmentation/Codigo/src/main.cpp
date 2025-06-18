#include <iostream>
#include <cstdlib>
#include <string>
#include "image.h"
#include "segmentalgorithm.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <caminho/para/imagem.png>" << std::endl;
        return 1;
    }

    std::string inputPngPath = argv[1];

    // Extrair nome base do arquivo
    size_t lastSlash = inputPngPath.find_last_of("/\\");
    std::string filename = inputPngPath.substr(lastSlash + 1);
    std::string basename = filename.substr(0, filename.find_last_of('.'));

    // Caminhos dos arquivos
    std::string dataFolder = "data/";
    std::string resultFolder = "result/";
    std::string inputPpmPath = dataFolder + basename + ".ppm";
    std::string outputPpmPath = resultFolder + basename + "_seg.ppm";
    std::string outputPngPath = resultFolder + basename + "_seg.png";

    // 1. PNG → PPM
    std::string convertToPPMCommand = "python3 scripts/pngToPpm.py " + inputPngPath + " " + inputPpmPath;
    std::cout << "Convertendo PNG para PPM..." << std::endl;
    if (system(convertToPPMCommand.c_str()) != 0) {
        std::cerr << "Erro ao converter imagem PNG para PPM." << std::endl;
        return 1;
    }

    // 2. Carregar imagem PPM
    Image image;
    if (!image.loadPPM(inputPpmPath)) {
        std::cerr << "Erro ao carregar imagem PPM." << std::endl;
        return 1;
    }

    // 3. Parâmetros da segmentação
    float k = 500.0f;
    int minSize = 20;

    // 4. Segmentar imagem
    Image segmented = segmentImage(image, k, minSize);
    if (!segmented.savePPM(outputPpmPath)) {
        std::cerr << "Erro ao salvar imagem segmentada em PPM." << std::endl;
        return 1;
    }

    // 5. PPM → PNG
    std::string convertToPNGCommand = "python3 scripts/ppmToPng.py " + outputPpmPath + " " + outputPngPath;
    std::cout << "Convertendo PPM segmentado para PNG..." << std::endl;
    if (system(convertToPNGCommand.c_str()) != 0) {
        std::cerr << "Erro ao converter imagem PPM para PNG." << std::endl;
        return 1;
    }

    std::cout << "Segmentação concluída com sucesso!" << std::endl;
    std::cout << "Resultado salvo em: " << outputPngPath << std::endl;

    return 0;
}
