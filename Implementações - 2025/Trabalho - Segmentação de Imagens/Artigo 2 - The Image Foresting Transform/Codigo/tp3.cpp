#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <cstdlib>
#include <array>
#include <climits>
#include <cmath>
#include <algorithm>

using namespace std;

struct QueueNode {
    int cost, x, y;
    QueueNode(int cost, int x, int y) : cost(cost), x(x), y(y) {}
    bool operator>(const QueueNode& other) const { return cost > other.cost; }
};

struct Pixel {
    int x, y, intensity;
    Pixel(int x=0, int y=0, int intensity=0) : x(x), y(y), intensity(intensity) {}
};

const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

struct NodeInfo {
    int cost;
    Pixel predecessor;
    bool visited;
    int seedId;
    NodeInfo() : cost(INT_MAX), predecessor(-1, -1), visited(false), seedId(-1) {}
};

vector<vector<Pixel>> image;
vector<vector<NodeInfo>> info;
vector<vector<array<int, 3>>> imagemRGB;
vector<vector<double>> gradientMagnitude;

void carregarImagemPPM(const string& caminho) {
    ifstream arq(caminho, ios::binary);
    if (!arq.is_open()) { cerr << "Erro ao abrir imagem " << caminho << endl; exit(1); }
    string tipo;
    arq >> tipo;
    if (tipo != "P6") { cerr << "Formato PPM não suportado. Use P6.\n"; exit(1); }
    int largura, altura, maxValor;
    arq >> largura >> altura >> maxValor;
    arq.ignore();
    
    image.resize(altura, vector<Pixel>(largura));
    info.resize(altura, vector<NodeInfo>(largura));
    imagemRGB.resize(altura, vector<array<int, 3>>(largura));
    gradientMagnitude.resize(altura, vector<double>(largura, 0.0));
    
    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            unsigned char rgb[3];
            arq.read(reinterpret_cast<char*>(rgb), 3);
            imagemRGB[y][x][0] = rgb[0];
            imagemRGB[y][x][1] = rgb[1];
            imagemRGB[y][x][2] = rgb[2];
            int intensidade = (rgb[0] + rgb[1] + rgb[2]) / 3;
            image[y][x] = Pixel(x, y, intensidade);
        }
    }
    arq.close();
}

// Gradiente melhorado com filtro Gaussiano para reduzir ruído
void calcularGradienteMelhorado() {
    int altura = image.size();
    int largura = image[0].size();
    
    // Primeiro aplica suavização Gaussiana
    vector<vector<double>> imagemSuavizada(altura, vector<double>(largura));
    double kernel[5][5] = {
        {1, 4, 7, 4, 1},
        {4, 16, 26, 16, 4},
        {7, 26, 41, 26, 7},
        {4, 16, 26, 16, 4},
        {1, 4, 7, 4, 1}
    };
    double soma = 273.0;
    
    for (int y = 2; y < altura-2; ++y) {
        for (int x = 2; x < largura-2; ++x) {
            double valor = 0.0;
            for (int dy = -2; dy <= 2; ++dy) {
                for (int dx = -2; dx <= 2; ++dx) {
                    valor += image[y+dy][x+dx].intensity * kernel[dy+2][dx+2];
                }
            }
            imagemSuavizada[y][x] = valor / soma;
        }
    }
    
    // Calcula gradiente na imagem suavizada
    for (int y = 1; y < altura-1; ++y) {
        for (int x = 1; x < largura-1; ++x) {
            double gx = imagemSuavizada[y][x+1] - imagemSuavizada[y][x-1];
            double gy = imagemSuavizada[y+1][x] - imagemSuavizada[y-1][x];
            gradientMagnitude[y][x] = sqrt(gx*gx + gy*gy);
        }
    }
}

