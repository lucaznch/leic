# Databases - 2023/2024- Project - Delivery 2
## 0. Database Loading
Create the “Saude” database in PostgreSQL and execute the commands to create the tables in this database presented in Appendix A

## 1. Integrity Constraints
Implement the following integrity restrictions in the “Saude” database, and may resort to procedural extensions (Triggers and Stored Procedures) if strictly necessary:
- **RI-1** - Os horários das consultas são à hora exata ou meia-hora no horário 8-13h e 14-19h
- **RI-2** - Um médico não se pode consultar a si próprio, embora possa ser paciente de outros médicos no sistema
- **RI-3** - Um médico só pode dar consultas na clínica em que trabalha no dia da semana correspondente à data da consulta

<br>

The ON DELETE CASCADE and ON UPDATE CASCADE mechanisms are not allowed.

## 2. Completing the Database
Populate all database tables consistently (after executing the previous point) with the following additional coverage requirements:

- 5 clínicas, de pelo menos 3 localidades diferentes do distrito de Lisboa
- 5-6 enfermeiros por clínica
- 20 médicos de especialidade ‘clínica geral’ e 40 outros distribuídos como entender por até 5 outras especialidades médicas (incluindo pelo menos, ‘ortopedia’ e ‘cardiologia’). Cada médico deve trabalhar em pelo menos duas clínicas, e em cada clínica a cada dia da semana (incluindo fins de semana), devem estar pelo menos 8 médicos
- Cerca de 5.000 pacientes
- Um número mínimo de consultas em 2023 e 2024 tais que cada paciente tem pelo menos uma consulta, e em cada dia há pelo menos 20 consultas por clínica, e pelo menos 2 consultas por médico
- ~80% das consultas tem receita médica associada, e as receitas têm 1 a 6 medicamentos em quantidades entre 1 e 3
- Todas as consultas têm 1 a 5 observações de sintomas (com parâmetro mas sem valor) e 0 a 3 observações métricas (com parâmetro e valor). Deve haver ~50 parâmetros diferentes para os sintomas (sem valor) e ~20 parâmetros diferentes para as observações métricas (com valor) e os dois conjuntos devem ser disjuntos.
- Todas as moradas são nacionais e seguem o formato Português, terminando com código postal: XXXX-XXX e de seguida a localidade.


## 3. Application Development
Create a RESTful web service prototype for managing queries through programmatic access to the ‘Saude’ database through an API that returns responses in JSON, implementing the following REST endpoints:

