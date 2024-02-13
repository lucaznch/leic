
:- set_prolog_flag(answer_write_options, [max_depth(0)]).
:- ['dados.pl'], ['keywords.pl'].


% QUALIDADE DE DADOS

% eventosSemSalas/1     eventosSemSalas( EventosSemSala )
% verdade se EventosSemSala e uma lista ordenada e sem elementos repetidos, de IDs de eventos sem sala
eventosSemSalas(EventosSemSala) :- findall(ID, evento(ID, _, _, _, 'semSala'), Lista), sort(Lista, EventosSemSala).



% eventosSemSalasDiaSemana/2     eventosSemSalasDiaSemana( DiaDaSemana, EventosSemSala )
% verdade se EventosSemSala e uma lista ordenada e sem elementos repetidos, de IDs de eventos sem sala que decorrem em DiaDaSemana
eventosSemSalasDiaSemana(DiaDaSemana, EventosSemSala) :- 
    findall(ID, horario(ID, DiaDaSemana, _, _, _, _), ListaEventosEmDiaDaSemana), % lista com todos IDs dos eventos em DiaDaSemana
    eventosSemSalas(ListaEventosSemSala), % lista com todos IDs dos eventos sem Sala
    intersection(ListaEventosSemSala, ListaEventosEmDiaDaSemana, Lista), % lista com todos IDs dos eventos sem DiaDaSemana sem sala (intersecao entre as 2 listas acima)
    sort(Lista, EventosSemSala).



% eventosSemSalasPeriodo/2     eventosSemSalasPeriodo( ListaPeriodos, EventosSemSala )
% verdade se ListaPeriodos e uma lista de periodos e EventosSemSala e uma lista ordenada e sem elementos repetidos, de IDs de eventos sem salas nos periodos de ListaPeriodos

eventosSemSalasPeriodoAux(Periodo, Lista) :-
    (Periodo == p1_2; Periodo == p3_4), !,
    findall(ID, horario(ID, _, _, _, _, Periodo), L1), % encontra todos os IDs, de eventos com periodo Periodo
    eventosSemSalas(L2), % todos os IDs de eventos sem salas
    intersection(L1, L2, Lista). % obtem todos os IDs de eventos em periodo Periodo sem salas

eventosSemSalasPeriodoAux(Periodo, Lista) :-
    (Periodo == p1; Periodo == p2), !,
    findall(ID, horario(ID, _, _, _, _, Periodo), L1),
    findall(ID, horario(ID, _, _, _, _, p1_2), L2),
    append(L1, L2, L3),
    eventosSemSalas(L4),
    intersection(L3, L4, Lista).

eventosSemSalasPeriodoAux(Periodo, Lista) :-
    (Periodo == p3; Periodo == p4), !,
    findall(ID, horario(ID, _, _, _, _, Periodo), L1),
    findall(ID, horario(ID, _, _, _, _, p3_4), L2),
    append(L1, L2, L3),
    eventosSemSalas(L4),
    intersection(L3, L4, Lista).


eventosSemSalasPeriodo(ListaPeriodos, EventosSemSala) :- eventosSemSalasPeriodo(ListaPeriodos, EventosSemSala, []).

eventosSemSalasPeriodo([], EventosSemSala, EventosSemSala) :- !.

eventosSemSalasPeriodo([P|R], EventosSemSala, Acumulador) :-
    eventosSemSalasPeriodoAux(P, Lista),
    append(Acumulador, Lista, AcumuladorAux1),
    sort(AcumuladorAux1, AcumuladorAux2),
    eventosSemSalasPeriodo(R, EventosSemSala, AcumuladorAux2).


% --------------------------------------------------------------------------------


% PESQUISA SIMPLES

% organizaEventos/3     organizaEventos( ListaEventos, Periodo, EventosNoPeriodo )
% verdade se EventosNoPeriodo e a lista ordenada e sem elementos repetidos, de IDs dos eventos de ListaEventos que ocorrem no periodo Periodo
organizaEventos(ListaEventos, Periodo, EventosNoPeriodo) :- organizaEventos(ListaEventos, Periodo, EventosNoPeriodo, []).

organizaEventos([], _, EventosNoPeriodo, EventosNoPeriodo).

organizaEventos([ID|R], Periodo, EventosNoPeriodo, Acumulador) :-
    (Periodo == p1; Periodo == p2),
    (horario(ID, _, _, _, _, Periodo); horario(ID, _, _, _, _, p1_2)), !, % se houver ID associado a evento com periodo Periodo ou periodo p1_2
    append(Acumulador, [ID], AcumuladorAux1),
    sort(AcumuladorAux1, AcumuladorAux2),
    organizaEventos(R, Periodo, EventosNoPeriodo, AcumuladorAux2).

