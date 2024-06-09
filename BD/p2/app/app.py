import os

import re
from datetime import datetime

from flask import Flask, jsonify, request

import psycopg
from psycopg.rows import namedtuple_row, dict_row
from psycopg_pool import ConnectionPool

date_pattern = re.compile(r'^\d{4}-\d{2}-\d{2}$')
time_pattern = re.compile(r'^\d{2}:\d{2}$')


# DATABASE_URL environment variable if it exists, otherwise use this.
# Format postgres://username:password@hostname/database_name.
DATABASE_URL = os.environ.get("DATABASE_URL", "postgres://postgres:postgres@postgres/saude")

pool = ConnectionPool( # The pool config was taken from lab 10
    conninfo=DATABASE_URL,
    kwargs={
        "autocommit": True,  # To avoid starting transactions automatically (transactions will be started explicitly)
        "row_factory": dict_row,
    },
    min_size=4,
    max_size=10,
    open=True,
    name="postgres_pool",
    timeout=5,
)

app = Flask(__name__)

@app.route(
    "/",
    methods= (
        'GET',
    )
)
def list_clinics_endpoint():
    with pool.connection() as connection:
        with connection.cursor() as cursor:
            try:
                clinics = cursor.execute(
                    '''
                    SELECT nome, morada 
                    FROM clinica;
                    '''
                ).fetchall()

            except Exception as e: # Connection Timeout, etc...
                return  jsonify({'message' : "Ocorreu um erro interno. Tente novamente mais tarde.", 'status' : 'error'}), 500 # Error message with status code 500 (Internal Server Error)
            else:
                return jsonify(clinics), 200

@app.route(
    "/c/<clinic>/",
    methods= (
        'GET',
    )
)
def list_clinic_specialties_endpoint(clinic):
    with pool.connection() as connection:
        with connection.cursor() as cursor:
            try:
                # Check if the clinic exists
                cursor.execute(
                    '''
                    SELECT nome
                    FROM clinica
                    WHERE nome = %(clinic)s;
                    ''',
                    {"clinic": clinic}
                )
                clinic_exists = cursor.fetchone()

                if not clinic_exists:
                    return jsonify({'message': "A clinica especificada não existe.", 'status': 'error'}), 404

                # Get specialties of the clinic
                cursor.execute(
                    '''
                    SELECT DISTINCT m.especialidade
                    FROM medico m
                    JOIN trabalha t ON m.nif = t.nif
                    WHERE t.nome = %(clinic)s;
                    ''',
                    {"clinic": clinic}
                )

                specialties = cursor.fetchall()
    
            except Exception as e: # DB Connection Timeout, etc...
                return jsonify({'message' : "Ocorreu um erro interno. Tente novamente mais tarde.", 'status' : 'error'}), 500 # Error message with status code 500 (Internal Server Error)
            else:
                return jsonify(specialties), 200

