# Testar a funcionalidade da aplicação
## Correr a aplicação
```bash
sudo docker compose -f docker-compose..app.yml up
```

### 3.2 Lista todas as especialidades oferecidas na \<clinica\>
```html
/c/<clinica>/
```
<br>

testes success:
* `http://127.0.0.1:8080/c/Clínica Lusíadas/` 
* `http://127.0.0.1:8080/c/Clínica S. João de Deus/`
* `http://127.0.0.1:8080/c/Clínica Boa Saúde/`
* `http://127.0.0.1:8080/c/Clínica Urgimédica/`
* `http://127.0.0.1:8080/c/Clínica CUF/`


testes fail:
* `http://127.0.0.1:8080/c/DROP DATABASE Saude;/`
* `http://127.0.0.1:8080/c/abcdefghijklmnopqrstuvwxyz/`

<br>

### 3.3 Lista todos os médicos da \<especialidade\> que trabalham na \<clinica\> e os primeiros 3 horarios disponiveis para consulta
```html
/c/<clinica>/<especialidade>
```
testes success:
* `http://127.0.0.1:8080/c/Clínica Lusíadas/clínica geral/`
* `http://127.0.0.1:8080/c/Clínica S. João de Deus/cardiologia/`
* `http://127.0.0.1:8080/c/Clínica Boa Saúde/ortopedia/`

testes fail:
* `http://127.0.0.1:8080/c/Clínica CUF/ortopedia/`
* `http://127.0.0.1:8080/c/abcdefghijklmnopqrstuvwxyz/clínica geral/`
* `http://127.0.0.1:8080/c/Clínica Lusíadas/abcdefghijklmnopqrstuvwxyz/`

<br>

### 3.4 - Registrar uma marcação de consulta na \<clínica\> na base de dados - Recebe como argumento: paciente, médico e uma data e hora
```html
/a/<clinica>/registar/
```
<br>

**Usar como referência o paciente 00000000009 e o médico 000000057**
```sql
-- consultas do paciente com ssn = 00000000009
select c.ssn as pacienteSSN, c.nif as medicoNIF, c.nome as clinicaNOME, c.data, c.hora from consulta c where c.ssn = '00000000009';

 pacientessn | mediconif |       clinicanome       |    data    |   hora   
-------------+-----------+-------------------------+------------+----------
 00000000009 | 000000076 | Clínica Lusíadas        | 2023-01-01 | 09:00:00
 00000000009 | 000000073 | Clínica S. João de Deus | 2023-01-20 | 10:30:00
 00000000009 | 000000032 | Clínica Boa Saúde       | 2023-02-08 | 08:00:00
 00000000009 | 000000046 | Clínica Urgimédica      | 2023-02-27 | 10:30:00
 00000000009 | 000000044 | Clínica CUF             | 2023-03-18 | 09:00:00
 00000000009 | 000000054 | Clínica Lusíadas        | 2023-09-08 | 15:00:00
 00000000009 | 000000056 | Clínica S. João de Deus | 2023-09-27 | 15:30:00
 00000000009 | 000000077 | Clínica Boa Saúde       | 2023-10-16 | 15:30:00
 00000000009 | 000000052 | Clínica Urgimédica      | 2023-11-04 | 16:30:00
 00000000009 | 000000063 | Clínica CUF             | 2023-11-23 | 18:30:00
 00000000009 | 000000086 | Clínica Lusíadas        | 2024-05-15 | 09:00:00
 00000000009 | 000000041 | Clínica S. João de Deus | 2024-06-03 | 08:00:00
 00000000009 | 000000086 | Clínica Boa Saúde       | 2024-06-22 | 11:00:00
 00000000009 | 000000036 | Clínica Urgimédica      | 2024-07-11 | 17:30:00
 00000000009 | 000000059 | Clínica CUF             | 2024-07-30 | 14:30:00

```