organizaEventos([ID|R], Periodo, EventosNoPeriodo, Acumulador) :-
    (Periodo == p3; Periodo == p4),
    (horario(ID, _, _, _, _, Periodo); horario(ID, _, _, _, _, p3_4)), !,
    append(Acumulador, [ID], AcumuladorAux1),
    sort(AcumuladorAux1, AcumuladorAux2),
    organizaEventos(R, Periodo, EventosNoPeriodo, AcumuladorAux2).

organizaEventos([ID|R], Periodo, EventosNoPeriodo, Acumulador) :-
    (Periodo == p1_2; Periodo == p3_4),
    (horario(ID, _, _, _, _, Periodo)), !,
    append(Acumulador, [ID], AcumuladorAux1),
    sort(AcumuladorAux1, AcumuladorAux2),
    organizaEventos(R, Periodo, EventosNoPeriodo, AcumuladorAux2).

organizaEventos([_|R], Periodo, EventosNoPeriodo, Acumulador) :- organizaEventos(R, Periodo, EventosNoPeriodo, Acumulador).



% eventosMenoresQue/2     eventosMenoresQue( Duracao, ListaEventosMenoresQue )
% verdade se ListaEventosMenoresQue e a lista ordenada e sem elementos repetidos dos IDs dos eventos que tem duracao menor ou igual a Duracao
eventosMenoresQue(Duracao, ListaEventosMenoresQue) :- findall(ID, (horario(ID, _, _, _, Tempo, _), Tempo =< Duracao), Lista), sort(Lista, ListaEventosMenoresQue).



% eventosMenoresQueBool/2     eventosMenoresQueBool( ID, Duracao )
% verdade se o evento identificado por ID tiver duracao igual ou menor a Duracao
eventosMenoresQueBool(ID, Duracao) :- horario(ID, _, _, _, Tempo, _), Tempo =< Duracao.



% procuraDisciplinas/2     procuraDisciplinas( Curso, ListaDisciplinas )
% verdade se ListaDisciplinas e a lista ordenada alfabeticamente do nome das disciplinas do curso Curso
procuraDisciplinas(Curso, ListaDisciplinas) :-
    findall(ID, turno(ID, Curso, _, _), ListaIDCurso),
    procuraDisciplinas(ListaIDCurso, ListaDisciplinas, []).

procuraDisciplinas([], ListaDisciplinas, ListaDisciplinas) :- !.

procuraDisciplinas([ID|Resto], ListaDisciplinas, Acumulador) :-
    findall(Disciplina, evento(ID, Disciplina, _, _, _), ListaComDisciplina),
    append(Acumulador, ListaComDisciplina, AcumuladorAux1),
    sort(AcumuladorAux1, AcumuladorAux2),
    procuraDisciplinas(Resto, ListaDisciplinas, AcumuladorAux2).



% organizaDisciplinas/3     organizaDisciplinas( ListaDisciplinas, Curso, Semestres )
% verdade se Semestres e uma lista com 2 listas, ordenadas alfabeticamente e sem elementos repetidos: 
% lista contendo as disciplinas de ListaDisciplinas do curso Curso que ocorrem no 1o semestre
% lista  ontendo as disciplinas de ListaDisciplinas do curso Curso que ocorrem no 2o semestre

% de forma recursiva, processo iterativo, com 2 acumuladores, um para semestre1 e outro para semestre2 obtem a lista Semestre com os 2 acumuladores dentro
organizaDisciplinas(ListaDisciplinas, Curso, Semestres) :- organizaDisciplinas(ListaDisciplinas, Curso, Semestres, [], []).

organizaDisciplinas([], _, [Semestre1|[Semestre2]], Semestre1, Semestre2). % caso terminal

organizaDisciplinas([Disciplina|R], Curso, Semestres, AcumuladorSemestre1, AcumuladorSemestre2) :-
    turno(ID, Curso, _, _), 
    evento(ID, Disciplina, _, _, _),
    horario(ID, _, _, _, _, Periodo), % se exitir um evento que para o mesmo ID, tenha turno com curso Curso, evento com disciplina Disciplina e horario com periodo Periodo
    (Periodo == p1; Periodo == p2; Periodo == p1_2), !, % semestre 1
    append(AcumuladorSemestre1, [Disciplina], AcumuladorSemestre1Aux),
    organizaDisciplinas(R, Curso, Semestres, AcumuladorSemestre1Aux, AcumuladorSemestre2).

organizaDisciplinas([Disciplina|R], Curso, Semestres, AcumuladorSemestre1, AcumuladorSemestre2) :-
    turno(ID, Curso, _, _),
    evento(ID, Disciplina, _, _, _),
    horario(ID, _, _, _, _, Periodo),
    (Periodo == p3; Periodo == p4; Periodo == p3_4), !, % semestre 2
    append(AcumuladorSemestre2, [Disciplina], AcumuladorSemestre2Aux),
    organizaDisciplinas(R, Curso, Semestres, AcumuladorSemestre1, AcumuladorSemestre2Aux).



