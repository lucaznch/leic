; *********************************************************************************
; Grupo 35
; Gabriel Silva, ist1102637
;
; * Ficheiro: grupo35.asm
; * Descrição: Programa do jogo "Beyond Mars"
; * Funções especiais de teclas:    0 - lança uma sonda para a esquerda
; *			                        1 - lança uma sonda para a frente
; *                                 2 - lança uma sonda para a direita
; *                                 C - inicia o jogo
; *                                 D - pausa o jogo
; *                                 E - encerra o jogo
; *       
; *********************************************************************************








; *********************************************************************************
; * Constantes
; *********************************************************************************

DEFINE_LINHA    	EQU 600AH               ; endereço do comando para definir a linha
DEFINE_COLUNA   	EQU 600CH               ; endereço do comando para definir a coluna
DEFINE_PIXEL    	EQU 6012H               ; endereço do comando para escrever um pixel
APAGA_AVISO     	EQU 6040H               ; endereço do comando para apagar o aviso de nenhum cenário selecionado
APAGA_ECRA	 	EQU 6002H               ; endereço do comando para apagar todos os pixels já desenhados
SELECIONA_FUNDO         EQU 6042H               ; endereço do comando para selecionar uma imagem de fundo
SELECIONA_SOM           EQU 605AH               ; endereço do comando para selecionar um som
RETIRA_SOM_SELECIONADO  EQU 6066H               ; endereço do comando para retirar um som
PAUSA_SOM               EQU 605EH               ; endereço do comando para pausar um som
CONTINUA_SOM            EQU 6060H               ; endereço do comando para continuar um som
DISPLAYS                EQU 0A000H              ; endereço dos displays de 7 segmentos (periférico POUT-1, 16 bits)
TEC_LIN                 EQU 0C000H              ; endereço das linhas do teclado (periférico POUT-2, 8 bits)
TEC_COL                 EQU 0E000H              ; endereço das colunas do teclado (periférico PIN, 8 bits)


NAVE_INTERIOR_COR	EQU 0F007H              ; cor do interior da nave
NAVE_FRONTEIRA_COR      EQU 0F00AH              ; cor da fronteira da nave 
MINERAVEL_COR           EQU 0F0A0H              ; cor do asteróide minerável 
NAO_MINERAVEL_COR       EQU 0FD00H              ; cor do asteróide não minerável
SONDA_COR               EQU 0FFF0H              ; cor da sonda
TRANSPARENTE            EQU 0000H               ; pixel transparente


FATOR                   EQU 1000                ; fator usado para a conversão de hexadecimal para decimal
DIVISOR                 EQU 10                  ; divisor usado para a conversão de hexadecimal para decimal
MASCARA                 EQU 0FH                 ; para isolar os 4 bits de menor peso, ao ler as colunas do teclado


OITO                    EQU 8                   ; constante usada para adicionar a um endereço, para ir a um endereço específico
DOZE                    EQU 12                  ; constante usada para adicionar a um endereço, para ir a um endereço específico
VINTE                   EQU 20                  ; constante usada para adicionar a um endereço, para ir a um endereço específico
VINTE_E_DOIS            EQU 22                  ; constante usada para adicionar a um endereço, para ir a um endereço específico
VINTE_E_QUATRO          EQU 24                  ; constante usada para adicionar a um endereço, para ir a um endereço específico


DISTANCIA_MAXIMA        EQU 12                  ; distância máxima que cada sonda pode percorrer
ULTIMA_LINHA            EQU 30                  ; última linha inicial que um asteróide pode ter


SONDA_ESQ_FRE_DIR_LIN   EQU 26                  ; linha inicial das sondas esquerda, frente e direita
SONDA_ESQ_COL           EQU 26                  ; coluna inicial da sonda esquerda
SONDA_FRE_COL           EQU 32                  ; coluna inicial da sonda frente
SONDA_DIR_COL           EQU 38                  ; coluna inicial da sonda direita



TESTE_1_COLUNA          EQU 24                  ; possível coluna inicial que um asteróide pode ter numa colisão com a nave
TESTE_2_COLUNA          EQU 31                  ; possível coluna inicial que um asteróide pode ter numa colisão com a nave
TESTE_3_COLUNA          EQU 38                  ; possível coluna inicial que um asteróide pode ter numa colisão com a nave
TESTE_4_COLUNA          EQU 28                  ; possível coluna inicial que um asteróide pode ter numa colisão com a nave
TESTE_5_COLUNA          EQU 34                  ; possível coluna inicial que um asteróide pode ter numa colisão com a nave



LINHA1                  EQU 1                   ; linha a testar (1ª linha, 0001b)
LINHA2                  EQU 2                   ; linha a testar (2ª linha, 0010b)
LINHA3                  EQU 4                   ; linha a testar (3ª linha, 0100b)
LINHA4                  EQU 8                   ; linha a testar (4ª linha, 1000b)



TECLA_E                 EQU 14                  ; valor hexadecimal da tecla E
TECLA_D                 EQU 13                  ; valor hexadecimal da tecla D
SEM_TECLA_PREMIDA       EQU 0010H               ; valor hexadecimal se nenhuma tecla premida



AST_1_COL               EQU 2                   ; coluna inicial do asteróide 1
AST_2_COL               EQU 16                  ; coluna inicial do asteróide 2
AST_3_COL               EQU 46                  ; coluna inicial do asteróide 3
AST_4_COL               EQU 59                  ; coluna inicial do asteróide 4




; *********************************************************************************
; * Zona de Dados
; *********************************************************************************


PLACE		1000H				


Pilha:
    STACK   100H                                ; espaço reservado para a pilha (200H bytes, pois são 100H words)


SP_inicial:                                     ; este é o endereço (1200H) com que o SP será incializado
                                                ; o primeiro endereço de retorno será 11FEH (1200H - 2)


Tab:                                            ; tabela das rotinas de interrupção
    WORD    int_asteroide
    WORD    int_sonda
    WORD    int_display
    WORD    int_nave


Evento_Int_Asteroide:
    WORD    0                                   ; se 1 indica que ocorreu interrupção


Evento_Int_Sonda:
    WORD    0                  


Evento_Int_Display:
    WORD    0                 


Evento_Int_Nave:
    WORD    0               



; *******************************************************************************************************************
; *	Tabelas que defines os objetos: Nave, Asteróides e Sondas                                                       *
; *******************************************************************************************************************
NaveFronteira:
    WORD    27                                  ; linha incial
    WORD    27                                  ; coluna inicial
    WORD    5                                   ; altura
    WORD    11                                  ; comprimento da primeira linha
    WORD    13                                  ; comprimento da segunda linha
    WORD    26                                  ; coluna incial da segunda linha
    WORD    15                                  ; comprimento da terceira linha
    WORD    25                                  ; coluna incial da terceira linha
    WORD    15                                  ; comprimento da quarta linha
    WORD    25                                  ; coluna incial da quarta linha
    WORD    15                                  ; comprimento da quinta e última linha
    WORD    25                                  ; coluna incial da quinta linha


NaveInterior:
    WORD    28                                  ; linha incial
    WORD    27                                  ; coluna inicial
    WORD    4                                   ; altura
    WORD    11                                  ; comprimento da primeira linha
    WORD    13                                  ; comprimento da segunda linha
    WORD    26                                  ; coluna incial da segunda linha
    WORD    13                                  ; comprimento da terceira linha
    WORD    26                                  ; coluna incial da terceira linha
    WORD    13                                  ; comprimento da quarta linha
    WORD    26                                  ; coluna incial da quarta linha


NaveVariacao:
    WORD    29                                  ; linha incial
    WORD    31                                  ; coluna inicial
    WORD    3                                   ; altura
    WORD    3                                   ; comprimento da primeira linha
    WORD    5                                   ; comprimento da segunda linha
    WORD    30                                  ; coluna incial da segunda linha
    WORD    3                                   ; comprimento da terceira linha
    WORD    31                                  ; coluna incial da terceira linha


Asteroide1:
    WORD    0                                   ; 1 é minerável, 0 é não minerável
    WORD    1                                   ; linha inicial
    WORD    2                                   ; coluna inicial
    WORD    5                                   ; altura
    WORD    3                                   ; comprimento da primeira linha
    WORD    5                                   ; comprimento da segunda linha
    WORD    1                                   ; coluna inicial da segunda linha
    WORD    5                                   ; comprimento da terceira linha
    WORD    1                                   ; coluna inicial da terceira linha
    WORD    5                                   ; comprimento da quarta linha
    WORD    1                                   ; coluna inicial da quarta linha
    WORD    3                                   ; comprimento da quinta e última linha
    WORD    2                                   ; coluna inicial da quinta linha
    WORD    0                                   ; 1 é diagonável, 0 é não diagonável
    WORD    0                                   ; 0 desce diagonalmente pela esquerda, 1 pela direita ... (esta informação só é usada caso seja diagonal!)


Asteroide2:
    WORD    1       
    WORD    1    
    WORD    16    
    WORD    5    
    WORD    3    
    WORD    5     
    WORD    15     
    WORD    5    
    WORD    15   
    WORD    5    
    WORD    15     
    WORD    3    
    WORD    16    
    WORD    0
    WORD    0


Asteroide3:
    WORD    0
    WORD    1    
    WORD    46     
    WORD    5    
    WORD    3   
    WORD    5     
    WORD    45    
    WORD    5      
    WORD    45    
    WORD    5    
    WORD    45     
    WORD    3       
    WORD    46   
    WORD    0
    WORD    0


Asteroide4:
    WORD    0
    WORD    1   
    WORD    59   
    WORD    5   
    WORD    3    
    WORD    5    
    WORD    58     
    WORD    5     
    WORD    58      
    WORD    5      
    WORD    58     
    WORD    3       
    WORD    59      
    WORD    0
    WORD    0       


SondaEsquerda:
    WORD    12                                  ; distância máxima que pode percorrer
    WORD    26                                  ; linha inicial
    WORD    26                                  ; coluna incial


SondaFrente:
    WORD    12
    WORD    26
    WORD    32


SondaDireita:
    WORD    12
    WORD    26
    WORD    38




