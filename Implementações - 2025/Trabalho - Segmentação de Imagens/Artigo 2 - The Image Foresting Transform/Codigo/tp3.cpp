#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <sstream>
#include <string>
#include <cstdlib>
#include <climits>
#include <random>
#include <cmath>
#include <algorithm>
#include <set>

using namespace std;

// Estrutura usada na fila de prioridade para o algoritmo IFT.
// Armazena o custo acumulado do caminho até o pixel, e suas coordenadas.
struct QueueNode {
    double cost;
    int x, y;
    int seedIndex; // Índice da semente que gerou este caminho

    QueueNode(double cost, int x, int y, int seedIndex) : cost(cost), x(x), y(y), seedIndex(seedIndex) {}

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

// Vetores de deslocamento para encontrar os 8 vizinhos (incluindo diagonais)
const int dx[8] = {-1, -1, -1,  0,  0,  1, 1, 1};
const int dy[8] = {-1,  0,  1, -1,  1, -1, 0, 1};
const double sqrt2 = 1.4142135623730951; // sqrt(2) para custos diagonais

// Informações associadas a cada nó/pixel durante o algoritmo IFT.
struct NodeInfo {
    double cost;              // Custo mínimo conhecido até esse pixel.
    Pixel predecessor;     // Pixel anterior no caminho ótimo.
    bool visited;          // Marca se o pixel já foi processado.
    int seedIndex;         // Índice da semente que gerou o caminho ótimo para este pixel

    NodeInfo() : cost(INT_MAX), predecessor(-1, -1), visited(false), seedIndex(-1) {}
};

// Matrizes globais que representam a imagem e os dados auxiliares do IFT.
vector<vector<Pixel>> image;
vector<vector<NodeInfo>> info;
vector<vector<double>> gradiente; // Gradiente da imagem para melhor função de custo
vector<vector<double>> suavizada; // Imagem suavizada

// Suavização da imagem usando filtro gaussiano simples
void suavizarImagem(int largura, int altura) {
    suavizada.resize(altura, vector<double>(largura, 0.0));
    
    // Kernel gaussiano 3x3 simples
    double kernel[3][3] = {
        {1.0/16, 2.0/16, 1.0/16},
        {2.0/16, 4.0/16, 2.0/16},
        {1.0/16, 2.0/16, 1.0/16}
    };
    
    for (int y = 1; y < altura - 1; ++y) {
        for (int x = 1; x < largura - 1; ++x) {
            double soma = 0.0;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    soma += image[y + dy][x + dx].intensity * kernel[dy + 1][dx + 1];
                }
            }
            suavizada[y][x] = soma;
        }
    }
}

// Calcula o gradiente da imagem usando operadores de Sobel
void calcularGradiente(int largura, int altura) {
    gradiente.resize(altura, vector<double>(largura, 0.0));
    
    for (int y = 1; y < altura - 1; ++y) {
        for (int x = 1; x < largura - 1; ++x) {
            // Operador de Sobel para gradiente
            double gx = (image[y-1][x+1].intensity + 2*image[y][x+1].intensity + image[y+1][x+1].intensity) -
                       (image[y-1][x-1].intensity + 2*image[y][x-1].intensity + image[y+1][x-1].intensity);
            
            double gy = (image[y+1][x-1].intensity + 2*image[y+1][x].intensity + image[y+1][x+1].intensity) -
                       (image[y-1][x-1].intensity + 2*image[y-1][x].intensity + image[y-1][x+1].intensity);
            
            gradiente[y][x] = sqrt(gx*gx + gy*gy);
        }
    }
}