% horasCurso/5     horasCurso( Periodo, Curso, Ano, TotalHoras )
% verdade se TotalHoras for o numero de horas total dos eventos associados ao curso Curso, no ano Ano e periodo Periodo

% horasCursoAux/2 de forma recursiva, processo iterativo, atraves de Lista que todos os IDs do curso Curso, obtem o Tempo em horas que um curso tem
horasCursoAux(Lista, Tempo) :- horasCursoAux(Lista, Tempo, 0).
horasCursoAux([], Tempo, Tempo) :- !.
horasCursoAux([ID|R], Tempo, Acumulador) :-
    horario(ID, _, _, _, Duracao, _),
    AcumuladorAux is Acumulador + Duracao,
    horasCursoAux(R, Tempo, AcumuladorAux).


horasCurso(Periodo, Curso, Ano, TotalHoras) :-
    (Periodo == p1; Periodo == p2), !,
    findall(ID, turno(ID, Curso, Ano, _), ListaIDCursoAnoAux), % encontra todos os IDs de eventos associados a turnos com curso Curso e ano Ano [1,1,5,5,5,6,9,9,9, ...]
    sort(ListaIDCursoAnoAux, ListaIDCursoAno), % [1,5,6,9 ...]
    findall(ID, horario(ID, _, _, _, _, Periodo), ListaIDPeriodo1), % encontra todos os IDs de eventos associados a horarios com periodo Periodo
    findall(ID, horario(ID, _, _, _, _, p1_2), ListaIDPeriodo2), % encontra todos os IDs de eventos associados a horarios com periodo p1_2
    append(ListaIDPeriodo1, ListaIDPeriodo2, ListaIDPeriodo3), 
    intersection(ListaIDCursoAno, ListaIDPeriodo3, Lista), % obtem a lista de IDs que interessam ao problema
    horasCursoAux(Lista, TotalHoras). % obtem TotalHoras

horasCurso(Periodo, Curso, Ano, TotalHoras) :-
    (Periodo == p3; Periodo == p4), !,
    findall(ID, turno(ID, Curso, Ano, _), ListaIDCursoAnoAux),
    sort(ListaIDCursoAnoAux, ListaIDCursoAno),
    findall(ID, horario(ID, _, _, _, _, Periodo), ListaIDPeriodo1),
    findall(ID, horario(ID, _, _, _, _, p3_4), ListaIDPeriodo2),
    append(ListaIDPeriodo1, ListaIDPeriodo2, ListaIDPeriodo3),
    intersection(ListaIDCursoAno, ListaIDPeriodo3, Lista),
    horasCursoAux(Lista, TotalHoras).

horasCurso(Periodo, Curso, Ano, TotalHoras) :-
    (Periodo == p1_2; Periodo == p3_4), !,
    findall(ID, turno(ID, Curso, Ano, _), ListaIDCursoAnoAux),
    sort(ListaIDCursoAnoAux, ListaIDCursoAno),
    findall(ID, horario(ID, _, _, _, _, Periodo), ListaIDPeriodo),
    intersection(ListaIDCursoAno, ListaIDPeriodo, Lista),
    horasCursoAux(Lista, TotalHoras).



% evolucaoHorasCurso/2     evolucaoHorasCurso( Curso, Evolucao )
% verdade se Evolucao for uma lista de tuplos na forma (Ano, Periodo, NumHoras), em que NumHoras = total de horas associadas ao curso Curso, no ano Ano e periodo Periodo
% Evolucao ordenado por ano (crescente) e periodo


% evolucaoHorasCursoTuplos/4 de forma recursiva, processo iterativo, obtem a lista com tuplos (Ano, Periodo, HorasCurso)
evolucaoHorasCursoTuplos(Periodos, Anos, Curso, Evolucao) :- evolucaoHorasCursoTuplos(Periodos, Anos, Curso, Evolucao, []).

evolucaoHorasCursoTuplos([], [], _, Evolucao, Evolucao) :- !.

evolucaoHorasCursoTuplos([Periodo|R1], [Ano|R2], Curso, Evolucao, Acumulador) :-
    horasCurso(Periodo, Curso, Ano, Tempo),
    append(Acumulador, [(Ano, Periodo, Tempo)], AcumuladorAux),
    evolucaoHorasCursoTuplos(R1, R2, Curso, Evolucao, AcumuladorAux).