```sql
-- consultas do medico com nif = 000000057 entre dia 2 e 10 de junho 2024
select c.ssn as pacientessn, c.nif as mediconif, c.nome as clinicanome, c.data, c.hora from consulta c where c.nif = '000000057' and c.data between '2024-06-02' and '2024-06-10';

 pacientessn | mediconif |       clinicanome       |    data    |   hora   
-------------+-----------+-------------------------+------------+----------
 00000002550 | 000000057 | Clínica S. João de Deus | 2024-06-02 | 11:30:00
 00000002552 | 000000057 | Clínica S. João de Deus | 2024-06-02 | 15:30:00
 00000003870 | 000000057 | Clínica CUF             | 2024-06-03 | 16:00:00
 00000003871 | 000000057 | Clínica CUF             | 2024-06-03 | 10:30:00
 00000003884 | 000000057 | Clínica CUF             | 2024-06-04 | 10:30:00
 00000003885 | 000000057 | Clínica CUF             | 2024-06-04 | 10:00:00
 00000000050 | 000000057 | Clínica S. João de Deus | 2024-06-05 | 10:30:00
 00000000051 | 000000057 | Clínica S. João de Deus | 2024-06-05 | 14:00:00
 00000000456 | 000000057 | Clínica Lusíadas        | 2024-06-06 | 15:00:00
 00000000457 | 000000057 | Clínica Lusíadas        | 2024-06-06 | 11:30:00
 00000004324 | 000000057 | Clínica Urgimédica      | 2024-06-07 | 15:00:00
 00000004325 | 000000057 | Clínica Urgimédica      | 2024-06-07 | 15:30:00
 00000000484 | 000000057 | Clínica Lusíadas        | 2024-06-08 | 17:30:00
 00000000485 | 000000057 | Clínica Lusíadas        | 2024-06-08 | 15:30:00
 00000002844 | 000000057 | Clínica S. João de Deus | 2024-06-09 | 09:30:00
 00000002846 | 000000057 | Clínica S. João de Deus | 2024-06-09 | 10:00:00
 00000004010 | 000000057 | Clínica CUF             | 2024-06-10 | 18:30:00
 00000004011 | 000000057 | Clínica CUF             | 2024-06-10 | 09:30:00

```

<br>

testes success:
* `http://127.0.0.1:8080/a/Clínica Lusíadas/registar/?paciente=00000000009&médico=000000057&data=2024-06-08&hora=10:30`
* `http://127.0.0.1:8080/a/Clínica CUF/registar/?paciente=00000000009&médico=000000057&data=2024-06-10&hora=10:30`

testes fail:
* unicidade - marcar duas vezes a mesma consulta: `http://127.0.0.1:8080/a/Clínica Lusíadas/registar/?paciente=00000000009&médico=000000057&data=2024-06-08&hora=10:30`
* marcar no passado: `http://127.0.0.1:8080/a/Clínica S. João de Deus/registar/?paciente=00000000009&médico=000000057&data=2024-06-02&hora=10:30`
* clínica inexistente: `http://127.0.0.1:8080/a/abcdefghijklmnopqrstuvwxyz/registar/?paciente=00000000009&médico=000000057&data=2024-06-08&hora=10:30`
* médico trabalha em outra clínica: `http://127.0.0.1:8080/a/Clínica CUF/registar/?paciente=00000000009&médico=000000057&data=2024-06-08&hora=10:30`
* hora inválida: `http://127.0.0.1:8080/a/Clínica Lusíadas/registar/?paciente=00000000009&médico=000000057&data=2024-06-08&hora=03:00`
* minutos inválidos: `http://127.0.0.1:8080/a/Clínica Lusíadas/registar/?paciente=00000000009&médico=000000057&data=2024-06-08&hora=10:33`
* paciente inválido: `http://127.0.0.1:8080/a/Clínica Lusíadas/registar/?paciente=01000000000&médico=000000057&data=2024-06-08&hora=10:30`
* médico inválido: `http://127.0.0.1:8080/a/Clínica Lusíadas/registar/?paciente=00000000009&médico=010000000&data=2024-06-08&hora=10:30`



### 3.5 - Cancelar uma marcação de consulta ainda não realizada na \<clínica\> na base de dados - Recebe como argumento: paciente, médico e uma data e hora
```html
/a/<clinica>/cancelar/
```

<br>

testes success:
* `http://127.0.0.1:8080/a/Clínica Lusíadas/cancelar/?paciente=00000000009&médico=000000057&data=2024-06-08&hora=10:30`
* `http://127.0.0.1:8080/a/Clínica CUF/cancelar/?paciente=00000000009&médico=000000057&data=2024-06-10&hora=10:30`

testes fail:
* ...
