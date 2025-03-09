#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <numeric>

using namespace std;

class Grafo {
private:
    vector<vector<int>> matrizAdj;
    int numVertices;

public:
    Grafo(int numVertices) : numVertices(numVertices) {
        matrizAdj.resize(numVertices, vector<int>(numVertices, 0));
    }

    void adicionarAresta(int i, int j) {
        matrizAdj[i][j] = 1;
        matrizAdj[j][i] = 1;
    }

    vector<vector<int>>& getMatrizAdj() {
        return matrizAdj;
    }
};

class DetectaCiclosCaminhamento {
private:
    set<string> ciclosUnicos;

    void buscarCiclos(int atual, int inicio, vector<int> caminho, vector<bool> visitados, int menorInicio, vector<vector<int>>& matrizAdj) {
        caminho.push_back(atual);
        visitados[atual] = true;

        for (int vizinho = 0; vizinho < matrizAdj.size(); vizinho++) {
            if (matrizAdj[atual][vizinho] == 1) {
                if (vizinho == inicio && caminho.size() > 2) {
                    vector<int> ciclo = caminho;
                    rotate(ciclo.begin(), min_element(ciclo.begin(), ciclo.end()), ciclo.end());
                    string cicloStr = "[";
                    for (int i = 0; i < ciclo.size(); i++) {
                        cicloStr += to_string(ciclo[i]) + (i < ciclo.size() - 1 ? ", " : "");
                    }
                    cicloStr += "]";
                    ciclosUnicos.insert(cicloStr);
                } else if (!visitados[vizinho] && vizinho > menorInicio) {
                    buscarCiclos(vizinho, inicio, caminho, visitados, menorInicio, matrizAdj);
                }
            }
        }
    }

public:
    void encontrarCiclos(Grafo& g) {
        vector<vector<int>>& matrizAdj = g.getMatrizAdj();
        int n = matrizAdj.size();

        for (int i = 0; i < n; i++) {
            vector<bool> visitados(n, false);
            buscarCiclos(i, i, {}, visitados, i, matrizAdj);
        }
        cout << "Total de ciclos encontrados: " << ciclosUnicos.size() / 2 << endl;
    }
};

class DetectaCiclosPermutacao {
private:
    bool PossuiCaminhoEntreVertice(vector<int> aresta, vector<vector<int>>& matrizAdj) {
        if (aresta.size() < 3) return false;
        for (int i = 1; i < aresta.size(); i++) {
            if (matrizAdj[aresta[i - 1]][aresta[i]] != 1) return false;
        }
        return matrizAdj[aresta.back()][aresta[0]] == 1;
    }

    void gerarPermutacoes(int totalVertices, int tamanhoGrupo, int tamanhoAtual, vector<int>& grupoAtual, vector<vector<int>>& totalPermutacoes) {
        if (tamanhoAtual == tamanhoGrupo) {
            totalPermutacoes.push_back(grupoAtual);
            return;
        }
        for (int i = 0; i < totalVertices; i++) {
            if (find(grupoAtual.begin(), grupoAtual.end(), i) == grupoAtual.end()) {
                grupoAtual.push_back(i);
                gerarPermutacoes(totalVertices, tamanhoGrupo, tamanhoAtual + 1, grupoAtual, totalPermutacoes);
                grupoAtual.pop_back();
            }
        }
    }

public:
    void encontrarCiclos(Grafo& g) {
        vector<vector<int>>& matrizAdj = g.getMatrizAdj();
        int totalVertices = matrizAdj.size();
        int totalCiclos = 0;

        for (int tamanhoGrupo = 3; tamanhoGrupo <= totalVertices; tamanhoGrupo++) {
            vector<vector<int>> grupos;
            vector<int> auxiliar;
            gerarPermutacoes(totalVertices, tamanhoGrupo, 0, auxiliar, grupos);

            int ciclosPorTamanho = 0;
            for (auto& grupo : grupos) {
                if (PossuiCaminhoEntreVertice(grupo, matrizAdj)) {
                    ciclosPorTamanho++;
                }
            }

            int ciclosAjustados = ciclosPorTamanho / (tamanhoGrupo * 2);
            totalCiclos += ciclosAjustados;
            cout << "Total de ciclos encontrados para tamanho " << tamanhoGrupo << ": " << ciclosAjustados << endl;
        }
        cout << "Total geral de ciclos encontrados: " << totalCiclos << endl;
    }
};

int main() {
    Grafo grafo(6);
    grafo.adicionarAresta(0, 1);
    grafo.adicionarAresta(0, 3);
    grafo.adicionarAresta(0, 4);
    grafo.adicionarAresta(1, 2);
    grafo.adicionarAresta(1, 3);
    grafo.adicionarAresta(1, 4);
    grafo.adicionarAresta(2, 3);
    grafo.adicionarAresta(2, 4);
    grafo.adicionarAresta(2, 5);
    grafo.adicionarAresta(3, 5);
    grafo.adicionarAresta(4, 5);

    int opcao;
    do {
        cout << "\n=========== MENU ===========" << endl;
        cout << "|  1 - Caminhamento       |" << endl;
        cout << "|  2 - Permutacao         |" << endl;
        cout << "|  0 - Sair               |" << endl;
        cout << "===========================" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        if (opcao == 1) {
            DetectaCiclosCaminhamento detector;
            detector.encontrarCiclos(grafo);
        } else if (opcao == 2) {
            DetectaCiclosPermutacao detector;
            detector.encontrarCiclos(grafo);
        } else if (opcao == 0){
            cout << "Saindo..." << endl;
        } else {
            cout << "Opcao invalida!" << endl;
        }
    } while(opcao != 0);

    return 0;
}