evolucaoHorasCurso(Curso, Evolucao) :- 
    evolucaoHorasCursoTuplos([p1,p2,p3,p4,p1,p2,p3,p4,p1,p2,p3,p4], [1,1,1,1,2,2,2,2,3,3,3,3], Curso, Evolucao). 
    % Como tanto as licenciaturas como os mestrados sao avaliados em 3 anos,
    % ListaComPeriodos = [p1,p2,p3,p4] * 3anos 
    % ListaComAnos = [1] * 4periodos + [2] * 4periodos + [3] * 4periodos


% --------------------------------------------------------------------------------


% OCUPACOES CRITICAS DE SALAS

% ocupaSlot/5     ocupaSlot( HoraInicioDada, HoraFimDada, HoraInicioEvento, HoraFimEvento, Horas )
% verdade se Horas for o numero de horas sobrepostas entre o evento que tem inicio em HoraInicioEvento e fim em HoraFimEvento, e o slot que tem inicio em HoraInicioDada e fim em HoraFimDada

% (A, B, C, D), sendo A < B e C < D

% se B < C, -----A--B-----C--D----- => nao ha intersecao

% se D < A, -----C--D-----A--B----- => nao ha intersecao 

ocupaSlot(HoraInicioDada, HoraFimDada, HoraInicioEvento, HoraFimEvento, Horas) :-
    not(HoraFimDada < HoraInicioEvento; HoraFimEvento < HoraInicioDada), % verificar se ha sobreposicao, ver exemplo acima
    msort([HoraInicioDada, HoraFimDada, HoraInicioEvento, HoraFimEvento], ListaAux), % criar uma lista ordenada com as horas, [8.5, 9, 10.5, 11]
    nth1(2, ListaAux, ElementoMeioEsquerda), % 9
    nth1(3, ListaAux, ElementoMeioDireita), % 10.5
    Horas is ElementoMeioDireita - ElementoMeioEsquerda. % 10.5 - 9



% numHorasOcupadas/6     numHorasOcupadas( Periodo, TipoSala, DiaSemana, HoraInicio, HoraFim, SomaHoras )
% verdade se SomaHoras for o numero de horas ocupadas nas salas do tipo TipoSala, no intervalo de tempo definido entre HoraInicio e HoraFim, no dia da semana DiaSemana, e no periodo Periodo


% maisIDs/2 de forma recursiva, processo iterativo, obtem todos os IDs de eventos com salas em ListaSalas
maisIDs(ListaSalas, Lista) :- maisIDs(ListaSalas, Lista, []).
maisIDs([], Lista, Lista).
maisIDs([Sala|R], Lista, Acumulador) :-
    findall(ID, evento(ID, _, _, _, Sala), ListaIDs),
    append(Acumulador, ListaIDs, AcumuladorAux),
    maisIDs(R, Lista, AcumuladorAux).

% numHorasOcupadasAux de forma recursiva, processo iterativo, determina o tempo de horas ocupadas usando o predicado ocupaSlot/5, para cada evento dado.
numHorasOcupadasAux(ListaID, HoraInicio, HoraFim, Tempo) :- numHorasOcupadasAux(ListaID, HoraInicio, HoraFim, Tempo, 0).
numHorasOcupadasAux([], _, _, Tempo, Tempo) :- !.

numHorasOcupadasAux([ID|R], HoraInicio, HoraFim, Tempo, Acumulador) :-
    horario(ID, _, HI, HF, _, _),
    ocupaSlot(HoraInicio, HoraFim, HI, HF, T), !,
    AcumuladorAux is Acumulador + T,
    numHorasOcupadasAux(R, HoraInicio, HoraFim, Tempo, AcumuladorAux).

numHorasOcupadasAux([_|R], HoraInicio, HoraFim, Tempo, Acumulador) :-
    numHorasOcupadasAux(R, HoraInicio, HoraFim, Tempo, Acumulador).


numHorasOcupadas(Periodo, TipoSala, DiaDaSemana, HoraInicio, HoraFim, SomaHoras) :-
    (Periodo == p1; Periodo == p2), !,
    findall(ID, horario(ID, DiaDaSemana, _, _, _, Periodo), ListaID1), % todos os IDs associados a eventos com horarios em DiaDaSemana e periodo Periodo
    findall(ID, horario(ID, DiaDaSemana, _, _, _, p1_2), ListaID2), % todos os IDs associados a eventos com horarios em DiaDaSemana e periodo p1_2
    append(ListaID1, ListaID2, ListaID3),
    salas(TipoSala, Salas), % obter a lista com as salas correspondentes a TipoSala, exemplo: TipoSala = grandesAnfiteatros => Sala = ['a1', 'a2']
    maisIDs(Salas, ListaID4), % obter todos os IDs associados a eventos com salas de TipoSala
    intersection(ListaID3, ListaID4, ListaID5),
    numHorasOcupadasAux(ListaID5, HoraInicio, HoraFim, SomaHoras). % obter SomaHoras

