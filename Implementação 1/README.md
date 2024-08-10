# Documentação

- Este projeto busca implementar o que foi solicitado no exercício "Implementação 1".
- Todos os arquivos de codigo possuem a classe No, (Exceto o arquivo Matriz, que foi feito de forma linear) a classe da sua respectiva estrutura e uma função Main para testes;

# Lista Encadeada em C++
## Classes

### `No`

Representa um nó na lista encadeada.

- **Atributos**:
  - `int conteudo`: Valor armazenado no nó.
  - `No* prox`: Ponteiro para o próximo nó.

- **Construtor**:
  - `No(int conteudo)`: Inicializa o nó com o valor fornecido e define `prox` como `nullptr`.

### `ListaEncadeada`

Representa a lista encadeada e fornece métodos para manipulação.

- **Atributos**:
  - `No* primeiro`: Ponteiro para o primeiro nó.
  - `No* ultimo`: Ponteiro para o último nó.

- **Métodos**:
  - `ListaEncadeada()`: Construtor que inicializa a lista como vazia.
  - `void inserirInicio(int conteudo)`: Insere um novo nó no início da lista.
  - `void inserirFim(int conteudo)`: Insere um novo nó no final da lista.
  - `void inserir(int conteudo, int posicao)`: Insere um novo nó na posição especificada.
  - `int removerInicio()`: Remove e retorna o valor do primeiro nó.
  - `int removerFim()`: Remove e retorna o valor do último nó.
  - `void remover(int posicao)`: Remove o nó na posição especificada.
  - `void mostrar()`: Exibe todos os valores dos nós na lista.
  - `void identificaElemento(int elemento)`: Verifica se um elemento está presente na fila. Imprime uma mensagem indicando se o elemento está ou não na lista. 

# Pilha em C++

## Estrutura da Classe

### `No`

Representa um nó na lista encadeada, utilziada na estruturação da pilha.

- **Atributos**:
  - `int conteudo`: Valor armazenado no nó.
  - `No* prox`: Ponteiro para o próximo nó.

- **Construtor**:
  - `No(int conteudo)`: Inicializa o nó com o valor fornecido e define `prox` como `nullptr`.
  
### `Pilha`

A classe `Pilha` gerencia a pilha e suas operações.

- **Atributos**:
  - `No* Topo`: Ponteiro para o topo da pilha.

- **Construtor**:
  - `Pilha()`: Inicializa a pilha com o topo como `nullptr`.

- **Métodos**:

  - `bool verificaVazio()`: Verifica se a pilha está vazia. Retorna `true` se a pilha estiver vazia e `false` caso contrário.
  - `void empilhar(int conteudo)`: Adiciona um novo elemento ao topo da pilha.
  - `int desempilhar()`: Remove e retorna o elemento do topo da pilha.  
  - `int consultarTopo()`: Retorna o valor do elemento no topo da pilha sem removê-lo.
  - `void identificaElemento(int elemento)`: Verifica se um elemento está presente na fila. Imprime uma mensagem indicando se o elemento está ou não na pilha. 
 
# Fila em C++

## Estrutura da Classe

### `No`

Representa um nó na lista encadeada, utilziada na estruturação da pilha.

- **Atributos**:
  - `int conteudo`: Valor armazenado no nó.
  - `No* prox`: Ponteiro para o próximo nó.

- **Construtor**:
  - `No(int conteudo)`: Inicializa o nó com o valor fornecido e define `prox` como `nullptr`.
  
### `Fila`

A classe `Fila` representa uma fila (FIFO) usando uma lista encadeada. Inclui operações básicas para manipular a fila, como enfileirar, desenfileirar, consultar o primeiro elemento e imprimir a fila.

- **Construtor**

  - `Fila()`: Cria uma nova fila vazia, com `primeiro` e `ultimo` como `nullptr`.

- **Métodos**:

  - `void enfileirar(int conteudo)`: Adiciona um novo elemento ao final da fila. Se a fila estiver vazia, o novo nó será o primeiro e o último elemento da fila.
  - `int desenfileirar()`: Remove e retorna o elemento no início da fila. Lança uma exceção (`std::runtime_error`) se a fila estiver vazia. Atualiza o ponteiro `ultimo` se a fila ficar vazia após a remoção.
  - `int consultarPrimeiro()`: Retorna o valor do primeiro elemento da fila sem removê-lo. Lança uma exceção (`std::runtime_error`) se a fila estiver vazia.
  - `void imprimir()`: Imprime todos os elementos da fila, do início ao fim.
  - `void identificaElemento(int elemento)`: Verifica se um elemento está presente na fila. Imprime uma mensagem indicando se o elemento está ou não na fila. 

# Matriz em C++

### Classe `Matriz`

Representa uma matriz de inteiros 3x3 e fornece métodos para manipulação e consulta dos elementos.

- **Atributos**:

  - **`int matriz[NUM_LIN][NUM_COL]`**: Matriz de inteiros com dimensões definidas por `NUM_LIN` e `NUM_COL` (3x3).

- **Métodos**:

  - `void preencherMatriz()`: Preenche a matriz com valores inseridos pelo usuário. Solicita ao usuário o valor para cada posição da matriz.  
  - `void exibirMatriz() const`: Exibe os valores atuais da matriz na tela. Cada linha da matriz é impressa em uma linha separada.  
  - `void identificaElemento(int elemento)`: Verifica se um elemento está presente na matriz. Imprime uma mensagem indicando se o elemento está ou não presente. 
  - `void removeElemento(int elemento)`: Remove o elemento especificado da matriz substituindo-o por `-1`. Se o elemento não for encontrado, imprime uma mensagem indicando que o elemento não está presente.


