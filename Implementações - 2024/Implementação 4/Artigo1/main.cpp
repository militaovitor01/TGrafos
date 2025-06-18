#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <string>
#include <cstdlib>
#include "filter.cpp"
#include "segment_graph.cpp"

struct RGB {
unsigned char r, g, b;
};

struct Image {
int width;
int height;
std::vector<RGB> pixels;
};

// Função para gerar cores RGB aleatórias
RGB random_rgb() {
static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<float> dis(0, 1);
return RGB{
    static_cast<unsigned char>(dis(gen) * 255),
    static_cast<unsigned char>(dis(gen) * 255),
    static_cast<unsigned char>(dis(gen) * 255)
};
}

// Função para converter PNG para PPM usando Python
bool convert_png_to_ppm(const std::string& input_png, const std::string& output_ppm) {
std::string command = "python pngToPpm.py \"" + input_png + "\" \"" + output_ppm + "\"";
std::cout << "Executing command: " << command << std::endl;
int result = system(command.c_str());
return result == 0;
}

// Função para converter PPM para PNG usando Python
bool convert_ppm_to_png(const std::string& input_ppm, const std::string& output_png) {
std::string command = "python ppmToPng.py \"" + input_ppm + "\" \"" + output_png + "\"";
std::cout << "Executing command: " << command << std::endl;
int result = system(command.c_str());
return result == 0;
}

// Função para ler arquivo PPM
Image read_ppm(const std::string& filename) {
std::ifstream file(filename, std::ios::binary);
if (!file) {
    throw std::runtime_error("Cannot open file: " + filename);
}

std::string format;
file >> format;
if (format != "P6") {
    throw std::runtime_error("Invalid PPM format. Only P6 is supported.");
}

Image img;
file >> img.width >> img.height;

int maxval;
file >> maxval;
if (maxval != 255) {
    throw std::runtime_error("Only 8-bit per channel PPM files are supported.");
}

// Consume newline
file.get();

img.pixels.resize(img.width * img.height);
file.read(reinterpret_cast<char*>(img.pixels.data()), img.pixels.size() * sizeof(RGB));

return img;
}

// Função para salvar arquivo PPM
void write_ppm(const std::string& filename, const Image& img) {
std::ofstream file(filename, std::ios::binary);
if (!file) {
    throw std::runtime_error("Cannot create file: " + filename);
}

file << "P6\n" << img.width << " " << img.height << "\n255\n";
file.write(reinterpret_cast<const char*>(img.pixels.data()), img.pixels.size() * sizeof(RGB));
}

void segment(const Image& in_image, float sigma, float k, int min_size, const std::string& output_ppm) {
auto start_time = std::chrono::steady_clock::now();

int height = in_image.height;
int width = in_image.width;
std::cout << "Height: " << height << std::endl;
std::cout << "Width: " << width << std::endl;

// Separar canais e suavizar
std::vector<float> red_channel(width * height);
std::vector<float> green_channel(width * height);
std::vector<float> blue_channel(width * height);

for (int i = 0; i < width * height; i++) {
    red_channel[i] = in_image.pixels[i].r;
    green_channel[i] = in_image.pixels[i].g;
    blue_channel[i] = in_image.pixels[i].b;
}

auto smooth_red = Filter::smooth(red_channel, width, height, sigma);
auto smooth_green = Filter::smooth(green_channel, width, height, sigma);
auto smooth_blue = Filter::smooth(blue_channel, width, height, sigma);


// Construir grafo
std::vector<Edge> edges;
edges.reserve(width * height * 4);

for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
        if (x < width - 1) {
            edges.push_back(Edge(
                y * width + x,
                y * width + (x + 1),
                Filter::diff(smooth_red, smooth_green, smooth_blue, width, x, y, x + 1, y)
            ));
        }
        if (y < height - 1) {
            edges.push_back(Edge(
                y * width + x,
                (y + 1) * width + x,
                Filter::diff(smooth_red, smooth_green, smooth_blue, width, x, y, x + 1, y)
            ));
        }
        if (x < width - 1 && y < height - 1) {
            edges.push_back(Edge(
                y * width + x,
                (y + 1) * width + (x + 1),
                Filter::diff(smooth_red, smooth_green, smooth_blue, width, x, y, x + 1, y)
            ));
        }
        if (x < width - 1 && y > 0) {
            edges.push_back(Edge(
                y * width + x,
                (y - 1) * width + (x + 1),
                Filter::diff(smooth_red, smooth_green, smooth_blue, width, x, y, x + 1, y)
            ));
        }
    }
}

// Segmentar
Universe u(width * height);
segment_graph(width * height, edges, k, u);

// Pós-processamento de componentes pequenos
for (const auto& edge : edges) {
    int a = u.find(edge.a);
    int b = u.find(edge.b);
    if (a != b && (u.size(a) < min_size || u.size(b) < min_size)) {
        u.join(a, b);
    }
}

// Criar imagem de saída
Image output;
output.width = width;
output.height = height;
output.pixels.resize(width * height);

std::vector<RGB> colors(width * height);
for (int i = 0; i < width * height; i++) {
    colors[i] = random_rgb();
}

for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
        int comp = u.find(y * width + x);
        output.pixels[y * width + x] = colors[comp];
    }
}

auto end_time = std::chrono::steady_clock::now();
auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
std::cout << "Execution time: " << elapsed.count() / 60 << " minute(s) and "
          << elapsed.count() % 60 << " seconds" << std::endl;

// Salvar resultado em PPM
write_ppm(output_ppm, output);
}

int main(int argc, char* argv[]) {
// Configurações dos diretórios
const std::string input_dir = "data/";
const std::string output_dir = "result/";
const std::string input_file = "bigTree.png";  // arquivo de entrada padrão

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