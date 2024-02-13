# Descrição do Problema

O Professor João está a fazer um estudo para a taskforce do Governo responsável pelo estudo de doenças transmissíveis em Portugal. 
A taskforce está particularmente interessada no tópico da transmissão de doenças entre a população portuguesa, de forma a estudar os melhores mecanismos de intervenção para conter a propagação de doenças.
Para isso, o Professor João teve acesso aos dados da rede social TugaNet, que acredita ser representativa das reais interacções sociais entre os indivíduos da população portuguesa.
Assim, de forma a estudar o pior caso de propagação de uma dada infecção em Portugal, o Professor João quer perceber qual o maior número de saltos que uma dada doença pode fazer.

No entanto, tendo em conta a densidade das cidades portuguesas, o Professor João decidiu considerar um pressuposto simplificador:
* indivíduos que se conhecem mutuamente de forma directa ou indirecta, ficam infectados instantaneamente.

## Input
**O ficheiro de entrada contém a informação sobre a rede TugaNet, que é definida como um grafo dirigido de relações entre dois indivíduos**, da seguinte forma:
- Uma linha contendo dois inteiros: o número **n** de indivíduos (n ≥ 2), e o número de relações **m** a indicar (m ≥ 0);
- Uma lista em que cada linha **i** contém dois inteiros **x** e **y**, representando que o indivíduo **x** conhece o indivíduo **y**.

Quaisquer inteiros numa linha estão separados por exactamente um espaço em branco, não contendo qualquer outro carácter, a não ser o fim de linha.
**Assuma que os grafos de input são dirigidos (potencialmente) cíclicos.**


## Output
O programa deverá escrever no output um inteiro **s** correspondendo ao **número máximo de saltos que uma doença pode fazer na rede TugaNet**.


### Exemplo 1
Input
```
7 8
3 4
3 2
4 6
4 5
6 2
6 5
5 7
2 7
```
Output esperado
`4`

> [!NOTE]
> No exemplo 1. o maior número de saltos corresponde ao caminho mais longo do grafo dado, que é 4.

### Exemplo 2
Input
```
8 9
2 5
3 2
3 7
4 6
5 7
6 5
7 8
8 1
8 5
```
Output esperado
`3`

> [!NOTE]
> No exemplo 2, o maior número de saltos é 3 e não 5. Note-se que o grafo dado é cíclico, por isso teremos que usar o pressuposto simplificador, que indivíduos que se conhecem mutuamente de forma directa ou indirecta, ficam infectados instantaneamente. Isto significa que os ciclos são contraídos em vértices únicos para computacionar o caminho mais longo.


## Implementação
A implementação do projecto deverá ser feita preferencialmente usando a linguagem de programação C++.
Importante: Observa-se que soluções recursivas podem esgotar o limite da pilha quando executadas sobre os testes de maior tamanho, pelo que se recomenda a implementação de algoritmos iterativos.
O tempo necessário para implementar este projecto é inferior a 15 horas.
Parâmetros de compilação:
C++: g++ -std=c++11 -O3 -Wall file.cpp -lm


## Optimizações
Dada a dificuldade reportada por alguns alunos em conseguirem ter um projecto correcto a passar os testes todos, e dado que para projectos com Grafos de baixa complexidade computacional, como é o caso deste projecto, todas as pequenas optimizações influenciarem as constantes multiplicativas, sugere-se:
* o uso de vector<vector<int>> em vez de vector<list<int>> dado que o overhead do uso de listas.
* o uso de std::ios::sync_with_stdio(0); // disable sync with c libs (printf/scanf)
* o uso de std::cin.tie(0); // discard cin buffer after each line of input