// Sementes mais estratégicas para melhor separação
vector<Pixel> inicializarSementesOtimizadas() {
    int altura = image.size();
    int largura = image[0].size();
    vector<Pixel> sementes;
    
    // ÁRVORE: Múltiplas sementes bem distribuídas na copa e tronco
    sementes.push_back(Pixel(largura/2, altura*2/5, image[altura*2/5][largura/2].intensity));     // Copa centro
    sementes.push_back(Pixel(largura*3/7, altura*2/5, image[altura*2/5][largura*3/7].intensity)); // Copa esquerda
    sementes.push_back(Pixel(largura*4/7, altura*2/5, image[altura*2/5][largura*4/7].intensity)); // Copa direita
    sementes.push_back(Pixel(largura/2, altura/2, image[altura/2][largura/2].intensity));         // Copa baixa
    sementes.push_back(Pixel(largura/2, altura*3/5, image[altura*3/5][largura/2].intensity));     // Tronco
    
    // FUNDO: Sementes bem distantes da árvore
    sementes.push_back(Pixel(largura/12, altura/12, image[altura/12][largura/12].intensity));         // Céu canto
    sementes.push_back(Pixel(largura*11/12, altura/12, image[altura/12][largura*11/12].intensity));   // Céu canto
    sementes.push_back(Pixel(largura/12, altura*11/12, image[altura*11/12][largura/12].intensity));   // Grama canto
    sementes.push_back(Pixel(largura*11/12, altura*11/12, image[altura*11/12][largura*11/12].intensity)); // Grama canto
    
    for (int i = 0; i < sementes.size(); ++i) {
        auto& semente = sementes[i];
        info[semente.y][semente.x].cost = 0;
        info[semente.y][semente.x].visited = false;
        info[semente.y][semente.x].seedId = i;
    }
    
    cout << "Criadas " << sementes.size() << " sementes otimizadas" << endl;
    return sementes;
}

// IFT-Watershed otimizado para melhor separação
void executarIFTOtimizado(const vector<Pixel>& sementes, int largura, int altura) {
    priority_queue<QueueNode, vector<QueueNode>, greater<QueueNode>> fila;
    
    for (const Pixel& s : sementes) {
        fila.push(QueueNode(0, s.x, s.y));
    }
    
    while (!fila.empty()) {
        QueueNode atual = fila.top();
        fila.pop();
        int x = atual.x, y = atual.y;
        
        if (info[y][x].visited) continue;
        info[y][x].visited = true;
        
        for (int i = 0; i < 8; ++i) {
            int nx = x + dx[i], ny = y + dy[i];
            if (nx < 0 || nx >= largura || ny < 0 || ny >= altura) continue;
            
            // Função de custo híbrida: watershed + diferença de cor
            double gradiente = gradientMagnitude[ny][nx];
            
            // Diferença de cor RGB para melhor discriminação
            double diffR = abs(imagemRGB[y][x][0] - imagemRGB[ny][nx][0]);
            double diffG = abs(imagemRGB[y][x][1] - imagemRGB[ny][nx][1]);
            double diffB = abs(imagemRGB[y][x][2] - imagemRGB[ny][nx][2]);
            double diffCor = (diffR + diffG + diffB) / 3.0;
            
            // Combina gradiente e diferença de cor
            int custoAresta = max(info[y][x].cost, (int)(gradiente * 2.0 + diffCor * 0.5));
            
            if (custoAresta < info[ny][nx].cost) {
                info[ny][nx].cost = custoAresta;
                info[ny][nx].predecessor = Pixel(x, y);
                info[ny][nx].seedId = info[y][x].seedId;
                fila.push(QueueNode(custoAresta, nx, ny));
            }
        }
    }
}