SondaEsquerdaStatus:
    WORD    0                                   ; 0 => sonda desativada, 1 => sonda ativada


SondaFrenteStatus:
    WORD    0                                  


SondaDireitaStatus:
    WORD    0                                  


ValorDisplay:                                   ; valor em hexadecimal mostrado no display
    WORD    64H                                 ; 64H convertido a decimal é 100



CoresNaveVariacao:                              ; conjunto de cores possíveis para variar a nave
    WORD    0FF00H
    WORD    0F0F0H
    WORD    0F00FH
    WORD    0FFF0H
    WORD    0FF0FH
    WORD    0F0FFH

Colunas_Aleatorias:                             ; conjunto de colunas possíveis para variar as colunas dos asteróides
    WORD    3
    WORD    13
    WORD    21
    WORD    31
    WORD    41
    WORD    49
    WORD    59





; *********************************************************************************
; * Código
; *********************************************************************************

PLACE   0				                        ; o código tem de começar em 0000H


inicio_programa:
    MOV  SP, SP_inicial                         ; incializar SP
    MOV  BTE, Tab                               ; incializar BTE
    MOV  [APAGA_AVISO], R1                      ; apaga o aviso de nenhum cenário selecionado (o valor de R1 não é relevante)
    MOV  [APAGA_ECRA], R1                       ; apaga todos os pixels já desenhados (o valor de R1 não é relevante)
    
    MOV  R1, 0                                  
    MOV  R2, DISPLAYS                           ; endereço dos displays de 7 segmentos
    MOV  [R2], R1                               ; incializar os displays a 0
    MOV  [SELECIONA_FUNDO], R1                  ; inserir o fundo número 0, *prima C para começar*

iniciar_espera_tecla_C:
    MOV  R2, TEC_LIN                            ; endereço do periférico das linhas
    MOV  R3, TEC_COL                            ; endereço do periférico das colunas
    MOV  R5, MASCARA                            ; para isolar os 4 bits de menor peso, ao ler as colunas do teclado

espera_tecla_C:                                 ; neste ciclo espera-se até a tecla C ser premida (linha 4, coluna 1) para iniciar o jogo
    MOV  R1, LINHA4                             ; testar a linha 4
    MOVB [R2], R1                               ; escrever no periférico de saída (linhas)
    MOVB R0, [R3]                               ; ler do periférico de entrada (colunas)
    AND  R0, R5                                 ; elimina bits para além dos bits 0-3
    CMP  R0, 1                                  ; há tecla C premida?
    JNZ  espera_tecla_C                         ; se não houver, voltar a repetir processo
inicio_jogo:                                    ; tecla C foi premida, logo o jogo começa
    MOV  R0, ValorDisplay                       ; endereço da variável que tem o valor dos Displays
    MOV  R1, 64H                                
    MOV  [R0], R1                               ; inicializar o valor da variável que tem o valor dos Displays a 64H (que convertido a decimal é 100)
    MOV  R2, DISPLAYS                           ; endereço dos displays de 7 segmentos
    MOV  R1, 100H                               
    MOV  [R2], R1                               ; inicializar os displays a 100 (total de energia da nave)
    MOV  R1, 1                  
    MOV  [SELECIONA_FUNDO], R1                  ; inserir o fundo número 1, *background do jogo*
    MOV  R1, 4  
    MOV  [SELECIONA_SOM], R1                    ; inserir o som número 4, *background sound*


                                                ; R9 e R11 são registos especiais do programa que só são alterados por rotinas específicas.
    MOV  R9, 0                                  ; R9 guarda 0, 1, 2, 3 ou 4. E é usado no corpo principal do programa para comparar com os valores 1, 2, 3 e 4.
                                                ; 4 = indica que um asteróide colidiu com nave (game over)
                                                ; 3 = indica que o valor de energida da nave chegou ao fim (game over)
                                                ; 2 = tecla D foi premida => pausar o jogo
                                                ; 1 = tecla E foi premida => desistir o jogo
                                                ; 0 = tudo normal!
    MOV  R11, 0                                 ; R11 guarda o valor, em hexadecimal, da tecla premida cada vez que passsa pela rotina do teclado
                                                ; e se não houver tecla premida guarda o valor da constante SEM_TECLA_PREMIDA
    
    
    CALL preparar_nave_desenho                  ; desenhar a nave

    
resetar_sistema:                                ; caso o jogo reinicie, temos que resetar o sistema
    MOV  R1, 1                                  ; linha 1
    MOV  R0, Asteroide1                         ; endereço da tabela que define o asteróide 1
    ADD  R0, 2                                  ; próximo endereço (da variável que guarda a linha incial do asteróide)
    MOV  [R0], R1                               ; atualizar a linha inicial para a linha 1
    MOV  R4, AST_1_COL                          ; coluna inicial do asteróide 1
    CALL acertar_colunas                        ; acertar colunas na tabela que o define
    ADD  R0, 2                                  ; próximo endereço (da variável que diz a informação da diagonalidade)
    MOV  R2, 0                                  
    MOV  [R0], R2                               ; atualizar informação de diagonalidade a 0 (não diagonal)
    
    MOV  R0, Asteroide2                         ; repetir processo 
    ADD  R0, 2
    MOV  [R0], R1
    MOV  R4, AST_2_COL
    CALL acertar_colunas
    ADD  R0, 2
    MOV  R2, 0
    MOV  [R0], R2

    MOV  R0, Asteroide3                         ; repetir processo 
    ADD  R0, 2
    MOV  [R0], R1
    MOV  R4, AST_3_COL
    CALL acertar_colunas
    ADD  R0, 2
    MOV  R2, 0
    MOV  [R0], R2

    MOV  R0, Asteroide4                         ; repetir processo 
    ADD  R0, 2
    MOV  [R0], R1
    MOV  R4, AST_4_COL
    CALL acertar_colunas
    ADD  R0, 2
    MOV  R2, 0
    MOV  [R0], R2


    MOV  R1, DISTANCIA_MAXIMA                   ; distância máxima que cada sonda pode percorrer
    MOV  R2, SONDA_ESQ_FRE_DIR_LIN              ; linha inicial da sonda esquerda, frente e direita

    MOV  R0, SondaEsquerda                      ; endereço da tabela que define a sonda esquerda
    MOV  [R0], R1                               ; atualizar distância da sonda em distância máxima
    ADD  R0, 2                                  ; próximo endereço
    MOV  [R0], R2                               ; atualizar linha inicial da sonda esquerda
    MOV  R3, SONDA_ESQ_COL                      ; coluna inicial da sonda esquerda
    ADD  R0, 2                                  ; próximo endereço
    MOV  [R0], R3                               ; atualizar coluna inicial da sonda esquerda
    MOV  R0, SondaEsquerdaStatus                ; endereço da variável que tem o status da sonda esquerda
    MOV  R3, 0                  
    MOV  [R0], R3                               ; atualizar variável que tem o status da sonda esquerda como 0 (desativada)

    MOV  R0, SondaFrente                        ; repetir processo
    MOV  [R0], R1
    ADD  R0, 2
    MOV  [R0], R2
    MOV  R3, SONDA_FRE_COL
    ADD  R0, 2
    MOV  [R0], R3
    MOV  R0, SondaFrenteStatus
    MOV  R3, 0
    MOV  [R0], R3

    MOV  R0, SondaDireita                       ; repetir processo 
    MOV  [R0], R1
    ADD  R0, 2
    MOV  [R0], R2
    MOV  R3, SONDA_DIR_COL
    ADD  R0, 2
    MOV  [R0], R3
    MOV  R0, SondaDireitaStatus
    MOV  R3, 0
    MOV  [R0], R3


                                                ; desenhar os 4 quatro asteróides
    MOV  R10, Asteroide1                        ; endereço da tabela que define o asteróide1
    CALL desenhar_asteroide                     ; desenhar
    MOV  R10, Asteroide2                        
    CALL desenhar_asteroide
    MOV  R10, Asteroide3
    CALL desenhar_asteroide
    MOV  R10, Asteroide4
    CALL desenhar_asteroide

    EI0                                         ; permite interrupções 0
    EI1                                         ; permite interrupções 1
    EI2                                         ; permite interrupções 2
    EI3                                         ; permite interrupções 3
    EI                                          ; permite interrupções gerais



; *******************************************************************************************************************
; *	CORPO PRINCIPAL DO PROGRAMA - chama rotinas cooperativas e faz verificações                                     *
; *																		                                            *
; * zona principal do programa onde todas as interrupções são atendidas e todas as ações das teclas são atendidas   *
; *                                                                                                                 *
; *******************************************************************************************************************
main:
    CALL teclado

    CALL verificar_teclado

    CALL animar_asteroides

    CALL animar_sondas

    CALL animar_display

    CALL animar_nave

    CMP  R9, 1                                  ; premiu-se a tecla E ?
    JZ   desistir_jogo

    CMP  R9, 2                                  ; premiu-se a tecla D ?
    JZ   pausa_jogo

    CMP  R9, 3                                  ; a nave ficou sem energia ?
    JZ   sem_energia

    CMP  R9, 4                                  ; a nave explodiu ?
    JZ   nave_explodiu

    JMP  main




; *******************************************************************************************************************
; *	Desistir Jogo - apaga todos os pixeis, muda fundo, e volta ao loop no inicio do programa de espera da tecla C   *
; *******************************************************************************************************************
desistir_jogo:                                  ; tecla E foi premida, temos que desistir o jogo
    DI                                          ; não permitir interrupções
    MOV  R9, 0                                  ; colocar R9 a zero, pois estamos a atender a tecla E que tinha R9 a 1
    MOV  R1, 4
    MOV  [RETIRA_SOM_SELECIONADO], R1           ; retira o som número 4, *background sound*
    MOV  [APAGA_ECRA], R1                       ; apaga todos os pixels já desenhados (o valor de R1 não é relevante)
    MOV  R1, 2 
    MOV  [SELECIONA_FUNDO], R1                  ; inserir o fundo número 2 *Game Over, Desistiu, C para reiniciar*
    MOV  [SELECIONA_SOM], R1                    ; inserir o som número 2
    JMP  iniciar_espera_tecla_C                 ; ir para o loop do início do jogo onde se espera pela tecla C para iniciar o jogo




