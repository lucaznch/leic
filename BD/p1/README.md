# First Delivery of Databases (2023/24)

## Entity-Association Modeling:

### Problem Domain:

A healthcare company has several clinics, each identified by its name and also having a unique address and telephone number.

The company has several healthcare professionals on contract, including doctors and nurses. Each healthcare professional is identified by their NIF, and is further characterized by: name, address, telephone number and IBAN. Only doctors also have a specialty chosen from a list of specialties recognized by the medical association (e.g. general practice, orthopedics). Nurses are allocated to a single clinic, while doctors can work in several clinics throughout the week, but each day of the week they are allocated to one and only one clinic, on a fixed basis.

The company provides health services to patients, who are identified by their SNS number, and characterized by name, NIF, date of birth, address and telephone number. Patients may have one or more health systems (e.g. ADSE, AdvanceCare), identified by the system name and the patient's national health system number.

Health services are divided into: nursing services, carried out by nurses; and consultations, carried out by doctors. Nursing services do not require prior booking, but consultations require booking through the company's information system.

Appointment scheduling consists of allocating a specific patient to a specific doctor, in a specific clinic, and in a specific period of 30 minutes between 8:00 and 20:00 on weekdays (e.g. 8:30, 17:00) . The appointment must obviously comply with common sense spatio-temporal restrictions (e.g., in each time period, either a patient or a doctor can only have one appointment).

At each consultation, any number of symptoms described by the patient and/or observed by the doctor are recorded, some of which are qualitative (i.e., consisting only of the name of the symptom, chosen from a list coming from the SNOMED CT controlled vocabulary) while others are quantitative (i.e., consisting of a value for an observed parameter, such as blood pressure or pulse rate). If the doctor prescribes medications as a result of the consultation, a medical prescription is also associated with the consultation record, identified by an SNS code, and which contains quantities of one or more medications chosen from the official Infarmed list. Each recipe is associated with a single query.

### To be evaluated:

- [ ] Design an Entity-Association model that represents the information requirements of an application with the above domain;
- [ ] Identify inconsistent situations in the problem domain, but which are permitted in the Entity-Association model presented, and define a set of integrity relationships that complete the proposed model in order to restrict invalid situations.

### Grades:

- The Entity-Association model must be expressed in the notation taught in classes, and drawn in a suitable modeling tool (for example [draw.io](http://draw.io));
- Integrity Restrictions to the Entity-Association model must be written as statements (obligatory or prohibited) expressed in terms of concepts in the entity-association model, that is, in terms of attributes, entities and relationships between them;
- The solution can be presented in Portuguese or English;
- The simplicity and coherence of the model will be evaluated.

## E-A Conversion -&gt; Relational:

### E-A model to represent:

![](https://holocron.so/uploads/691627ee-screenshot-from-2024-04-19-21-16-47.png)

### To be evaluated:

- [ ] Develop a Relational Model corresponding to the Entity-Association model presented above, using the notation exposed in the theoretical classes (briefly demonstrated below);
- [ ] Identify all Integrity Restrictions of the Entity-Association model presented that are not subject to conversion to the Relational Model;
- [ ] Complement the Relational Model with a set of Integrity Constraints that represent the E-A model presented with greater fidelity, including its Integrity Constraints.

### Understanding the notation to use:

```
relationship_name (attribute1, …, attributeN)
- attributeX, … , attributeZ: FK (destino_relation: attributeA,..., attributeC)
- unique(attributoA, … ,attributoC)
- other restriction
```

In this example:

- Relationships are expressed by a name followed by attributes in parentheses;
- The primary key is expressed by underlining the attributes that compose it;
- Foreign keys for other relationships are expressed through the list of attributes that compose it, followed by “: FK(DestinationRelation)”. The order of the attributes that make up the FK must correspond to the order of the key attributes in the target relationship;
- Candidate keys are expressed using the expression “unique”, followed by the list of attributes that compose it in parentheses;
- The results integrity constraints to the Relational Model are described in terms of the concepts of this model;
- In the conversion presentation logic, each relation is presented after the conversions of the relations on which it depends (in terms of integrity constraints) have been presented. Thus, for example, the conversion of an 'association' between entities can only be presented after the conversion of the corresponding entities.

## Relational Algebra and SQL:

### Relational Schema to take into account:

```
clinic (name, address)
  unique(address)

doctor (TIN, name, specialty)

patient (SSN, NIF, name, address)
  unique(NIF)

query (period, name, NIF, SSN)
  name: FK(clinic:name)
  NIF: FK (doctor: NIF)
  SSN : FK(patient:SSN)
```

### To be evaluated:

Before solving the following questions, a direct conversion of the schema to SQL must be assumed, in which the **period is of type TIMESTAMP** and the **remaining attributes are of type VARCHAR**.

The questions are as follows:

1. What is the most concise relational algebra expression for the question “which patients consulted doctors of all specialties”?

2. Indicate the question in natural language that corresponds to the following relational algebra expression:

 ![](https://holocron.so/uploads/bb0d7495-screenshot-from-2024-04-19-21-29-53.png)


3. Indicate the question in natural language that corresponds to the following SQL expression:

 ```plaintext
 SELECT p.SSN, p.name
 FROM patient p JOIN consultation c ON p.SSN = c.SSN
 GROUP BY p.SSN, p.name, DATE(c.period)
 HAVING COUNT(*) > 1;
 ```

4. The aim is to determine which doctor has more loyal patients (i.e., patients who consult other doctors in the same specialty less frequently in proportion to the number of times they consult the doctor). Comment on the following SQL expression presented by ChatGPT for this question indicating whether the expression is semantically correct (in relation to the intended question) and if not, which aspects of the intended question are not adequately covered by the expression:

 ```
 SELECT m.name AS doctor_name,
 COUNT(c.period) THE medical_consultations,
 COUNT(DISTINCT p.NIF) AS total_patients,
 COUNT(c.periodo) / COUNT(DISTINCT p.NIF) AS
 proportion_fidelity
 FROM doctor m
 INNER JOIN query c ON m.NIF = c.NIF
 INNER JOIN patient p ON c.SSN = p.SSN
 WHERE c.period IN (
 SELECT MIN(period)
 FROM query
 WHERE NIF = m.NIF
 GROUP BY SSN
 )
 GROUP BY m.name
 ORDER BY proportion_fidelity DESC;
 ```

 ## Report Delivery:

 ### General information:

 The evaluation of the project will be **carried out based on the report presented by the students** containing the answers to the items requested above.

 The quotes are as follows:

 ![](https://holocron.so/uploads/d4443718-screenshot-from-2024-04-19-21-48-31.png)

 ### Delivery Rules:

 The report must have a cover page which contains:
 - [ ] the **title “BD Project - Part 1”**;
 - [ ] the **name** and **number** of the students;
 - [ ] the **relative percentage of contribution from each student** as well as the **number of hours dedicated by each one** to the project;
 - [ ] the **group number**;
 - [ ] the **shift to which the group belongs**;
 - [ ] the **name of the Laboratory Teacher** of the group.

 The report must be submitted in the Fénix system with:
 - [ ] type name "**delivery-bd-01-GG.pdf**" where GG is the group number.
