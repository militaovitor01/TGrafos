#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <sstream>
#include <string>
#include <queue>
#include <algorithm>
#include <numeric>
#include <cmath>

using namespace std;

// Estrutura para representar uma aresta com destino, capacidade e fluxo
struct Edge
{
    int to;           // Nó de destino
    double capacity;  // Capacidade da aresta
    double flow;      // Fluxo atual na aresta
};

// Classe para representar um grafo com capacidades associadas às arestas
class Graph
{
public:
    int n;                           // Número de nós
    vector<vector<Edge>> adjList;    // Lista de adjacências

    Graph(int nodes) : n(nodes), adjList(nodes) {}

    // Adiciona uma aresta entre dois nós com capacidade especificada
    void addEdge(int u, int v, double cap)
    {
        adjList[u].push_back({v, cap, 0});   // Aresta direta
        adjList[v].push_back({u, cap, 0});   // Aresta de retorno (capacidade residual)
    }

    // Realiza uma busca em largura (BFS) para encontrar um caminho aumentante.
    double bfs(int s, int t, vector<int> &parent)
    {
        vector<bool> visited(n, false);
        queue<int> q;
        q.push(s);
        visited[s] = true;
        parent[s] = -1;

        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            for (auto &edge : adjList[u])
            {
                int v = edge.to;
                if (!visited[v] && edge.capacity - edge.flow > 0)  // Verifica capacidade residual
                {
                    parent[v] = u;
                    if (v == t)    // Se alcançou o destino, retorna o fluxo disponível
                        return edge.capacity - edge.flow;
                    q.push(v);
                    visited[v] = true;
                }
            }
        }
        return 0;  // Nenhum caminho aumentante encontrado
    }

    // Implementa o algoritmo Edmonds-Karp para calcular o fluxo máximo.
    double edmondsKarp(int s, int t)
    {
        vector<int> parent(n, -1);
        double maxFlow = 0;

        while (true)
        {
            double flow = bfs(s, t, parent);  // Encontra caminho aumentante
            if (flow == 0)
                break;

            maxFlow += flow;  // Adiciona o fluxo encontrado
            int v = t;

            // Atualiza os fluxos nas arestas do caminho aumentante
            while (v != s)
            {
                int u = parent[v];
                for (auto &edge : adjList[u])
                {
                    if (edge.to == v)
                    {
                        edge.flow += flow;
                        break;
                    }
                }
                for (auto &edge : adjList[v])
                {
                    if (edge.to == u)
                    {
                        edge.flow -= flow;
                        break;
                    }
                }
                v = parent[v];
            }
        }
        return maxFlow;  // Retorna o fluxo máximo calculado
    }

    // Computa os t-links usando análise de histograma
    void computeTLinks(const vector<vector<vector<int>>>& image, 
                       vector<double>& sourceCost, 
                       vector<double>& sinkCost)
    {
        int height = image.size();
        int width = image[0].size();

        // Calcular o histograma de intensidade da imagem
        vector<int> histogram(256, 0);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Calcular intensidade média do pixel
                int intensity = (image[y][x][0] + image[y][x][1] + image[y][x][2]) / 3;
                histogram[intensity]++;
            }
        }

        // Calcular limiar usando método de Otsu
        int total = height * width;
        double sum = 0;
        for (int i = 0; i < 256; i++) {
            sum += i * histogram[i];
        }

        double sumB = 0;
        int wB = 0;
        int wF = 0;
        double varMax = 0;
        int threshold = 0;

        for (int i = 0; i < 256; i++) {
            wB += histogram[i];
            if (wB == 0) continue;
            
            wF = total - wB;
            if (wF == 0) break;

            sumB += i * histogram[i];
            double mB = sumB / wB;
            double mF = (sum - sumB) / wF;

            double var = (double)wB * (double)wF * pow(mB - mF, 2);

            if (var > varMax) {
                varMax = var;
                threshold = i;
            }
        }

        // Computar custos de t-links baseado no limiar
        sourceCost.resize(height * width);
        sinkCost.resize(height * width);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int node = y * width + x;
                int intensity = (image[y][x][0] + image[y][x][1] + image[y][x][2]) / 3;

                // Atribuir custos de acordo com o limiar de Otsu
                if (intensity < threshold) {
                    sourceCost[node] = 1.0;  // Mais propenso a ser objeto
                    sinkCost[node] = 0.1;
                } else {
                    sourceCost[node] = 0.1;
                    sinkCost[node] = 1.0;  // Mais propenso a ser fundo
                }
            }
        }
    }

    // Computa os n-links (arestas entre pixels vizinhos)
    void computeNLinks(const vector<vector<vector<int>>>& image, double sigma)
    {
        int height = image.size();
        int width = image[0].size();

        // Adicionar n-links entre pixels vizinhos
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int node = y * width + x;

                // Conexão horizontal
                if (x < width - 1) {
                    int rightNode = node + 1;
                    double weight = computeEdgeWeight(image[y][x], image[y][x+1], sigma);
                    addEdge(node, rightNode, weight);
                }

                // Conexão vertical
                if (y < height - 1) {
                    int bottomNode = node + width;
                    double weight = computeEdgeWeight(image[y][x], image[y+1][x], sigma);
                    addEdge(node, bottomNode, weight);
                }
            }
        }
    }

    // Função para calcular o peso da aresta entre dois pixels
    double computeEdgeWeight(const vector<int>& pixel1, const vector<int>& pixel2, float sigma)
    {
        double diff = 0.0;
        for (int i = 0; i < 3; i++) {
            diff += pow(pixel1[i] - pixel2[i], 2);
        }
        return exp(-diff / (2 * sigma * sigma));
    }
};