; *******************************************************************************************************************
; *	Pausar Jogo - muda fundo e espera-se que a tecla C seja premida para voltar ao jogo                             *
; *******************************************************************************************************************
pausa_jogo:                                     ; tecla D for premida, temos que pausar o jogo
    DI                                          ; não permitir interrupções gerais
    MOV  R9, 0                                  ; colocar R9 a zero, pois estamos a atender a tecla D que tinha R9 a 2
    MOV  R1, 4
    MOV  [PAUSA_SOM], R1                        ; pausar o som número 4, *background sound*
    MOV  R1, 5                                  ; 
    MOV  [SELECIONA_FUNDO], R1                  ; inserir o fundo número 5 *Pausa, C para reiniciar*

    MOV  R2, TEC_LIN                            ; endereço do periférico das linhas
    MOV  R3, TEC_COL                            ; endereço do periférico das colunas
    MOV  R5, MASCARA                            ; para isolar os 4 bits de menor peso, ao ler as colunas do teclado

pausa_loop:                                     ; neste ciclo espera-se até a tecla C ser premida para voltar ao jogo (linha 4, coluna 1)
    MOV  R1, LINHA4                             ; testar a linha 4
    MOVB [R2], R1                               ; escrever no periférico de saída (linhas)
    MOVB R0, [R3]                               ; ler do periférico de entrada (colunas)
    AND  R0, R5                                 ; elimina bits para além dos bits 0-3
    CMP  R0, 1                                  ; há tecla C premida?
    JNZ  pausa_loop                             ; se não houver, voltar a repetir processo
    EI                                          ; voltar a permitir interrupções gerais
    MOV  R1, 1                  
    MOV  [SELECIONA_FUNDO], R1                  ; inserir o fundo número 1, *background do jogo*
    MOV  R1, 4
    MOV  [CONTINUA_SOM], R1                     ; continuar o som número 4 que foi pausado, *background sound*
    JMP  main                                   ; voltar ao ciclo do corpo principal do programa



; *******************************************************************************************************************
; *	Sem Energia - apaga todos os pixeis, muda fundo e volta ao loop no início do programa de espera da tecla C      *
; *******************************************************************************************************************
sem_energia:
    DI                                          ; não permitir interrupções gerais
    MOV  R9, 0
    MOV  [APAGA_ECRA], R1                       ; apagar todos os pixeis desenhados
    MOV  R1, 4
    MOV  [RETIRA_SOM_SELECIONADO], R1           ; retirar o som número 4, *background sound*
    MOV  R1, 3
    MOV  [SELECIONA_FUNDO], R1                  ; selecionar o fundo número 3, *game over sem energia*
    MOV  R1, 6
    MOV  [SELECIONA_SOM], R1                    ; selecionar som número 6, *no energy*
    JMP  iniciar_espera_tecla_C                 ; ir para o loop inicial do programa onde se espera pela tecla C para iniciar o jogo




; *******************************************************************************************************************
; *	Nave Explodiu - apaga todos os pixeis, muda fundo e volta ao loop no início do programa de espera da tecla C      *
; *******************************************************************************************************************
nave_explodiu:
    DI                                          ; não permitir interrupções gerais
    MOV  R9, 0
    MOV  [APAGA_ECRA], R1                       ; apagar todos os pixeis desenhados
    MOV  R1, 4
    MOV  [SELECIONA_FUNDO], R1                  ; selecionar o fundo número 4, *game over nave explodiu*
    MOV  [RETIRA_SOM_SELECIONADO], R1           ; retira o som número 4, *background sound*
    MOV  R1, 1
    MOV  [SELECIONA_SOM], R1                    ; seleciona o som número 1, *explosão*
    JMP  iniciar_espera_tecla_C                 ; ir para o loop inicial do programa onde se espera pela tecla C para iniciar o jogo





; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -





; ************************************************************************************************
; * Teclado - rotina cooperativa que deteta a tecla premida do teclado e converte em hexadecimal *
; *                                                                                              *
; * argumentos:             nenhum                                                               *
; * return:                 R11 - tecla premida em hexadecimal ou constante SEM_TECLA_PREMIDA    *
; *                                                                                              *
; * de notar que durante a rotina os registos                                                    *
; *         - R1 guarda a linha testada (1, 2, 4, 8)                                             *
; *         - R0 guarda a coluna lida (0, 1, 2, 4, 8)                                            *
; ************************************************************************************************
teclado:
    MOV  R2, TEC_LIN                            ; endereço do periférico das linhas
    MOV  R3, TEC_COL                            ; endereço do periférico das colunas
    MOV  R4, DISPLAYS                           ; endereço do periférico dos displays
    MOV  R5, MASCARA                            ; para isolar os 4 bits de menor peso, ao ler as colunas do teclado

    MOV  R1, LINHA4                             ; testar a linha 4
    MOVB [R2], R1                               ; escrever no periférico de saída (linhas)
    MOVB R0, [R3]                               ; ler do periférico de entrada (colunas)
    AND  R0, R5                                 ; elimina bits para além dos bits 0-3
    CMP  R0, 0                                  ; há tecla premida?
    JNZ  sai_teclado

    MOV  R1, LINHA3                             ; testar a linha 3 repetindo o processo
    MOVB [R2], R1
    MOVB R0, [R3]
    AND  R0, R5
    CMP  R0, 0
    JNZ  sai_teclado

    MOV  R1, LINHA2                             ; testar a linha 2 repetindo o processo
    MOVB [R2], R1
    MOVB R0, [R3]
    AND  R0, R5
    CMP  R0, 0
    JNZ  sai_teclado
    
    MOV  R1, LINHA1                             ; testar a linha 1 repetindo o processo
    MOVB [R2], R1
    MOVB R0, [R3]
    AND  R0, R5
    CMP  R0, 0
    JNZ  sai_teclado

    MOV  R11, SEM_TECLA_PREMIDA                 ; nenhuma tecla foi premida
    RET                                         ; podemos sair



; temos que converter a tecla premida no valor hexadecimal correspondente, fórumula: (Linha * 4) + Coluna
; em que Linha e Coluna é necessário converter o valor da linha e da coluna na sua posição entre 0- 3
sai_teclado:                                    
    MOV  R2, 0                                  ; contador que guardará a posição da linha entre 0 - 3
    MOV  R3, 0                                  ; contador que guardará a posição da coluna entre 0 - 3
converter_linha:                                ; vamos começar por converter a linha
    SHR  R1, 1                                  ; contar quantos SHR são possíveis incrementando o contador, até ser 0
    JNZ  incrementar_linha                      
converter_coluna:                               ; mesmo processo
    SHR  R0, 1
    JNZ  incrementar_coluna

converter:                                      ; ja tendo convertido a linha e a coluna
    SHL  R2, 2                                  ; Linha * 4
    ADD  R2, R3                                 ; (Linha * 4) + Coluna
    MOV  R11, R2                                ; guardar o valor da tecla premida em hexadecimal em R11
    RET                                         ; sair


incrementar_linha:                              ; funções auxiliares para incrementar os contadores
    ADD  R2, 1
    JMP  converter_linha
incrementar_coluna:
    ADD  R3, 1
    JMP  converter_coluna





; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -





; ****************************************************************************************************************************
; * Verificar Teclado - rotina cooperativa que verifica, se uma tecla foi premida, se foi uma das teclas com funcionalidades *
; *                                                                                                                          *
; * argumentos:             nenhum                                                                                           *
; * return:                 nenhum                                                                                           *
; *                                                                                                                          *
; *  R11 guarda o valor hexadecimal da tecla premida ou a constante SEM_TECLA_PREMIDA                                        *
; *                                                                                                                          *
; ****************************************************************************************************************************
verificar_teclado:
    CMP  R11, 0                                 ; premiu-se a tecla 0? (disparar sonda esquerda)
    JZ   tecla_0                                ; se sim, atender

    CMP  R11, 1                                 ; premiu-se a tecla 1? (disparar sonda frente)
    JZ   tecla_1                                ; se sim, atender

    CMP  R11, 2                                 ; premiu-se a tecla 2? (disparar sonda direita)
    JZ   tecla_2                                ; se sim, atender

    MOV  R1, TECLA_D                        
    CMP  R11, R1                                ; premiu-se a tecla D? (pausar o jogo)
    JZ   tecla_D                                ; se sim, atender

    MOV  R1, TECLA_E
    CMP  R11, R1                                ; premiu-se a tecla E? (desistir o jogo)
    JZ   tecla_E                                ; se sim, atender
    RET




; *************************************************************************************************************************************************
; *	Tecla 0 - rotina que verifica o status da sonda esquerda (ativada ou desativada), e se estiver desativada então a sonda é disparada e ativada *
; * argumentos:             nenhum                                                                                                                *
; * return:                 nenhum                                                                                                                *
; *************************************************************************************************************************************************
tecla_0:
    MOV  R0, SondaEsquerdaStatus                ; endereço da variável com o status
    MOV  R1, [R0]                               ; status da sonda
    CMP  R1, 0                                  ; status = 0? isto é, sonda desativada?
    JNZ  sair_tecla_0                           ; se status = 1, isto é sonda ativada, sair!
    MOV  R1, 1                                  ; caso esteja desativado
    MOV  [R0], R1                               ; atualizar status para ativado (=1)

    MOV  R0, SondaEsquerda                      ; endereço da tabela que define a sonda esquerda
    MOV  R1, [R0]                               ; obter a distância que sonda pode percorrer
    SUB  R1, 1                                  ; diminuir por 1 unidade
    MOV  [R0], R1                               ; atualizar distância que sonda pode percorrer com nova distância

    CALL desenhar_sonda                         ; desenhar sonda
    CALL diminuir_energia                       ; diminuir energia da nave
    MOV  R1, 0              
    MOV  [SELECIONA_SOM], R1                    ; inserir o som número 0 de disparo da sonda
sair_tecla_0:
    RET