// Função para encontrar sementes mais inteligentes baseadas em características da imagem
vector<Pixel> encontrarSementesInteligentes(int largura, int altura) {
    vector<Pixel> sementes;
    
    // Calcula histograma de intensidade para encontrar regiões homogêneas
    vector<int> histograma(256, 0);
    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            histograma[image[y][x].intensity]++;
        }
    }
    
    // Encontra os picos do histograma (regiões homogêneas)
    vector<pair<int, int>> picos; // (intensidade, frequência)
    for (int i = 1; i < 255; ++i) {
        if (histograma[i] > histograma[i-1] && histograma[i] > histograma[i+1] && histograma[i] > 100) {
            picos.push_back({i, histograma[i]});
        }
    }
    
    // Ordena por frequência e pega os 3 picos mais significativos
    sort(picos.begin(), picos.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    
    // Adiciona sementes nos picos mais significativos
    for (int i = 0; i < min(3, (int)picos.size()); ++i) {
        int intensidade = picos[i].first;
        // Encontra um pixel com essa intensidade
        for (int y = 10; y < altura - 10; ++y) {
            for (int x = 10; x < largura - 10; ++x) {
                if (abs(image[y][x].intensity - intensidade) < 5) {
                    sementes.push_back(Pixel(x, y, image[y][x].intensity));
                    goto proximo_pico;
                }
            }
        }
        proximo_pico:;
    }
    
    // Adiciona sementes em bordas significativas
    vector<pair<double, pair<int, int>>> bordas; // (gradiente, (x, y))
    for (int y = 10; y < altura - 10; ++y) {
        for (int x = 10; x < largura - 10; ++x) {
            if (gradiente[y][x] > 50) { // Limiar para bordas significativas
                bordas.push_back({gradiente[y][x], {x, y}});
            }
        }
    }
    
    // Ordena bordas por gradiente e adiciona as mais significativas
    sort(bordas.begin(), bordas.end(), greater<pair<double, pair<int, int>>>());
    for (int i = 0; i < min(3, (int)bordas.size()); ++i) {
        int x = bordas[i].second.first;
        int y = bordas[i].second.second;
        sementes.push_back(Pixel(x, y, image[y][x].intensity));
    }
    
    // Adiciona sementes em posições estratégicas (cantos, centro)
    sementes.push_back(Pixel(largura/2, altura/2, image[altura/2][largura/2].intensity));
    sementes.push_back(Pixel(largura/4, altura/4, image[altura/4][largura/4].intensity));
    sementes.push_back(Pixel(3*largura/4, altura/4, image[altura/4][3*largura/4].intensity));
    sementes.push_back(Pixel(largura/4, 3*altura/4, image[3*altura/4][largura/4].intensity));
    sementes.push_back(Pixel(3*largura/4, 3*altura/4, image[3*altura/4][3*largura/4].intensity));
    
    return sementes;
}

// Função de custo mais sofisticada baseada no IFT original
double calcularCustoAresta(int x1, int y1, int x2, int y2, int largura, int altura) {
    if (x2 < 0 || x2 >= largura || y2 < 0 || y2 >= altura) return INT_MAX;
    
    // Diferença de intensidade na imagem suavizada
    double diffIntensidade = abs(suavizada[y1][x1] - suavizada[y2][x2]);
    
    // Gradiente médio
    double gradMedio = (gradiente[y1][x1] + gradiente[y2][x2]) / 2.0;
    
    // Verifica se é movimento diagonal
    bool isDiagonal = (abs(x2 - x1) + abs(y2 - y1)) == 2;
    double fatorDistancia = isDiagonal ? sqrt2 : 1.0;
    
    // Função de custo baseada no IFT original
    // f(p,q) = |I(p) - I(q)| + α * G(p,q)
    double alpha = 0.3; // Peso do gradiente
    double custo = diffIntensidade + alpha * gradMedio;
    
    // Penaliza fortemente bordas muito fortes
    if (gradMedio > 150) {
        custo *= 5.0;
    }
    
    return custo * fatorDistancia;
}

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
    
    // Calcula o gradiente da imagem
    calcularGradiente(largura, altura);
    suavizarImagem(largura, altura);
}

