#ifndef CONVERTER_H
#define CONVERTER_H

#include <string>

// Função para converter PNG para PPM usando Python
bool convert_png_to_ppm(const std::string& input_png, const std::string& output_ppm);

// Função para converter PPM para PNG usando Python
bool convert_ppm_to_png(const std::string& input_ppm, const std::string& output_png);

#endif // CONVERTER_H 