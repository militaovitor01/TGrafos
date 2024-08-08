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
            ultimo = primeiro;
        }
    
        void inserirInicio (int conteudo){
            No *elemento = new No(conteudo);     
            elemento->prox = primeiro->prox;
            primeiro = elemento;

            if (primeiro == ultimo){    // Verifica se a lista está vazia
                ultimo = elemento;
            }

            elemento = nullptr;
        }

        void inserirFim (int conteudo){
            if (primeiro == ultimo){
                inserirInicio(conteudo);
            } else {
                No *elemento = new No(conteudo);
                ultimo->prox = elemento;
                ultimo = elemento;
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
                No *elemento = new No(conteudo);
                No *i = primeiro;

                // Percorre a lista até a posição anterior àquela que se deseja inserir
                for (int j = 0; j < posicao - 1; j++){
                    if (i == nullptr) {
                        std::cout << "Posição fora dos limites da lista!" << std::endl;
                        return;
                    }

                    i = i->prox;
                }

                elemento->prox = i->prox;   // Conecta o novo nó ao que vinha depois de "i"
                i->prox = elemento;     // Insere "elemento" na lista ao fazer com que "i" aponte para ele
                elemento = i = nullptr;
            }
        }

        int removerInicio() {
            if (primeiro == ultimo) {
                throw std::runtime_error("Erro ao remover (lista vazia)!");
            }
        
            No* auxiliar = primeiro;
            int elementoRetornado = auxiliar->conteudo;
            primeiro = primeiro->prox;         
            delete auxiliar; 
         
            return elementoRetornado; 
        }

        int removerFim() {
            if (primeiro == ultimo){
                throw std::runtime_error("Erro ao remover (lista vazia)!");
            }

            No* i;
            for (i = primeiro; i->prox != ultimo; i = i->prox);
            int elementoRetornado = ultimo->conteudo;
            ultimo = i; // O ponteiro "ultimo" passa a apontar para o penúltimo elemento
            i = ultimo->prox = nullptr; // O ponteiro "prox", passa a apontar para nulo, indicando que agora o elemento em questão é o novo último 

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

        void mostrar (){
            No *elemento = primeiro;

            while (elemento != nullptr){
                cout << elemento->conteudo << " ";
                elemento = elemento->prox;
            }

            cout << ultimo->conteudo << endl;
        }
};

int main (){

    return 0;
}