// Função para ler a imagem PPM em formato binário 
bool readPPM(const string &filename, vector<vector<vector<int>>> &image, int &width, int &height)
{
    ifstream file(filename, ios::binary);
    if (!file)
    {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return false;
    }

    string line;
    getline(file, line); // Verifica formato P6
    if (line != "P6")
    {
        cerr << "Formato PPM inválido!" << endl;
        return false;
    }

    file >> width >> height; // Lê as dimensões da imagen
    int maxVal;
    file >> maxVal;
    file.ignore(); // Ignorar o caractere de nova linha após os valores da imagem

    image.resize(height, vector<vector<int>>(width, vector<int>(3)));

    // Agora, ler corretamente os valores dos pixels
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned char pixel[3];                        // Cada pixel tem 3 componentes: R, G, B
            file.read(reinterpret_cast<char *>(pixel), 3); // Lê os 3 bytes (RGB)
            image[y][x][0] = pixel[0];                     // Red
            image[y][x][1] = pixel[1];                     // Green
            image[y][x][2] = pixel[2];                     // Blue
        }
    }

    file.close();
    return true;
}

// Função para salvar a imagem segmentada no formato PPM
void savePPM(const string &filename, const vector<vector<vector<int>>> &image, int width, int height)
{
    ofstream file(filename, ios::binary);
    if (!file)
    {
        cerr << "Erro ao salvar a imagem!" << endl;
        return;
    }

    file << "P6\n"
         << width << " " << height << "\n255\n";
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Verificar se o pixel é foreground (0,0,0) ou background (255,255,255)
            unsigned char r = (image[y][x][0] == 0) ? 0 : 255;
            unsigned char g = (image[y][x][1] == 0) ? 0 : 255;
            unsigned char b = (image[y][x][2] == 0) ? 0 : 255;
            file.write(reinterpret_cast<const char *>(&r), 1);
            file.write(reinterpret_cast<const char *>(&g), 1);
            file.write(reinterpret_cast<const char *>(&b), 1);
        }
    }

    file.close();
}

int main()
{
    string inputImage = "input/input.ppm";   // Imagem original

    // Carregar a imagem
    int width, height;
    vector<vector<vector<int>>> image;

    if (!readPPM(inputImage, image, width, height))
    {
        return -1;
    }

    float sigma = 10.0f; // Controle da métrica de similaridade

    // Criar grafo com nós para cada pixel
    Graph g(width * height * 2 + 2);
    int source = 0, sink = width * height * 2 + 1;

    // Computar os t-links
    vector<double> sourceCost, sinkCost;
    g.computeTLinks(image, sourceCost, sinkCost);

    // Adicionar arestas de source e sink
    for (size_t i = 0; i < sourceCost.size(); ++i) {
        g.addEdge(source, i + 1, sourceCost[i]);
        g.addEdge(i + 1, sink, sinkCost[i]);
    }

    // Computar os n-links
    g.computeNLinks(image, sigma);

    // Executar o algoritmo de max-flow / min-cut
    double maxFlow = g.edmondsKarp(source, sink);

    // Criar a imagem segmentada com base no corte
    vector<vector<vector<int>>> segmentedImage(height, vector<vector<int>>(width, vector<int>(3, 0)));

    // Verificar quais pixels estão no corte (lado da fonte ou do sumidouro)
    vector<bool> visited(g.n, false);
    queue<int> q;
    q.push(source);
    visited[source] = true;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (auto &edge : g.adjList[u])
        {
            if (!visited[edge.to] && edge.capacity - edge.flow > 0)
            {
                visited[edge.to] = true;
                q.push(edge.to);
            }
        }
    }

    // Colorir os pixels de acordo com o lado do corte
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int node = y * width + x;
            if (visited[node + 1]) // Lado da fonte
            {
                segmentedImage[y][x] = {0, 0, 0}; // Preto (foreground)
            }
            else
            {
                segmentedImage[y][x] = {255, 255, 255}; // Branco (background)
            }
        }
    }

    // Salvar a imagem segmentada
    savePPM("output/segmented_image.ppm", segmentedImage, width, height);

    cout << "Imagem segmentada salva com sucesso! Fluxo máximo: " << maxFlow << endl;
    return 0;
}