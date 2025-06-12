# Efficient Graph-Based Image Segmentation

Este projeto implementa o algoritmo de segmentação de imagens baseado em grafos, conforme descrito no artigo "Efficient Graph-Based Image Segmentation" de Pedro F. Felzenszwalb e Daniel P. Huttenlocher.

## Estrutura do Projeto

O projeto está organizado nas seguintes pastas:

- `include/`: Contém os arquivos de cabeçalho (.h)
- `src/`: Contém os arquivos de implementação (.cpp)
- `data/`: Armazena as imagens de entrada
- `result/`: Armazena as imagens de saída após a segmentação
- `scripts/`: Contém scripts auxiliares

## Componentes Principais

### 1. Union-Find (Disjoint Set)
- **Arquivos**: `include/disjoint_set.h`, `src/disjoint_set.cpp`
- **Descrição**: Implementa a estrutura de dados Union-Find (Disjoint Set) que é fundamental para o algoritmo de segmentação.
- **Funcionalidades**:
  - `find(x)`: Encontra o representante do conjunto
  - `unite(x, y)`: Une dois conjuntos
  - `connected(x, y)`: Verifica se dois elementos estão no mesmo conjunto

### 2. Manipulação de Imagens
- **Arquivos**: `include/image.h`, `src/image.cpp`
- **Descrição**: Classe responsável pela leitura, manipulação e salvamento de imagens no formato PPM.
- **Funcionalidades**:
  - Carregamento de imagens PPM
  - Salvamento de imagens PPM
  - Acesso e modificação de pixels
  - Gerenciamento de dimensões da imagem

### 3. Grafo
- **Arquivos**: `include/graph.h`, `src/graph.cpp`
- **Descrição**: Implementa a estrutura de grafo necessária para a segmentação.
- **Funcionalidades**:
  - Construção do grafo a partir de uma imagem
  - Cálculo de diferenças entre pixels
  - Gerenciamento de arestas
  - Conversão entre coordenadas de pixels e índices lineares

## Como Usar

1. Coloque suas imagens de entrada no formato PPM na pasta `data/`
2. Compile o projeto usando um compilador C++ compatível
3. Execute o programa
4. As imagens segmentadas serão salvas na pasta `result/`

## Requisitos

- Compilador C++ compatível com C++11 ou superior
- Bibliotecas padrão do C++

## Formato de Imagem

O programa trabalha com imagens no formato PPM (Portable Pixmap Format). As imagens de entrada devem estar neste formato.

## Algoritmo

O algoritmo de segmentação implementado segue os seguintes passos:

1. Constrói um grafo onde cada pixel é um vértice
2. Calcula as diferenças entre pixels adjacentes para definir os pesos das arestas
3. Ordena as arestas por peso
4. Aplica o algoritmo de segmentação baseado em grafos
5. Gera a imagem segmentada como resultado

## Contribuição

Sinta-se à vontade para contribuir com o projeto através de pull requests ou reportando issues. 