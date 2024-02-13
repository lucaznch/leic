# Descrição do Problema

O Engenheiro João Caracol foi contratado pela fábrica SuperMarble para optimizar uma das suas linhas de corte de chapas de mármore.

A linha recebe uma chapa de mármore que deve ser cortada de modo a produzir peças com dimenções requeridas pelos clientes da fábrica.

A linha dispõe de uma máquina de dois discos que consegue cortar chapas de um lado ao outro.

O **processo de corte** funciona da seguinte maneira:
* a chapa é cortada verticalmente ou horizontalmente;
* cada uma das duas novas chapas produzidas volta a entrar na linha de corte ou sai da linha, caso corresponda às dimensões de uma das peças a serem produzidas ou já não seja possível convertê-la numa peça.


A fábrica consegue neste momento dar curso toda a produção pelo que deve ser dada prioridade ao fabrico de peças de valor mais elevado.

O objectivo do Eng. Caracol é:
* **construir um programa que dada uma chapa de mármore calcula o valor máximo que pode ser obtido a patir da mesma cortando-a em peças correspondentes às dimensões solicitadas pelos clientes.**


O Eng. Caracol pode produzir várias cópias de uma mesma peça conforme considere adequado.

Mais concretamente: a linha recebe uma chapa rectangular de mármore de dimensões **X × Y**.

Além disso, o Eng. Caracol tem acesso a uma lista com os **n** tipos de peças a serem produzidos, todos com dimensões diferentes.

Cada tipo de peça **i ∈ {1, ..., n}** corresponde a um retângulo de mármore com dimensões **ai × bi** e é vendido a um preço **pi**.

Nota: **as peças podem ser rodas => 2x3 <=> 3x2**


## Input

O ficheiro de entrada contém as dimensões da chapa a ser cortada e as dimensões dos vários tipos de peças solicitados.
Estes valores são representados da seguinte forma:
* a primeira linha contém dois inteiros positivos **X** e **Y** , separados por um espaço em branco, que correspondem às dimensões da chapa;
* a segunda linha contém um inteiro positivo **n**, que corresponde ao número de tipos de peças que podem ser produzidas;
* **n** linhas que descrevem cada um dos **i** tipos de peças que podem ser produzidas. Cada linha é composta por três inteiros positivos **ai** , **bi** e **pi** separados por um espaço em branco, onde **ai × bi** correspondem às dimensões do tipo de peça e **pi** ao preço da mesma.


## Output

Deverá escrever no output o valor máximo que pode ser obtido a partir da chapa dada como input. 
Caso nenhuma peça possa ser produzida deverá simplesmente imprimir 0.


## Exemplos

**Input 1**
```
1 3
2
1 1 1
1 3 10
```
**Output 1**

`10`


**Input 2**
```
3 20
3
2 2 4
1 5 10
3 7 20
```
**Output 2**

`120`


**Input 3**
```
7 4
2
6 3 130
1 2 5
```
**Output 3**

`155`


**Input 4**
```
4 3
2
3 3 10
3 2 6
```
**Output 4**

`12`



### Implementação
O tempo necessário para implementar este projecto é inferior a 15 horas.

Parâmetros de compilação:
C++: g++ -std=c++11 -O3 -Wall file.cpp -lm