@app.route(
    "/c/<clinic>/<specialty>/",
    methods= (
        'GET',
    )
)
def list_doctors_and_free_hours_endpoint(clinic, specialty):    
    with pool.connection() as connection:
        with connection.cursor() as cursor:
            try:
                # Check if the clinic exists
                cursor.execute(
                    '''
                    SELECT nome
                    FROM clinica
                    WHERE nome = %(clinic)s;
                    ''',
                    {"clinic": clinic}
                )

                if cursor.rowcount == 0:
                    return jsonify({'message': "A clinica especificada não existe.", 'status': 'error'}), 404
                
                # Verify if the specialty exists
                cursor.execute(
                    '''
                    SELECT
                        m.nome,
                        m.nif
                    FROM
                        medico m
                    JOIN
                        trabalha t ON m.nif = t.nif
                    WHERE
                        t.nome = %(clinic)s AND m.especialidade = %(specialty)s
                    ORDER BY
                        m.nome;
                    ''',
                    {"clinic": clinic, 'specialty': specialty}
                )

                specified_doctors = cursor.fetchall()

                if cursor.rowcount == 0:
                    return jsonify({'message': "A especialidade especificada não existe na clinica especificada.", 'status': 'error'}), 404

                free_slots = []
                print(specified_doctors)
                for doctor in specified_doctors:
                    cursor.execute(
                        '''
                        SELECT
                            h.data_e_hora as horario
                        FROM 
                            medico m
                            JOIN trabalha t ON m.nif = t.nif
                            JOIN horario h ON EXTRACT(DOW FROM h.data_e_hora) = t.dia_da_semana
                            LEFT JOIN consulta c ON m.nif = c.nif AND h.data_e_hora = (c.data || ' ' || c.hora)::TIMESTAMP
                        WHERE
                            m.nif = %(nif)s
                            AND c.id IS NULL
                            AND h.data_e_hora > CURRENT_TIMESTAMP
                        ORDER BY
                            h.data_e_hora
                        LIMIT 3;
                        ''',
                        {'nif': doctor['nif']}
                    )

                    # For improved presentation of each doctor's schedule: {name: 'doctor', 'schedules': [date1, date2, date3]}
                    free_slots.append({'nome': doctor['nome'], 'horarios': [doctor_dictionary['horario'] for doctor_dictionary in cursor.fetchall()]})

            except Exception as e: # DB Connection Timeout, etc..
                return  jsonify({'message' : "Ocorreu um erro interno. Tente novamente mais tarde.", 'status' : 'error'}), 500 # Error message with status code 500 (Internal Server Error)
            else:
                return jsonify(free_slots), 200

