import random
from datetime import datetime, timedelta


NUM_CLINICAS = 5
NUM_ENFERMEIROS_POR_CLINCA = 6
NUM_MEDICOS_GERAL = 20
NUM_MEDICOS_ESPECIALIZADOS = 40
NUM_PACIENTES = 5000 - (NUM_CLINICAS * NUM_ENFERMEIROS_POR_CLINCA + NUM_MEDICOS_GERAL + NUM_MEDICOS_ESPECIALIZADOS)
NUM_CONSULTAS = 73000
NUM_CONSULTAS_POR_DIA_POR_CLINICA = 20
NUM_RECEITAS = 58400
NUM_OBSERVACOES = 73000
start_date = datetime(2023, 1, 1)
end_date = datetime(2024, 12, 31)



primeiro_nome = ('João', 'José', 'Manuel', 'Rui', 'Pedro', 'António', 'Paulo', 'Guilherme', 'Moisés')
segundo_nome = ('Luís', 'Gabriel', 'Jorge', 'Fernando', 'Carlos', 'Vasco', 'Filipe', 'Alberto', 'Diogo') 
terceiro_nome = ('Lucas', 'Ricardo', 'Mário','Nuno', 'Hugo', 'Joaquim', 'Miguel', 'Rafael', 'Tiago')
ultimo_nome = ('Silva', 'Santos', 'Fernandes', 'Pereira', 'Oliveira', 'Costa', 'Rodrigues', 'Martins', 'Jesus')


primeiro_nome_rua = ('Rua', 'Avenida', 'Travessa', 'Largo', 'Praça', 'Alameda', 'Estrada', 'Calçada', 'Beco')
segundo_nome_rua = ('Amarela', 'Feliz', 'Grande', 'Principal', 'Azul', 'Principal', 'Pequeno', 'Alto', 'do Mar')
cidades = ('Lisboa', 'Sintra', 'Cascais', 'Oeiras', 'Amadora')
codigos_postais = { 'Lisboa':'1249-970',
                    'Sintra':'2710-430',
                    'Cascais':'2645-543',
                    'Oeiras':'2780-117',
                    'Amadora':'1150-343'}


especialidades = ('ortopedia', 'cardiologia', 'oftalmologia', 'pediatria', 'neurologia', 'psiquiatria', 'dermatologia', 'ginecologia', 'urologia', 'otorrinolaringologia')

medicamentos = ('Paracetamol', 'Ibuprofeno', 'Aspirina', 'Omeprazol', 'Amoxicilina', 'Diazepam', 'Clonazepam', 'Sertralina', 'Fluoxetina', 'Risperidona', 'Quetiapina', 'Lorazepam', 'Venlafaxina', 'Mirtazapina', 'Pregabalina', 'Bupropiona', 'Duloxetina', 'Metadona', 'Tramadol', 'Codeína', 'Morfina', 'Oxicodona', 'Fentanilo', 'Metilfenidato', 'Modafinil', 'Atomoxetina', 'Lisdexanfetamina', 'Buprenorfina', 'Naloxona', 'Naltrexona', 'Benzodiazepinas', 'Anfetaminas', 'Cocaína', 'Heroína', 'MDMA', 'LSD', 'Cannabis')

observacoes_sem_valor = ('Febre', 'Dor de cabeça', 'Dor de garganta', 'Dor de estômago', 'Dor nas costas', 'Dor nas pernas', 'Dor nos braços', 'Dor nos pés', 'Dor nos olhos', 'Dor nos ouvidos', 'Dor nos dentes', 'Dor nos rins', 'Dor nos pulmões', 'Dor no coração', 'Dor no fígado', 'Dor no pâncreas', 'Dor no baço', 'Dor no intestino', 'Dor no estômago', 'Dor no cérebro', 'Dor no sangue', 'Dor no sistema nervoso', 'Dor no sistema circulatório', 'Dor no sistema respiratório', 'Dor no sistema digestivo', 'Dor no sistema urinário', 'Tontura', 'Náuseas', 'Vómitos', 'Diarreia', 'Prisão de ventre', 'Sangue nas fezes', 'Sangue na urina', 'Sangue no vómito', 'Sangue no esperma', 'Sangue na saliva', 'Sangue no suor', 'Sangue nas lágrimas', 'Sangue no leite materno', 'Sangue no líquido cefalorraquidiano', 'Sangue no líquido sinovial', 'Sangue no líquido pleural', 'Sangue no líquido pericárdico', 'Sangue no líquido peritoneal', 'Sangue no líquido amniótico', 'Sangue no líquido intersticial', 'Tosse', 'Expectoração', 'Falta de ar', 'Dificuldade em respirar', 'Dor ao respirar', 'Dor ao engolir')

