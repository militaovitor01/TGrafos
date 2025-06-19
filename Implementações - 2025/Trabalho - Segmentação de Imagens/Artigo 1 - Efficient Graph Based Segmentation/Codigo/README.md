# Segmentação de Imagens por Grafo - Efficient Graph Based Segmentation

## 📋 Descrição

Este projeto implementa um algoritmo de segmentação de imagens baseado em **Árvore Geradora Mínima (AGM)** em grafos, seguindo a abordagem proposta por Felzenszwalb e Huttenlocher. O algoritmo utiliza uma variação gulosa do algoritmo de Kruskal para agrupar pixels similares em segmentos homogêneos.

## 🎯 Funcionalidades

- **Segmentação automática** de imagens PNG
- **Filtro de suavização gaussiano** para redução de ruído
- **Algoritmo guloso** baseado em AGM para agrupamento de pixels
- **Threshold adaptativo** que se ajusta ao tamanho dos componentes
- **Pós-processamento** para eliminar segmentos muito pequenos
- **Conversão automática** entre formatos PNG e PPM

## 🏗️ Arquitetura do Projeto

```
Codigo/
│
├── 📁 data/                    # Imagens de entrada
│   ├── bigTree.png
│   ├── homer.png
│   └── horse.png
│
├── 📁 result/                  # Imagens segmentadas de saída
│   ├── bigTree.png
│   ├── homer.png
│   └── horse.png
│
├── 📁 include/                 # Arquivos de cabeçalho (.h)
│   ├── image.h                 # Estruturas e funções de imagem
│   ├── converter.h             # Conversão PNG ↔ PPM
│   ├── segmenter.h             # Função principal de segmentação
│   ├── filter.h                # Filtros de suavização
│   └── segment_graph.h         # Algoritmo AGM e Union-Find
│
├── 📁 src/                     # Implementações (.cpp)
│   ├── image.cpp               # Leitura/escrita PPM, cores aleatórias
│   ├── converter.cpp           # Chamadas para scripts Python
│   ├── segmenter.cpp           # Pipeline de segmentação
│   ├── filter.cpp              # Filtro gaussiano e cálculo de diferenças
│   └── segment_graph.cpp       # Implementação do AGM e Union-Find
│
├── 📁 scripts/                 # Scripts Python para conversão
│   ├── pngToPpm.py             # Converte PNG para PPM
│   └── ppmToPng.py             # Converte PPM para PNG
│
├── main.cpp                    # Fluxo principal de execução
├── main.exe                    # Executável compilado
└── README.md                   # Esta documentação
```

## 📄 Descrição dos Arquivos

### **Headers (include/)**

#### `image.h`
- **Estruturas:** `RGB`, `Image`
- **Funções:** `random_rgb()`, `read_ppm()`, `write_ppm()`
- **Propósito:** Manipulação de imagens e cores

#### `converter.h`
- **Funções:** `convert_png_to_ppm()`, `convert_ppm_to_png()`
- **Propósito:** Interface com scripts Python para conversão de formato

#### `segmenter.h`
- **Função:** `segment()`
- **Propósito:** Função principal que orquestra todo o processo de segmentação

#### `filter.h`
- **Namespace:** `Filter`
- **Funções:** `smooth()`, `diff()`
- **Propósito:** Filtro gaussiano e cálculo de diferenças entre pixels

#### `segment_graph.h`
- **Estruturas:** `Edge`, `Universe`
- **Função:** `segment_graph()`
- **Propósito:** Algoritmo AGM e estrutura Union-Find

### **Implementações (src/)**

#### `image.cpp`
- Implementa leitura/escrita de arquivos PPM
- Gera cores aleatórias para visualização dos segmentos
- Gerencia estruturas de dados de imagem

#### `converter.cpp`
- Executa scripts Python via `system()`
- Gerencia conversões entre PNG e PPM
- Trata erros de conversão

#### `segmenter.cpp`
- **Pipeline principal:**
  1. Separa canais RGB
  2. Aplica filtro de suavização
  3. Constrói grafo de pixels
  4. Executa segmentação por AGM
  5. Aplica pós-processamento
  6. Gera imagem de saída

#### `filter.cpp`
- **Filtro Gaussiano:** Suaviza a imagem para reduzir ruído
- **Cálculo de Diferenças:** Computa distância euclidiana entre cores de pixels
- **Máscara Adaptativa:** Tamanho baseado no parâmetro sigma

#### `segment_graph.cpp`
- **Algoritmo AGM:** Ordena arestas e processa guloso
- **Union-Find:** Estrutura eficiente para gerenciar componentes
- **Threshold Adaptativo:** `threshold = c / tamanho_componente`

### **Scripts Python (scripts/)**

#### `pngToPpm.py`
- Converte imagens PNG para formato PPM (P6)
- Usa biblioteca Pillow (PIL)
- Preserva qualidade da imagem

#### `ppmToPng.py`
- Converte imagens PPM de volta para PNG
- Mantém a qualidade da segmentação
- Gera arquivo final visualizável

## 🚀 Como Executar

### **Pré-requisitos**

1. **Compilador C++** (g++)
2. **Python 3.x**
3. **Biblioteca Pillow:**
   ```bash
   pip install pillow
   ```

### **Compilação**

```bash
g++ -o main.exe main.cpp src/image.cpp src/converter.cpp src/segmenter.cpp src/filter.cpp src/segment_graph.cpp -std=c++11
```

### **Execução**

```bash
.\main.exe
```

### Execução por Linha de Comando e Medição de Tempo

O programa aceita argumentos para facilitar testes com diferentes imagens e parâmetros, além de exibir um relatório detalhado de tempo de execução.