// Estratégia de sementes mais sofisticada
vector<Pixel> encontrarSementesAvancadas(int largura, int altura) {
    vector<Pixel> sementes;
    set<pair<int, int>> posicoesUsadas; // Evita sementes muito próximas
    
    // 1. Sementes baseadas em k-means simplificado
    vector<int> centros = {50, 100, 150, 200}; // Intensidades típicas
    for (int centro : centros) {
        for (int tentativa = 0; tentativa < 10; ++tentativa) {
            int x = 10 + rand() % (largura - 20);
            int y = 10 + rand() % (altura - 20);
            
            if (abs(image[y][x].intensity - centro) < 20 && 
                posicoesUsadas.find({x, y}) == posicoesUsadas.end()) {
                sementes.push_back(Pixel(x, y, image[y][x].intensity));
                posicoesUsadas.insert({x, y});
                break;
            }
        }
    }
    
    // 2. Sementes em regiões homogêneas (baixo gradiente)
    vector<pair<double, pair<int, int>>> regioesHomogeneas;
    for (int y = 10; y < altura - 10; ++y) {
        for (int x = 10; x < largura - 10; ++x) {
            if (gradiente[y][x] < 30 && posicoesUsadas.find({x, y}) == posicoesUsadas.end()) {
                regioesHomogeneas.push_back({gradiente[y][x], {x, y}});
            }
        }
    }
    sort(regioesHomogeneas.begin(), regioesHomogeneas.end());
    for (int i = 0; i < min(3, (int)regioesHomogeneas.size()); ++i) {
        int x = regioesHomogeneas[i].second.first;
        int y = regioesHomogeneas[i].second.second;
        sementes.push_back(Pixel(x, y, image[y][x].intensity));
        posicoesUsadas.insert({x, y});
    }
    
    // 3. Sementes em posições estratégicas
    vector<pair<int, int>> posicoesEstrategicas = {
        {largura/2, altura/2},           // Centro
        {largura/4, altura/4},           // Canto superior esquerdo
        {3*largura/4, altura/4},         // Canto superior direito
        {largura/4, 3*altura/4},         // Canto inferior esquerdo
        {3*largura/4, 3*altura/4}        // Canto inferior direito
    };
    
    for (auto& pos : posicoesEstrategicas) {
        if (posicoesUsadas.find(pos) == posicoesUsadas.end()) {
            sementes.push_back(Pixel(pos.first, pos.second, image[pos.second][pos.first].intensity));
            posicoesUsadas.insert(pos);
        }
    }
    
    return sementes;
}

// Inicializa sementes
vector<Pixel> inicializarSementes() {
    int h = image.size();
    int w = image[0].size();
    
    vector<Pixel> sementes = encontrarSementesAvancadas(w, h);

    for (int i = 0; i < sementes.size(); ++i) {
        auto& semente = sementes[i];
        info[semente.y][semente.x].cost = 0;
        info[semente.y][semente.x].visited = false;
        info[semente.y][semente.x].seedIndex = i;
    }

    return sementes;
}

// Algoritmo IFT melhorado
void executarIFT(const vector<Pixel>& sementes, int largura, int altura) {
    priority_queue<QueueNode, vector<QueueNode>, greater<QueueNode>> fila;

    for (int i = 0; i < sementes.size(); ++i) {
        const Pixel& s = sementes[i];
        fila.push(QueueNode(0, s.x, s.y, i));
    }

    while (!fila.empty()) {
        QueueNode atual = fila.top();
        fila.pop();

        int x = atual.x;
        int y = atual.y;

        if (info[y][x].visited) continue;
        info[y][x].visited = true;
        info[y][x].seedIndex = atual.seedIndex;

        for (int i = 0; i < 8; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx < 0 || nx >= largura || ny < 0 || ny >= altura) continue;

            double custoAresta = calcularCustoAresta(x, y, nx, ny, largura, altura);
            double novoCusto = info[y][x].cost + custoAresta;

            if (novoCusto < info[ny][nx].cost) {
                info[ny][nx].cost = novoCusto;
                info[ny][nx].predecessor = Pixel(x, y);
                info[ny][nx].seedIndex = atual.seedIndex;
                fila.push(QueueNode(novoCusto, nx, ny, atual.seedIndex));
            }
        }
    }
}

// Pós-processamento avançado
void posProcessamentoAvancado(vector<vector<int>>& segmentacao, int largura, int altura, int numSementes) {
    // 1. Remove regiões muito pequenas
    vector<int> tamanhoRegiao(numSementes, 0);
    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            if (segmentacao[y][x] >= 0) {
                tamanhoRegiao[segmentacao[y][x]]++;
            }
        }
    }
    
    int limiarMinimo = (largura * altura) / 200; // 0.5% da imagem
    
    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            if (segmentacao[y][x] >= 0 && tamanhoRegiao[segmentacao[y][x]] < limiarMinimo) {
                segmentacao[y][x] = -1;
            }
        }
    }
    
    // 2. Suavização morfológica
    vector<vector<int>> temp = segmentacao;
    for (int y = 1; y < altura - 1; ++y) {
        for (int x = 1; x < largura - 1; ++x) {
            if (segmentacao[y][x] == -1) {
                // Voto majoritário dos vizinhos
                vector<int> votos(numSementes, 0);
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (segmentacao[y + dy][x + dx] >= 0) {
                            votos[segmentacao[y + dy][x + dx]]++;
                        }
                    }
                }
                
                int melhorRegiao = -1;
                int maxVotos = 0;
                for (int i = 0; i < numSementes; ++i) {
                    if (votos[i] > maxVotos) {
                        maxVotos = votos[i];
                        melhorRegiao = i;
                    }
                }
                
                if (melhorRegiao >= 0) {
                    temp[y][x] = melhorRegiao;
                }
            }
        }
    }
    segmentacao = temp;
}

