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
};

int main (){

    return 0;
}