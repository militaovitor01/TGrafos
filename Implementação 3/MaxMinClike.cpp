#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <climits>
#include <string>
using namespace std;

// Função Dijkstra para distâncias mínimas
unordered_map<string, int> dijkstra(const unordered_map<string, unordered_map<string, int>>& graph, const string& source) {
    unordered_map<string, int> dist;

    // Inicializa as distâncias com infinito
    for (const auto& node : graph) {
        dist[node.first] = INT_MAX;
    }
    dist[source] = 0;

    // Fila de prioridade para processar os vértices
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [current_dist, current_node] = pq.top();
        pq.pop();

        // Se a distância atual for maior do que a já registrada, continue
        if (current_dist > dist[current_node]) {
            continue;
        }

        // Verifica os vizinhos do nó atual
        for (const auto& neighbor : graph.at(current_node)) {
            int distance = neighbor.second;

            // Relaxamento: Se encontrar um caminho mais curto, atualize a distância
            if (distance < dist[neighbor.first]) {
                dist[neighbor.first] = distance;
                pq.push({distance, neighbor.first});
            }
        }
    }

    return dist;
}

// Função Dijkstra para distâncias máximas
unordered_map<string, int> dijkstra_min(const unordered_map<string, unordered_map<string, int>>& grafo, const string& origem) {
    unordered_map<string, int> distancia;

    // Inicializa as distâncias com -infinito
    for (const auto& node : grafo) {
        distancia[node.first] = INT_MIN;
    }
    distancia[origem] = 0;

    // Fila de prioridade para processar os vértices
    priority_queue<pair<int, string>> pq;
    pq.push({0, origem});

    while (!pq.empty()) {
        auto [dist_atual, V_atual] = pq.top();
        pq.pop();

        // Se a distância atual for maior do que a já registrada, continue
        if (dist_atual > distancia[V_atual]) {
            continue;
        }

        // Verifica os vizinhos do nó atual
        for (const auto& vizinho : grafo.at(V_atual)) {
            int dist_alternativa = vizinho.second;

            // Se encontrar um caminho mais curto, atualize a distância
            if (dist_alternativa > distancia[vizinho.first]) {
                distancia[vizinho.first] = dist_alternativa;
                pq.push({dist_alternativa, vizinho.first});
            }
        }
    }

    return distancia;
}

int main() {
    // Grafo direcionado representado como um unordered_map de unordered_maps
    unordered_map<string, unordered_map<string, int>> graph = {
        {"A", {{"B", 5}, {"C", 4}}},
        {"B", {{"D", 9}}},
        {"C", {{"B", 7}, {"D", 2}}},
        {"D", {}}
    };

    string source = "A";
    auto dist_min = dijkstra(graph, source);

    // Imprime as distâncias mínimas a partir do vértice fonte
    cout << "Distâncias MÍNIMAS a partir do vértice " << source << ":\n";
    for (const auto& [node, distance] : dist_min) {
        cout << "Vértice " << node << " - Distância: " << distance << endl;
    }

    // Grafo para o cálculo de distâncias máximas
    unordered_map<string, unordered_map<string, int>> grafo = {
        {"A", {{"B", 5}, {"C", 4}}},
        {"B", {{"D", 9}}},
        {"C", {{"B", 7}, {"D", 2}}},
        {"D", {}}
    };

    string fonte = "A";
    auto dist_max = dijkstra_min(grafo, fonte);

    // Imprime as distâncias máximas a partir do vértice fonte
    cout << "Distâncias MÁXIMAS a partir do vértice " << fonte << ":\n";
    for (const auto& [num, distancia] : dist_max) {
        cout << "Vértice " << num << " - Distância: " << distancia << endl;
    }

    return 0;
}
