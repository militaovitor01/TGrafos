#include "../include/converter.h"
#include <iostream>
#include <cstdlib>

// Função para converter PNG para PPM usando Python
bool convert_png_to_ppm(const std::string& input_png, const std::string& output_ppm) {
    std::string command = "python scripts/pngToPpm.py \"" + input_png + "\" \"" + output_ppm + "\"";
    std::cout << "Executing command: " << command << std::endl;
    int result = system(command.c_str());
    return result == 0;
}

// Função para converter PPM para PNG usando Python
bool convert_ppm_to_png(const std::string& input_ppm, const std::string& output_png) {
    std::string command = "python scripts/ppmToPng.py \"" + input_ppm + "\" \"" + output_png + "\"";
    std::cout << "Executing command: " << command << std::endl;
    int result = system(command.c_str());
    return result == 0;
} 