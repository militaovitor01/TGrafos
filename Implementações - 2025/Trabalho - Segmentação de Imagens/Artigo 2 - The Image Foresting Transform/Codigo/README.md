# Segmentação de Imagens usando IFT-Watershed

Este projeto implementa um algoritmo de segmentação de imagens baseado no **Image Foresting Transform (IFT)** combinado com a técnica **Watershed**. O algoritmo é otimizado para segmentar imagens naturais, especialmente paisagens com árvores e fundos complexos.

## 📋 Índice

- [Visão Geral](#visão-geral)
- [Estrutura do Projeto](#estrutura-do-projeto)
- [Requisitos](#requisitos)
- [Instalação](#instalação)
- [Como Usar](#como-usar)
- [Arquivos e Métodos](#arquivos-e-métodos)
- [Algoritmo](#algoritmo)
- [Exemplos](#exemplos)
- [Troubleshooting](#troubleshooting)

## 🎯 Visão Geral

O projeto implementa uma técnica avançada de segmentação de imagens que combina:

- **Image Foresting Transform (IFT)**: Algoritmo baseado em grafos para encontrar caminhos ótimos
- **Watershed**: Técnica de segmentação baseada em bacias hidrográficas
- **Filtros Gaussianos**: Para redução de ruído e melhor detecção de bordas
- **Pós-processamento**: Suavização e eliminação de fragmentação

O algoritmo é especialmente eficaz para segmentar imagens naturais, separando objetos principais (como árvores) do fundo (céu, grama, etc.).

## 📁 Estrutura do Projeto

```
Codigo/
├── tp3.cpp                 # Implementação principal do algoritmo
├── scripts/
│   ├── png_to_ppm.py      # Conversor PNG → PPM
│   └── ppm_to_png.py      # Conversor PPM → PNG
├── imagens/               # Imagens de entrada (PNG)
│   ├── arvore.png
│   ├── cavalo.png
│   ├── homer.png
│   ├── monalisa.png
│   └── vangogh.png
├── PPM/                   # Arquivos PPM intermediários
├── resultado/             # Imagens de saída segmentadas
└── output/                # Arquivos de saída compilados
```

## ⚙️ Requisitos

### Software Necessário
- **Compilador C++**: GCC, Clang ou MSVC
- **Python 3.x**: Para scripts de conversão
- **Pillow (PIL)**: Biblioteca Python para processamento de imagens

### Dependências Python
```bash
pip install Pillow
```

## 🚀 Instalação

1. **Clone ou baixe o projeto**
2. **Instale as dependências Python**:
   ```bash
   pip install Pillow
   ```
3. **Compile o programa principal**:
   ```bash
   g++ -o tp3.exe tp3.cpp -O2
   ```

## 📖 Como Usar

### Execução Básica

1. **Coloque sua imagem PNG** na pasta `imagens/`
2. **Modifique o arquivo** `tp3.cpp` na linha 280 para usar sua imagem:
   ```cpp
   string entrada_png = "imagens/sua_imagem.png";
   ```
3. **Execute o programa**:
   ```bash
   ./tp3.exe
   ```
4. **Resultado**: A imagem segmentada será salva em `resultado/sua_imagemOut.png`

### Exemplo de Uso

```bash
# Compilar
g++ -o tp3.exe tp3.cpp -O2

# Executar (processa vangogh.png por padrão)
./tp3.exe

# Verificar resultado
ls resultado/
# Saída: vangoghOut.png
```

## 📚 Arquivos e Métodos

### `tp3.cpp` - Arquivo Principal

#### Estruturas de Dados

```cpp
struct QueueNode {
    int cost, x, y;
    // Nó para fila de prioridade do algoritmo IFT
}

struct Pixel {
    int x, y, intensity;
    // Representa um pixel com coordenadas e intensidade
}

struct NodeInfo {
    int cost;
    Pixel predecessor;
    bool visited;
    int seedId;
    // Informações de cada nó durante o processamento
}
```

#### Funções Principais

##### `carregarImagemPPM(const string& caminho)`
- **Função**: Carrega uma imagem PPM e converte para estruturas internas
- **Parâmetros**: Caminho do arquivo PPM
- **Processo**:
  1. Abre arquivo PPM binário (formato P6)
  2. Lê dimensões e valores RGB
  3. Converte para escala de cinza
  4. Inicializa estruturas de dados

##### `calcularGradienteMelhorado()`
- **Função**: Calcula gradientes usando filtro Gaussiano para reduzir ruído
- **Processo**:
  1. Aplica suavização Gaussiana 5x5
  2. Calcula gradientes nas direções X e Y
  3. Computa magnitude do gradiente
- **Vantagem**: Reduz ruído e melhora detecção de bordas

##### `inicializarSementesOtimizadas()`
- **Função**: Define sementes estratégicas para segmentação
- **Estratégia**:
  - **Árvore** (sementes 0-4): Distribuídas na copa e tronco
  - **Fundo** (sementes 5-8): Posicionadas nos cantos (céu/grama)
- **Retorna**: Vector com pixels das sementes

##### `executarIFTOtimizado(const vector<Pixel>& sementes, int largura, int altura)`
- **Função**: Implementa o algoritmo IFT-Watershed
- **Algoritmo**:
  1. Inicializa fila de prioridade com sementes
  2. Processa pixels em ordem de custo crescente
  3. Usa função de custo híbrida (gradiente + diferença de cor)
  4. Propaga labels das sementes

##### `suavizarSegmentacaoAgressiva(int largura, int altura)`
- **Função**: Pós-processamento para eliminar fragmentação
- **Processo**:
  1. Filtro de mediana 5x5
  2. Voto majoritário 3x3
- **Resultado**: Segmentação mais limpa e coesa

##### `corMelhorada(int seedId, int r, int g, int b)`
- **Função**: Define cores para visualização da segmentação
- **Esquema de Cores**:
  - **Árvore**: Tons de verde, amarelo, ciano
  - **Fundo**: Rosa/lilás (céu), laranja (grama)

### Scripts Python

#### `scripts/png_to_ppm.py`
- **Função**: Converte imagens PNG para formato PPM
- **Uso**: `python scripts/png_to_ppm.py entrada.png saida.ppm`
- **Dependência**: Biblioteca Pillow

#### `scripts/ppm_to_png.py`
- **Função**: Converte imagens PPM para formato PNG
- **Uso**: `python scripts/ppm_to_png.py entrada.ppm saida.png`
- **Dependência**: Biblioteca Pillow

## 🔬 Algoritmo

### Fluxo de Processamento

1. **Entrada**: Imagem PNG
2. **Conversão**: PNG → PPM (usando script Python)
3. **Carregamento**: PPM → Estruturas de dados C++
4. **Pré-processamento**: 
   - Cálculo de gradientes com filtro Gaussiano
   - Inicialização de sementes estratégicas
5. **Segmentação**: IFT-Watershed otimizado
6. **Pós-processamento**: Suavização e eliminação de fragmentação
7. **Saída**: Conversão PPM → PNG (usando script Python)

### Função de Custo Híbrida

O algoritmo usa uma função de custo que combina:
- **Gradiente**: Detecta bordas e transições
- **Diferença de Cor RGB**: Melhora discriminação entre regiões
- **Fórmula**: `custo = max(custo_atual, gradiente * 2.0 + diffCor * 0.5)`

### Estratégia de Sementes

- **9 sementes total**: 5 para árvore, 4 para fundo
- **Posicionamento inteligente**: Baseado em características da imagem
- **Distribuição espacial**: Cobre diferentes regiões da imagem

## 🖼️ Exemplos

### Imagens de Teste Incluídas

- `arvore.png` - Árvore em paisagem natural
- `cavalo.png` - Cavalo em fundo simples
- `homer.png` - Personagem em fundo colorido
- `monalisa.png` - Pintura famosa
- `vangogh.png` - Pintura de Van Gogh

### Resultados Esperados

O algoritmo deve produzir segmentações onde:
- **Árvores** são destacadas em tons de verde
- **Céu** aparece em tons de rosa/lilás
- **Grama/solo** aparece em tons de laranja
- **Bordas** são bem definidas e suaves

## 🔧 Troubleshooting

### Problemas Comuns

#### Erro de Compilação
```bash
# Solução: Verificar se o compilador está instalado
g++ --version
```

#### Erro de Conversão PNG → PPM
```bash
# Solução: Instalar Pillow
pip install Pillow
```

#### Imagem não encontrada
```bash
# Verificar se a imagem existe em imagens/
ls imagens/
```

#### Resultado com muito ruído
- **Causa**: Sementes mal posicionadas
- **Solução**: Ajustar posições em `inicializarSementesOtimizadas()`

#### Segmentação fragmentada
- **Causa**: Parâmetros de suavização inadequados
- **Solução**: Ajustar tamanhos de filtros em `suavizarSegmentacaoAgressiva()`

### Otimizações Possíveis

1. **Paralelização**: Usar OpenMP para processamento em paralelo
2. **GPU**: Implementar versão CUDA para processamento mais rápido
3. **Interface Gráfica**: Adicionar GUI para seleção de sementes
4. **Múltiplas Imagens**: Processamento em lote

## 📄 Licença

Este projeto foi desenvolvido para fins educacionais e de pesquisa em processamento de imagens.

## 👥 Autores

Desenvolvido como parte de um trabalho acadêmico sobre técnicas de segmentação de imagens.

---

**Nota**: Este projeto demonstra técnicas avançadas de processamento de imagens e pode ser usado como base para implementações mais complexas de segmentação semântica. 