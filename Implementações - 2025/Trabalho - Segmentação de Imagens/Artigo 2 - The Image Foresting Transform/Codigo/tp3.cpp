#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

// Estrutura usada na fila de prioridade para o algoritmo IFT.
// Armazena o custo acumulado do caminho até o pixel, e suas coordenadas.
struct QueueNode {
    int cost;
    int x, y;

    QueueNode(int cost, int x, int y) : cost(cost), x(x), y(y) {}

    // Sobrecarga do operador > para tornar a fila uma min-heap (menor custo tem prioridade).
    bool operator>(const QueueNode& other) const {
        return cost > other.cost;
    }
};

// Representação de um pixel com coordenadas (x, y) e valor de intensidade (em tons de cinza).
struct Pixel {
    int x, y, intensity;
    Pixel(int x = 0, int y = 0, int intensity = 0) : x(x), y(y), intensity(intensity) {}
};

// Vetores de deslocamento para encontrar os 4 vizinhos (cima, baixo, esquerda, direita).
const int dx[4] = {-1, 1, 0, 0};
const int dy[4] = {0, 0, -1, 1};

// Informações associadas a cada nó/pixel durante o algoritmo IFT.
struct NodeInfo {
    int cost;              // Custo mínimo conhecido até esse pixel.
    Pixel predecessor;     // Pixel anterior no caminho ótimo.
    bool visited;          // Marca se o pixel já foi processado.

    NodeInfo() : cost(INT_MAX), predecessor(-1, -1), visited(false) {}
};

// Matrizes globais que representam a imagem e os dados auxiliares do IFT.
vector<vector<Pixel>> image;
vector<vector<NodeInfo>> info;

// Lê uma imagem PPM no formato binário (P6), converte para tons de cinza e preenche as estruturas globais.
void carregarImagemPPM(const string& caminho) {
    ifstream arq(caminho, ios::binary);
    if (!arq.is_open()) {
        cerr << "Erro ao abrir imagem " << caminho << endl;
        exit(1);
    }

    string tipo;
    arq >> tipo;
    if (tipo != "P6") {
        cerr << "Formato PPM não suportado. Use P6.\n";
        exit(1);
    }

    int largura, altura, maxValor;
    arq >> largura >> altura >> maxValor;
    arq.ignore(); // Ignora o '\n' após o cabeçalho

    // Inicializa as matrizes com o tamanho da imagem
    image.resize(altura, vector<Pixel>(largura));
    info.resize(altura, vector<NodeInfo>(largura));

    // Lê os pixels RGB, converte para intensidade (escala de cinza) e armazena
    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            unsigned char rgb[3];
            arq.read(reinterpret_cast<char*>(rgb), 3);
            int intensidade = (rgb[0] + rgb[1] + rgb[2]) / 3;
            image[y][x] = Pixel(x, y, intensidade);
        }
    }

    arq.close();
}

// Inicializa sementes fixas e configura seus custos como 0 no vetor de info.
// Essas sementes representam os pontos iniciais da segmentação.
vector<Pixel> inicializarSementes() {
    vector<Pixel> sementes;
    sementes.push_back(Pixel(30, 30, image[30][30].intensity));     // Exemplo: centro da região de interesse
    sementes.push_back(Pixel(100, 100, image[100][100].intensity)); // Exemplo: fora da região

    // Marca custo 0 para cada semente
    for (auto& semente : sementes) {
        info[semente.y][semente.x].cost = 0;
        info[semente.y][semente.x].visited = false;
    }

    return sementes;
}

// Executa o algoritmo Image Foresting Transform (IFT), uma variação de Dijkstra,
// para propagar caminhos mínimos a partir das sementes e segmentar a imagem.
void executarIFT(const vector<Pixel>& sementes, int largura, int altura) {
    // Fila de prioridade com base no menor custo acumulado
    priority_queue<QueueNode, vector<QueueNode>, greater<QueueNode>> fila;

    // Insere todas as sementes com custo 0
    for (const Pixel& s : sementes)
        fila.push(QueueNode(0, s.x, s.y));

    // Loop principal do algoritmo (variação de Dijkstra)
    while (!fila.empty()) {
        QueueNode atual = fila.top();
        fila.pop();

        int x = atual.x;
        int y = atual.y;

        if (info[y][x].visited) continue;
        info[y][x].visited = true;

        // Visita os 4 vizinhos
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            // Ignora vizinhos fora da imagem
            if (nx < 0 || nx >= largura || ny < 0 || ny >= altura) continue;

            // Calcula o custo com base na diferença de intensidade
            int custoAresta = abs(image[y][x].intensity - image[ny][nx].intensity);
            int novoCusto = info[y][x].cost + custoAresta;

            // Atualiza se o novo caminho for melhor
            if (novoCusto < info[ny][nx].cost) {
                info[ny][nx].cost = novoCusto;
                info[ny][nx].predecessor = Pixel(x, y);
                fila.push(QueueNode(novoCusto, nx, ny));
            }
        }
    }
}

int main() {
    // Caminhos das imagens organizados por pastas
    string entrada_png = "imagens/torreEiffel.png";               // Imagem original
    string entrada_ppm = "PPMs/torreEiffel.ppm";                  // Conversão temporária em PPM
    string saida_ppm = "PPMs/torreEiffelOut.ppm";                 // Resultado segmentado em PPM
    string saida_png = "resultados/torreEiffelOut.png";           // Resultado final em PNG

    // 1. Converte PNG → PPM usando script Python dentro da pasta scripts/
    string comando1 = "python scripts/png_to_ppm.py " + entrada_png + " " + entrada_ppm;
    if (system(comando1.c_str()) != 0) {
        cerr << "Erro ao converter PNG para PPM.\n";
        return 1;
    }

    // 2. Processa a imagem convertida com IFT
    carregarImagemPPM(entrada_ppm);
    int altura = image.size();
    int largura = image[0].size();

    vector<Pixel> sementes = inicializarSementes();
    executarIFT(sementes, largura, altura);

    // 3. Salva imagem segmentada em PPM com binarização simples (custo > 100 → branco)
    ofstream out(saida_ppm, ios::binary);
    out << "P6\n" << largura << " " << altura << "\n255\n";

    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            int v = info[y][x].cost > 100 ? 255 : 0;
            out.put((unsigned char)v); // R
            out.put((unsigned char)v); // G
            out.put((unsigned char)v); // B
        }
    }
    out.close();

    // 4. Converte o PPM segmentado de volta para PNG (mais leve e visual)
    string comando2 = "python scripts/ppm_to_png.py " + saida_ppm + " " + saida_png;
    if (system(comando2.c_str()) != 0) {
        cerr << "Erro ao converter PPM para PNG.\n";
        return 1;
    }

    cout << "Processamento concluído! Resultado salvo em: " << saida_png << endl;
    return 0;
}