numHorasOcupadas(Periodo, TipoSala, DiaDaSemana, HoraInicio, HoraFim, SomaHoras) :-
    (Periodo == p3; Periodo == p4), !,
    findall(ID, horario(ID, DiaDaSemana, _, _, _, Periodo), ListaID1),
    findall(ID, horario(ID, DiaDaSemana, _, _, _, p3_4), ListaID2), 
    append(ListaID1, ListaID2, ListaID3),
    salas(TipoSala, Salas), 
    maisIDs(Salas, ListaID4), 
    intersection(ListaID3, ListaID4, ListaID5),
    numHorasOcupadasAux(ListaID5, HoraInicio, HoraFim, SomaHoras). 



% ocupacaoMax/5     ocupacaoMax( TipoSala, HoraInicio, HoraFim, Max )
% verdade se Max for o numero de horas possiveis de ser ocupadas por salas do tipo TipoSala, no intervalo de tempo definido entre HoraInicio e HoraFim
ocupacaoMax(TipoSala, HoraInicio, HoraFim, Max) :-
    salas(TipoSala, ListaComSalas),
    length(ListaComSalas, Len),
    Max is (HoraFim - HoraInicio) * Len.



% percentagem/3     percentagem( SomaHoras, Max, Percentagem )
% verdade se Percentagem for a divisao de SomaHoras por Max, multiplicada por 100
percentagem(SomaHoras, Max, Percentagem) :- Percentagem is (SomaHoras / Max) * 100.



% ocupacaoCritica/4     ocupacaoCritica( HoraInicio, HoraFim, Threshold, Resultados )
% verdade se Resultados for uma lista ordenada de tuplos do tipo casosCriticos( DiaSemana, TipoSala, Percentagem ) 
% em que DiaSemana, TipoSala e Percentagem sao, um dia da semana, um tipo de sala e a sua percentagem de ocupacao, no intervalo de tempo entre HoraInicio e HoraFim supondo que a Percentagem estiver acima do valor critico Threshold

% aux333 percorre Lista com Dias da semana, para UM Tipo de Sala e UM Periodo e verifica se ha casoCritico
aux333(TipoSala, Periodo, ListaComDias, HoraInicio, HoraFim, Threshold, Res) :- aux333(TipoSala, Periodo, ListaComDias, HoraInicio, HoraFim, Threshold, Res, []).

aux333(_, _, [], _, _, _, Res, Res) :- !.

aux333(TipoSala, Periodo, [DiaDaSemana|R], HoraInicio, HoraFim, Threshold, Res, Acumulador) :-
    numHorasOcupadas(Periodo, TipoSala, DiaDaSemana, HoraInicio, HoraFim, SomaHoras),
    ocupacaoMax(TipoSala, HoraInicio, HoraFim, Max),
    percentagem(SomaHoras, Max, PercentagemAux),
    PercentagemAux > Threshold, !,
    Percentagem is ceiling(PercentagemAux),
    append(Acumulador, [casosCriticos(DiaDaSemana, TipoSala, Percentagem)], AcumuladorAux),
    aux333(TipoSala, Periodo, R, HoraInicio, HoraFim, Threshold, Res, AcumuladorAux).

aux333(TipoSala, Periodo, [_|R], HoraInicio, HoraFim, Threshold, Res, Acumulador) :-
    aux333(TipoSala, Periodo, R, HoraInicio, HoraFim, Threshold, Res, Acumulador).

%-------
% aux222 percorre Lista com Periodos para UM Tipo de Sala
aux222(TipoSala, ListaComPeriodos, HoraInicio, HoraFim, Threshold, Res) :- aux222(TipoSala, ListaComPeriodos, HoraInicio, HoraFim, Threshold, Res, []).

aux222(_ , [], _, _, _, Res, Res) :- !.

aux222(TipoSala, [Periodo|R], HoraInicio, HoraFim, Threshold, Res, Acumulador) :-
    aux333(TipoSala, Periodo, [segunda-feira, terca-feira, quarta-feira, quinta-feira, sexta-feira], HoraInicio, HoraFim, Threshold, X),
    append(Acumulador, X, AcumuladorAux),
    aux222(TipoSala, R, HoraInicio, HoraFim, Threshold, Res, AcumuladorAux).

%-------
% aux111 percorre Lista com Tipos de Sala
aux111(ListaComTipoSalas, HoraInicio, HoraFim, Threshold, Res) :- aux111(ListaComTipoSalas, HoraInicio, HoraFim, Threshold, Res, []).

aux111([], _, _, _, Res, Res) :- !.

