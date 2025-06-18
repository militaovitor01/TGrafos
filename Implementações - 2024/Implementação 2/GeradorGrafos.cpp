#include <iostream>
#include <cmath>
#include <vector>

int fatorial(int x) {
    if (x <= 1){
        return 1;
    }else {
        return x * fatorial(x - 1);
    }
}

int totalArestas(int k) {
    int arestas = (k * (k - 1)) / 2;
    return pow(2, arestas);
}

int combinacao(int n, int p){
    int fatParen = fatorial((n - p)); //(n-p)!
    int divisor = fatorial(p) * fatParen; // p!(n-p)!
    return fatorial(n) / divisor; //  n!/p!(n-p)!
}

int totalSubgrafos(int n) {
    int numSubgrafos = 0;
    for (int i = 1; i <= n; i++) {  // Começa com 1 pois nao conta com o grafo vazio 
        int comb = combinacao(n, i);
        int arestas = totalArestas(i);
        numSubgrafos += comb * arestas;
    }

    return numSubgrafos;
}

void gerarSubgrafos(int n) {
    int subgrafoCount = 0;

    // Gera todos os subconjuntos de vértices
    for (int subGrafo = 1; subGrafo < pow(2,n); ++subGrafo) {  // Começa em 1 para ignorar o grafo vazio       
        std::vector<int> vertices;

        //Verifica se o vértice i está no subconjunto atual. (1 << i) desloca o bit 1 para a esquerda i posições, e subGrafo & (1 << i) verifica se o bit correspondente está definido em subGrafo.
        for (int i = 0; i < n; ++i) {
            if (subGrafo & (1 << i)) {//
                vertices.push_back(i);
            }
        }

        // Gera todos os subgrafos possíveis para o subconjunto de vértices atual
        int numVertices = vertices.size();
        int numArestas = totalArestas(numVertices);

        // Tera sobre todas as combinações possíveis de arestas no subconjunto atual.
        for (int arestas = 0; arestas < numArestas; ++arestas){          
            std::cout << "Subgrafo " << ++subgrafoCount << ": Vertices [ ";

            for (int v : vertices) {
                std::cout << (v + 1) << " ";
            } 

            std::cout << "] Arestas [ ";                      
            int arestaCount = 0;
            for (int i = 0; i < numVertices; ++i) {
                for (int j = i + 1; j < numVertices; ++j) {
                    if (arestas & (1 << arestaCount)) {
                        std::cout << "(" << (vertices[i] + 1) << ", " << (vertices[j] + 1) << ") ";
                    }
                    ++arestaCount;
                }
            }
            std::cout << "]" << std::endl;
        }
    }
}

int main() {
    int n;
    std::cout << "Gerador de subgrafos de um Grafo Completo" << std::endl;
    std::cout << "Digite o numero de vertices do grafo: ";
    std::cin >> n;

    gerarSubgrafos(n);
    return 0;
}