; *************************************************************************************************************************************************
; *	Tecla 1 - rotina que verifica o status da sonda frente (ativada ou desativada), e se estiver desativada então a sonda é disparada e ativada   *
; * argumentos:             nenhum                                                                                                                *
; * return:                 nenhum                                                                                                                *
; *************************************************************************************************************************************************
tecla_1:
    MOV  R0, SondaFrenteStatus                  ; endereço da variável com o status
    MOV  R1, [R0]                               ; status da sonda
    CMP  R1, 0                                  ; status == 0, isto é, sonda desativada?
    JNZ  sair_tecla_1                           ; se status = 1, isto é sonda ativada, sair!
    MOV  R1, 1                                  ; R1 = ativado
    MOV  [R0], R1                               ; atualizar status para ativado

    MOV  R0, SondaFrente                        ; endereço da tabela que define a sonda esquerda
    MOV  R1, [R0]                               ; obter a distância que sonda pode percorrer
    SUB  R1, 1                                  ; diminuir por 1 unidade
    MOV  [R0], R1                               ; atualizar distância que sonda pode percorrer com nova distância

    CALL desenhar_sonda                         ; desenhar sonda
    CALL diminuir_energia                       ; diminuir energia da nave
    MOV  R1, 0
    MOV  [SELECIONA_SOM], R1                    ; inserir o som número 0 de disparo da sonda
sair_tecla_1:
    RET




; *************************************************************************************************************************************************
; *	Tecla 2 - rotina que verifica o status da sonda direita (ativada ou desativada), e se estiver desativada então a sonda é disparada e ativada  *
; * argumentos:             nenhum                                                                                                                *
; * return:                 nenhum                                                                                                                *
; *************************************************************************************************************************************************
tecla_2:
    MOV  R0, SondaDireitaStatus                 ; endereço da variável com o status
    MOV  R1, [R0]                               ; status da sonda
    CMP  R1, 0                                  ; status == 0, isto é, sonda desativada?
    JNZ  sair_tecla_2                           ; se status = 1, isto é sonda ativada, sair!
    MOV  R1, 1                                  ; R1 = ativado
    MOV  [R0], R1                               ; atualizar status para ativado

    MOV  R0, SondaDireita                       ; endereço da tabela que define a sonda esquerda
    MOV  R1, [R0]                               ; obter a distância que sonda pode percorrer
    SUB  R1, 1                                  ; diminuir por 1 unidade
    MOV  [R0], R1                               ; atualizar distância que sonda pode percorrer com nova distância

    CALL desenhar_sonda                         ; desenhar sonda
    CALL diminuir_energia                       ; diminuir energia da nave
    MOV  R1, 0
    MOV  [SELECIONA_SOM], R1                    ; inserir o som número 0 de disparo da sonda
sair_tecla_2:
    RET




; *******************************************************************************************************************
; *	Tecla D - rotina que coloca R9 a 2, para o corpo principal do programa saber que a tecla D for premida          *
; * argumentos:             nenhum                                                                                  *
; * return:                 R9                                                                                      *
; *******************************************************************************************************************
tecla_D:
    MOV  R9, 2
    RET




; *******************************************************************************************************************
; *	Tecla E - rotina que coloca R9 a 1, para o corpo principal do programa saber que a tecla E for premida          *
; * argumentos:             nenhum                                                                                  *
; * return:                 R9                                                                                      *
; *******************************************************************************************************************
tecla_E:
    MOV  R9, 1
    RET





; *******************************************************************************************************************
; *	Diminuir Energia - rotina que diminui a energia da nave a 5 %                                                   *
; * argumentos:             nenhum                                                                                  *
; * return:                 nenhum                                                                                      *
; *******************************************************************************************************************
diminuir_energia:
    MOV  R1, ValorDisplay                       ; endereço da variável que tem o valor dos displays
    MOV  R0, [R1]                               ; valor
    CMP  R0, 4                                  ; valor dos display é menor ou igual a 4 ?
    JLE  sair_diminuir_energia                  ; se sim não decrementamos, pois fica uma valor negativo e não queremos.
                                                ; se não, decrementar
    SUB  R0, 5                                  ; valor menos 5
    MOV  [R1], R0                               ; atualizar valor
    CALL decimal                                ; converter valor para decimal
    MOV  R1, DISPLAYS                           ; endereço dos displays
    MOV  [R1], R0                               ; meter novo valor nos displays
sair_diminuir_energia:
    RET






; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -





; ************************************************************************************************
; * Animar Asteroides - rotina cooperativa que movimenta os asteroides                           *
; *                                                                                              *
; * argumentos:             nenhum                                                               *
; * return:                 nenhum                                                               *
; *                                                                                              *
; ************************************************************************************************
animar_asteroides:
    MOV  R0, Evento_Int_Asteroide               ; endereço da variável que diz se ocorreu interrupção desejada
    MOV  R1, [R0]                               ; valor da variável (0 ocorreu, 1 não ocorreu)
    CMP  R1, 0                                  ; não ocorreu interrupção ?
    JZ   sair_animar_asteroides                 ; se não, sair! (não se anima nada) 
                                                ; se sim, animar
    MOV  R1, 0                                  
    MOV  [R0], R1                               ; atualizar variável com 0 (interrupção atendida)
asteroide1:
    MOV  R10, Asteroide1                        ; endereço do asteróide1
    CALL limpar_asteroide                       ; apagar asteróide
    CALL descer_asteroide                       ; atualizar coordenadas e dados do asteróide para ele descer 1 linha
    CALL desenhar_asteroide                     ; desenhar asteroide com as novas coordenadas e dados
asteroide2:                                     ; repetir processo para o asteróide2
    MOV  R10, Asteroide2
    CALL limpar_asteroide
    CALL descer_asteroide
    CALL desenhar_asteroide
asteroide3:                                     ; repetir processo para o asteróide3
    MOV  R10, Asteroide3
    CALL limpar_asteroide
    CALL descer_asteroide
    CALL desenhar_asteroide
asteroide4:                                     ; repetir processo para o asteróide4
    MOV  R10, Asteroide4
    CALL limpar_asteroide
    CALL descer_asteroide
    CALL desenhar_asteroide
sair_animar_asteroides:
    RET




; ***********************************************************************************************************************
; * Descer Asteróide - rotina que atualiza coordenadas do asteróide que faz ele descer ou nascer no topo outra vez      *
; *                                                                                                                     *
; * argumentos:             R10 - endereço da tabela que define o asteróide                                             *
; * return:                 nenhum                                                                                      *
; *                                                                                                                     *
; ***********************************************************************************************************************
descer_asteroide:
    PUSH R0
    MOV  R0, R10                                ; endereço da tabela que define o asteróide
    ADD  R0, 2                                  ; próximo endereço
    MOV  R1, [R0]                               ; linha inicial
    MOV  R2, ULTIMA_LINHA                       ; última linha inicial possível para os asteróides
    CMP  R1, R2                                 ; a linha inicial do asteróide é a última linha inicial possível?
    JZ   resetar_asteroide                      ; se sim, temos que resetar o asteróide (move-lo para o topo outra vez)
    JMP  finalizar_descer_asteroide             ; ; se não, atualizamos as suas coordenadas para se mover
resetar_asteroide:                              ; para resetar o asteróide, ele nasce de volta na linha 1 e numa coluna aleatória
    MOV  R1, 1                                  ; linha 1
    MOV  [R0], R1                               ; atualizar linha inicial do asteróide para a linha 1

    CALL numero_aleatorio                       ; gera número aleatório em R2 = {0, 1, ..., 14, 15}
    MOV  R3, 7
    MOD  R2, R3                                 ; R2 = {0, 1, 2, 3, 4, 5, 6}
    SHL  R2, 1                                  ; R2 = {0, 2, 4, 6, 8, 10, 12}
    MOV  R3, Colunas_Aleatorias                 ; endereço da tabela que contém 7 colunas possíveis para nascerem asteróides
    ADD  R3, R2                                 ; obter endereço aleatório
    MOV  R4, [R3]                               ; indexar a tabela obtendo uma coluna aleatória

    CALL acertar_colunas                        ; atualizar colunas do asteróide na tabela que o define

                                                ; agora falta resetar informação de diagonalidade, que faremos aleatoriamente também
    ADD  R0, 2                                  ; próximo endereço que guarda informação se é ou não diagonável
    
                                                ; MAS antes, há uma exceção: se a coluna aleatoria é uma coluna do canto então TEM que ser diagonal !!!!!!!
    CMP  R2, 0                                  ; R2 = 0 ? foi para o primeiro índice da tabela com colunas aleatórias (tem a coluna do canto esquerdo) ?
    JZ   canto_esquerdo                         ; se sim, ele é obrigatoriamente diagonável e cai para a direita
    MOV  R1, DOZE
    CMP  R2, R1                                 ; R2 = 12 ? foi para o último índice (tem a coluna do canto direito) ?
    JZ   canto_direito                          ; se sim, ele é obrigatoriamente diagonável e cai para a esquerda
                                                ; se não, escolher aleatoriamente se é ou não diagonável
    
    CALL numero_aleatorio                       ; R2 = {0, 1, ..., 14, 15}
    MOV  R3, 2
    MOD  R2, R3                                 ; R2 = {0, 1} em que 0 é não diagonável, e 1 é!
    
    CMP  R2, 1                                  ; é diagonável?
    JZ   e_diagonavel                           ; se sim, saltar e temos ainda que escolher aleatoriamente se cai esquerda ou direita

    MOV  R1, 0                                  ; se não
    MOV  [R0], R1                               ; atualizar info de diagonalidade como não diagonável e terminamos
    JMP  finalizado     

e_diagonavel:                                   ; caso seja diagonável, temos que escolher aleatoriamente se cai para a esquerda ou direita
    MOV  R1, 1                                  
    MOV  [R0], R1                               ; atualizar info de diagonalidade como diagonável
    CALL numero_aleatorio                       ; R2 = {0, 1, ..., 14, 15}
    MOD  R2, R3                                 ; R2 = {0, 1} em que 0 cai pela esquerda e 1 cai pela direita
    ADD  R0, 2                                  ; próximo endereço
    MOV  [R0], R2                               ; atualizar informação do lado que cai
    JMP  finalizado                            

canto_esquerdo:                                 ; se a coluna aleatória foi a do canto esquerdo
    MOV  R1, 1                      
    MOV  [R0], R1                               ; atualizar informação de diagonalidade como diagonável (=1) 
    ADD  R0, 2
    MOV  [R0], R1                               ; atualizar informacação do lado que cai como direito (=1)
    JMP  finalizado

