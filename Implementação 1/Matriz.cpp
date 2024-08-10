#include <iostream>
#include <string>
#include <stdexcept>
#define NUM_LIN 3
#define NUM_COL 3
using namespace std;

class Matriz{
    private:
        int matriz [NUM_LIN][NUM_COL];

    public:

        void preencherMatriz() {
            int valor;
            for (int i = 0; i < NUM_LIN; i++) {
                for (int j = 0; j < NUM_COL; j++) {
                    cout << "Insira o valor para a posicao (" << i << ", " << j << "): ";
                    cin >> valor;
                    matriz[i][j] = valor;  
                }
            }
        }

        void exibirMatriz() const {
            cout << "\nMatriz inserida:\n";
            for (int i = 0; i < NUM_LIN; i++) {
                for (int j = 0; j < NUM_COL; j++) {
                    cout << matriz[i][j] << " ";  
                }
                cout << endl;
            }
        }

        void identificaElemento (int elemento){
            for(int i = 0; i < NUM_LIN; i++){
                for (int j = 0; j < NUM_COL; j++){
                   if(elemento == matriz[i][j]){
                        cout << "Elemento Presente na Matriz!" << endl;
                        return;
                   }
                }               
            }

            cout << "Elemento nao Presente na Matriz!" << endl;
        }

        void removeElemento (int elemento){
            bool encontrado = false;
            for (int i = 0; i < NUM_LIN; i++) {
                for (int j = 0; j < NUM_COL; j++) {
                    if (elemento == matriz[i][j]) {
                        matriz[i][j] = -1;  // Substituindo o valor pelo valor especial
                        cout << "Elemento Deletado com sucesso!" << endl;
                        encontrado = true;
                        break;
                    }
                }
                if (encontrado) break;
            }

            if (!encontrado) {
                cout << "Elemento nao Presente na Matriz!" << endl;
            }
        }
};

int main (){
    Matriz matriz;
    int elemento, K;

    matriz.preencherMatriz();
    matriz.exibirMatriz();

    cout << "Qual valor deseja buscar?" << endl;
    cin >> elemento;
    matriz.identificaElemento(elemento);

    cout << "Quantos valores deseja remover?" << endl;
    cin >> K;

    // Removendo k itens da estrutura
    for (int i = 0; i < K; i++) {
        cout << "Insira o valor do " << (i + 1) << " item a ser removido: ";
        cin >> elemento;
        matriz.removeElemento(elemento);
    }

    matriz.exibirMatriz();

    return 0;
}