// Pós-processamento mais agressivo para eliminar fragmentação
void suavizarSegmentacaoAgressiva(int largura, int altura) {
    vector<vector<int>> novoSeedId(altura, vector<int>(largura));
    
    // Primeira passada: filtro de mediana 5x5
    for (int y = 2; y < altura-2; ++y) {
        for (int x = 2; x < largura-2; ++x) {
            vector<int> vizinhos;
            for (int dy = -2; dy <= 2; ++dy) {
                for (int dx = -2; dx <= 2; ++dx) {
                    vizinhos.push_back(info[y+dy][x+dx].seedId);
                }
            }
            
            sort(vizinhos.begin(), vizinhos.end());
            novoSeedId[y][x] = vizinhos[vizinhos.size()/2]; // Mediana
        }
    }
    
    // Segunda passada: voto majoritário 3x3
    for (int y = 1; y < altura-1; ++y) {
        for (int x = 1; x < largura-1; ++x) {
            vector<int> contadores(10, 0); // Assume máximo 10 sementes
            
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int id = novoSeedId[y+dy][x+dx];
                    if (id >= 0 && id < 10) contadores[id]++;
                }
            }
            
            int maxVotos = 0, melhorId = novoSeedId[y][x];
            for (int i = 0; i < 10; ++i) {
                if (contadores[i] > maxVotos) {
                    maxVotos = contadores[i];
                    melhorId = i;
                }
            }
            
            info[y][x].seedId = melhorId;
        }
    }
}

// Cores mais organizadas e contrastantes
array<int, 3> corMelhorada(int seedId, int r, int g, int b) {
    int intensidade = (r + g + b) / 3;
    
    // ÁRVORE (sementes 0-4): Tons de verde, amarelo, ciano
    if (seedId >= 0 && seedId <= 4) {
        if (intensidade > 160) return {150, 255, 200};      // Verde-ciano muito claro
        else if (intensidade > 130) return {100, 240, 160}; // Verde-ciano claro
        else if (intensidade > 100) return {80, 200, 120};  // Verde médio
        else if (intensidade > 70) return {200, 255, 120};  // Verde-amarelo
        else if (intensidade > 40) return {60, 160, 100};   // Verde escuro
        else return {180, 120, 255};                        // Roxo para tronco
    }
    // FUNDO (sementes 5-8): Tons bem contrastantes
    else {
        if (seedId == 5 || seedId == 6) { // Céu
            if (intensidade > 150) return {255, 180, 255};   // Rosa claro
            else return {200, 150, 255};                     // Lilás
        } else { // Grama
            if (intensidade > 100) return {255, 200, 120};   // Laranja claro
            else return {255, 160, 100};                     // Laranja médio
        }
    }
}

int main() {
    string entrada_png = "imagens/monalisapb.png";
    string entrada_ppm = "PPMs/monalisapb.ppm";
    string saida_ppm = "PPMs/monalisapbOut.ppm";
    string saida_png = "resultados/monalisapbOut.png";

    string comando1 = "python scripts/png_to_ppm.py " + entrada_png + " " + entrada_ppm;
    if (system(comando1.c_str()) != 0) {
        cerr << "Erro ao converter PNG para PPM.\n";
        return 1;
    }

    carregarImagemPPM(entrada_ppm);
    int altura = image.size();
    int largura = image[0].size();

    cout << "Calculando gradiente melhorado..." << endl;
    calcularGradienteMelhorado();

    vector<Pixel> sementes = inicializarSementesOtimizadas();
    
    cout << "Executando IFT-Watershed otimizado..." << endl;
    executarIFTOtimizado(sementes, largura, altura);
    
    cout << "Aplicando suavização agressiva..." << endl;
    suavizarSegmentacaoAgressiva(largura, altura);

    ofstream out(saida_ppm, ios::binary);
    out << "P6\n" << largura << " " << altura << "\n255\n";
    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            array<int, 3> cor = corMelhorada(
                info[y][x].seedId,
                imagemRGB[y][x][0],
                imagemRGB[y][x][1], 
                imagemRGB[y][x][2]
            );
            out.put((unsigned char)cor[0]);
            out.put((unsigned char)cor[1]);
            out.put((unsigned char)cor[2]);
        }
    }
    out.close();

    string comando2 = "python scripts/ppm_to_png.py " + saida_ppm + " " + saida_png;
    if (system(comando2.c_str()) != 0) {
        cerr << "Erro ao converter PPM para PNG.\n";
        return 1;
    }

    cout << "IFT-Watershed otimizado concluído! Resultado salvo em: " << saida_png << endl;
    return 0;
}