observacoes_com_valor = ('Pressão Arterial alta', 'Glicemia alta', 'Colesterol alto', 'Triglicéridos altos', 'Ácido úrico alto', 'Ureia alta', 'Creatinina alta', 'Hemoglobina alta', 'Hematócritos altos', 'Leucócitos altos', 'Plaquetas altas', 'VCM alto', 'HCM alto', 'CHCM alto', 'RDW alto', 'Volume plaquetário médio alto', 'Hemoglobina corpuscular média alta', 'Glicemia baixa', 'Colesterol baixo', 'Triglicéridos baixos', 'Ácido úrico baixo', 'Ureia baixa', 'Creatinina baixa', 'Hemoglobina baixa', 'Hematócritos baixos', 'Leucócitos baixos', 'Plaquetas baixas', 'VCM baixo', 'HCM baixo', 'CHCM baixo', 'RDW baixo', 'Volume plaquetário médio baixo', 'Hemoglobina corpuscular média baixa')



all_dates = []
current_date = start_date
while current_date <= end_date:
    all_dates.append(current_date.strftime('%Y-%m-%d'))
    current_date += timedelta(days=1)



clinicas = (
    ('Clínica AB', '900000000', 'Rua da Clínica AB 1249-970 Lisboa'),
    ('Clínica BC', '900000001', 'Rua da Clínica BC 1249-970 Lisboa'),
    ('Clínica CD', '900000002', 'Rua da Clínica CD 596C-+5M Porto'),
    ('Clínica DE', '900000003', 'Rua da Clínica DE 2388-+H2 Faro'),
    ('Clínica EF', '900000004', 'Rua da Clínica EF 6H4F-+48 Coimbra')) # 5 clinicas
enfermeiros = [] # 30 enfermeiros (6 enfermeiros por clínica = 6 * 5)
medicos = [] # 60 médicos (20 médicos gerais + 40 médicos especializados)
trabalhar = [] # 420 trabalhar (60 médicos * 7 dias da semana = 60 * 7)
pacientes = []
consultas = []
receitas = []
observacoes = []



# Enfermeiros - NIF, Nome, Telefone, Morada, Nome da Clínica
# Primary key: NIF
# Foreign key: Nome da Clínica
# Unique: Nome
nomes_de_enfermeiros_set = set()
TELEFONE = 910000000
NIF = 100000000
for i in range(NUM_CLINICAS):
    for j in range(NUM_ENFERMEIROS_POR_CLINCA):
        e = []
        e.append(str(NIF))
        NIF += 1 
        
        nome = f'{random.choice(primeiro_nome)} {random.choice(segundo_nome)} {random.choice(terceiro_nome)} {random.choice(ultimo_nome)}'
        while nome in nomes_de_enfermeiros_set:
            nome = f'{random.choice(primeiro_nome)} {random.choice(segundo_nome)} {random.choice(terceiro_nome)} {random.choice(ultimo_nome)}'
        nomes_de_enfermeiros_set.add(nome)
        
        e.append(nome)
        e.append(str(TELEFONE))
        TELEFONE += 1

        cidade = random.choice(cidades)
        e.append(f'{random.choice(primeiro_nome_rua)} {random.choice(segundo_nome_rua)} {codigos_postais[cidade]} {cidade}')

        e.append(clinicas[i][0])

        enfermeiros.append(tuple(e))


# Medicos (geral) - NIF, Nome, Telefone, Morada, Especialidade
# Primary key: NIF
# Unique: Nome
nomes_de_medicos_set = set()
for i in range(NUM_MEDICOS_GERAL):
    m = []
    m.append(str(NIF))
    NIF += 1 
    
    nome = f'{random.choice(primeiro_nome)} {random.choice(segundo_nome)} {random.choice(terceiro_nome)} {random.choice(ultimo_nome)}'
    while nome in nomes_de_medicos_set:
        nome = f'{random.choice(primeiro_nome)} {random.choice(segundo_nome)} {random.choice(terceiro_nome)} {random.choice(ultimo_nome)}'
    nomes_de_medicos_set.add(nome)
    
    m.append(nome)
    m.append(str(TELEFONE))
    TELEFONE += 1

    cidade = random.choice(cidades)
    m.append(f'{random.choice(primeiro_nome_rua)} {random.choice(segundo_nome_rua)} {codigos_postais[cidade]} {cidade}')

    m.append('clinica geral')

    medicos.append(tuple(m))

