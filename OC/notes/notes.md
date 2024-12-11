# organização de computadores
notas responsáveis para a minha negativa no primeiro teste!

<br>

contents:
- [performance](#performance)
- [instruções](#instruções)
    - [ISA](#instruction-set-architecture-isa)
    - [RISC vs CISC](#isas)
    - [MIPS](#microprocessor-without-interlocked-pipelined-stages-mips)
        - [formato de instrução](#formatos-de-instrução-mips-32)
        - [registos](#registos)
        - [instruções de acesso a memória](#instruções-de-acesso-a-memória)
        - [instruções de controlo](#instruções-de-controlo)
- [memória](#memória)

## performance
dentro do processador:
- **control unit** - sequencia o datapath, memória, entre outros
- **datapath** - executa operações sobre dados, e, por isso, contém arithmetic logic unit (ALU), Register File, Program Counter, etc.

<br>

camadas:
- **application software** - está escrito numa linguagem de alto nível
- **system software** - dividido entre o *compilador*, que traduz as linguagens de alto nível para código de máquina e o *sistema operativo*, que gere o input/output, memória e armazenamento, assim como o escalonamento de tarefas e partilhas de recursos
- **hardware** - onde realmente corre o nosso programa, após ser convertido para código máquina pelo compilador

<br>

níveis de código:
- **compiler(** *high level code* **)** = *assembly code*
- **assembler(** *assembly code* **)** = *machine code*

<br>

factores de performance:
- **algoritmo**: determina o número de operações executadas
- **linguagem de programação, compilador, arquitectura**: determinam o número de instruções ao nível da máquina executadas por operação
- **processador**, **sistema de memória**: determinam o quão rápido as instruções são executadas
- **sistema de I/O**: determinam o quão rápido as operações I/O são executadas

<br>

definições de performance:
- **response time** - quanto tempo demora completar uma tarefa
    - book def: the total time required for the computer to complete a task, including disk accesses, memory accesses, I/O activities, operating system overhead, CPU execution time, and so on.
    - *scope* - broader term that considers all aspects of how long a user or system waits for the completion of a task, including network delays, disk access times, and any other waiting time. response time is often used in user-centric contexts, where the focus is on how long the user or system waits for the completion of a task.
    - geral
- **execution time** - amount of time the CPU spends actively working on a task. it's a subset of response time, focusing specifically on the time the processor is running the instructions of a program
    - geral
    - *scope* - includes two components:
        - **user CPU time** - time spent executing the user’s instructions (your application’s code). *the CPU time spent in a program itself*
        - **system CPU time** - the time spent executing system or kernel instructions on behalf of your application (e.g., for I/O or system calls). *the CPU time spent in the operating system performing tasks on behalf of the program*
- **delay** - delay before a process starts, or the time it takes for data to travel from one point to another in a system
- **throughput (bandwidth)** - *trabalho realizado por unidade de tempo* / *number of tasks completed per unit time*

<br>

- **performance** em relação ao execution time

$$
\text{performance}_X = \frac{1}{\text{execution time}_X}
$$

$$
\text{menor execution time} => \text{maior/melhor performance}
$$

<br>

- **performance relativa** (speedup)

$$
\text{speedup}_X = \frac{\text{performance}_X}{\text{performance}_Y} = \frac{\text{execution time}_Y}{\text{execution time}_X} = n
$$

$$
\text{X é } n \text{ vezes mais rápido do que Y}
$$

<br>

**example** - if computer A runs a program in 10 seconds and computer B runs the same program in 15 seconds, how much faster is A than B?

$$
\frac{\text{performance}_A}{\text{performance}_B} = \frac{\text{execution time}_B}{\text{execution time}_A} = \frac{15}{10} = 1.5
$$

$$
\text{A is 1.5 times faster than B} \lor \text{B is 1.5 times slower than A}
$$

<br>

the most straightforward definition of time is called wall clock time, response time, or elapsed time. these terms mean the total time to complete a task, including disk accesses, memory accesses, input/output (I/O) activities, operating system overhead—everything.

- **elapsed time (wall-clock time)** - tempo total de resposta, incluindo todos os aspectos (processing, I/O, OS overhead, idle time, etc). *the total time that passes between the start and end of a program’s execution, as measured by an external clock. this is the actual "real-world" time from the moment you start a task to when it finishes*
    - *scope* - often used in performance analysis to measure the entire duration of a task, including time spent by the CPU, I/O operations, and waiting for resources.

- **cpu time** - tempo gasto pelo processador numa tarefa (descontando syscalls, tempo bloqueado e o tempo que o processador gasta noutros processos)
    - book def: the actual time the CPU spends computing for a specific task (does not include time spent waiting for I/O or running other programs. remember, though, that the response time experienced by the user will be the elapsed time of the program, not the CPU time)

$$
\text{CPU time} = T_{\text{CPU}} = \text{CPU clock cycles} \times {T_{\text{clock cycle}}}
= \frac{\text{CPU clock cycles}}{{f_{\text{clock}}}}
$$


**example** - computer A runs a program in 10 seconds which has a 2GHz clock. computer B runs the same program in 6 seconds. computer B requires 1.2 times as many clock cycles as computer A for the program. what is computer B clock rate?

<br>

$$
{\text{CPU time}}_A = \frac{{\text{clock cycles}_A}}{{f_{{\text{clock}}_A}}}
$$

$$
=> \text{10 seconds} = \frac{{\text{clock cycles}_A}}{2 \times 10^9 \text{cycles per second}}
$$

$$
=> {\text{CPU time}}_A = \text{10 seconds} \times 2 \times 10^9 \text{cycles per second} = 20 \times 10^9 \text{cycles}
$$

<br>
<br>

$$
{\text{CPU time}}_B = 1.2 \times \frac{{\text{clock cycles}_B}}{{f_{{\text{clock}}_B}}}
$$

$$
=> \text{6 seconds} = \frac{1.2 \times 20 \times 10^9 \text{cycles}}{{f_{{\text{clock}}_B}}}
$$

$$
{f_{{\text{clock}}_B}} = \frac{1.2 \times 20 \times 10^9 \text{cycles}}{\text{6 seconds}} = \text{4 GHz}
$$

<br>
<br>
<br>

características:
- **relógio** - oscilador que gera um sinal constante utilizado para sincronizar a execução de instruções pelo processador.
- **frequência do relógio** (clock rate) - taxa de geração pulsos, i.e., o número de ciclos do relógio por segundo. 
- **período do relógio** - tempo de duração de um ciclo do relógio.
- **ciclo do relógio** - corresponde a uma oscilação completa do sinal.
$$
\text{f}_{clock} = \frac{1}{\text{T}_{clock}}
$$


$$
\text{CPU time} = \text{num of clock cycles} \times \text{T}_{clock} = \frac{\text{num of clock cyles}}{\text{f}_{clock}}
$$


<br>

instruções:
- **cycles per instruction (CPI)**: número médio de ciclos do relógio para executar uma instrução.

$$
\text{clock cycles} = \text{instruction count} \times \text{CPI}
$$

$$
\text{clock cycles} = \sum_{i=1}^n \left({{CPI}_i} \times \text{instructions}_i\right)
$$

- **classic CPU performance equation**

$$
\text{CPU time} = \text{instruction count} \times \text{CPI} \times \text{clock cycle time}
$$

$$
\text{CPU time} = \frac{\text{instruction count} \times \text{CPI}}{f_\text{clock}}
$$



<br>
<br>

**caches**:



## instruções
### instruction set architecture (ISA)
- **instruction set** - conjunto de instruções do processador / reportório das instruções de um computador. diferentes processadores têm diferentes instruction sets (mas com muitos aspetos comuns)
- **instruction set architecture (ISA)** - uma interface abstracta entre o hardware e o software de baixo nı́vel, que encapsula toda a informação necessária para escrever programas em linguagem máquina. essa informação inclui instruções, registos, acessos à memória e input/output.
    - avaliar ISAs:
        - design-time metrics
        - static metrics: quantos bytes o programa ocupa em memória?
        - dynamic metrics:
            - quantas instruções são executadas?
            - quantos bytes o processador lê para executar o programa?
            - quantos clock cycles são necessários por instrução?
        - metric of interest: time to execute the program!
### ISA's
- **Complex Instruction Set Computer (CISC)**
    - the primary goal of CISC architecture is to complete a task in as few lines of assembly as possible.
    - this is achieved by building processor hardware that is capable of understanding and executing a series of operations.
    - for this particular task, a CISC processor would come prepared with a specific instruction (we'll call it "MULT").
    - when executed, this instruction loads the two values into separate registers, multiplies the operands in the execution unit, and then stores the product in the appropriate register. thus, the entire task of multiplying two numbers can be completed with one instruction.

    ```assembly
        MULT 2:3, 5:2
    ```

    -  MULT is what is known as a "complex instruction." it operates directly on the computer's memory banks and does not require the programmer to explicitly call any loading or storing functions. It closely resembles a command in a higher level language. 

- **Reduced Instruction Set Computer (RISC)** 
    - RISC processors only use simple instructions that can be executed within one clock cycle. 
    - thus, the "MULT" command described above could be divided into three separate commands: "LOAD," which moves data from the memory bank to a register, "PROD," which finds the product of two operands located within the registers, and "STORE," which moves data from a register to the memory banks.
    - in order to perform the exact series of steps described in the CISC approach, a programmer would need to code four lines of assembly:

    ```assembly
        LOAD A, 2:3
        LOAD B, 5:2
        PROD A, B
        STORE 2:3, A
    ```

    - at first, this may seem like a much less efficient way of completing the operation. because there are more lines of code, more RAM is needed to store the assembly level instructions. the compiler must also perform more work to convert a high-level language statement into code of this form.
    - however, the RISC strategy also brings some very important advantages. 
        - because each instruction requires only one clock cycle to execute, the entire program will execute in approximately the same amount of time as the multi-cycle "MULT" command.
        - these RISC "reduced instructions" require less transistors of hardware space than the complex instructions, leaving more room for general purpose registers.
        - because all of the instructions execute in a uniform amount of time (i.e. one clock), pipelining is possible.

<br>

arch | emphasis | clock, instructions               | memory               | cycles, code                       | transistors 
-----|----------|-----------------------------------|----------------------|------------------------------------|-------------------------------------------
CISC | hardware | multi-clock complex instructions  | memory-to-memory     | high cycles per s, small code size | used for storing complex instructions 
RISC | software | single-clock reduced instructions | register-to-register | low cycles per s, larger code size | spends more transistors on memory registers


factores de diferenciação:
- número de instruções - RISC < CISC
- complexidade das operações que são implementadas por uma única instrução - RISC < CISC 
- número de operandos - fixed format often with 3 operands =\= varies, from 0 to more than 3
- modos de endereçamento - RISC (único) < CISC (vários modos)
- acesso à memória - LOAD/STORE model (instrução separada) =\= direct memory access in multiple instructions


### microprocessor without interlocked pipelined stages (MIPS)
implementção de uma arquitectura RISC que será usada durante a cadeira como exemplo de estudo, mais especificamente MIPS-32 ISA. 

- simplicidade
    - instruções de tamanho fixo
    - opcodes de tamanho fixo (sempre definido nos primeiros 6 bits)
    - número limitado de formatos de instrução (R, I, J)
- mais pequeno é mais rápido
    - instruction set limitado
    - número limitado de registos
    - número limitado de modos de endereçamento
- fazer o caso comum rápido
    - operandos aritméticos sempre em registo (não podem ser constantes nem estar em memória)
    - permitir que instruções contenham operandos imediatos
- bom design envolve bons compromissos:
    - há três formatos de instrução


<br>

categorias de instrução:
- computacional;
- load/store;
- jump e branch;
- floating point;
- memory management;
- special


### formatos de instrução MIPS-32

- recordar - **PC** refere-se a *Program Counter*, que indica o endereço de memória no qual o processador está a ler a instrução atual, sendo que este é incrementado sempre de 4 em 4 bytes pois uma instruções é de 32 bits = 4 bytes 

- definições
    - *op* - operation code
    - *rs* - first source register
    - *rt* - second source register (R-format) or destination register (I-format)
    - *rd* - destination register
    - *sa/shamt* - shift amout
    - *funct* - function code - extensão do opcode!
    - *immediate* - uma constante
    - *jump target* - target address for the jump 

- **R format** - `op | rs | rt | rd | sa | funct `
    - op (6 bits)
    - rs (5 bits)
    - rt (5 bits)
    - rd (5 bits)
    - sa (5 bits)
    - funct (5 bits)
        - example: `add $t0, $t1, $t2` adds the values in `$t1` and `$t2` and stores the result in `$t0`

- **I format** - `op | rs | rt | immediate`
    - op (6 bits)
    - rs (5 bits)
    - rt (5 bits)
    - immediate (16 bits)
        - example: `addi $t0, $t1, 10` adds the immediate value `10` to `$t1` and stores the result in `$t0`
    - **how can you represent a 32-bit constat with 16 bits?**

- **J format** - `op | jump target`
    - op (6 bits)
    - immediate (26 bits)
        - example: `j 4000` jumps to address `4000` 

    - **how can you represent a 32-bit address with 26 bits?**



### registos

| register name | number    | usage                                             |
| ------------- | --------- | ------------------------------------------------- |
| **$zero**     | $0        | constant 0 (hardware)                             |
| **$at**       | $1        | reserved for assembler                            |
| **$v0 - $v1** | $2 - $3   | function return values                            |
| **$a0 - $a3** | $4 - $7   | function arguments                                |
| **$t0 - $t7** | $8 - $15  | temporary registers                               |
| **$s0 - $s7** | $16 - $23 | saved registers (preserved across function calls) |
| **$t8 - $t9** | $24 - $25 | additional temporary registers                    |
| **$k0 - $k1** | $26 - $27 | reserved for kernel use, exceptions               |
| **$gp**       | $28       | global pointer                                    |
| **$sp**       | $29       | stack pointer                                     |
| **$fp**       | $30       | frame pointer                                     |
| **$ra**       | $31       | return address for function calls (hardware)      |

- o MIPS tem 32 registos (+PC, HI, LO) - cada um armazena uma palavra de 32 bits = 4 bytes
- o banco de registos tem 2 portas de leitura e 1 porta de escrita - permite ler valores de dois registos e, simultaneamente, escrever um valor num registo

<br>
<br>

**instruções**

- **R-format example** - `op6 | rs5 | rt5 | rd5 | sa5 | funct6 `
    - `add $t0, $s1, $s2`

        ```text
        op     | $s1   | $s2   | $t0   | 0      | add
        0      | 17    | 18    | 8     | 0      | 32
        000000 | 10001 | 10010 | 01000 | 000000 | 100000
        
        instrução em código máquina:
        0b00000010001100100100000000100000 = 0x02324020
        ```

- **I(mediat)-format example** - `op6 | rs5 | rt5 | immediate16 `
    - sempre que temos uma constante
    - `addi $t0, $zero, 5`

        ```text
        op     | $zero | $st0   | 5
        8      | 0     | 8      | 5
        001000 | 00000 | 01000  | 0000000000000101
        
        instrução em código máquina:
        0b00100000000010000000000000000101 = 0x20080005
        ```
    
    - nota: não existe subtração imediata, pelo que temos que usar uma adição imediata com uma constante negativa
        - `addi $s2, $s1, -1        # guarda em $s2 o valor de $s1 - 1`

- idk
    - `add $t0, $t1, $t2` - `$t0` gets `$t1` + `$t2`

        ```c
        // C code
        f = (g + h) - (i + j);
        ```

        ```assembly
        # assembly code
        add $t0, $s1, $s2   # temp_1 = g + h
        add $t1, $s3, $s4   # temp_2 = i + j
        sub $s0, $t0, $t1   # f =  temp_1 + temp_2
        ```

- carregar uma constante de 32 bits num registo (I-format) - `op6 | rs5 | rt5 | immediate16 `

    - **load upper immediate** - `lui $t0, 1010101010101010`
        - load 16-bit immediate value into the upper 16 bits of a register
        - $t0 = 1010101010101010 0000000000000000
    - **bitwise or immediate** - `ori $t0, $t0, 1010101010101010`
        - $t0 = 1010101010101010 1010101010101010

- signed vs unsigned
    - add => signed => checks for overflow
    - addu => unsigned => does not check for overflow
        - treats the result as an unsigned value, ignoring overflow. if the result exceeds 32 bits, the extra bits are discarded without causing an exception.
    
- **operações lógicas**

- operações de deslocamento - exemplo - `sll $t0, $t0, 3`
    - multiplicar o valor de $t0 por $2^3$

| operação    | C       | Java    | MIPS         |
| ----------- | ------- | ------- | ------------ |
| shift left  | `<<`    | `<<`    | `sll`        |
| shift right | `>>`    | `>>>`   | `srl`        |
| bitwise AND | `&`     | `&`     | `and`,`andi` |
| bitwise OR  | `\|`    | `\|`    | `or`, `ori`  |
| bitwise NOT | `~`     | `~`     | there isnt   |
| bitwise NOR | `~(\|)` | `~(\|)` | `nor`        |
| bitwise NAND|         |         | there isnt   |
| bitwise XOR | `~(\|)` | `~(\|)` | `xor`, `xori`|

- **NOT** - fazer um **NOR** com **zero**
- **NAND** - fazer um **AND** e um **NOR** com **zero**



### instruções de acesso a memória
MIPS has two basic data transfer instructions for accessing memory

```assembly
lw $t0, 4($s3)      # load word from memory
sw $t0, 8($s3)      # store word to memory
```

- the data is **loaded into (lw)** or **stored from (sw)** a register in the register file

<br>

- `lw $t0, <offset>($<base address>)` - carregar o endereço `base address` + `offset` da memória para `$t0`.
- `sw $t0, <offset>($<base address>)` - escrever no endereço `base address` + `offset` a memória do registo `$t0`.
- `lh $t0, <offset>($<base address>)` - carregar 16 bits do endereço `base address` + `offset` da memória para `$t0`, extendendo como signed.
- `lhu $t0, <offset>($<base address>)` - carregar 16 bits do endereço `base address` + `offset` da memória para `$t0`, extendendo como unsigned.
- `sh $t0, <offset>($<base address>)` - escrever 16 bits no endereço `base address` + `offset` a memória do registo `$t0`.
- `lb $t0, <offset>($<base address>)` - carregar 1 byte do endereço `base address` + `offset` da memória para `$t0` extendendo como signed.
- `lbu $t0, <offset>($<base address>)` - carregar 1 byte do endereço `base address` + `offset` da memória para `$t0` extendendo como unsigned.
- `sb $t0, <offset>($<base address>)` - escrever 1 byte no endereço `base address` + `offset` a memória do registo `$t0`.

    - note-se que o **base address é um registo de 32 bits e o offset é uma constante de 16 bits**


- a memória principal é usada para guardar tanto dados simples (e.g. inteiros, bytes, etc) como dados compostos, tais como arrays, estruturas e dados dinâmicos.

> [!IMPORTANT]
> a memória do MIPS é **endereçada em bytes**, ou seja, cada endereço identifica 1 byte (8 bits). é muito importante que as palavras estejam alinhadas em memória com um endereço que seja um múltiplo de quatro, de forma a preservar a eficiência do processador e evitar bugs. usualmente, o compilador trata disto por nós.


- MIPS é **Big-Endian** => most-significant byte (big end) at lowest address of a word
```text
e.g. 0x12345678
    ...
    address X:      0x12     - most significant byte (lowest address)
    address X+1:    0x34
    address X+2:    0x56
    address X+3:    0x78     - least significant bye
    ...
```


- examples
    - **g** in `$s1`, **h** in `$s2`, **base address of A** in `$s3`
        ```c
        g = h + A[8];
        ```

        ```assembly
        lw $t0, 32($s3)     # load word
        add $s1, $s2, $t0
        ```

        - array indexing - como em MIPS a memória é endereçada em bytes e cada elemento em A é uma palavra (4 bytes) => o index 8, é a oitava palavra, e precisa de ser multiplicado por 4 para ter o offset correto

        ```text
                                                                        |
                                                                        v
        | b0 | b1 | b2 | b3 |       ...         | b28 | b29 | b30 | b31 | b32 | b33 | b34 | b35 |
        |      A[0]         |       ...         |         A[7]          |        A[8]           |
        ```
    - **h** in `$s2`, **base address of A** in `$s3`
        ```c
            A[12] = h + A[8];
        ```

        ```assembly
            lw $t0, 32($s3)     # load A[8] (8 x 4 = 32)
            add $t0, $s2, $t0   # add A[8] + h
            sw $t0, 48($s3)     # store in A[12] (12 x 4 = 48)
        ```

- aligned memory access

```text
| width                || b0 | b1 | b2 | b3 || b4 | b5 | b6 | b7 ||
| 1 byte               ||  v |  v |  v |  v || v  |  v |  v |  v ||
| 2 bytes (half word)  ||    v    |    v    ||     v   |    v    ||
| 4 bytes (word)       ||         v         ||         v         ||
| 8 words (dword)      ||                    v                   ||
```


### instruções de controlo

**J-instruction** - `op6 | jump-target26`

- **jump** - `j <target>`
    - jumps to a specific address
    - the address is calculated by shifting the 26-bit immediate value in the instruction left by 2 bits (to account for the word-aligned nature of instructions) and combining it with the upper 4 bits of the current program counter (PC) => só podemos saltar para uma certa secção do programa

    ```text
    e.g. PC = [0101] 1011 0110 1101 0101 1010 1010 0110 (32 bits)

    se efetuarmos um Jump para a instrução 0101 0011 0101 0010 0001 0000 01 (26 bits)

    vamos acabar no endereço [0101] 0101 0011 0101 0010 0001 0000 01{00}

    ```

- **jump and link** - `jal <target>`
    - used to call a subroutine (i.e., a function or a procedure). in addition to jumping to the target address, it also saves the return address (the address of the instruction after jal) in the `$ra` (return address) register
    - it calculates the target address in the same way as j, but it also saves the return address (PC + 4) in $ra.

- **jump register** - `jr <$register>`
    -  jumps to an address stored in a register. this is typically used to return from a subroutine when the return address is stored in the $ra register.
    - it transfers control to the address stored in the specified register (commonly $ra for subroutine return)

<br>

- **branching (I-format)** - `| op6 | rs5 | rt5 | offset16 |`

    - **branch if equal** - `beq <sourceregister1>, <sourceregister2>, <target>`
        - funciona como `if (rs == rt) branch to target`

    - **branch if not equal** - `bnq <sourceregister1>, <sourceregister2>, <target>`
        - funciona como `if (rs != rt) branch to target`

    - Ao fazermos um branch, o endereço de destino é dado por
        - **Target address = PC + offset × 4**

se quisermos efetuar outro tipo de condições, como maior e menor, temos de usar as instruções _set if less than_ ou _set if less than immediate_.
realçar que não existem instruções no hardware para efetuar saltos com comparações `>`, `<`, `>=`, `<=`, etc.

- _set if less than_: **slt rd, rs, rt**
  - Funciona como `if (rs < rt) rd = 1; else rd = 0`
- _set if less than immediate_: **slti rd, rs, constant**  
  - Funciona como `if (rs < constant) rd = 1; else rd = 0`

use in combination with beq, bne
```assembly
    slt $t0, $s1, $s2       # if ($s1 < $s2)
    bne $t0, $zero, L       # branch to L
```

<br>
<br>

apesar de um _Branch_ e um _Jump_ fazerem sensivelmente a mesma coisa, um _Jump_ refere-se a um **salto absoluto e incondicional**, enquanto que um Branch é um **salto relativo e condicional**.
para além disso, podemos não conseguir fazer saltos **muito longos**: num _jump_, temos $26$ bits, enquanto que num _branch_ temos $16$ bits, ambos inferiores aos $30$ bits necessários para endereçar todas as instruções possíveis.

caso tentemos fazer um _branch_ para uma instrução que está demasiado longe, o _assembler_ vai reescrever o nosso código com um _jump_:

```assembly
    beq $s0, $s0, L1  # L1 está muito longe!
```

o _assembler_ vai inverter a condição e inserir um _jump_:

```assembly
    bne $s0, $s1, L2  # A condição é invertida para que
    j L1              # a instrução seguinte seja o salto.
L2: ...               # L2 aponta para as instruções
                      # que se seguiam ao beq.
```