aux111([TipoSala|R], HoraInicio, HoraFim, Threshold, Res, Acumulador) :-
    aux222(TipoSala, [p1, p2, p3, p4], HoraInicio, HoraFim, Threshold, X),
    append(Acumulador, X, AcumuladorAux),
    aux111(R, HoraInicio, HoraFim, Threshold, Res, AcumuladorAux).



ocupacaoCritica(HoraInicio, HoraFim, Threshold, Resultados) :-
    aux111([grandesAnfiteatros, pequenosAnfiteatros, salasAula, labsPC, labsElectro, labsQuimica, labsFisica, labsRedes, labsJogos, videoConf], HoraInicio, HoraFim, Threshold, ResultadosAux),
    sort(ResultadosAux, Resultados).


% --------------------------------------------------------------------------------


% ORGANIZACAO MESA

% ocupacaoMesa/3     ocupacaoMesa( ListaPessoas, ListaRestricoes, OcupacaoMesa ) - verdade se ListaPessoas for a lista com o nome das pessoas a sentar a mesa,
% ListaRestricoes for a lista de restricoes a verificar e OcupacaoMesa for uma lista com 3 listas: cada uma com pessoas no seu lado respetivo. [[x1, x2, x3], [x4, x5], [x6, x7, x8]]

% todas as combinacoes de possibilidades das restricoes cab1, cab2, honra, lado, frente, naoLado, naoFrente
cab1(Pessoa, [[_, _, _], [Pessoa, _], [_, _, _]]).

cab2(Pessoa, [[_, _, _], [_, Pessoa], [_, _, _]]).

honra(P1, P2, [[_, _, _], [P1, _], [P2, _, _]]).
honra(P1, P2, [[_, _, P2], [_, P1], [_, _, _]]).

lado(P1, P2, [[P1, P2, _], [_, _], [_, _, _]]).
lado(P1, P2, [[_, P1, P2], [_, _], [_, _, _]]).
lado(P1, P2, [[P2, P1, _], [_, _], [_, _, _]]).
lado(P1, P2, [[_, P2, P1], [_, _], [_, _, _]]).
lado(P1, P2, [[_, _, _], [_, _], [P1, P2, _]]).
lado(P1, P2, [[_, _, _], [_, _], [_, P1, P2]]).
lado(P1, P2, [[_, _, _], [_, _], [P2, P1, _]]).
lado(P1, P2, [[_, _, _], [_, _], [_, P2, P1]]).

frente(P1, P2, [[P1, _, _], [_, _], [P2, _, _]]).
frente(P1, P2, [[_, P1, _], [_, _], [_, P2, _]]).
frente(P1, P2, [[_, _, P1], [_, _], [_, _, P2]]).
frente(P1, P2, [[P2, _, _], [_, _], [P1, _, _]]).
frente(P1, P2, [[_, P2, _], [_, _], [_, P1, _]]).
frente(P1, P2, [[_, _, P2], [_, _], [_, _, P1]]).