canto_direito:                                  ; se a coluna aleatória foi a do canto direito
    MOV  R1, 1
    MOV  [R0], R1                               ; atualizar informação de diagonalidade como diagonável (=1)
    ADD  R0, 2
    MOV  R1, 0                          
    MOV  [R0], R1                               ; atualizar informação do lado que cai como esquerdo (=0)
    JMP  finalizado

finalizar_descer_asteroide:                     ; se linha inicial do asteróide != última linha inicial possível dos asteróides, ele tem que DESCER
    ADD  R1, 1                                  ; linha inicial = linha inicial + 1
    MOV  [R0], R1                               ; atualizar nova linha inicial do asteróide
    ADD  R0, 2                                  ; próximo endereço
    MOV  R4, [R0]                               ; coluna inicial do asteróide
    MOV  R2, VINTE_E_DOIS                       
    ADD  R0, R2                                 ; saltar 11 endereços
    MOV  R2, [R0]                               ; informação se asteróide desce diagonalmente
    CMP  R2, 0                                  ; é Não Diagonal ??
    JZ   quase_finalizado                       ; se sim, as coordenadas já foram atualizadas para descer, Sair!!!
                                                ; se não, ele é diagonável, atualizar colunas também!
    MOV  R0, R10                                ; endereço da tabela que define o asteróide
    ADD  R0, 4                                  ; saltar 2 endereços
    MOV  R4, [R0]                               ; coluna inicial do asteróide
                                                
                                                ; asteróide desce diagonalmente, falta saber se desce pela direita ou esquerda
    MOV  R1, R0                                 ; cópia do endereço do asteróide
    MOV  R2, VINTE_E_QUATRO                     
    ADD  R1, R2                                 ; saltar 12 endereços
    MOV  R2, [R1]                               ; informação se desce diagonalmente pela esquerda ou direita
    CMP  R2, 1                                  ; desce diagonalmente pela direita?
    JZ   descer_diagonalmente_direita           ; se sim...
descer_diagonalmente_esquerda:
    SUB  R4, 1                                  ; se não, ir para a coluna de trás
    JMP  acertar_todas_colunas
descer_diagonalmente_direita:
    ADD  R4, 1                                  ; ...ir para a coluna seguinte
acertar_todas_colunas:
    SUB  R0, 2                                  ; ir 1 endereço para trás (variável da linha do asteróide)
    CALL acertar_colunas                        ; acertar colunas do asteróide na tabela que o define
quase_finalizado:                               ; já tendo as coordenadas do asteróide atualizadas fazendo ele descer
    CALL verifica_colisao_com_nave              ; falta verificar se nessas coordenadas ele colide com a nave
finalizado:
    POP  R0
    RET




; ***********************************************************************************************************************
; * Acertar Coluna - rotina que atualiza colunas iniciais da linha 1, 2, 3, 4 e 5 do asteróide                          *
; *                                                                                                                     *
; * argumentos:             R4 - coluna inicial da primeira e última linha do asteróide                                 *
; *                         R0 - endereço da tabela que define o asteróide                                              *
; * return:                 nenhum                                                                                      *
; *                                                                                                                     *
; ***********************************************************************************************************************
acertar_colunas:
    MOV  R5, R4                                 ; coluna inicial do asteróide
    SUB  R5, 1                                  ; coluna anterior à coluna inicial
    MOV  R6, OITO                               

    ADD  R0, 2                                  ; próximo endereço
    MOV  [R0], R4                               ; atualizar coluna inicial da primeira linha

    ADD  R0, R6                                 ; saltar quatro endereços ( R6 igual a 8)
    MOV  [R0], R5                               ; atualizar coluna inicial da segunda linha

    ADD  R0, 4                                  ; saltar 2 endereços
    MOV  [R0], R5                               ; atualizar coluna inicial da terceira linha

    ADD  R0, 4                                  ; saltar 2 endereços
    MOV  [R0], R5                               ; atualizar coluna inicial da quarta linha

    ADD  R0, 4                                  ; saltar 2 endereços
    MOV  [R0], R4                               ; atualizar coluna inicial da quinta e última linha do asteróide
    RET




; ***********************************************************************************************************************
; * Numero Aleatorio - rotina que gera um numero aleatorio entre 0 a 15                                                 *
; *                                                                                                                     *
; * argumentos:             nenhum                                                                                      *
; * return:                 R2 - numero aleatorio                                                                       *
; *                                                                                                                     *
; ***********************************************************************************************************************
numero_aleatorio:
    MOV  R1, TEC_COL                            ; endereço das linhas do teclado
    MOVB R2, [R1]                               ; ler
    SHR  R2, 4                                  ; colocar os bits 7 a 4 (aleatórios) nos bits 3 a 0 ficando com um valor aleatório
    RET




; *************************************************************************************************************************
; * Verifica Colisão com Nave - rotina que verifica se asteróide colide com nave, sabendo as colunas possíveis de colisão *
; *                                                                                                                       *
; * argumentos:             R10 - endereço da tabela que define o asteróide                                               *
; * return:                                                                                                               *
; *                         se houver colisão: R9 = 4 para o corpo principal do programa saber que houve colisão          *
; *                         se não houver colisão: nenhum                                                                 *
; *                                                                                                                       *
; *************************************************************************************************************************
verifica_colisao_com_nave:
    MOV  R0, R10                                ; endereço da tabela que define o asteróide
    ADD  R0, 2
    MOV  R1, [R0]                               ; linha inicial do asteróide
    ADD  R0, 2
    MOV  R2, [R0]                               ; coluna inicial do asteróide

    MOV  R3, VINTE_E_DOIS                       ; linha vinte e dois
    CMP  R1, R3                                 ; linha do asteróide encontra-se na linha 22 ??? (a única linha possível de colisão)
    JNZ  sair_verificacao_colisao_com_nave      ; se não, sair !!! (não há colisão)
                                                ; se sim, testar todas as colunas possíveis de colisão
    MOV  R3, TESTE_1_COLUNA                     
    CMP  R2, R3                                 ; encontra-se na coluna de colisão 1 ?
    JZ   ha_colisao_com_nave                    ; se sim, sair!
    MOV   R3, TESTE_2_COLUNA                    ; se não, testar colunas
    CMP  R2, R3                                 ; encontra-se na coluna de colisão 2 ?
    JZ   ha_colisao_com_nave                    
    MOV  R3, TESTE_3_COLUNA
    CMP  R2, R3                                 ; encontra-se na coluna de colisão 3 ?
    JZ   ha_colisao_com_nave
    MOV  R3, TESTE_4_COLUNA
    CMP  R2, R3                                 ; encontra-se na coluna de colisão 4 ?
    JZ   ha_colisao_com_nave
    MOV  R3, TESTE_5_COLUNA
    CMP  R2, R3                                 ; encontra-se na coluna de colisão 5 ?
    JZ   ha_colisao_com_nave

sair_verificacao_colisao_com_nave:
    RET

ha_colisao_com_nave:
    MOV  R9, 4                                  ; R9 = 4 para o corpo principal do programa saber que houve colisão com  a nave
    RET





; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -





; ************************************************************************************************
; * Animar Nave - rotina cooperativa que anima uma parte da nave, alterando a cor dessa parte    *
; *                                                                                              *
; * argumentos:             nenhum                                                               *
; * return:                 nenhum                                                               *
; *                                                                                              *
; ************************************************************************************************
animar_nave:
    MOV  R0, Evento_Int_Nave                    ; endereço da variável que diz se ocorreu interrupção desejada
    MOV  R1, [R0]                               ; valor da variável (0 ocorreu, 1 não ocorreu)
    CMP  R1, 0                                  ; não ocorreu interrupção?
    JZ   sair_animar_nave                       ; se não, sair!
                                                ; se sim, animar
    MOV  R1, 0
    MOV  [R0], R1                               ; atualizar variável com 0 (interrupção atendida)
    
    CALL numero_aleatorio                       ; gera número aleatório em R2 = {0, 1, ..., 14, 15}
    MOV  R1, 6                          
    MOD  R2, R1                                 ; R2 = {0, 1, 2, 3, 4, 5}
    SHL  R2, 1                                  ; R2 = {0, 2, 4, 6, 8, 10}
    MOV  R0, CoresNaveVariacao                  ; endereço da tabela com 6 cores possíveis para variar a nave
    ADD  R0, R2                                 ; endereço aleatório (endereço inicial + salto aleatório)
    MOV  R5, [R0]                               ; indexar a tabela aleatóriamente, e obter cor 
    
    CALL desenhar_nave_variacao                 ; desenhar nave variação com R5 já com a cor
sair_animar_nave:
    RET





; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -





; *************************************************************************************************
; * Animar Display - rotina cooperativa que anima os displays                                     *
; *                                                                                               *
; * argumentos:             nenhum                                                                *
; * return:                 se display ficar a 0: R9 = 3 para o corpo principal do programa saber *
; *                         se não: nenhum
; *                                                                                               *
; *************************************************************************************************
animar_display:
    MOV  R0, Evento_Int_Display                 ; endereço da variável que diz se ocorreu interrupção desejada
    MOV  R1, [R0]                               ; valor da variável (0 ocorreu, 1 não ocorreu)
    CMP  R1, 0                                  ; não ocorreu interrupção?
    JZ   sair_animar_display                    ; se não, sair!
                                                ; se sim, animar
    MOV  R1, 0
    MOV  [R0], R1                               ; atualizar variável com 0 (interrupção atendida)

    MOV  R1, ValorDisplay                       ; endereço da variável que tem o valor dos displays
    MOV  R0, [R1]                               ; valor 
    SUB  R0, 3                                  ; decrementar por 3
    MOV  [R1], R0                               ; atualizar valor

    CMP  R0, 0                                  ; o valor atualizado do display é menor ou igual a 0 ???
    JLE  valor_display_acabou                   ; se sim, não se coloca o valor nos displays

                                                ; se não, podemos colocar o valor nos displays, mas antes temos que
    CALL decimal                                ; converter valor para decimal para por nos displays

    MOV  R2, DISPLAYS                           ; endereço do periférico dos displays
    MOV  [R2], R0                               ; inserir novo valor nos displays
