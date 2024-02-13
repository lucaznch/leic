# Descrição do Problema

O Professor Natalino Caracol foi contratado pela empresa UbiquityInc, em Rovaniemi na Lapónia, para desenvolver um programa que permita estimar o lucro máximo que pode ser obtido com a produção e venda de brinquedos durante o Natal.

A empresa produz diariamente um conjunto de n brinquedos de madeira {x1 , . . . , xn }, onde cada brinquedo xi tem um lucro li.


Para além de um limite máximo de produção de cada brinquedo devido a restrições de linha de montagem, a empresa está limitada a uma quantidade máxima total de brinquedos que podem ser produzidos por dia, devido a restrições de corte da floresta boreal.

Adicionalmente, este Natal a empresa decidiu, para além de vender cada brinquedo individualmente, vender também pacotes especiais contendo três brinquedos distintos, cujo lucro é maior do que a soma dos lucros individuais dos brinquedos que o constituem.

O objectivo consiste em indicar ao Rüdolf, CEO da UbiquityInc, qual o lucro máximo que se pode obter diariamente.

A UbiquityInc, tratará posteriormente do problema da distribuição.

A implementação deve ser baseada em Python com recurso à biblioteca PuLP para resolução de problemas LP (https://pypi.org/project/PuLP/).

Exemplos disponíveis em https://github.com/coin-or/pulp/tree/master/examples.


**Nota**: Informação sobre o que incluir no relatório pode ser encontrada no template a ser disponibilizado na página da disciplina.


## Input
O ficheiro de entrada contém informação sobre os n produtos, o lucro e a capacidade de produção da empresa de cada um, da seguinte forma:

* Uma linha contendo três inteiros: t indicando o número de diferentes brinquedos passíveis de serem produzidos, p indicando o número de pacotes especiais, e max indicando o número máximo de brinquedos que podem ser produzidos por dia;

* Uma lista de n linhas, em que cada linha contém dois inteiros li e ci, indicando o lucro e a capacidade de produção do brinquedo i.

* Uma lista de p linhas, em que cada linha contém quatro inteiros i, j, k, e lijk, indicando o lucro lijk do pacote especial {i, j, k}, e o nome dos produtos i, j, e k que o constituem.

Quaisquer inteiros numa linha estão separados por exactamente um espaço em branco, não contendo qualquer outro carácter, a não ser o fim de linha.


## Output
O programa deverá escrever no output um inteiro correspondendo ao lucro máximo que o Rüdolf pode obter diariamente.

### Exemplo 1
**Input**
```
5 2 150
50 27
30 33
45 30
40 37
35 35
1 3 5 130
2 3 4 130
```
**Output**
`6440`

### Exemplo 2
## Input
```
5 2 15
50 27
30 33
45 30
40 37
35 35
1 3 5 129
2 3 4 130
```
## Output
`750`

## Implementação
A implementação do projecto deverá ser feita obrigatoriamente usando a linguagem de programação Python.

O tempo necessário para implementar este projecto é inferior a 15 horas.

A biblioteca PuLP deve ser instalada utilizando o comando:
* python -m pip install pulp

Devem ter algum solver de PL instalado, como por exemplo o GLPK (https://www.gnu.org/software/glpk/) ou o LP_solve.
Para instalar o GLPK (por exemplo no Ubuntu): sudo apt-get install glpk-utils

Parâmetros de execução:
Python: python3 vossoprograma.py < ficheiro_de_input