naoLado(P1, P2, [[P1, _, P2], [_, _], [_, _, _]]).
naoLado(P1, P2, [[P1, _, _], [_, _], [P2, _, _]]).
naoLado(P1, P2, [[P1, _, _], [_, _], [_, P2, _]]).
naoLado(P1, P2, [[P1, _, _], [_, _], [_, _, P2]]).
naoLado(P1, P2, [[P2, _, P1], [_, _], [_, _, _]]).
naoLado(P1, P2, [[P2, _, _], [_, _], [P1, _, _]]).
naoLado(P1, P2, [[P2, _, _], [_, _], [_, P1, _]]).
naoLado(P1, P2, [[P2, _, _], [_, _], [_, _, P1]]).
naoLado(P1, P2, [[_, P1, _], [_, _], [P2, _, _]]).
naoLado(P1, P2, [[_, P1, _], [_, _], [_, P2, _]]).
naoLado(P1, P2, [[_, P1, _], [_, _], [_, _, P2]]).
naoLado(P1, P2, [[_, P2, _], [_, _], [P1, _, _]]).
naoLado(P1, P2, [[_, P2, _], [_, _], [_, P1, _]]).
naoLado(P1, P2, [[_, P2, _], [_, _], [_, _, P1]]).
naoLado(P1, P2, [[_, _, P1], [_, _], [P2, _, _]]).
naoLado(P1, P2, [[_, _, P1], [_, _], [_, P2, _]]).
naoLado(P1, P2, [[_, _, P1], [_, _], [_, _, P2]]).
naoLado(P1, P2, [[_, _, P2], [_, _], [P1, _, _]]).
naoLado(P1, P2, [[_, _, P2], [_, _], [_, P1, _]]).
naoLado(P1, P2, [[_, _, P2], [_, _], [_, _, P1]]).
naoLado(P1, P2, [[_, _, _], [_, _], [P2, _, P1]]).
naoLado(P1, P2, [[_, _, _], [_, _], [P1, _, P2]]).%22
naoLado(P1, P2, [[P2, _, _], [P1, _], [_, _, _]]).
naoLado(P1, P2, [[_, P2, _], [P1, _], [_, _, _]]).
naoLado(P1, P2, [[_, _, P2], [P1, _], [_, _, _]]).
naoLado(P1, P2, [[_, _, _], [P1, P2], [_, _, _]]).
naoLado(P1, P2, [[_, _, _], [P1, _], [P2, _, _]]).
naoLado(P1, P2, [[_, _, _], [P1, _], [_, P2, _]]).
naoLado(P1, P2, [[_, _, _], [P1, _], [_, _, P2]]).
naoLado(P1, P2, [[P1, _, _], [P2, _], [_, _, _]]).
naoLado(P1, P2, [[_, P1, _], [P2, _], [_, _, _]]).
naoLado(P1, P2, [[_, _, P1], [P2, _], [_, _, _]]).
naoLado(P1, P2, [[_, _, _], [P2, P1], [_, _, _]]).
naoLado(P1, P2, [[_, _, _], [P2, _], [P1, _, _]]).
naoLado(P1, P2, [[_, _, _], [P2, _], [_, P1, _]]).
naoLado(P1, P2, [[_, _, _], [P2, _], [_, _, P1]]).
naoLado(P1, P2, [[P2, _, _], [_, P1], [_, _, _]]).
naoLado(P1, P2, [[_, P2, _], [_, P1], [_, _, _]]).
naoLado(P1, P2, [[_, _, P2], [_, P1], [_, _, _]]).
naoLado(P1, P2, [[_, _, _], [_, P1], [P2, _, _]]).
naoLado(P1, P2, [[_, _, _], [_, P1], [_, P2, _]]).
naoLado(P1, P2, [[_, _, _], [_, P1], [_, _, P2]]).
naoLado(P1, P2, [[P1, _, _], [_, P2], [_, _, _]]).
naoLado(P1, P2, [[_, P1, _], [_, P2], [_, _, _]]).
naoLado(P1, P2, [[_, _, P1], [_, P2], [_, _, _]]).
naoLado(P1, P2, [[_, _, _], [_, P2], [P1, _, _]]).
naoLado(P1, P2, [[_, _, _], [_, P2], [_, P1, _]]).
naoLado(P1, P2, [[_, _, _], [_, P2], [_, _, P1]]).


