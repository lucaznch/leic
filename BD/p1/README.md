# First Delivery of Databases (2023/24)

## Entity-Association Modeling:

### Problem Domain:

Uma empresa de prestação de cuidados de saúde possui várias clínicas, cada uma identificada pelo seu nome e tendo ainda uma morada e um número de telefone únicos.

A empresa tem a contrato vários profissionais de saúde que incluem médicos e enfermeiros. Cada profissional de saúde é identificado pelo seu NIF, e é ainda caracterizado por: nome, morada, telefone e IBAN. Apenas os médicos têm ainda uma especialidade escolhida de uma lista de especialidades reconhecidas pela ordem dos médicos (e.g. clínica geral, ortopedia). Os enfermeiros estão alocados a uma única clínica, ao passo que os médicos podem trabalhar em várias ao longo da semana, mas a cada dia da semana estão alocados a uma e apenas uma clínica, num regime fixo.

A empresa presta serviços de saúde a pacientes, sendo estes identificados pelo seu número do SNS, e caracterizados por nome, NIF, data de nascimento, morada e número de telefone. Os pacientes podem ter um ou mais sistemas de saúde (e.g. ADSE, AdvanceCare), identificados pelo nome do sistema e pelo número de sistema nacional de saúde do paciente.

Os serviços de saúde dividem-se em: serviços de enfermagem, realizados por enfermeiros; e consultas, realizadas por médicos. Os serviços de enfermagem não carecem de marcação prévia, mas as consultas requerem marcação através do sistema de informação da empresa.

A marcação de consultas consiste na alocação de um determinado paciente a um determinado médico, numa determinada clínica, e num determinado período de 30 minutos entre as 8:00 e as 20:00 dos dias úteis (e.g. 8:30, 17:00). A marcação tem evidentemente de obedecer às restrições espaço-temporais de senso comum (e.g., em cada período de tempo, quer um paciente quer um médico só pode ter uma marcação).

Em cada consulta são registados qualquer número de sintomas descritos pelo paciente e/ou observados pelo médico, alguns dos quais são qualitativos (i.e., consistem apenas no nome do sintoma, escolhido de uma lista proveniente do vocabulário controlado SNOMED CT) ao passo que outros são quantitativos (i.e., consistem num valor para um parâmetro observado, como pressão arterial ou pulsação). No caso do médico prescrever medicamentos em resultado da consulta, está ainda associado ao registo da consulta uma receita médica, identificada por um código do SNS, e que contém quantidades de um ou mais medicamentos escolhidos da lista oficial do Infarmed. Cada receita está associada a uma única consulta.

### To be evaluated:

- [X] Design an Entity-Association model that represents the information requirements of an application with the above domain;
- [X] Identify inconsistent situations in the problem domain, but which are permitted in the Entity-Association model presented, and define a set of integrity relationships that complete the proposed model in order to restrict invalid situations.


## E-A Conversion -&gt; Relational:

### E-A model to represent:

![](https://holocron.so/uploads/691627ee-screenshot-from-2024-04-19-21-16-47.png)

### To be evaluated:

- [X] Develop a Relational Model corresponding to the Entity-Association model presented above, using the notation exposed in the theoretical classes (briefly demonstrated below);
- [X] Identify all Integrity Restrictions of the Entity-Association model presented that are not subject to conversion to the Relational Model;
- [X] Complement the Relational Model with a set of Integrity Constraints that represent the E-A model presented with greater fidelity, including its Integrity Constraints.


## Relational Algebra and SQL:

### Relational Schema to take into account:

```sql
clinica (nome, morada)
		unique(morada)

medico (NIF, nome, especialidade)

paciente (SSN, NIF, nome, morada)
		unique(NIF)

consulta (periodo, nome, NIF, SSN)
		nome : FK(clinica:nome)
		NIF : FK (medico:NIF)
		SSN : FK(paciente:SSN)
```

### To be evaluated:

Before solving the following questions, a direct conversion of the schema to SQL must be assumed, in which the **period is of type TIMESTAMP** and the **remaining attributes are of type VARCHAR**.

The questions are as follows:

1. What is the most concise relational algebra expression for the question “quais os pacientes que consultaram médicos de todas as especialidades”?

2. Indicate the question in natural language that corresponds to the following relational algebra expression:

 ![](https://holocron.so/uploads/bb0d7495-screenshot-from-2024-04-19-21-29-53.png)


3. Indicate the question in natural language that corresponds to the following SQL expression:

```sql
SELECT p.SSN, p.nome
FROM paciente p JOIN consulta c ON p.SSN = c.SSN
GROUP BY p.SSN, p.nome, DATE(c.periodo)
HAVING COUNT(*) > 1;
```

4. The aim is to determine which doctor has more loyal patients (i.e., patients who consult other doctors in the same specialty less frequently in proportion to the number of times they consult the doctor). Comment on the following SQL expression presented by ChatGPT for this question indicating whether the expression is semantically correct (in relation to the intended question) and if not, which aspects of the intended question are not adequately covered by the expression:

```sql
SELECT m.nome AS nome_medico,
COUNT(c.periodo) AS consultas_medico,
COUNT(DISTINCT p.NIF) AS total_pacientes,
COUNT(c.periodo) / COUNT(DISTINCT p.NIF) AS
proporcao_fidelidade
FROM medico m
INNER JOIN consulta c ON m.NIF = c.NIF
INNER JOIN paciente p ON c.SSN = p.SSN
WHERE c.periodo IN (
SELECT MIN(periodo)
FROM consulta
WHERE NIF = m.NIF
GROUP BY SSN
)
GROUP BY m.nome
ORDER BY proporcao_fidelidade DESC;
```

