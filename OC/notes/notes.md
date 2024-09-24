# organização de computadores
contents:
- [performance](#performance)
- [instruções](#instruções)
    - [ISA](#instruction-set-architecture-isa)
    - [RISC vs CISC](#isas)
    - [MIPS](#microprocessor-without-interlocked-pipelined-stages-mips)
        - [formato de instrução](#formatos-de-instrução-mips-32)
        - [registos](#registos)
        - [operações](#operações)

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


**example** - computer A runs a program in 10 seconds which has a 2GHz clock. computer B runs the same program in 6 seconds. computer B requires 1.2 times as many clock cycles as computer A for the program. what is computer B *clock rate* ?

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

### operações

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
0x12345678
    ...
    address X:      0x12     - most significant byte (lowest address)
    address X+1:    0x34
    address X+2:    0x56
    address X+3:    0x78     - least significant bye
    ...
```


- example
    - **g** in `$s1`, **g** in `$s2`, **base address of A** in `$s3`
    ```c
    g + h + A[8];
    ```

    ```assembly
    lw $t0, 32($s3)     # load word
    add $s1, $s2, $t0
    ```












<br>
<br>
<br>
<br>

## direct-mapped cache
- how do we know if a data item is in the cache?
    - if it is, how do we find it?

> simple cache in which the processor requests are each one word and the blocks also consist of a single word.

the simplest way to assign a location in the cache for each word in memory is to assign the cache location based on the address of the word in memory.
this cache structure is called direct mapped, since each memory location is mapped directly to exactly one location in the cache.

the typical mapping between addresses and cache locations for a direct- mapped cache is usually simple.
for example, almost all direct-mapped caches use this mapping to find a block:
$$
\text{block index} = \text{(block address)} \% \text{(number of blocks in the cache)}
$$

> if the number of entries in the cache is a power of 2, then modulo can be computed simply by using the low-order log2 (cache size in blocks) bits of the address



because each cache location can contain the contents of a number of different memory locations, how do we know whether the data in the cache corresponds to a requested word? that is, *how do we know whether a requested word is in the cache or not*? we answer this question by adding a set of **tags** to the cache.
- the tags contain the address information required to identify whether a word in the cache corresponds to the requested word. **the tag needs only to contain the upper portion of the address**, **corresponding to the bits that are not used as an index into the cache**.

we also need a way to recognize that a cache block does not have valid information. for instance, when a processor starts up, the cache does not have good data, and the tag fields will be meaningless. even after executing many instructions, some of the cache entries may still be empty. 
thus, we need to know that the tag should be ignored for such entries. the most common method is to add a valid bit to indicate whether an entry contains a valid address. 
if the bit is not set, there cannot be a match for this block.


> we need only have the upper 2 of the 5 address bits in the tag, since the lower 3-bit index field of the address selects the block. architects omit the index bits because they are redundant, since by definition the index field of any address of a cache block must be that block number.

> because there are eight words in the cache, an address X maps to the direct-mapped cache word X modulo 8.



decimal address of reference | binary address of reference | hit or miss | assigned cache block
-----------------------------|-----------------------------|-------------|---------------------
22                           | 10110                       | miss        | (10110 % 8) = 110
26                           | 11010                       | miss        | (11010 % 8) = 010
22                           | 10110                       | hit         | (10110 % 8) = 110
26                           | 11010                       | hit         | (11010 % 8) = 010
16                           | 10000                       | miss        | (10000 % 8) = 010
3                            | 00011                       | miss        | (00011 % 8) = 000

> above is a sequence of six memory references to an empty eight-block cache, including the action for each reference.

index | V | tag | data
------|---|-----|--------------
000   | Y | 10  | memory(10000)
001   | N |     |
010   | Y | 11  | memory(11010)
011   | Y | 00  | **memory(00011)**
100   | N |     |
101   | N |     |
110   | Y | 10  | memory(10110)
111   | N |     |




the index of a cache block, together with the tag contents of that block, uniquely specifies the memory address of the word contained in the cache block.
because the index field is used as an address to reference the cache, and because an n-bit field has 2n values, the total number of entries in a direct-mapped cache must be a power of 2.
in the MIPS architecture, since words are aligned to multiples of four bytes, the least significant two bits of every address specify a byte within a word.
hence, the least significant two bits are ignored when selecting a word in the block.
the **total number of bits needed for a cache is a function of the cache size and the address size**, because the cache includes both the storage for the data and the tags.
the size of the block above was one word, but normally it is several.
for the following situation:
- 32-bit addresses
- a direct-mapped cache
- the cache size is $2^n$ blocks, so $n$ bits are used for the index
- the block size is $2^m$ words ($2^{m+2}$ bytes), so $m$ bits are used for the word within the block, and two bits are used for the byte part of the address the size of the tag field is:

$$
32 - (n + m + 2)
$$

the total number of bits in a direct-mapped cache is

$$
2^{n} \times \text{(block size + tag size + valid field size)}
$$

<br>
<br>

**address**

tag             | index       | byte offset
----------------|-------------|------------
\| 31 ... 12 \| | 11 ... 2 \| | 1 . 0     \|

> tag(20), index(10), offset(2)


i    |valid | tag | data 
-----|------|-------|------
0    |      |       |     
1    |      |       |
...  |      | (20)  | (32)
1022 |      |       |
1023 |      |       |




since the block size is 2m words (2m⫹5 bits), and we need 1 bit for the valid field, the number of bits in such a cache is

$$
2^{n} \times (2^{m} \times 32 + (32 - n - m - 2) + 1)
= 2n \times (2^{m} \times 32 + 31 - n + m)
$$

> **although this is the actual size in bits, the naming convention is to exclude the size of the tag and valid field and to count only the size of the data**. Thus, the cache in is called a 4 KiB cache.

























<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>

ciclo do processador:
1. fetch: ler instrução da memória
2. decode: interpretar a instrução
3. execute: executar a instrução
4. store: armazenar o resultado

> unidade de controlo, unidade de dados, alu, registos, cache(sram), ram(dram), disco (ssd/hdd), cisc vs risc, bus, PC, MIPS, ISA, big endian. ROM RAM

complemento p 2 negativo overflow 4 bits => -8 (1000), 7 (0111)





You raise a good point! In a write-back cache policy, if you are writing to a cache line that isn't currently loaded in cache, it might seem unnecessary to load it from RAM just to overwrite it. Here's how this situation is typically handled:

Write-Allocate vs. No-Write Allocate
Write-Allocate (or Fetch on Write):

In this approach, when there's a cache miss on a write, the entire cache line is loaded from RAM into the cache before performing the write.
This is useful if subsequent reads to that memory address are expected. It optimizes for cases where data is often read after being written.
No-Write Allocate (or Write-Through):

In this approach, when there's a cache miss on a write, the write is performed directly to RAM, bypassing the cache.
This avoids loading the cache line just to overwrite it, which can be more efficient if writes are infrequent or if the written data won’t be used immediately afterward.
Summary:
Write-Allocate: Loads the cache line from RAM into cache before writing. This is beneficial if you expect to read the data soon after writing.
No-Write Allocate: Writes directly to RAM on a miss without loading the cache line first. This is more efficient when you don't expect to read the written data immediately.
The choice between these two strategies depends on the specific application and expected access patterns. In many systems, the write-allocate approach is used, but in cases where writes are more common than reads, no-write allocate can be more efficient.