| Endpoint | Descrição |
|:---:|:---|
| **/** | Lista todas as clínicas (nome e morada) |
| **/c/\<clinica\>/** | Lista todas as especialidades oferecidas na <clinica> |
| **/c/\<clinica\>/\<especialidade\>/** | Lista todos os médicos (nome) da <especialidade> que trabalham na <clínica> e os primeiros três horários disponíveis para consulta de cada um deles (data e hora) |
| **/a/\<clinica\>/cancelar/** | Registra uma marcação de consulta na <clinica> na base de dados (populando a respectiva tabela). Recebe como argumentos um paciente, um médico, e uma data e hora (posteriores ao momento de agendamento) |
| **/a/\<clinica\>/cancelar/** | Cancela uma marcação de consulta que ainda não se realizou na <clinica> (o seu horário é posterior ao momento do cancelamento), removendo a entrada da respectiva tabela na base de dados. Recebe como argumentos um paciente, um médico, e uma data e hora |

> [!NOTE]
> Um horário está disponível para consulta se é posterior ao momento da interrogação, cumpre as restrições horárias da tabela consulta (RI-1 do ponto 1), e o médico não tem ainda entrada na tabela consulta nesse horário

<br>

**Bruno API Client**

<br>


## 4. Views
Create a materialized view that details the most important information about **consultas dos pacientes** by combining information from several database tables. The view must have the following layout:

```sql
historial_paciente(id, ssn, nif, nome, data, ano, mes, dia_do_mes, localidade, especialidade, tipo, chave, valor)
```

on what:
- id, ssn, nif, nome e data: correspondem ao atributos homónimos da tabela consulta
- ano, mes e dia_do_mes: são derivados do atributo data da tabela consulta2
- localidade: é derivado do atributo morada da tabela clinica
- especialidade: corresponde ao atributo homónimo da tabela medico
- tipo: toma os valores ‘observacao’ ou ‘receita’ consoante o preenchimento dos campos seguintes
- chave: corresponde ao atributo parametro da tabela observacao ou ao atributo medicamento da tabela receita
- valor: corresponde ao atributo valor da tabela observacao ou ao atributo quantidade da tabela receita

## 5. Data Analysis (SQL and OLAP)
Using the view developed in the previous point, complemented with other tables from the ‘Saude’ database when necessary, present the most succinct SQL query for each of the following analytical objectives. You can use the ROLLUP, CUBE, GROUPING SETS statements or the UNION of GROUP BY clauses for whatever purposes you see fit.


1. Determinar que paciente(s) tiveram menos progresso no tratamento das suas doenças do foro ortopédico para atribuição de uma consulta gratuita. Considera-se que o indicador de falta de progresso é o intervalo temporal máximo entre duas observações do mesmo sintoma (i.e. registos de tipo ‘observacao’ com a mesma chave e com valor NULL) em consultas de ortopedia.
2. Determinar que medicamentos estão a ser usados para tratar doenças crónicas do foro cardiológico. Considera-se que qualificam quaisquer medicamentos receitados ao mesmo paciente (qualquer que ele seja) pelo menos uma vez por mês durante os últimos doze meses, em consultas de cardiologia.
3. Explorar as quantidades totais receitadas de cada medicamento em 2023, globalmente, e com drill down nas dimensões espaço (localidade > clinica), tempo (mes > dia_do_mes), e médico (especialidade > nome [do médico]), separadamente.
4. Determinar se há enviesamento na medição de algum parâmetros entre clínicas, especialidades médicas ou médicos, sendo para isso necessário listar o valor médio e desvio padrão de todos os parâmetros de observações métricas (i.e. com valor não NULL) com drill down na dimensão médico (globalmente > especialidade > nome [do médico]) e drill down adicional (sobre o anterior) por clínica.

> [!NOTE]
> You can use the EXTRACT() function to obtain parts of dates or timestamps and you can use the SUBSTRING() function specifying a POSIX standard to extract the *localidade* name after the postal code of the address.

## 6. Indexes
Present the SQL instructions for creating indexes to improve the times of each of the queries listed below on the ‘Saude’ database. Justify your choice of table(s), attribute(s) and index type(s), explaining which operations would be optimized and how. Assume that there are no indexes on the tables other than those implied by declaring primary and foreign keys, and for the purposes of this exercise, assume that the size of the tables exceeds the available memory by several orders of magnitude.

### 6.1
```sql
SELECT nome
FROM paciente
JOIN consulta USING (ssn)
JOIN observacao USING (id)
WHERE parametro = ‘pressão diastólica’
AND valor >= 9;
```

## 6.2
```sql
SELECT especialidade, SUM(quantidade) AS qtd
FROM medico
JOIN consulta USING (nif)
JOIN receita USING (codigo_ssn)WHERE data BETWEEN ‘2023-01-01’ AND ‘2023-12-31’
GROUP BY especialidade
ORDER BY qtd;
```

<br>
<br>
<br>
<br>
<br>
<br>


#### Appendix A - SQL Schema
```sql
DROP TABLE IF EXISTS clinica CASCADE;
DROP TABLE IF EXISTS enfermeiro CASCADE;
DROP TABLE IF EXISTS medico CASCADE;
DROP TABLE IF EXISTS trabalha CASCADE;
DROP TABLE IF EXISTS paciente CASCADE;
DROP TABLE IF EXISTS receita CASCADE;
DROP TABLE IF EXISTS consulta CASCADE;
DROP TABLE IF EXISTS observacao CASCADE;
CREATE TABLE clinica(
nome VARCHAR(80) PRIMARY KEY,
telefone VARCHAR(15) UNIQUE NOT NULL CHECK (telefone ~ '^[0-9]+$'),
morada VARCHAR(255) UNIQUE NOT NULL
);
CREATE TABLE enfermeiro(
nif CHAR(9) PRIMARY KEY CHECK (nif ~ '^[0-9]+$'),
nome VARCHAR(80) UNIQUE NOT NULL,
telefone VARCHAR(15) NOT NULL CHECK (telefone ~ '^[0-9]+$'),
morada VARCHAR(255) NOT NULL,
nome_clinica VARCHAR(80) NOT NULL REFERENCES clinica (nome)
);
CREATE TABLE medico(
nif CHAR(9) PRIMARY KEY CHECK (nif ~ '^[0-9]+$'),
nome VARCHAR(80) UNIQUE NOT NULL,
telefone VARCHAR(15) NOT NULL CHECK (telefone ~ '^[0-9]+$'),
morada VARCHAR(255) NOT NULL,
especialidade VARCHAR(80) NOT NULL
);
CREATE TABLE trabalha(
nif CHAR(9) NOT NULL REFERENCES medico,
nome VARCHAR(80) NOT NULL REFERENCES clinica,
dia_da_semana SMALLINT,
PRIMARY KEY (nif, dia_da_semana)
);
CREATE TABLE paciente(
ssn CHAR(11) PRIMARY KEY CHECK (ssn ~ '^[0-9]+$'),
nif CHAR(9) UNIQUE NOT NULL CHECK (nif ~ '^[0-9]+$'),
nome VARCHAR(80) NOT NULL,
telefone VARCHAR(15) NOT NULL CHECK (telefone ~ '^[0-9]+$'),
morada VARCHAR(255) NOT NULL,
data_nasc DATE NOT NULL
);CREATE TABLE consulta(
id SERIAL PRIMARY KEY,
ssn CHAR(11) NOT NULL REFERENCES paciente,
nif CHAR(9) NOT NULL REFERENCES medico,
nome VARCHAR(80) NOT NULL REFERENCES clinica,
data DATE NOT NULL,
hora TIME NOT NULL,
codigo_sns CHAR(12) UNIQUE CHECK (codigo_sns ~ '^[0-9]+$'),
UNIQUE(ssn, data, hora),
UNIQUE(nif, data, hora)
);
CREATE TABLE receita(
codigo_sns VARCHAR(12) NOT NULL REFERENCES consulta (codigo_sns),
medicamento VARCHAR(155) NOT NULL,
quantidade SMALLINT NOT NULL CHECK (quantidade > 0),
PRIMARY KEY (codigo_sns, medicamento)
);
CREATE TABLE observacao(
id INTEGER NOT NULL REFERENCES consulta,
parametro VARCHAR(155) NOT NULL,
valor FLOAT,
PRIMARY KEY (id, parametro)
);
```