sair_animar_display:
    RET

valor_display_acabou:                           ; caso valor atualizado seja <= 0
    MOV  R9, 3                                  ; R9 = 3 para o corpo principal do programa saber que a energia da nave acabou !!!
    MOV  R2, DISPLAYS           
    MOV  R1, 0          
    MOV  [R2], R1                               ; colocar os displays a 0 !!
    RET



; ************************************************************************************************
; * Decimal - rotina que converte o hexadecimal para decimal                                     *
; *                                                                                              *
; * argumentos:             R0 -  valor hexadecimal                                              *
; * return:                 R0 - valor decimal                                                   *
; *                                                                                              *
; ************************************************************************************************
decimal:
    MOV  R1, FATOR                              ; fator
    MOV  R2, DIVISOR                            ; divisor
    MOV  R3, R0                                 ; valor hexadecimal
    MOV  R0, 0                                  ; resultado
converter_decimal:
    MOD  R3, R1                                 ; hexadecimal % fator = resto
    MOV  R4, R3                                 ; resto
    DIV  R1, R2                                 ; fator // divisor (tirar último digito do fator)
    DIV  R4, R1                                 ; resto // novo_fator = n
    SHL  R0, 4                                  ; resultado = resultado * 16
    ADD  R0, R4                                 ; resultado = resultado + n
    CMP  R1, 1                                  ;  fator tem um digito apenas ?
    JNZ  converter_decimal                      ; se não, repetir!
    RET





; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -





; ****************************************************************************************************************************
; * Animar Sondas - rotina cooperativa que movimenta as sondas, verificandos os limites e colisões                           *
; *                                                                                                                          *
; * argumentos:             nenhum                                                                                           *
; * return:                 nenhum                                                                                           *
; *                                                                                                                          *
; ****************************************************************************************************************************
sem_int_sair:                                   ; caso não haja interrupção
    RET


                                                ; a rotina é grande porque lida separadamente com a sonda esquerda, frente e direita
                                                ; mas há muitos saltos ao longo da rotina
animar_sondas:                                  
    MOV  R0, Evento_Int_Sonda                   ; endereço da variável que diz se ocorreu interrupção desejada
    MOV  R1, [R0]                               ; valor da variável (0 ocorreu, 1 não ocorreu)
    CMP  R1, 0                                  ; não ocorreu interrupção?
    JZ   sem_int_sair                           ; se não, sair!
                                                ; se im, animar
    MOV  R1, 0                  
    MOV  [R0], R1                               ; atualizar variável com 0 (interrupção atendida)
    JMP  animar_sonda_esquerda

desativar_sonda_esq:                            ; é feito JMP nesta label mais em baixo se a distância percorrida da sonda for igual a 0 ou se houver colisão com asteróide
    MOV  R2, DISTANCIA_MAXIMA                   ; distância máxima que cada sonda pode percorrer
    MOV  [R0], R2                               ; resetar distância da sonda com distância máxima
    ADD  R0, 2                                  ; próximo endereço
    MOV  R2, SONDA_ESQ_FRE_DIR_LIN              ; linha incial da sonda
    MOV  [R0], R2                               ; resetar linha incial
    ADD  R0, 2                                  ; próximo endereço
    MOV  R2, SONDA_ESQ_COL                      ; coluna incial da sonda esquerda
    MOV  [R0], R2                               ; resetar coluna inicial
    MOV  R0, SondaEsquerdaStatus                ; endereço da variável que tem o status da sonda
    MOV  R2, 0                                  ; desativado
    MOV  [R0], R2                               ; atualizar status da sonda para desativado
    JMP  animar_sonda_frente                    ; ir para próxima sonda


animar_sonda_esquerda:                          ; começamos por analisar a sonda esquerda
    MOV  R0, SondaEsquerdaStatus                ; endereço que tem o status da sonda
    MOV  R1, [R0]                               ; status da sonda
    CMP  R1, 0                                  ; status = ativado?
    JZ   animar_sonda_frente                    ; se sim, sair! tratar da próxima sonda
                                                ; se sonda estiver ativa, temos que animá-la, depois de verificações
    MOV  R0, SondaEsquerda                      ; endereço da tabela que define a sonda esquerda
    CALL limpar_sonda                           ; apagar a sonda

    MOV  R1, [R0]                               ; distância que sonda pode percorrer
    SUB  R1, 1                                  ; diminuir distância por 1 unidade
    CMP  R1, 0                                  ; distância = 0 ?
    JZ   desativar_sonda_esq                    ; se sim, temos que desativar a sonda e acabamos de atender a sonda esquerda
                                                ; se não temos que atualizar as coordenadas da sonda e

    MOV  [R0], R1                               ; atualizar a sonda com nova distânica
    ADD  R0, 2                                  ; próximo endereço
    MOV  R1, [R0]                               ; linha inicial da sonda
    SUB  R1, 1                                  ; linha menos 1
    MOV  [R0], R1                               ; atualizar nova linha inicial
    ADD  R0, 2                                  ; próximo endereço
    MOV  R2, [R0]                               ; coluna inicial da sonda
    SUB  R2, 1                                  ; coluna menos 1
    MOV  [R0], R2                               ; atualizar nova coluna inicial

                                                ; antes de desenhar sonda temos que verificar colisão com asteróides !!!!!
colisao_sonda_esq_asteroide1:
    MOV  R0, Asteroide1                         ; endereço da tabela que define o asteróide1
    CALL verifica_colisao_com_asteroide         ; retorna R3 com 0 se não houver colisão, e 1 se houver
    CMP  R3, 1                                  ; há colisão?
    JZ   ha_colisao_sonda_esq                   ; se sim, saímos e tratamos da colisão! e acabamos de atender a sonda esquerda

colisao_sonda_esq_asteroide2:                   ; mesmo processo para o asteróide 2
    MOV  R0, Asteroide2                        
    CALL verifica_colisao_com_asteroide         
    CMP  R3, 1                                 
    JZ   ha_colisao_sonda_esq                   

colisao_sonda_esq_asteroide3:                   ; mesmo processo para o asteróide 3
    MOV  R0, Asteroide3                        
    CALL verifica_colisao_com_asteroide       
    CMP  R3, 1                                 
    JZ   ha_colisao_sonda_esq          

colisao_sonda_esq_asteroide4:                   ; mesmo processo para o asteróide 4
    MOV  R0, Asteroide4                        
    CALL verifica_colisao_com_asteroide       
    CMP  R3, 1                                 
    JNZ  desenhar_sonda_esquerda                ; se não houver colisão, desenhamos a sonda e acabamos de atender a sonda esquerda
                                                ; se sim, temos que apagar apagar asteróide e resetar sonda
ha_colisao_sonda_esq:
    MOV  R7, [R0]                               ; informação se é minerável ou não
    CMP  R7, 1                                  ; é minerável?
    JNZ  continuar_col_esq                      ; se não sair desta secção
    CALL aumentar_energia
    MOV  R6, 5
    MOV  [SELECIONA_SOM], R6
    JMP  continuar_col_esq_2
continuar_col_esq:
    MOV  R6, 3
    MOV  [SELECIONA_SOM], R6
continuar_col_esq_2:
    MOV  R10, R0                                ; endereço da tabela que define o asteróide com colisão
    CALL limpar_asteroide                       ; apagar asteróide

    MOV  R1, ULTIMA_LINHA                       ; última linha inicial possível para um asteróide
    ADD  R10, 2                                 ; próximo endereço da tabela que define o asteróide (linha inicial)
    MOV  [R10], R1                              ; atualizar linha inicial do asteróide
    MOV  R10, R0                                ; endereço da tabela que define o asteróide
    CALL descer_asteroide                       ; descer a linha do asteróide, MAS como atualizámos a linha deste para a última possível
                                                ; a rotina descer_asteróide vai resetar o asteróide 
    CALL desenhar_asteroide                     ; desenhar o asteróide, que foi resetado, agora no topo
    MOV  R0, SondaEsquerda                      ; endereço da tabela que define a sonda
    JMP desativar_sonda_esq                     ; agora falta só desativar a sonda
    
desenhar_sonda_esquerda:                        
    MOV  R0, SondaEsquerda                      ; endereço da tabela que define a sonda esquerda
    CALL desenhar_sonda                         ; desenhar sonda atualizada
    JMP  animar_sonda_frente



desativar_sonda_fre:                            ; repetir processo de desativar sonda
                                                ; diferindo no facto que a nova coluna é específica à sonda frente
    MOV  R2, DISTANCIA_MAXIMA                  
    MOV  [R0], R2                            
    ADD  R0, 2                                 
    MOV  R2, SONDA_ESQ_FRE_DIR_LIN             
    MOV  [R0], R2                            
    ADD  R0, 2                               
    MOV  R2, SONDA_FRE_COL                      ; coluna inicial da sonda frente  
    MOV  [R0], R2                              
    MOV  R0, SondaFrenteStatus                 
    MOV  R2, 0                                 
    MOV  [R0], R2                             
    JMP  animar_sonda_direta  



animar_sonda_frente:                            ; repetir processo de animar sonda
                                                ; diferindo no facto que a nova coluna não é atualizada (pois é constante)
    MOV  R0, SondaFrenteStatus                  
    MOV  R1, [R0]                               
    CMP  R1, 0                                 
    JZ   animar_sonda_direta                   

    MOV  R0, SondaFrente                       
    CALL limpar_sonda                           

    MOV  R1, [R0]                               
    SUB  R1, 1                                  
    CMP  R1, 0                                  
    JZ   desativar_sonda_fre                    

    MOV  [R0], R1                               
    ADD  R0, 2                                  
    MOV  R1, [R0]                               
    SUB  R1, 1                                  ; linha menos 1
    MOV  [R0], R1                               
    ADD  R0, 2
    MOV  R2, [R0]                               ; coluna não alterada!

colisao_sonda_fre_asteroide1:
    MOV  R0, Asteroide1
    CALL verifica_colisao_com_asteroide
    CMP  R3, 1
    JZ   ha_colisao_sonda_fre
