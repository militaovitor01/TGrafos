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

class ListaEncadeada{
    private:
        No* primeiro;
        No* ultimo;

    public:

        ListaEncadeada (){
            primeiro = nullptr;
            ultimo = nullptr;
        }

        No* getPrimeiro() const {
            return primeiro;
        }

        No* getUltimo() const {
            return ultimo;
        }
    
        void inserirInicio(int conteudo) {
            No* auxiliar = new No(conteudo);
            if (primeiro == nullptr) {
                primeiro = auxiliar;
                ultimo = auxiliar;
            } else {
                auxiliar->prox = primeiro;
                primeiro = auxiliar;
            }
        }


        void inserirFim (int conteudo){
            if (primeiro == nullptr){
                inserirInicio(conteudo);
            } else {
                No *auxiliar = new No(conteudo);
                ultimo->prox = auxiliar;
                ultimo = auxiliar;
            }
        }

        void inserir (int conteudo, int posicao){
            if (posicao < 0) {
                // Posição inválida
                std::cout << "Posição inválida!" << std::endl;
                return;
            }

            if (posicao == 0){
                inserirInicio(conteudo);
            } else {
                No *auxiliar = new No(conteudo);
                No *i = primeiro;

                // Percorre a lista até a posição anterior àquela que se deseja inserir
                for (int j = 0; j < posicao - 1; j++){
                    if (i == nullptr) {
                        std::cout << "Posição fora dos limites da lista!" << std::endl;
                        return;
                    }

                    i = i->prox;
                }

                auxiliar->prox = i->prox;   // Conecta o novo nó ao que vinha depois de "i"
                i->prox = auxiliar;     // Insere "auxiliar" na lista ao fazer com que "i" aponte para ele
                auxiliar = i = nullptr;
            }
        }

        int removerInicio() {
            if (primeiro == nullptr) {
                throw std::runtime_error("Erro ao remover (lista vazia)!");
            }
        
            No* auxiliar = primeiro;
            int elementoRetornado = auxiliar->conteudo;
            primeiro = primeiro->prox;         
            delete auxiliar; 
         
            return elementoRetornado; 
        }

        int removerFim() {
            if (primeiro == nullptr){
                throw std::runtime_error("Erro ao remover (lista vazia)!");
            }

            No* i;
            for (i = primeiro; i->prox != ultimo; i = i->prox);
            int elementoRetornado = ultimo->conteudo;
            ultimo = i; // O ponteiro "ultimo" passa a apontar para o penúltimo auxiliar
            i = ultimo->prox = nullptr; // O ponteiro "prox", passa a apontar para nulo, indicando que agora o auxiliar em questão é o novo último 

            return elementoRetornado;
        }

        void remover (int posicao){
            if (posicao < 0) {
                // Posição inválida
                std::cout << "Posição inválida!" << std::endl;
                return;
            }

            if (posicao == 0){
                removerInicio();
            } else {
                No *i = primeiro;
                // Percorre a lista até a posição anterior àquela que se deseja remover
                for (int j = 0; j < posicao - 1; j++){
                    if (i == nullptr) {
                        std::cout << "Posição fora dos limites da lista!" << std::endl;
                        return;
                    }

                    i = i->prox;
                }

                No *auxiliar = i->prox;

                if (auxiliar == nullptr) {
                    std::cout << "Posição fora dos limites da lista!" << std::endl;
                    return;
                }

                // Conecta o nó anterior ao que vem depois do que se deseja remover
                i->prox = auxiliar->prox;
                delete auxiliar;
                auxiliar = nullptr;
            }
        }

        void mostrar() {
            No* auxiliar = primeiro;

            while (auxiliar != nullptr) {
                cout << auxiliar->conteudo << " ";
                auxiliar = auxiliar->prox;
            }

            cout << endl;
        }

        void identificaElemento(int elemento) {
            No* atual = primeiro;

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
    ListaEncadeada lista;
    int N, elemento, elementoIncluido, K;

    // Realizar a inclusao de N elementos da Estrutura
    cout << "Quantos elementos deseja incluir?" << endl;
    cin >> N;
    
    cout << "Quais elementos deseja incluir?" << endl;
    for (int i = 0; i < N; i++){
        cin >> elementoIncluido;
        if (lista.getPrimeiro() == nullptr){
            lista.inserirInicio(elementoIncluido);
        }else{
            lista.inserir(elementoIncluido,i);
        }
    }

    // Identificar se um elemento específico consta na Estrutura
    cout << "Qual elemento deseja buscar?" << endl;
    cin >> elemento;
    lista.identificaElemento(elemento);
    
    // Remover k elementos da Estrutura
    cout << "Quantos elementos deseja remover?" << endl;
    cin >> K;

    int elementosRemovidos = 0;
    for (int i = 0; i < K; i++) {
        try {
            lista.removerInicio();
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
    lista.mostrar();

    return 0;
}