# Medicos (especializados) - NIF, Nome, Telefone, Morada, Especialidade
# Primary key: NIF
# Unique: Nome
for i in range(NUM_MEDICOS_ESPECIALIZADOS):
    m = []
    m.append(str(NIF))
    NIF += 1 
    
    nome = f'{random.choice(primeiro_nome)} {random.choice(segundo_nome)} {random.choice(terceiro_nome)} {random.choice(ultimo_nome)}'
    while nome in nomes_de_medicos_set:
        nome = f'{random.choice(primeiro_nome)} {random.choice(segundo_nome)} {random.choice(terceiro_nome)} {random.choice(ultimo_nome)}'
    nomes_de_medicos_set.add(nome)
    
    m.append(nome)
    m.append(str(TELEFONE))
    TELEFONE += 1

    cidade = random.choice(cidades)
    m.append(f'{random.choice(primeiro_nome_rua)} {random.choice(segundo_nome_rua)} {codigos_postais[cidade]} {cidade}')

    m.append(random.choice(especialidades))

    medicos.append(tuple(m))


# Trabalhar - NIF, Nome da Clínica, Dia da semana
# Primary key: (NIF, Dia da semana)
for m in medicos:
    for i in range(7):
        t = []

        if i > 3:
            c = random.choice(['Clínica AB', 'Clínica BC', 'Clínica CD'])
        else:
            c = random.choice(['Clínica DE', 'Clínica EF'])
        
        t.append(m[0]) # NIF do médico

        t.append(c) # Nome da clínica

        t.append(i + 1) # Dia da semana

        trabalhar.append(tuple(t))
        


# Pacientes - SSN, NIF, Nome, Telefone, Morada, Data de nascimento
SSN = 10000000000
for e in enfermeiros:
    p = []
    p.append(str(SSN))
    SSN += 1

    p.append(e[0]) # NIF

    p.append(e[1]) # Nome

    p.append(e[2]) # Telefone

    p.append(e[3]) # Morada

    delta = end_date - start_date
    random_days = random.randint(0, delta.days)
    birth_date = start_date + timedelta(days=random_days)
    p.append(birth_date.strftime('%Y-%m-%d'))

    pacientes.append(tuple(p))

# Pacientes - SSN, NIF, Nome, Telefone, Morada, Data de nascimento
for m in medicos:
    p = []
    p.append(str(SSN))
    SSN += 1

    p.append(m[0]) # NIF

    p.append(m[1]) # Nome

    p.append(m[2]) # Telefone

    p.append(m[3]) # Morada

    delta = end_date - start_date
    random_days = random.randint(0, delta.days)
    birth_date = start_date + timedelta(days=random_days)
    p.append(birth_date.strftime('%Y-%m-%d'))

    pacientes.append(tuple(p))

# Pacientes - SSN, NIF, Nome, Telefone, Morada, Data de nascimento
for i in range(NUM_PACIENTES):
    p = []
    p.append(str(SSN))
    SSN += 1

    p.append(str(NIF))
    NIF += 1

    nome = f'{random.choice(primeiro_nome)} {random.choice(segundo_nome)} {random.choice(terceiro_nome)} {random.choice(ultimo_nome)}'
    p.append(nome)

    p.append(str(TELEFONE))
    TELEFONE += 1

    cidade = random.choice(cidades)
    p.append(f'{random.choice(primeiro_nome_rua)} {random.choice(segundo_nome_rua)} {codigos_postais[cidade]} {cidade}')

    delta = end_date - start_date
    random_days = random.randint(0, delta.days)
    birth_date = start_date + timedelta(days=random_days)
    p.append(birth_date.strftime('%Y-%m-%d'))

    pacientes.append(tuple(p))


CODIGO_SNS = 100000000000
# Consultas - SNS, NIF do médico, Nome da clínica, Data, Hora, Código SNS
for p in pacientes:
    c = []

    c.append(p[0]) # SSN

    c.append(random.choice(medicos)[0]) # NIF do médico

    c.append(random.choice(clinicas)[0]) # Nome da clínica

    delta = end_date - start_date
    random_days = random.randint(0, delta.days)
    date = start_date + timedelta(days=random_days)
    c.append(date.strftime('%Y-%m-%d'))

    hour = random.choice(range(8, 14))
    minute = random.choice([0, 30])
    c.append(f'{hour}:{minute}')

    c.append(str(CODIGO_SNS))
    CODIGO_SNS += 1

    consultas.append(tuple(c))

# Consultas - SNS, NIF do médico, Nome da clínica, Data, Hora, Código SNS
for m in medicos:
    i = 2
    while i != 0:
        c = []

        c.append(random.choice(pacientes)[0]) # SSN

        c.append(m[0]) # NIF do médico

        c.append(random.choice(clinicas)[0]) # Nome da clínica

        delta = end_date - start_date
        random_days = random.randint(0, delta.days)
        date = start_date + timedelta(days=random_days)
        c.append(date.strftime('%Y-%m-%d'))

        hour = random.choice(range(8, 14))
        minute = random.choice([0, 30])
        c.append(f'{hour}:{minute}')

        c.append(str(CODIGO_SNS))
        CODIGO_SNS += 1

        consultas.append(tuple(c))

        i -= 1