naoFrente(P1, P2, [[P1, P2, _], [_, _], [_, _, _]]).
naoFrente(P1, P2, [[P1, _, P2], [_, _], [_, _, _]]).
naoFrente(P1, P2, [[P1, _, _], [_, _], [_, P2, _]]).
naoFrente(P1, P2, [[P1, _, _], [_, _], [_, _, P2]]).
naoFrente(P1, P2, [[P2, P1, _], [_, _], [_, _, _]]).
naoFrente(P1, P2, [[P2, _, P1], [_, _], [_, _, _]]).
naoFrente(P1, P2, [[P2, _, _], [_, _], [_, P1, _]]).
naoFrente(P1, P2, [[P2, _, _], [_, _], [_, _, P1]]).
naoFrente(P1, P2, [[_, P1, P2], [_, _], [_, _, _]]).
naoFrente(P1, P2, [[_, P1, _], [_, _], [P2, _, _]]).
naoFrente(P1, P2, [[_, P1, _], [_, _], [_, _, P2]]).
naoFrente(P1, P2, [[_, P2, P1], [_, _], [_, _, _]]).
naoFrente(P1, P2, [[_, P2, _], [_, _], [P1, _, _]]).
naoFrente(P1, P2, [[_, P2, _], [_, _], [_, _, P1]]).
naoFrente(P1, P2, [[_, _, P1], [_, _], [P2, _, _]]).
naoFrente(P1, P2, [[_, _, P1], [_, _], [_, P2, _]]).
naoFrente(P1, P2, [[_, _, P2], [_, _], [P1, _, _]]).
naoFrente(P1, P2, [[_, _, P2], [_, _], [_, P1, _]]).
naoFrente(P1, P2, [[_, _, _], [_, _], [_, P2, P1]]).
naoFrente(P1, P2, [[_, _, _], [_, _], [P2, _, P1]]).
naoFrente(P1, P2, [[_, _, _], [_, _], [_, P1, P2]]).
naoFrente(P1, P2, [[_, _, _], [_, _], [P1, _, P2]]).
naoFrente(P1, P2, [[_, _, _], [_, _], [P2, P1, _]]).
naoFrente(P1, P2, [[_, _, _], [_, _], [P1, P2, _]]).
naoFrente(P1, P2, [[_, _, _], [P1, P2], [_, _, _]]).
naoFrente(P1, P2, [[_, _, _], [P2, P1], [_, _, _]]).
naoFrente(P1, P2, [[P2, _, _], [P1, _], [_, _, _]]).
naoFrente(P1, P2, [[_, P2, _], [P1, _], [_, _, _]]).
naoFrente(P1, P2, [[_, _, P2], [P1, _], [_, _, _]]).
naoFrente(P1, P2, [[_, _, _], [P1, P2], [_, _, _]]).
naoFrente(P1, P2, [[_, _, _], [P1, _], [P2, _, _]]).
naoFrente(P1, P2, [[_, _, _], [P1, _], [_, P2, _]]).
naoFrente(P1, P2, [[_, _, _], [P1, _], [_, _, P2]]).
naoFrente(P1, P2, [[P1, _, _], [P2, _], [_, _, _]]).
naoFrente(P1, P2, [[_, P1, _], [P2, _], [_, _, _]]).
naoFrente(P1, P2, [[_, _, P1], [P2, _], [_, _, _]]).
naoFrente(P1, P2, [[_, _, _], [P2, P1], [_, _, _]]).
naoFrente(P1, P2, [[_, _, _], [P2, _], [P1, _, _]]).
naoFrente(P1, P2, [[_, _, _], [P2, _], [_, P1, _]]).
naoFrente(P1, P2, [[_, _, _], [P2, _], [_, _, P1]]).
naoFrente(P1, P2, [[P2, _, _], [_, P1], [_, _, _]]).
naoFrente(P1, P2, [[_, P2, _], [_, P1], [_, _, _]]).
naoFrente(P1, P2, [[_, _, P2], [_, P1], [_, _, _]]).
naoFrente(P1, P2, [[_, _, _], [_, P1], [P2, _, _]]).
naoFrente(P1, P2, [[_, _, _], [_, P1], [_, P2, _]]).
naoFrente(P1, P2, [[_, _, _], [_, P1], [_, _, P2]]).
naoFrente(P1, P2, [[P1, _, _], [_, P2], [_, _, _]]).
naoFrente(P1, P2, [[_, P1, _], [_, P2], [_, _, _]]).
naoFrente(P1, P2, [[_, _, P1], [_, P2], [_, _, _]]).
naoFrente(P1, P2, [[_, _, _], [_, P2], [P1, _, _]]).
naoFrente(P1, P2, [[_, _, _], [_, P2], [_, P1, _]]).
naoFrente(P1, P2, [[_, _, _], [_, P2], [_, _, P1]]).


% para verficar e adicionar convidado restante
addicionar(P, [[P, _, _], [_, _], [_, _, _]]).
addicionar(P, [[_, P, _], [_, _], [_, _, _]]).
addicionar(P, [[_, _, P], [_, _], [_, _, _]]).
addicionar(P, [[_, _, _], [P, _], [_, _, _]]).
addicionar(P, [[_, _, _], [_, P], [_, _, _]]).
addicionar(P, [[_, _, _], [_, _], [P, _, _]]).
addicionar(P, [[_, _, _], [_, _], [_, P, _]]).
addicionar(P, [[_, _, _], [_, _], [_, _, P]]).


% caso terminal final
ocupacaoMesa([], [], _) :- !.

% caso terminal para quando ja foram feitas todas as restricoes => ListaRestricoes = []
ocupacaoMesa([P|R], [], OcupacaoMesa) :-
    addicionar(P, OcupacaoMesa), % verificar se todos os convidados da ListaPessoas estao em lugares, caso nao esteja um, este e adicionado
    ocupacaoMesa(R, [], OcupacaoMesa).


ocupacaoMesa(ListaPessoas, [P|R], OcupacaoMesa) :-
    P =.. [Nome|Arg], % obter 1 restricao da lista
    length(Arg, Len), 
    Len == 1, !, % se restricao tiver apenas um argumento, ex: cab1(maria)
    Arg = [Argumento|_],
    Restricao =.. [Nome|[Argumento, OcupacaoMesa]], % acrescentar 1 argumento no predicado para unificar com as possibilidades -> cab1(maria, X)
    Restricao, 
    ocupacaoMesa(ListaPessoas, R, OcupacaoMesa).

ocupacaoMesa(ListaPessoas, [P|R], OcupacaoMesa) :-
    P =.. [Nome|Arg], 
    length(Arg, Len),
    Len == 2, !, % se restricao tiver apenas um argumento, ex: honra(joao, guga)
    Arg = [Arg1, Arg2|_],
    Restricao =.. [Nome|[Arg1, Arg2, OcupacaoMesa]], % acrescentar 1 argumento no predicado para unificar com as possibilidades -> honra(joao, guga, X)
    Restricao,
    ocupacaoMesa(ListaPessoas, R, OcupacaoMesa).