int main() {
    cout << "Iniciando processamento de segmentação..." << endl;
    
    // Caminhos das imagens organizados por pastas
    string entrada_png = "imagens/cavalo.png";               // Imagem original
    string entrada_ppm = "PPMs/cavalo.ppm";                  // Conversão temporária em PPM
    string saida_ppm = "PPMs/cavaloOut.ppm";                 // Resultado segmentado em PPM
    string saida_png = "resultados/cavaloOut.png";           // Resultado final em PNG

    cout << "Convertendo PNG para PPM..." << endl;
    // 1. Converte PNG → PPM usando script Python dentro da pasta scripts/
    string comando1 = "python scripts/png_to_ppm.py " + entrada_png + " " + entrada_ppm;
    if (system(comando1.c_str()) != 0) {
        cerr << "Erro ao converter PNG para PPM.\n";
        return 1;
    }

    cout << "Carregando imagem PPM..." << endl;
    // 2. Processa a imagem convertida com IFT
    carregarImagemPPM(entrada_ppm);
    int altura = image.size();
    int largura = image[0].size();
    cout << "Imagem carregada: " << largura << "x" << altura << endl;

    cout << "Inicializando sementes..." << endl;
    vector<Pixel> sementes = inicializarSementes();
    cout << "Sementes criadas: " << sementes.size() << endl;

    cout << "Executando algoritmo IFT..." << endl;
    executarIFT(sementes, largura, altura);

    cout << "Aplicando pós-processamento..." << endl;
    // Cria matriz de segmentação
    vector<vector<int>> segmentacao(altura, vector<int>(largura, -1));
    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            segmentacao[y][x] = info[y][x].seedIndex;
        }
    }
    
    // Aplica pós-processamento
    posProcessamentoAvancado(segmentacao, largura, altura, sementes.size());

    cout << "Salvando resultado segmentado..." << endl;
    // 3. Salva imagem segmentada em PPM com cores aleatórias para cada região
    ofstream out(saida_ppm, ios::binary);
    out << "P6\n" << largura << " " << altura << "\n255\n";

    // Gera cores aleatórias para cada semente
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> color_dist(50, 255); // Cores mais vibrantes
    
    vector<vector<int>> cores(sementes.size(), vector<int>(3));
    for (int i = 0; i < sementes.size(); ++i) {
        cores[i][0] = color_dist(gen); // R
        cores[i][1] = color_dist(gen); // G
        cores[i][2] = color_dist(gen); // B
        cout << "Cor da semente " << i << ": RGB(" << cores[i][0] << "," << cores[i][1] << "," << cores[i][2] << ")" << endl;
    }

    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            int seedIdx = segmentacao[y][x];
            
            if (seedIdx >= 0 && seedIdx < sementes.size()) {
                // Pixel pertence a uma região segmentada - usa cor da semente
                out.put((unsigned char)cores[seedIdx][0]); // R
                out.put((unsigned char)cores[seedIdx][1]); // G
                out.put((unsigned char)cores[seedIdx][2]); // B
            } else {
                // Pixel não foi alcançado por nenhuma semente - preto
                out.put(0); // R
                out.put(0); // G
                out.put(0); // B
            }
        }
    }
    out.close();

    // Após salvar a imagem segmentada
    vector<int> contagem(sementes.size(), 0);
    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            int seedIdx = segmentacao[y][x];
            if (seedIdx >= 0 && seedIdx < sementes.size()) {
                contagem[seedIdx]++;
            }
        }
    }
    for (int i = 0; i < sementes.size(); ++i) {
        cout << "Pixels segmentados pela semente " << i << ": " << contagem[i] << endl;
    }

    cout << "Convertendo resultado para PNG..." << endl;
    // 4. Converte o PPM segmentado de volta para PNG (mais leve e visual)
    string comando2 = "python scripts/ppm_to_png.py " + saida_ppm + " " + saida_png;
    if (system(comando2.c_str()) != 0) {
        cerr << "Erro ao converter PPM para PNG.\n";
        return 1;
    }

    cout << "Processamento concluído! Resultado salvo em: " << saida_png << endl;
    return 0;
}