@app.route(
    "/a/<clinic>/registar/",
    methods=(
        "POST",
    ),
)
def book_appointment(clinic):
    patient_ssn = request.args.get('paciente')
    doctor_nif = request.args.get('médico')
    date = request.args.get('data')
    time = request.args.get('hora')

    error = None
    if not patient_ssn:
        error = "A especificação do SSN do paciente é obrigatória."
    elif not patient_ssn.isdigit():
        error = "O SSN do paciente deve ser constituido apenas por algarismos de 0 a 9."
    elif len(patient_ssn) != 11:
        error = "O SSN do paciente deve ter exatamente 11 algarismos." 
    elif not doctor_nif:
        error = "A especificação do NIF do médico é obrigatória."
    elif not doctor_nif.isdigit():
        error = "O NIF do médico deve ser constituido apenas por algarismos de 0 a 9." 
    elif len(doctor_nif) != 9:
        error = "O NIF do médico deve ter exatamente 9 algarismos."  
    elif not date:
        error = "A especificação da data é obrigatória."
    elif not date_pattern.match(date):
        error = "A data deve estar no formato AAAA-MM-DD."
    elif not time:
        error = "A especificação da hora é obrigatória."
    elif not time_pattern.match(time):
        error = "A hora deve estar no formato HH:MM."    
    elif datetime.strptime(date + " " + time, "%Y-%m-%d %H:%M") <= datetime.now():
        error = "A data da marcação não pode ser no passado."
    else:
        hour, minute = map(int, time.split(':'))
        if not ((8 <= hour <= 12) or (14 <= hour <= 18)):
            error = "A hora deve ser entre as 9:00 às 12:30 ou entre as 14:00 às 18:30."
        elif minute != 0 and minute != 30:
            error = "A hora da consulta deve ser exata ou ser à meia hora."

    if error is not None:
        return jsonify({'message' : error, 'status' : "error"}), 400 # Return error message with status code 400 (Bad Request)
    else:
        with pool.connection() as connection:
            with connection.cursor() as cursor:
                try:
                    with connection.transaction(): # The BEGIN statement is executed automatically, and a COMMIT will be executed after the end of this "with" block if no exception occurs.
                        # Check if the clinic exists
                        cursor.execute(
                            '''
                            SELECT *
                            FROM clinica
                            WHERE nome = %(clinic)s;
                            ''',
                            {"clinic": clinic}
                        )

                        if cursor.rowcount == 0:
                            return jsonify({'message': "A clinica especificada não existe.", 'status': 'error'}), 404


                        # Check if the patient exists
                        cursor.execute(
                            '''
                            SELECT *
                            FROM paciente
                            WHERE ssn = %(patient)s;
                            ''',
                            {"patient": patient_ssn}
                        )

                        if cursor.rowcount == 0:
                            return jsonify({'message': "O paciente especificado não está registado.", 'status': 'error'}), 404
                        
                        # Check if the doctor exists
                        cursor.execute(
                            '''
                            SELECT *
                            FROM medico
                            WHERE nif = %(doctor)s;
                            ''',
                            {"doctor": doctor_nif}
                        )

                        if cursor.rowcount == 0:
                            return jsonify({'message': "O médico especificado não está registado.", 'status': 'error'}), 404
                        
                        cursor.execute(
                            '''
                            INSERT INTO consulta (ssn, nif, nome, data, hora)
                            VALUES (%(patient_ssn)s, %(doctor_nif)s, %(clinic)s, %(date)s, %(time)s);
                            ''',
                            {'patient_ssn': patient_ssn, 'date': date, 'time': time, 'clinic': clinic, 'doctor_nif': doctor_nif}
                        )

                except psycopg.IntegrityError as e:
                    error_code = e.sqlstate               
                    if error_code == psycopg.errors.UniqueViolation or error_code == '23505':
                        if "(nif, data, hora)" in e.diag.message_detail:
                            message, code = "Não foi possível agendar a consulta. O médico já se encontra ocupado nesse horário.", 409  # Error message with status code 409 (Conflict)
                        elif "(ssn, data, hora)" in e.diag.message_detail:
                            message, code = "Não foi possível agendar a consulta. O paciente possui outra consulta nesse horário.", 409  # Error message with status code 409 (Conflict)
                        return (jsonify({'message': message, 'status': 'success'}) if code == 200 else jsonify({'message': message, 'status': 'error'})), code
                    else:
                        message, code = "Ocorreu um erro interno. Tente novamente mais tarde.", 500 # Error message with status code 500 (Internal Server Error)
                        return (jsonify({'message': message, 'status': 'success'}) if code == 200 else jsonify({'message': message, 'status': 'error'})), code

                except psycopg.DatabaseError as e:
                    error_code = e.sqlstate
                    if error_code == psycopg.errors.TriggeredActionException or error_code == 'P0001':
                        if "medico_trabalha_clinica_check()" in e.diag.context:
                            message, code = "Não foi possível agendar esta consulta. O médico não trabalha na clínica especificada no horário especificado.", 409  # Error message with status code 409 (Conflict)
                        elif "autoconsulta_check()" in e.diag.context:
                            message, code = "Não foi possível agendar esta consulta. O médico não se pode consultar a si mesmo.", 409  # Error message with status code 409 (Conflict)
                        
                        print(error_code)
                        return (jsonify({'message': message, 'status': 'success'}) if code == 200 else jsonify({'message': message, 'status': 'error'})), code
                    

                except Exception as e:
                    message, code = "Ocorreu um erro interno. Tente novamente mais tarde.", 500 # Error message with status code 500 (Internal Server Error)
                    return (jsonify({'message': message, 'status': 'success'}) if code == 200 else jsonify({'message': message, 'status': 'error'})), code

                else:
                    if cursor.rowcount == 0: 
                        message, code = "Não foi possível marcar esta consulta.", 404 # Error message with status code 404 (Not Found)
                    else:
                        message, code = "A consulta foi marcada com sucesso.", 200 # Sucess message with status code 200 (Sucess)
                    return (jsonify({'message': message, 'status': 'success'}) if code == 200 else jsonify({'message': message, 'status': 'error'})), code
                    