#### Uso:
```bash
.\main.exe [imagem] [sigma] [k] [min]
```
- **imagem**: nome do arquivo PNG em `data/` (ex: `tower.png`)
- **sigma**: parâmetro de suavização (ex: `0.5`)
- **k**: threshold de segmentação (ex: `500`)
- **min**: tamanho mínimo do segmento (ex: `50`)

#### Exemplos:
```bash
.\main.exe                    # Usa valores padrão (horse.png, 0.5, 500, 50)
.\main.exe tower.png          # Processa tower.png com parâmetros padrão
.\main.exe homer.png 1.0 300 100  # Processa homer.png com parâmetros customizados
```

#### Saída de tempo de execução:
O programa exibe ao final um resumo como este:
```
=== Tempos de Execução ===
• Conversão PNG→PPM: 155 ms
• Leitura da imagem:  8 ms
• Segmentação:        924 ms
• Conversão PPM→PNG: 131 ms
• Tempo total:        1.224 segundos

=== Distribuição do Tempo ===
• Conversão PNG→PPM: 12.7%
• Leitura da imagem:  0.7%
• Segmentação:        75.5%
• Conversão PPM→PNG: 10.7%

=== Performance ===
• Pixels processados: 228933
• Tempo por pixel:    0.005 ms/pixel
• Pixels por segundo: 187037 pixels/s
```

### **Configuração**

Para alterar a imagem de entrada, edite `main.cpp`:

```cpp
const std::string input_file = "sua_imagem.png";  // Coloque sua imagem em data/
```

## ⚙️ Parâmetros do Algoritmo

### **Parâmetros Principais (main.cpp)**

```cpp
float sigma = 0.5;  // Suavização gaussiana (0.1 - 2.0)
float k = 500;      // Threshold de segmentação (100 - 1000)
int min = 50;       // Tamanho mínimo do segmento (10 - 100)
```

### **Efeito dos Parâmetros**

| **Parâmetro** | **Valor Baixo** | **Valor Alto** |
|---------------|-----------------|----------------|
| `sigma` | Menos suavização, mais detalhes | Mais suavização, menos ruído |
| `k` | Mais segmentos pequenos | Menos segmentos grandes |
| `min` | Mantém segmentos pequenos | Remove segmentos pequenos |

## 🔬 Algoritmo Detalhado

### **1. Pré-processamento**
- **Suavização Gaussiana:** Reduz ruído preservando bordas
- **Separação de Canais:** RGB processado independentemente

### **2. Construção do Grafo**
- **Vértices:** Cada pixel da imagem
- **Arestas:** Conexões entre pixels vizinhos (8-vizinhança)
- **Pesos:** Distância euclidiana entre cores RGB

### **3. Algoritmo AGM**
```cpp
// Ordena arestas por peso crescente
std::sort(edges.begin(), edges.end());

// Processa guloso
for (const auto& edge : edges) {
    if (edge.w <= threshold) {
        unir_componentes(edge.a, edge.b);
    }
}
```

### **4. Threshold Adaptativo**
- **Fórmula:** `threshold = c / tamanho_componente`
- **Vantagem:** Componentes grandes são mais restritivos
- **Resultado:** Segmentos internos homogêneos

### **5. Pós-processamento**
- **Eliminação:** Segmentos menores que `min_size`
- **União:** Segmentos pequenos com vizinhos mais similares

## 📊 Complexidade

- **Tempo:** O(n log n) onde n = pixels da imagem
- **Espaço:** O(n) para armazenar grafo e estruturas
- **Eficiência:** Union-Find com path compression

## 🎨 Resultados

### **Entrada vs Saída**
- **Entrada:** Imagem PNG colorida
- **Saída:** Imagem PNG com segmentos coloridos aleatoriamente
- **Cada segmento:** Região homogênea com mesma cor

### **Exemplos de Uso**
- **Segmentação de objetos:** Identificar regiões similares
- **Redução de ruído:** Agrupar pixels similares
- **Análise de imagem:** Estudo de texturas e padrões

## 🛠️ Manutenção e Extensões

### **Adicionar Novas Imagens**
1. Coloque a imagem em `data/`
2. Altere `input_file` em `main.cpp`
3. Recompile e execute

### **Modificar Parâmetros**
- Ajuste `sigma`, `k`, `min` em `main.cpp`
- Experimente diferentes valores para otimizar resultados

### **Extensões Possíveis**
- **Interface gráfica:** Para ajuste interativo de parâmetros
- **Múltiplas imagens:** Processamento em lote
- **Métricas de qualidade:** Avaliação automática da segmentação
- **Outros filtros:** Median, bilateral, etc.

## 🐛 Solução de Problemas

### **Erro de Compilação**
```bash
# Verificar se todos os arquivos estão presentes
ls include/ src/ scripts/
```

### **Erro de Execução Python**
```bash
# Verificar instalação do Pillow
python -c "import PIL; print('Pillow OK')"
```

### **Imagem não encontrada**
- Verificar se a imagem existe em `data/`
- Verificar permissões de leitura

### **Resultado insatisfatório**
- Ajustar parâmetros `sigma`, `k`, `min`
- Verificar qualidade da imagem de entrada

## 📚 Referências

- **Felzenszwalb, P. F., & Huttenlocher, D. P.** (2004). Efficient graph-based image segmentation. *International Journal of Computer Vision*, 59(2), 167-181.

## 👥 Autores
- Vitor Militão
- Vitor Alexandre
- Diego Polanski
- Projeto desenvolvido para o trabalho dinal da disciplina de Teoria dos Grafos e Computabilidade.

---

**Versão:** 1.0  
**Data:** 2025  
**Licença:** Educacional 
