# Documentação: Implementação 2

**Autores:** Bruno Braga Guimarães, Vitor Dias de Britto Militão, Vitor Lucio de Oliveira  

## Introdução

Este documento explica o funcionamento do código fornecido, que realiza cálculos relacionados a grafos completos e gera todos os subgrafos possíveis de um grafo completo com um número dado de vértices.

## Descrição do Código

### Função `fatorial`

Esta função calcula o fatorial de um número inteiro \(x\). O fatorial é calculado recursivamente:

- Se \(x \leq 1\), a função retorna 1.
- Caso contrário, retorna \(x \times \text{fatorial}(x-1)\).

### Função `totalArestas`

Esta função calcula o número total de arestas possíveis em um subgrafo de \(k\) vértices de um grafo completo. O número de arestas em um grafo completo com \(k\) vértices é dado por \(\frac{k(k-1)}{2}\). O valor retornado é \(2^{\text{arestas}}\), que representa todas as possíveis combinações de arestas.

### Função `combinacao`

Esta função calcula a combinação \(C(n, p)\), que é o número de maneiras de escolher \(p\) elementos de um conjunto de \(n\) elementos, sem considerar a ordem. A fórmula utilizada é:

\[
C(n, p) = \frac{n!}{p!(n-p)!}
\]

onde \(n!\) é o fatorial de \(n\), calculado pela função `fatorial`.

### Função `totalSubgrafos`

Esta função calcula o número total de subgrafos possíveis para um grafo completo com \(n\) vértices. Para cada número de vértices \(i\) de 1 a \(n\), a função calcula o número de combinações \(C(n, i)\) e o número de arestas \(A(i)\) para esse subgrafo, somando o produto \(C(n, i) \times A(i)\) ao total.

### Função `gerarSubgrafos`

A função `gerarSubgrafos` tem como objetivo gerar e exibir todos os subgrafos possíveis de um grafo completo com \(n\) vértices. Para compreender melhor o funcionamento dessa função, vamos detalhar cada uma de suas etapas principais:

1. **Inicialização**:
    - A função começa declarando uma variável `subgrafoCount`, inicializada em 0, para manter o controle do número de subgrafos gerados.

2. **Geração de Subconjuntos de Vértices**:
    - A função utiliza um laço `for` para iterar sobre todos os possíveis subconjuntos de vértices do grafo completo.
    - O laço itera de `subGrafo = 1` até \(2^n - 1\), onde \(n\) é o número de vértices. Isso corresponde a todas as combinações possíveis de vértices, exceto o grafo vazio (representado por 0).
    - Para cada valor de `subGrafo`, um vetor `vertices` é criado para armazenar os vértices presentes no subconjunto correspondente.
    - O laço interno verifica se cada vértice \(i\) (de 0 a \(n-1\)) pertence ao subconjunto atual. Isso é feito através da expressão `subGrafo & (1 << i)`, que checa se o \(i\)-ésimo bit de `subGrafo` está definido.
    - Se o vértice pertence ao subconjunto, ele é adicionado ao vetor `vertices`.

3. **Geração de Subgrafos a Partir dos Subconjuntos**:
    - Após determinar os vértices presentes no subconjunto atual, a função calcula o número total de arestas possíveis entre esses vértices, utilizando a função `totalArestas`.
    - Em seguida, a função itera sobre todas as combinações possíveis de arestas para o conjunto atual de vértices. Isso é feito através de um laço que varia de `arestas = 0` até `numArestas - 1`.
    - Para cada combinação de arestas, a função exibe os vértices presentes e as arestas que formam o subgrafo atual.

4. **Exibição do Subgrafo**:
    - Para cada subgrafo gerado, a função incrementa o contador `subgrafoCount` e exibe uma linha no formato:

    ```
    Subgrafo X: Vértices { ... } Arestas { (...) }
    ```

    onde \(X\) é o número do subgrafo, `{...}` é o conjunto de vértices presentes, e `{(...)}` é o conjunto de arestas entre esses vértices.
    - A exibição das arestas é feita através de dois laços aninhados que percorrem todos os pares de vértices no subconjunto atual. Se o par de vértices \(i\) e \(j\) forma uma aresta no subgrafo atual, ela é exibida.

A função `gerarSubgrafos` é eficiente em termos de cobertura de todos os subgrafos possíveis, mas o tempo de execução cresce exponencialmente com o número de vértices, devido ao número de combinações possíveis de vértices e arestas. Essa abordagem garante que todos os subgrafos, desde os menores com apenas um vértice até o grafo completo, sejam gerados e exibidos.

### Função `main`

A função principal solicita ao usuário o número de vértices \(n\) do grafo completo. Em seguida, chama a função `gerarSubgrafos` para gerar e exibir todos os subgrafos possíveis.

## Conclusão

O código apresentado permite a geração de todos os subgrafos possíveis de um grafo completo com \(n\) vértices. As funções auxiliares implementadas facilitam o cálculo de fatorial, combinações, e o total de arestas, proporcionando uma solução completa para a tarefa proposta.
