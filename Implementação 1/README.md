# Documentação

- Este projeto busca implementar o que foi solicitado no exercício "Implementação 1".
- Todos os arquivos de codigo possuem a classe No, a classe da sua respectiva estrutura e uma função Main para testes;

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

# Pilha em C++

A classe `Pilha` representa uma pilha de inteiros utilizando uma lista encadeada dinâmica. A pilha suporta operações básicas como empilhar (adicionar) um elemento, desempilhar (remover) um elemento e consultar o elemento no topo da pilha.

## Estrutura da Classe

### `No`

Representa um nó na lista encadeada, utilziada na estruturação da pilha.

- **Atributos**:
  - `int conteudo`: Valor armazenado no nó.
  - `No* prox`: Ponteiro para o próximo nó.

- **Construtor**:
  - `No(int conteudo)`: Inicializa o nó com o valor fornecido e define `prox` como `nullptr`.
  
### 'Pilha'

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
 