# Consultas - SNS, NIF do médico, Nome da clínica, Data, Hora, Código SNS
for d in all_dates:
    for c in clinicas:
        for i in range(NUM_CONSULTAS_POR_DIA_POR_CLINICA):
            c = []

            c.append(random.choice(pacientes)[0]) # SSN

            c.append(random.choice(medicos)[0]) # NIF do médico

            c.append(c[0]) # Nome da clínica

            c.append(d)

            hour = random.choice(range(8, 14))
            minute = random.choice([0, 30])
            c.append(f'{hour}:{minute}')

            c.append(str(CODIGO_SNS))
            CODIGO_SNS += 1

            consultas.append(tuple(c))







# Receitas - Código SNS, Medicamento, Quantidade
for c in consultas:
    if NUM_RECEITAS == 0:
        break

    r = []

    r.append(c[5]) # Código SNS

    r.append(random.choice(medicamentos))

    r.append(random.choice(range(1, 3)))

    receitas.append(tuple(r))

    NUM_RECEITAS -= 1


# Observações (com e sem valor) - Código SNS, Observação, Valor
i = 1
while NUM_OBSERVACOES > 0:
    o = []

    o.append(i)

    o.append(random.choice(observacoes_sem_valor))

    # o.append("NULL")

    observacoes.append(tuple(o))

    boolean = random.choice([True, False])

    if boolean:
        o = []

        o.append(i)

        o.append(random.choice(observacoes_com_valor))

        random_float = random.uniform(0.0, 10.0)

        o.append(random_float)

        observacoes.append(tuple(o))

        i += 1
        NUM_OBSERVACOES -= 1





"""
print('última clinica:') 
print(clinicas[NUM_CLINICAS - 1], end='\n\n')
print('último enfermeiro:')
print(enfermeiros[NUM_CLINICAS * NUM_ENFERMEIROS_POR_CLINCA - 1], end='\n\n')
print('último médico:')
print(medicos[NUM_MEDICOS_GERAL + NUM_MEDICOS_ESPECIALIZADOS - 1], end='\n\n')
print('último trabalhar:')
print(trabalhar[(NUM_MEDICOS_GERAL + NUM_MEDICOS_ESPECIALIZADOS) * 2 + (NUM_CLINICAS * 7)- 1], end='\n\n')
print('último paciente:')
print(pacientes[4999], end='\n\n')
print('última consulta:')
print(consultas[NUM_CONSULTAS - 1], end='\n\n')
print('última receita:')
print(receitas[NUM_RECEITAS - 1], end='\n\n')
print('última observação:')
print(observacoes[NUM_OBSERVACOES - 1], end='\n\n')
"""

def write_insert_statements(file, table_name, columns, data):
    if not data:
        return

    file.write(f"INSERT INTO {table_name} ({', '.join(columns)}) VALUES\n")
    for i, row in enumerate(data):
        values = ', '.join(f"'{str(value)}'" for value in row)
        if i == len(data) - 1:
            file.write(f"({values});\n")
        else:
            file.write(f"({values}),\n")

with open('populate.sql', 'w') as f:
    # Insert clinica
    write_insert_statements(f, 'clinica', ['nome', 'telefone', 'morada'], clinicas)
    
    # Insert enfermeiro
    write_insert_statements(f, 'enfermeiro', ['nif', 'nome', 'telefone', 'morada', 'nome_clinica'], enfermeiros)
    
    # Insert medico
    write_insert_statements(f, 'medico', ['nif', 'nome', 'telefone', 'morada', 'especialidade'], medicos)
    
    # Insert trabalha
    write_insert_statements(f, 'trabalha', ['nif', 'nome', 'dia_da_semana'], trabalhar)
    
    # Insert paciente
    write_insert_statements(f, 'paciente', ['ssn', 'nif', 'nome', 'telefone', 'morada', 'data_nasc'], pacientes)
    
    # Insert consulta
    write_insert_statements(f, 'consulta', ['ssn', 'nif', 'nome', 'data', 'hora', 'codigo_sns'], consultas)
    
    # Insert receita
    write_insert_statements(f, 'receita', ['codigo_sns', 'medicamento', 'quantidade'], receitas)
    
    # Insert observacao
    write_insert_statements(f, 'observacao', ['id', 'parametro', 'valor'], observacoes)

print('Done!')