@app.route(
    "/a/<clinic>/cancelar/",
    methods=(
        "POST",
    ),
)
def remove_appointment(clinic):
    patient_ssn = request.args.get('paciente')
    doctor_nif = request.args.get('médico')
    date = request.args.get('data')
    time = request.args.get('hora')

    error = None
    if not patient_ssn:
        error = "A especificação do SSN do paciente é obrigatória."
    elif not patient_ssn.isdigit():
        error = "O SSN do paciente deve ser constituido apenas por algarismos de 0 a 9."
    elif len(patient_ssn) != 11:
        error = "O SSN do paciente deve ter exatamente 11 algarismos." 
    elif not doctor_nif:
        error = "A especificação do NIF do médico é obrigatória."
    elif not doctor_nif.isdigit():
        error = "O NIF do médico deve ser constituido apenas por algarismos de 0 a 9." 
    elif len(doctor_nif) != 9:
        error = "O NIF do médico deve ter exatamente 9 algarismos."  
    elif not date:
        error = "A especificação da data é obrigatória."
    elif not date_pattern.match(date):
        error = "A data deve estar no formato AAAA-MM-DD."
    elif not time:
        error = "A especificação da hora é obrigatória."
    elif not time_pattern.match(time):
        error = "A hora deve estar no formato HH:MM."
    elif datetime.strptime(date + " " + time, "%Y-%m-%d %H:%M") <= datetime.now():
        error = "A consulta a desmarcar deve ser futura."
    else:
        hour, minute = map(int, time.split(':'))
        if not ((8 <= hour <= 12) or (14 <= hour <= 18)):
            error = "A hora deve ser entre as 9:00 às 12:30 ou entre as 14:00 às 18:30."
        elif minute != 0 and minute != 30:
            error = "A hora da consulta deve ser exata ou ser à meia hora."
    
    if error is not None:
        return jsonify({'message' : error, 'status' : "error"}), 400 # Return error message with status code 400 (Bad Request)
    else:
        with pool.connection() as connection:
            with connection.cursor() as cursor:
                try:
                    with connection.transaction(): # BEGIN is executed
                        # Check if the clinic exists
                        cursor.execute(
                            '''
                            SELECT *
                            FROM clinica
                            WHERE nome = %(clinic)s;
                            ''',
                            {"clinic": clinic}
                        )

                        if cursor.rowcount == 0:
                            return jsonify({'message': "A clinica especificada não existe.", 'status': 'error'}), 404

                        # Check if the patient exists
                        cursor.execute(
                            '''
                            SELECT *
                            FROM paciente
                            WHERE ssn = %(patient)s;
                            ''',
                            {"patient": patient_ssn}
                        )

                        if cursor.rowcount == 0:
                            return jsonify({'message': "O paciente especificado não está registado.", 'status': 'error'}), 404
                        
                        # Check if the doctor exists
                        cursor.execute(
                            '''
                            SELECT *
                            FROM medico
                            WHERE nif = %(doctor)s;
                            ''',
                            {"doctor": doctor_nif}
                        )

                        if cursor.rowcount == 0:
                            return jsonify({'message': "O médico especificado não está registado.", 'status': 'error'}), 404

                        cursor.execute(
                            '''
                            DELETE FROM consulta
                            WHERE ssn = %(patient_ssn)s
                            AND data = %(date)s
                            AND hora = %(time)s
                            AND nome = %(clinic)s
                            AND nif = %(doctor_nif)s
                            ''',
                            {'patient_ssn': patient_ssn, 'date': date, 'time': time, 'clinic': clinic, 'doctor_nif': doctor_nif}
                        )
                        
                except Exception as e:
                    message, code = "Ocorreu um erro interno. Tente novamente mais tarde.", 500 # Error message with status code 500 (Internal Server Error)
                    return (jsonify({'message': message, 'status': 'success'}) if code == 200 else jsonify({'message': message, 'status': 'error'})), code
                else:
                    if cursor.rowcount == 0:
                        message, code = "A consulta a cancelar não foi encontrada.", 404 # Error message with status code 404 (Not Found)
                    else:
                        message, code = "A consulta foi cancelada com sucesso.", 200  # Sucess message with status code 200 (Sucess)
                    return (jsonify({'message': message, 'status': 'success'}) if code == 200 else jsonify({'message': message, 'status': 'error'})), code

if __name__ == '__main__':
    app.run()