colisao_sonda_fre_asteroide2:
    MOV  R0, Asteroide2
    CALL verifica_colisao_com_asteroide
    CMP  R3, 1
    JZ   ha_colisao_sonda_fre
colisao_sonda_fre_asteroide3:
    MOV  R0, Asteroide3
    CALL verifica_colisao_com_asteroide
    CMP  R3, 1
    JZ   ha_colisao_sonda_fre
colisao_sonda_fre_asteroide4:
    MOV  R0, Asteroide4
    CALL verifica_colisao_com_asteroide
    CMP  R3, 1
    JNZ   desenhar_sonda_frente

ha_colisao_sonda_fre:
    MOV  R7, [R0]
    CMP  R7, 1
    JNZ  continuar_col_fre
    CALL aumentar_energia
    MOV  R6, 5
    MOV  [SELECIONA_SOM], R6
    JMP  continuar_col_fre_2

continuar_col_fre:
    MOV  R6, 3
    MOV  [SELECIONA_SOM], R6
continuar_col_fre_2:
    MOV  R10, R0
    CALL limpar_asteroide

    MOV  R1, ULTIMA_LINHA
    ADD  R10, 2
    MOV  [R10], R1
    MOV  R10, R0
    CALL descer_asteroide
    
    CALL desenhar_asteroide
    MOV  R0, SondaFrente 
    JMP  desativar_sonda_fre

desenhar_sonda_frente:
    MOV  R0, SondaFrente                        
    CALL desenhar_sonda    
    JMP  animar_sonda_direta



desativar_sonda_dir:                            ; repetir processo de desativar sonda
                                                ; diferindo no facto que a nova coluna é específica à sonda direita
    MOV  R2, DISTANCIA_MAXIMA                
    MOV  [R0], R2                             
    ADD  R0, 2                              
    MOV  R2, SONDA_ESQ_FRE_DIR_LIN            
    MOV  [R0], R2                             
    ADD  R0, 2                               
    MOV  R2, SONDA_DIR_COL                      ; coluna inicial da sonda direita
    MOV  [R0], R2                          
    MOV  R0, SondaDireitaStatus              
    MOV  R2, 0                            
    MOV  [R0], R2                             
    RET                           



animar_sonda_direta:                            ; repetir processo de animar sonda
                                                ; diferindo no facto que a nova coluna é a seguinte (+ 1)
    MOV  R0, SondaDireitaStatus                 
    MOV  R1, [R0]                               
    CMP  R1, 0                                  
    JZ   sair_animar_sonda                               

    MOV  R0, SondaDireita                       
    CALL limpar_sonda                         

    MOV  R1, [R0]                               
    SUB  R1, 1                                 
    CMP  R1, 0                                 
    JZ   desativar_sonda_dir                    

    MOV  [R0], R1                          
    ADD  R0, 2                                 
    MOV  R1, [R0]                               
    SUB  R1, 1                                  ; linha menos 1
    MOV  [R0], R1                             
    ADD  R0, 2                                
    MOV  R2, [R0]                               
    ADD  R2, 1                                  ; coluna mais 1 
    MOV  [R0], R2

colisao_sonda_dir_asteroide1:
    MOV  R0, Asteroide1
    CALL verifica_colisao_com_asteroide
    CMP  R3, 1
    JZ   ha_colisao_sonda_dir
colisao_sonda_dir_asteroide2:
    MOV  R0, Asteroide2
    CALL verifica_colisao_com_asteroide
    CMP  R3, 1
    JZ   ha_colisao_sonda_dir
colisao_sonda_dir_asteroide3:
    MOV  R0, Asteroide3
    CALL verifica_colisao_com_asteroide
    CMP  R3, 1
    JZ   ha_colisao_sonda_dir
colisao_sonda_dir_asteroide4:
    MOV  R0, Asteroide4
    CALL verifica_colisao_com_asteroide
    CMP  R3, 1
    JNZ  desenhar_sonda_direita

ha_colisao_sonda_dir:
    MOV  R7, [R0]
    CMP  R7, 1
    JNZ  continuar_col_dir
    CALL aumentar_energia
    MOV  R6, 5
    MOV  [SELECIONA_SOM], R6
    JMP  continuar_col_dir_2

continuar_col_dir:
    MOV  R6, 3
    MOV  [SELECIONA_SOM], R6
continuar_col_dir_2:
    MOV  R10, R0
    CALL limpar_asteroide

    MOV  R1, ULTIMA_LINHA
    ADD  R10, 2
    MOV  [R10], R1
    MOV  R10, R0
    CALL descer_asteroide
    CALL desenhar_asteroide
    MOV  R0, SondaDireita
    JMP  desativar_sonda_dir

desenhar_sonda_direita:
    MOV  R0, SondaDireita                       
    CALL desenhar_sonda 


sair_animar_sonda:
    RET




; *********************************************************************************************************************************************
; *	Verifica Colisao com Asteroide - verifica a colisao da sonda com um asteroide, verificando coordenadas dos pixeis do asteróide com sonda  *
; * argumentos:             R0 - endereço do asteróide                                                                                        *
; *                         R1 - nova linha da sonda                                                                                          *
; *                         R2 - nova coluna da sonda                                                                                         *
; * return:                 R3 - 0 se não houve colisão, e 1 se houve colisão                                                                 *
; *********************************************************************************************************************************************

; considera-se o asteróideum quadrado 5x5

; X = pixeis que verifica e  O = pixeis que nao verifica

; XOOOX   
; XOOOX    
; XOOOX    
; XXXXX     
; XXXXX

verifica_colisao_com_asteroide:
    PUSH R0
    PUSH R1
    PUSH R2
    ADD  R0, 2                                  ; próximo endereço do asteróide                 
    MOV  R3, [R0]                               ; linha inicial do asteróide                    
    ADD  R0, 2                                  ; próximo endereço                              
    MOV  R4, [R0]                               ; coluna inicial do asteróide                   
    SUB  R4, 1                                  ; queremos usar a coluna anterior à coluna inicial para comparar
primeira_linha:                                 ; vamos comparar pixeis da primeira linha do asteróide
    CMP  R1, R3                                 ; linha da sonda e linha inicial do asteróide é a mesma ???
    JNZ  segunda_linha                          ; se não estiverem na mesma linha, vamos verificar na segunda linha do asteróide (linha inicial + 1) 
                                                ; se estiverem na mesma linha, vamos verificar colunas
    CMP  R2, R4                                 ; coluna da sonda e coluna inicial do asteróide é a mesma ???
    JZ   ha_colisao                             ; se sim, há colisão!!! e podemos sair
    MOV  R5, R4                                 ; coluna inicial do asteróide
    ADD  R5, 4                                  ; coluna final do asteróide
    CMP  R2, R5                                 ; coluna da sonda e coluna final do asteróide é a mesma ???
    JZ   ha_colisao                             ; se sim, há colisão!!! e podemos sair

segunda_linha:                                  ; repetir processo para a segunda linha
    ADD  R3, 1                                  ; mas na linha 2
    CMP  R1, R3                            
    JNZ  terceira_linha
    CMP  R2, R4                        
    JZ   ha_colisao
    MOV  R5, R4
    ADD  R5, 4
    CMP  R2, R5
    JZ   ha_colisao
terceira_linha:                                 ; repetir processo para a terceira linha
    ADD  R3, 1                                  ; mas na linha 3
    CMP  R1, R3                            
    JNZ  quarta_linha
    CMP  R2, R4                             
    JZ   ha_colisao
    MOV  R5, R4
    ADD  R5, 4
    CMP  R2, R5
    JZ   ha_colisao
quarta_linha:                                   ; processo diferente agora
    ADD  R3, 1                                  ; linha 4
    CMP  R1, R3                                 ; linha sonda = 4a linha do asteróide?
    JNZ  quinta_e_ultima_linha                  ; se não, verificar da 5a e última linha
                                                ; se sim, verificar colunas
    MOV  R5, R4                                 ; coluna inicial do asteróide
    CMP  R2, R5                                 ; mesma coluna ?
    JZ   ha_colisao                             ; se sim, há colisão!!! podemos sair

    ADD  R5, 1                                  ; testar próxima coluna até à última coluna
    CMP  R2, R5
    JZ   ha_colisao

    ADD  R5, 1
    CMP  R2, R5
    JZ   ha_colisao

    ADD  R5, 1
    CMP  R2, R5
    JZ   ha_colisao

    ADD  R5, 1                                  ; coluna final
    CMP  R2, R5
    JZ   ha_colisao
quinta_e_ultima_linha:                          ; repetir processo
    ADD  R3, 1                                  ; mas na linha 5 e última
    CMP  R1, R3
    JNZ  nao_ha_colisao

    CMP  R2, R4
    JZ   ha_colisao

    ADD  R4, 1
    CMP  R2, R4
    JZ   ha_colisao

    ADD  R4, 1
    CMP  R2, R4
    JZ   ha_colisao

    ADD  R4, 1
    CMP  R2, R4
    JZ   ha_colisao

    ADD  R4, 1
    CMP  R2, R4
    JZ   ha_colisao

nao_ha_colisao:
    MOV  R3, 0                                  ; valor a retornar
    POP  R2
    POP  R1
    POP  R0
    RET
ha_colisao:
    MOV  R3, 1                                  ; valor a retornar
    POP  R2
    POP  R1
    POP  R0
    RET




; ************************************************************************************************
; * Aumentar Energia - rotina que aumenta a energia da nave a 20 %                               *
; *                                                                                              *
; * argumentos:             nenhum                                                               *
; * return:                 nenhum                                                               *
; *                                                                                              *
; ************************************************************************************************
aumentar_energia:
    PUSH R0
    MOV  R1, ValorDisplay
    MOV  R0, [R1]
    MOV  R2, VINTE
    ADD  R0, R2
    MOV  [R1], R0
    CALL decimal
    MOV  R2, DISPLAYS
    MOV  [R2], R0
    POP R0
    RET





; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -





; ********************************************************************************************************************************
; *	Rotinas de Interrupção do Asteróide, Sonda, Display e Nave - assinalam em memória específica que occoreu interrupção     *
; * argumentos:             nenhum                                                                                               *
; * return:                 nenhum                                                                                               *
; ********************************************************************************************************************************
int_asteroide:
    PUSH R0
    PUSH R1
    MOV  R0, Evento_Int_Asteroide               ; endereço da variável que diz se houve interrupção
    MOV  R1, 1                                  ; 1 = há interrupção
    MOV  [R0], R1                               ; guardar informação que houve interrupção
    POP  R1
    POP  R0
    RFE



int_sonda:
    PUSH R0
    PUSH R1
    MOV  R0, Evento_Int_Sonda                   ; endereço da variável que diz se houve interrupção
    MOV  R1, 1                                  ; 1 = há interrupção
    MOV  [R0], R1                               ; guardar informação que houve interrupção
    POP  R1
    POP  R0
    RFE



int_display:
    PUSH R0
    PUSH R1
    MOV  R0, Evento_Int_Display                 ; endereço da variável que diz se houve interrupção
    MOV  R1, 1                                  ; 1 = há interrupção
    MOV  [R0], R1                               ; guardar informação que houve interrupção
    POP  R1
    POP  R0
    RFE



int_nave:
    PUSH R0
    PUSH R1
    MOV  R0, Evento_Int_Nave                    ; endereço da variável que diz se houve interrupção
    MOV  R1, 1                                  ; 1 = há interrupção
    MOV  [R0], R1                               ; guardar informação que houve interrupção
    POP  R1
    POP  R0
    RFE





; ***********************************************************************************************************************
; *                                         Rotinas Auxiliares de Desenho                                               *
; ***********************************************************************************************************************



; *************************************************************************************************************
; * Desenha Objeto - rotina que desenha um objeto (nave ou asteróide) com uma tabela padronizada que o define *
; *                                                                                                           *
; * argumentos:             R0 - endereço do objeto a desenhar na variável do primeiro comprimento            *
; *                         R1 - linha inicial do objeto                                                      *
; *                         R2 - coluna inicial do objeto                                                     *
; *                         R3 - altura do objeto                                                             *
; *                         R4 - primeiro comprimento do objeto                                               *
; *                         R5 - cor do objeto                                                                *
; *                                                                                                           *
; * return:                 sem return                                                                        *
; *                                                                                                           *
; *************************************************************************************************************
desenha_objeto:
    MOV  [DEFINE_LINHA], R1                 ; seleciona a linha
    MOV  [DEFINE_COLUNA], R2                ; seleciona a coluna
    MOV  [DEFINE_PIXEL], R5                 ; altera a cor do pixel na linha e coluna selecionados
    ADD  R2, 1                              ; próxima coluna
    SUB  R4, 1                              ; diminuir comprimento dessa linha
    JNZ  desenha_objeto                     ; se comprimento igual a 0, linha já foi desenhada
    ADD  R0, 2                              ; próximo endereço
    MOV  R4, [R0]                           ; comprimento da próxima linha
    ADD  R0, 2                              ; próximo endereço
    MOV  R2, [R0]                           ; coluna incial da próxima linha
    ADD  R1, 1                              ; próxima linha
    SUB  R3, 1                              ; diminuir altura do objeto
    JNZ  desenha_objeto                     ; se altura igual a o, objeto ja foi desenhado
    RET



; *********************************************************************************************************************
; * Preparar Nave Desenho - rotina que prepara (inicializa os argumentos para chamar desenha_objeto) e desenha a nave *
; *                                                                                                                   *
; * argumentos:             sem argumentos                                                                            *
; * return:                 sem return                                                                                *
; *                                                                                                                   *
; *********************************************************************************************************************
preparar_nave_desenho:
    MOV  R0, NaveFronteira                  ; endereço da tabela que define a nave (fronteira)
    MOV  R1, [R0]                           ; linha incial
    ADD  R0, 2                              ; próximo endereço
    MOV  R2, [R0]                           ; coluna incial
    ADD  R0, 2                              ; próximo endereço
    MOV  R3, [R0]                           ; altura
    ADD  R0, 2                              ; próximo endereço
    MOV  R4, [R0]                           ; primeiro comprimento
    MOV  R5, NAVE_FRONTEIRA_COR             ; cor
    CALL desenha_objeto                     ; desenhar
    MOV  R0, NaveInterior                   ; endereço da tabela que define a nave (interior)
    MOV  R1, [R0]                           ; linha incial
    ADD  R0, 2                              ; próximo endereço
    MOV  R2, [R0]                           ; coluna incial
    ADD  R0, 2                              ; próximo endereço
    MOV  R3, [R0]                           ; altura
    ADD  R0, 2                              ; próximo endereço
    MOV  R4, [R0]                           ; primeiro comprimento
    MOV  R5, NAVE_INTERIOR_COR              ; cor
    CALL desenha_objeto                     ; desenhar
    RET



; ***********************************************************************************************************************
; * Desenhar Asteroide - rotina que prepara (inicializa os argumentos para chamar desenha_objeto) e desenha o asteróide *
; *                                                                                                                     *
; * argumentos:             R10 - endereço do asteróide a desenhar                                                      *
; * return:                 sem return                                                                                  *
; *                                                                                                                     *
; ***********************************************************************************************************************
desenhar_asteroide:
    MOV  R0, R10                                ; endereço do asteróide
    MOV  R1, [R0]                               ; informação se é ou não minerável (0 não é, 1 é)
    CMP  R1, 1                                  ; é minerável?
    JZ   e_mineravel                            ; se sim saltamos para secção dos mineráveis 
e_nao_mineravel:                                ; se não é
    MOV  R5, NAO_MINERAVEL_COR                  ; cor dos não mineráveis
    JMP  finalizar_preparacao_asteroide         ; finalizar preparação do asteróide
e_mineravel:
    MOV  R5, MINERAVEL_COR                      ; cor dos mineráveis
finalizar_preparacao_asteroide:
    ADD  R0, 2                                  ; próximo endereço
    MOV  R1, [R0]                               ; linha inicial
    ADD  R0, 2                                  ; próximo endereço
    MOV  R2, [R0]                               ; coluna inicial
    ADD  R0, 2                                  ; próximo endereço
    MOV  R3, [R0]                               ;  altura
    ADD  R0, 2                                  ; próximo endereço
    MOV  R4, [R0]                               ; primeiro comprimento
    CALL desenha_objeto                         ; desenhar
    RET



; ***********************************************************************************************************************
; * Limpar Asteroide - rotina que prepara (inicializa os argumentos para chamar desenha_objeto) e apaga o asteróide     *
; *                                                                                                                     *
; * argumentos:             R10 - endereço do asteróide a desenhar                                                      *
; * return:                 sem return                                                                                  *
; *                                                                                                                     *
; ***********************************************************************************************************************
limpar_asteroide:
    PUSH R0
    MOV  R0, R10                                ; endereço do asteróide
    ADD  R0, 2                                  ;  próximo endereço
    MOV  R1, [R0]                               ; linha inicial
    ADD  R0, 2                                  ; próximo endereço
    MOV  R2, [R0]                               ; coluna inicial
    ADD  R0, 2                                  ; próximo endereço
    MOV  R3, [R0]                               ; altura
    ADD  R0, 2                                  ; próximo endereço
    MOV  R4, [R0]                               ; primeiro comprimento
    MOV  R5, TRANSPARENTE                       ; cor
    CALL desenha_objeto                         ; desenhar (desenhar o objeto com pixeis transparentes)
    POP  R0
    RET



; ***********************************************************************************************************************
; * Desenhar Sonda - rotina que prepara e desenha a sonda                                                               *
; *                                                                                                                     *
; * argumentos:             R0 - endereço da sonda a desenhar                                                           *
; * return:                 sem return                                                                                  *
; *                                                                                                                     *
; ***********************************************************************************************************************
desenhar_sonda:
    PUSH R0
    ADD  R0, 2                                  ; próximo endereço
    MOV  R1, [R0]                               ; linha incial da sonda
    ADD  R0, 2                                  ; próximo endereço
    MOV  R2, [R0]                               ; coluna inicial da sonda
    MOV  R3, SONDA_COR                          ; cor da sonda
    MOV  [DEFINE_LINHA], R1                     ; seleciona a linha
    MOV  [DEFINE_COLUNA], R2                    ; seleciona a coluna
    MOV  [DEFINE_PIXEL], R3                     ; altera a cor do pixel na linha e coluna selecionados para a cor da sonda
    POP  R0
    RET



; ***********************************************************************************************************************
; * Limpar Sonda - rotina que prepara e apaga a sonda                                                                   *
; *                                                                                                                     *
; * argumentos:             R0 - endereço da sonda a desenhar                                                           *
; * return:                 sem return                                                                                  *
; *                                                                                                                     *
; ***********************************************************************************************************************
limpar_sonda:
    PUSH R0
    ADD  R0, 2                                  ; próximo endereço
    MOV  R1, [R0]                               ; linha incial da sonda
    ADD  R0, 2                                  ; próximo endereço
    MOV  R2, [R0]                               ; coluna inicial da sonda
    MOV  R3, TRANSPARENTE                       ; cor da sonda
    MOV  [DEFINE_LINHA], R1                     ; seleciona a linha
    MOV  [DEFINE_COLUNA], R2                    ; seleciona a coluna
    MOV  [DEFINE_PIXEL], R3                     ; altera a cor do pixel na linha e coluna selecionados para a cor da sonda
    POP  R0
    RET



; **************************************************************************************************************************
; * Desenhar Nave Variação - rotina que prepara (inicializa os argumentos para chamar desenha_objeto) e desenha a variação *
; *                                                                                                                        *
; * argumentos:             R5 - cor da variação                                                                           *
; * return:                 nenhum                                                                                         *
; *                                                                                                                        *
; **************************************************************************************************************************
desenhar_nave_variacao:
    MOV  R0, NaveVariacao                       ;  endereço da tabela que define a nave
    MOV  R1, [R0]                               ; linha incial
    ADD  R0, 2
    MOV  R2, [R0]                               ; coluna incial
    ADD  R0, 2
    MOV  R3, [R0]                               ; altura
    ADD  R0, 2
    MOV  R4, [R0]                               ; primeiro comprimento
    CALL desenha_objeto
    RET

