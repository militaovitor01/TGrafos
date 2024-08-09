#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

class No {
    public:
        int conteudo;
        No* prox;
    
        No (int conteudo) {
            this->conteudo = conteudo;
            this->prox = nullptr;
        }

        No () {
            this->conteudo = conteudo;
            this->prox = nullptr;
        }
};

class Fila {
    private:
        No* primeiro;
        No* ultimo;

    public:

        Fila() {
            primeiro = nullptr;
            ultimo = nullptr;
        }

        void enfileirar(int conteudo) {
            No* novoNo = new No(conteudo);

            if (primeiro == nullptr) {
                // Se a fila estiver vazia, tanto primeiro quanto ultimo devem apontar para o novo nó
                primeiro = novoNo;
                ultimo = novoNo;
            } else {
                ultimo->prox = novoNo;
                ultimo = novoNo;
            }
        }

        int desenfileirar (){
            if (primeiro == ultimo){
                throw std::runtime_error("Fila vazia!");
            } else {
                No* auxiliar = primeiro;
                int conteudo = primeiro->conteudo;
                primeiro = primeiro->prox;

                // Se a fila ficar vazia após o desenfileiramento, atualize o ponteiro "ultimo" também
                if (primeiro == nullptr) {  
                    ultimo = nullptr;       
                }

                delete auxiliar;
                return conteudo;
            }
        }

        int consultarPrimeiro (){
            if (primeiro == nullptr){
                throw std::runtime_error("Fila vazia!");
            } else {
                return primeiro->conteudo;
            }
        }

        void imprimir() {
            No* atual = primeiro;

            while (atual != nullptr) {
                cout << atual->conteudo << " ";
                atual = atual->prox;
            }

            cout << endl;
        }

        void identificaElemento(int elemento) {
            No* atual = primeiro;

            while (atual != nullptr) {
                if (atual->conteudo == elemento){
                    cout << "O elemento esta presente na estrutura" << endl;
                }else{
                    cout << "O elemento nao esta presente na estrutura" << endl;
                }
            }

            cout << endl;
        }
};

int main() {   
    Fila fila;
    int N, elemento, K;

    // Realizar a inclusao de N elementos da Estruura
    cout << "Quantos elementos deseja incluir?" << endl;
    cin >> N;
    
    for (int i = 0; i < N; i++){
        fila.enfileirar(i);
    }

    // Identificar se um elemento específico consta na Estrutura
    cout << "Qual elemento deseja buscar?" << endl;
    cin >> elemento;
    fila.identificaElemento(elemento);
    
    // Remover k elementos da Estrutura
    cout << "Quantos elementos deseja remover?" << endl;
    cin >> K;

    int elementosRemovidos = 0;
    for (int i = 0; i < K; i++) {
        try {
            fila.desenfileirar();
            elementosRemovidos++;
        } catch (const runtime_error& e) {
            cerr << "Erro ao remover elemento: " << e.what() << endl;
            break; // Interrompe o loop se a fila estiver vazia
        }
    }

    if (elementosRemovidos < K) {
        cout << "Foi possível remover apenas " << elementosRemovidos << " elementos." << endl;
    }

    return 0;
}
