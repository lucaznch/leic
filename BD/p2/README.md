# Databases - 2023/2024- Project - Delivery 2
## 0. Database Loading
Create the “Saude” database in PostgreSQL and execute the commands to create the tables in this database presented in Appendix A

## 1. Integrity Constraints
Implement the following integrity restrictions in the “Saude” database, and may resort to procedural extensions (Triggers and Stored Procedures) if strictly necessary:
- RI-1 Appointment times are exact or half-hourly between 8am and 2pm and 2pm
- RI-2 A doctor cannot consult himself, although he can be a patient of other doctors in the system
- RI-3 A doctor can only give consultations at the clinic where he works on the day of the week corresponding to the date of the consultation

<br>

The ON DELETE CASCADE and ON UPDATE CASCADE mechanisms are not allowed.

## 2. Completing the Database
Populate all database tables consistently (after executing the previous point) with the following additional coverage requirements:

- 5 clinics, from at least 3 different locations in the Lisbon district
- 5-6 nurses per clinic
- 20 doctors specializing in ‘general practice’ and 40 others distributed as you see fit across up to 5 other medical specialties (including at least ‘orthopedics’ and ‘cardiology’). Each doctor must work in at least two clinics, and in each clinic every day of the week (including weekends), there must be at least 8 doctors
- Around 5,000 patients
- A minimum number of consultations in 2023 and 2024 such that each patient has at least one consultation, and on each day there are at least 20 consultations per clinic, and at least 2 consultations per doctor
- ~80% of consultations have an associated medical prescription, and prescriptions contain 1 to 6 medications in quantities between 1 and 3
- All queries have 1 to 5 symptom observations (with parameter but no value) and 0 to 3 metric observations (with parameter and value). There must be ~50 different parameters for the symptoms (no value) and ~20 different parameters for the metric observations (with value) and the two sets must be disjoint.
- All addresses are national and follow the Portuguese format, ending with postal code: XXXX-XXX and then the location.

<br>

You must also ensure that all consultations necessary to carry out the following points of the project produce a **non-empty result**.

<br>

You can fill the database using a “populate.sql” file with INSERT commands, running in your database, or alternatively through tabular text files (one per table) using COPY commands in the database to populate the tables.
In either case, all commands and files used must be included in the project delivery. You can use generative AI tools (e.g. chatGPT) to generate INSERT commands or tabular text files, as long as the result meets the requirements presented.

## 3. Application Development
Create a RESTful web service prototype for managing queries through programmatic access to the ‘Saude’ database through an API that returns responses in JSON, implementing the following REST endpoints:

| Endpoint | Description |
|:---:|:---|
| **/** | List all clinics (name and address) |
| **/c/\<clinic\>/** | List all specialties offered at <clinic> |
| **/c/\<clinic\>/\<specialty\>/** | List all doctors (name) of the <specialty> who work at the <clinic> and the first three appointment times available for each of them (date and time) |
| **/a/\<clinica\>/cancel/** | Registers an appointment at <clinic> in the database (populating the respective table). Receives as arguments a patient, a doctor, and a date and time (after the scheduling moment) |
| **/a/\<clinica\>/cancel/** | Cancels an appointment that has not yet taken place at <clinica> (its appointment is after the time of cancellation), removing the entry from the respective table in the database. Receives as arguments a patient, a doctor, and a date and time |

> [!NOTE]
> A time is available for consultation if it is after the time of questioning, meets the time restrictions of the consultation table (RI-1 in point 1), and the doctor does not yet have an entry in the consultation table at that time


The solution must value security, preventing SQL injection attacks, and must guarantee the atomicity of operations on the database using transactions.

To make scheduling appointments easier, you can build an additional table with all the times for 2024 that comply with the time restrictions in the appointment table. If you do so, you must include the respective code along with the application code.

The appointment scheduling and cancellation endpoints must return explicit messages either confirming that data was inserted/removed or indicating why the operation could not be performed.

All application code must be includedor in project delivery. The application must also be available online, in the docker development environment provided for the course, for demonstration during the discussion.


## 4. Views
Create a materialized view that details the most important information about patient visits by combining information from several database tables. The view must have the following layout:

```sql
patient_history(id, ssn, nif, name, date, year, month, day_of_month, location, specialty, type, key, value)
```

on what:
- id, ssn, nif, name and date: correspond to the homonymous attributes of the query table
- year, month and day_of_month: are derived from the date attribute of the consulta2 table
- location: is derived from the address attribute of the clinical table
- specialty: corresponds to the homonymous attribute of the doctor table
- type: takes the values ​​‘observation’ or ‘recipe’ depending on the completion of the following fields
- key: corresponds to the parameter attribute from the observation table or the medicine attribute from the recipe table
- value: corresponds to the value attribute from the observation table or the quantity attribute from the recipe table

## 5. Data Analysis (SQL and OLAP)
Using the view developed in the previous point, complemented with other tables from the ‘Saude’ database when necessary, present the most succinct SQL query for each of the following analytical objectives. You can use the ROLLUP, CUBE, GROUPING SETS statements or the UNION of GROUP BY clauses for whatever purposes you see fit.


1. Determine which patient(s) have made the least progress in treating their orthopedic diseases in order to receive a free consultation. The lack of progress indicator is considered to be the maximum time interval between two observations of the same symptom (i.e. ‘observation’ type records with the same key and NULL value) in orthopedic consultations.
2. Determine which medications are being used to treat chronic cardiological diseases. Any medications prescribed to the same patient (whoever they may be) at least once a month during the last twelve months, in cardiology consultations, are considered to qualify.
3. Explore the total prescribed quantities of each medicine in 2023, globally, and with drill-down in the dimensions space (location > clinic), time (month > day_of_month), and doctor (specialty > [doctor's] name), separately.
4. Determine whether there is bias in the measurement of any parameters between clinics, medical specialties or doctors, for which it is necessary to list the average value and standard deviation of all metric observation parameters (i.e. with a non-NULL value) with drill down in the doctor dimension (globally > specialty > [doctor's] name) and additional drill-down (over the previous one) by clinic.

> [!NOTE]
> You can use the EXTRACT() function to obtain parts of dates or timestamps and you can use the SUBSTRING() function specifying a POSIX standard to extract the city name after the postal code of the address.

## 6. Indexes
Present the SQL instructions for creating indexes to improve the times of each of the queries listed below on the ‘Saude’ database. Justify your choice of table(s), attribute(s) and index type(s), explaining which operations would be optimized and how. Assume that there are no indexes on the tables other than those implied by declaring primary and foreign keys, and for the purposes of this exercise, assume that the size of the tables exceeds the available memory by several orders of magnitude.

### 6.1
```sql
SELECT name
FROM patient
JOIN query USING (ssn)
JOIN observation USING (id)
WHERE parameter = ‘diastolic pressure’
AND value >= 9;
```

## 6.2
```sql
SELECT specialty, SUM(quantity) AS qty
FROM doctor
JOIN query USING (nif)
JOIN recipe USING (codigo_ssn)WHERE data BETWEEN ‘2023-01-01’ AND ‘2023-12-31’
GROUP BY specialty
ORDER BY qty;
```

<br>
<br>
<br>
<br>
<br>
<br>


#### Appendix A - SQL Schema
```sql
DROP TABLE IF EXISTS CASCADE clinic;
DROP TABLE IF EXISTS nurse CASCADE;
DROP TABLE IF EXISTS doctor CASCADE;
DROP TABLE IF EXISTS works CASCADE;
DROP TABLE IF EXISTS patient CASCADE;
DROP TABLE IF EXISTS recipe CASCADE;
DROP TABLE IF EXISTS query CASCADE;
DROP TABLE IF EXISTS CASCADE observation;

CREATE TABLE clinic(
name VARCHAR(80) PRIMARY KEY,
phone VARCHAR(15) UNIQUE NOT NULL CHECK (phone ~ '^[0-9]+$'),
address VARCHAR(255) UNIQUE NOT NULL);

CREATE TABLE nurse(
nif CHAR(9) PRIMARY KEY CHECK (nif ~ '^[0-9]+$'),
name VARCHAR(80) UNIQUE NOT NULL,
phone VARCHAR(15) NOT NULL CHECK (phone ~ '^[0-9]+$'),
address VARCHAR(255) NOT NULL,
clinic_name VARCHAR(80) NOT NULL REFERENCES clinic (name));

CREATE TABLE doctor(
nif CHAR(9) PRIMARY KEY CHECK (nif ~ '^[0-9]+$'),
name VARCHAR(80) UNIQUE NOT NULL,
phone VARCHAR(15) NOT NULL CHECK (phone ~ '^[0-9]+$'),
address VARCHAR(255) NOT NULL,
specialty VARCHAR(80) NOT NULL);

CREATE TABLE works(
nif CHAR(9) NOT NULL REFERENCES doctor,
name VARCHAR(80) NOT NULL REFERENCES clinic,
day_of_week SMALLINT,
PRIMARY KEY (nif, day_of_week));

CREATE TABLE patient(
ssn CHAR(11) PRIMARY KEY CHECK (ssn ~ '^[0-9]+$'),
nif CHAR(9) UNIQUE NOT NULL CHECK (nif ~ '^[0-9]+$'),
name VARCHAR(80) NOT NULL,
phone VARCHAR(15) NOT NULL CHECK (phone ~ '^[0-9]+$'),
address VARCHAR(255) NOT NULL,
data_nasc DATE NOT NULL);

CREATE TABLE query(
id SERIAL PRIMARY KEY,
ssn CHAR(11) NOT NULL REFERENCES patient,
nif CHAR(9) NOT NULL REFERENCES doctor,
name VARCHAR(80) NOT NULL REFERENCES clinic,
date DATE NOT NULL,
time TIME NOT NULL,
code_sns CHAR(12) UNIQUE CHECK (code_sns ~ '^[0-9]+$'),
UNIQUE(ssn, date, time),
UNIQUE(nif, date, time));

CREATE TABLE recipe(
code_sns VARCHAR(12) NOT NULL REFERENCES query (codigo_sns),
medicine VARCHAR(155) NOT NULL,
quantity SMALLINT NOT NULL CHECK (quantity > 0),
PRIMARY KEY (code_sns, medicine));

CREATE TABLE note(
id INTEGER NOT NULL REFERENCES query,
parameter VARCHAR(155) NOT NULL,
FLOAT value,
PRIMARY KEY (id, parameter)
);
```
