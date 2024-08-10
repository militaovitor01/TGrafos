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
};

class Pilha {

    private:
        No* Topo;
    
    public:

        Pilha (){
            Topo = nullptr;
        }

        bool verificaVazio (){
            if (Topo == nullptr){
                return true;
            } else {
                return false;
            }
        }

        void empilhar (int conteudo){
            No* auxiliar = new No(conteudo);
            auxiliar->prox = Topo;
            Topo = auxiliar;
            auxiliar = nullptr;
        }

        int desempilhar (){
            if (verificaVazio() == true){
                throw std::runtime_error("Pilha vazia!");
            } else {
                No* auxiliar = Topo;
                Topo = Topo->prox;
                int conteudo = auxiliar->conteudo;
                delete auxiliar;
                return conteudo;
            }  
        }

        int consultarTopo (){
            if (verificaVazio() == true){
                throw std::runtime_error("Pilha vazia!");
            } else {
                return Topo->conteudo;
            }
        }

        void imprimir (){
            if (verificaVazio() == true){
                cout << "Pilha vazia!" << endl;
            } else {
                No* auxiliar = Topo;
                while (auxiliar != nullptr){
                    cout << auxiliar->conteudo << " ";
                    auxiliar = auxiliar->prox;
                }
                cout << endl;
            }
        }

        void identificaElemento(int elemento) {
            No* atual = Topo;

            bool encontrado = false;

            while (atual != nullptr) {
                if (atual->conteudo == elemento) {
                    encontrado = true;
                    break; // Sai do loop assim que o elemento for encontrado
                }
                atual = atual->prox; // Atualiza o ponteiro para o próximo nó
            }

            if (encontrado) {
                cout << "O elemento esta presente na estrutura" << endl;
            } else {
                cout << "O elemento nao esta presente na estrutura" << endl;
            }

            cout << endl;
        }
};

int main() {   
    Pilha pilha;
    int N, elemento, elementoIncluido, K;

    // Realizar a inclusao de N elementos da Estrutura
    cout << "Quantos elementos deseja incluir?" << endl;
    cin >> N;
    
    cout << "Quais elementos deseja incluir?" << endl;
    for (int i = 0; i < N; i++){
        cin >> elementoIncluido;
        pilha.empilhar(elementoIncluido);
    }

    // Identificar se um elemento específico consta na Estrutura
    cout << "Qual elemento deseja buscar?" << endl;
    cin >> elemento;
    pilha.identificaElemento(elemento);
    
    // Remover k elementos da Estrutura
    cout << "Quantos elementos deseja remover?" << endl;
    cin >> K;

    int elementosRemovidos = 0;
    for (int i = 0; i < K; i++) {
        try {
            pilha.desempilhar();
            elementosRemovidos++;
        } catch (const runtime_error& e) {
            cerr << "Erro ao remover elemento: " << e.what() << endl;
            break; // Interrompe o loop se a pilha estiver vazia
        }
    }

    if (elementosRemovidos < K) {
        cout << "Foi possível remover apenas " << elementosRemovidos << " elementos." << endl;
    }

    cout << "O resultado apos remocao e: " << endl;
    pilha.imprimir();

    return 0;
}
