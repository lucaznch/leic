# Databases summary
This summary was made as a way to study for my exam, and definitely not to use it to copy during the exam!<br>
**Contents**:
1. [Databases and Database Management Systems](#databases-and-database-management-systems-dbms)
    * [Database](#database)
    * [Database Management System](#database-management-system-dbms)
    * [Architecture](#ansisparc-dbms-architecture)
    * [Data Model](#data-models)
    * [Database Design](#database-design)
2. [SQL](#structured-query-language-sql)
    * [Basic](#sql-in-practice-basic)
    * [Advanced](#sql-in-practice-advanced)


## Databases and Database Management Systems (DBMS)
### Database
A **database** is an organized collection of data, structured to facilitate the storage, retrieval, modification, and deletion of data, that is often organized into **tables**, rows, and columns.

### Database Management System (DBMS)
A **Database Management System** is a software system that provides an interface for users to interact with databases. It manages data, the database engine, and the database schema, facilitating the organization, storage, and retrieval of data.<br>

**Advantages** of DBMS:
- **Data Security**: Provides mechanisms for data access control, user authentication, and authorization.
- **Transaction Management**: Ensures data integrity and consistency through ACID properties (Atomicity, Consistency, Isolation, Durability).
- **Backup and Recovery**: Offers tools for backing up data and recovering it in case of system failures.
- **Concurrency Control**: Manages concurrent data access by multiple users to ensure data consistency and integrity.
- **Data Independence**: There is an abstraction layer between the representation of data when it is stored and what applications have access to.
- **Efficient Data Access**: At the storage level, DBMSs use several sophisticated techniques to make data storage and collection an efficient process. 
- **Data Integrity**: Constraints and rules, ensuring data accuracy and consistency. Unlike other forms of data storage, such as files.
- **Persistent Storage**: When we use a DBMS we are sure that, when we finish executing an instruction (e.g. insert, update, delete, etc.), that instruction was executed and saved persistently. If there is a failure immediately after executing any instruction, we are guaranteed that we will not lose this data.
- **Data Administration**: Allows separation of functions: whoever develops the application does not need to be the same person who manages the database. Thus, you can have a much more experienced professional just focused on the DBMS, improving data storage and collection.
- **Reducing Application Development Time**: With all the tools made available by a DBMS, as well as their own interrogation languages ​​dedicated to data (e.g. SQL), the work of programmers is immensely easier. These languages ​​provide a high-level interface for data, which, as we have already seen, allow the creation of various abstractions that simplify application development.

**Disadvantages** of DBMS:
- **Complex Applications**: Sometimes, we can have very complex applications in which it is not possible for us to model our data in a relational system.
- **Unacceptable Performance (e.g. for real-time applications)**: For some applications, such as a stock market, data changes so quickly that the performance of the DBMS is not sufficient.
- **Lack of Flexible Analysis for Certain Data Types**: Mostly for text, where we may want to do advanced searches (e.g. full text search, fuzzy search), something that most DBMSs do not support.
- **Lack of necessity**: For extremely simple applications or with mostly immutable data, it may not make sense to introduce a DBMS, no matter how small the overhead of doing so may be nowadays. An application of this type would not benefit from a DBMS.

### ANSI/SPARC DBMS Architecture
Defines a three-level architecture for database systems, designed to separate the user's view of the database from the physical storage of data, enhancing data abstraction, independence, and flexibility.<br>

![ansi-sparc-architecture](media/ansi-sparc.png)

1. **External Level** - describes how users see data:
    - **User Views**: The external level represents the user's view of the database. It includes multiple external schemas or user views, which define how individual users or groups of users see the data.
    - **Customization**: Each user can have a customized view that includes only the data relevant to their specific needs, which can enhance security by restricting access to sensitive data.
    - **Data Independence**: Changes at the external level do not affect the conceptual or internal levels, providing logical data independence.

2. **Conceptual Level** - projects into the logical structure:
    - **Community View**: The conceptual level represents the community view of the database. It provides a unified and abstracted view of the entire database, independent of how data is stored physically.
    - **Conceptual Schema**: This level includes the conceptual schema, which defines the logical structure of the entire database, including entities, attributes, relationships, constraints, and security.
    - **Data Integrity**: Ensures the overall consistency, integrity, and correctness of the data.
    - **Data Independence**: Changes at the conceptual level do not affect the external or internal levels, providing physical data independence.

3. **Internal Level** - describes files (tables and indexes):
    - **Physical Storage**: The internal level represents the physical storage of the database. It includes the internal schema, which defines how the data is stored on the storage medium (e.g., hard drives, SSDs).
    - **Data Structures**: Details the physical data structures, such as indexes, file organizations, and storage methods.
    - **Optimization**: Handles data access paths, storage allocation, and optimization techniques to ensure efficient data retrieval and storage.
    - **Data Independence**: Changes at the internal level do not affect the conceptual or external levels.

#### Advantages of ANSI/SPARC Architecture
1. **Data Abstraction**: Separates the way data is viewed by users from the way it is stored, promoting clear data abstraction.
2. **Data Independence**: Provides both logical and physical data independence, allowing changes at one level without affecting others.
   - **Logical Data Independence**: Ability to change the conceptual schema without altering the external schemas or application programs.
   - **Physical Data Independence**: Ability to change the internal schema without altering the conceptual schema.
3. **Security and Privacy**: Enhances security by allowing different views for different users, ensuring that users only have access to data relevant to them.
4. **Consistency**: Maintains consistency and integrity across different user views and the physical storage of data.

#### Example of ANSI/SPARC Architecture in Action
- **External Level**: A university database might have different views for students, professors, and administrative staff. Students might see their courses and grades, professors might see courses they teach and student performance, and administrative staff might have a broader view including financial data.
- **Conceptual Level**: The unified schema might include entities like `Student`, `Professor`, `Course`, and `Enrollment`, with relationships and constraints defining how these entities interact.
- **Internal Level**: The actual data might be stored in various files, with indexes to speed up queries and data distributed across multiple storage devices for performance and reliability.

### Data Models
Data models define how data is structured, stored, and manipulated within a DBMS, providing a way to represent the logical structure of data, the relationships among data, and the constraints that apply to the data.
1. **Relational Data Model**
    - **Structure**: Data is organized into **tables** (relations) consisting of **rows** (tuples) and **columns** (attributes).
    
    ```sql
    user123=> select * from customer;
    customer_name | customer_street | customer_city  
    --------------+-----------------+----------------
    Adams         | Main Street     | Lisbon
    Brown         | Main Street     | Oporto
    Cook          | Main Street     | Lisbon
    Davis         | Church Street   | Oporto
    Evans         | Forest Street   | Coimbra
    Flores        | Station Street  | Braga
    Gonzalez      | Sunny Street    | Faro
    Iacocca       | Spring Steet    | Coimbra
    Johnson       | Red Street      | Cascais
    King          | Garden Street   | Aveiro
    Lopez         | Grand Street    | Vila R
    Martin        | Royal Street    | Braga
    Nguyen        | School Street   | Castelo Branco
    Oliver        | 1st Stret       | Oporto
    Parker        | Hope Street     | Oporto
    (15 rows)
    ```

2. **Semi-Structured**
    - It represents data in a flexible and hierarchical manner, often allowing for variability in the data structure. This model is particularly well-suited for data that doesn't fit neatly into rows and columns, such as documents, web pages, and various types of unstructured data.<br>

    **Examples of Semi-Structured Data Formats**

    1. **XML (eXtensible Markup Language):**
    - Hierarchical data format that uses tags to define elements and attributes.
    - Example:
        ```xml
        <person>
        <name>Jim Halpert</name>
        <email>johndoe@example.com</email>
        <phones>
            <phone type="home">123-456-7890</phone>
            <phone type="work">098-765-4321</phone>
        </phones>
        </person>
        ```

    2. **JSON (JavaScript Object Notation):**
    - Lightweight data-interchange format that's easy for humans to read and write and for machines to parse and generate.
    - Example:
        ```json
        {
        "name": "Jim Halpert",
        "email": "jimhalpert@gmail.com",
        "phones": {
            "home": "123-456-7890",
            "work": "098-765-4321"
        }
        }
        ```

    3. **YAML (YAML Ain't Markup Language):**
    - Human-readable data format that's often used for configuration files and data serialization.
    - Example:
        ```yaml
        name: Jim Halpert
        email: imhalpert@gmail.com
        phones:
        home: 123-456-7890
        work: 098-765-4321
        ```

3. **Entity-Relationship Model**
    - **Structure**: Data is modeled using **entities** (*objects*) and **relationships** between them, often represented diagrammatically.
    - **Usage**: Conceptual design phase, used to design databases before implementation.

4. **Hierarchical Data Model**
5. **Object-Oriented Data Model**
6. **Document Data Model**
7. **...**


### Database Design

1. **Requirements Specification**

    Gather and document detailed requirements from stakeholders, including data, functional, and non-functional requirements.

    - **Identify Stakeholders**: Determine who will use the database and gather requirements from them.
    - **Define Objectives**: Understand the purpose of the database, what problems it will solve, and what benefits it should provide.
    - **Data Requirements**: Identify what data needs to be stored, including data types, data volumes, and data relationships.
    - **Functional Requirements**: Define the operations that need to be supported, such as data entry, updates, deletions, and queries.
    - **Non-Functional Requirements**: Specify performance requirements, security needs, backup and recovery procedures, and constraints.

    **Example: University Database**

    - **Stakeholders**: Students, instructors, administrative staff.
    - **Objectives**: Manage student enrollment, course offerings, and grades.
    - **Data Requirements**: Store information about students, courses, instructors, and enrollments.
    - **Functional Requirements**: Register students for courses, assign grades, generate reports.
    - **Non-Functional Requirements**: Ensure data security, handle concurrent access, provide backup and recovery.

2. **Conceptual Model (Semantic Data Model)**
    
    High-level representation of the data and its relationships. It serves as a bridge between the requirements specification and the relational schema.
    The **semantic data model** provides a more abstract and user-centric view of data, which is a good starting point for database design.

    **Examples of Semantic Data Models**:

    1. **Entity-Association (E-A) Model**:
    Graphically describes entities and the relationships between them.
        - **Entities**: Represent objects or concepts (e.g., Student, Course).
        - **Attributes**: Properties of entities (e.g., StudentID, Name).
        - **Relationships**: Connections between entities (e.g., Enrolls, Teaches).

    2. **UML Class Diagram**:
    Used in object-oriented design to represent classes (entities) and their relationships.
        - **Classes**: Represent entities.
        - **Attributes**: Represent properties of classes.
        - **Methods**: Represent behaviors (often not included in a database context).
        - **Associations**: Represent relationships between classes.

3. **Relational Schema**

    The relational schema is the implementation of the conceptual model into a relational database structure. It involves defining tables, columns, primary keys, foreign keys, and constraints.



## Structured Query Language (SQL)

**SQL** is a standardized programming language used to manage and manipulate relational databases. It is the primary language for interacting with databases and allows users to perform a variety of operations.<br>

It serves as the primary **interface between users or applications and the DBMS**.

- Users interact with the database by writing SQL queries, commands, and statements to perform operations on the data stored in the DBMS.

- The DBMS interprets and executes SQL commands received from users or applications. It processes queries, retrieves data, performs data manipulations, and enforces constraints based on the SQL statements provided.

SQL provides a standardized way to communicate with different types of DBMSs. While there are variations in SQL syntax and features across different DBMS implementations (such as MySQL, PostgreSQL, Oracle, SQL Server), the core SQL commands and concepts remain consistent.<br>

SQL includes both **DDL (Data Definition Language)** and **DML (Data Manipulation Language)** components.<br>

- **DDL (Data Definition Language)**
    - is used to define and manage the structure of the database objects.
    ```sql
    CREATE TABLE client (
    full_name VARCHAR(80) PRIMARY KEY,
    birth DATE NOT NULL
    );

    ALTER TABLE client ADD age INT;

    DROP TABLE IF EXISTS client;
    ```

- **DML (Data Manipulation Language)**
    - is used to manipulate data within the database objects.
    ```sql
    SELECT full_name, age
    FROM client
    WHERE age > 17;

    INSERT INTO client (full_name, birth, age)
    VALUES ('Jim Halpert', '2000-02-20', 23);

    UPDATE client
    SET full_name = 'Jimothy Halpert'
    WHERE full_name = 'Jim Halpert';

    DELETE FROM client
    WHERE age < 18;
    ```
> [!NOTE]
> If we do not indicate a condition with the `WHERE` clause, the `UPDATE`/`DELETE` will rewrite/delete all existing rows in the table.





**Data Types**
- **Numeric Types**:
    - `INTEGER`
        - `BIGINT`
        - `SMALLINT`
        - `SERIAL`: integer that is **incremented** with each line inserted.
    - `NUMERIC`: Exact numeric type with user-specified precision and scale (e.g., NUMERIC(10,2) for a number with 10 digits total, 2 after the decimal point).
    - `FLOAT`
        - `DOUBLE`

- **Character Types**:
    - `CHAR(n)`: **Fixed-length** character string with a **maximum length of n** characters.
    - `VARCHAR(n)`: **Variable-length** character string with a **maximum length of n** characters.
    - `TEXT`: **Variable-length** character string with **no specified length limit**.

- **Time Types**:
    - `DATE`: Calendar date (year, month, day).
    - `TIME`: Time of day (hours, minutes, seconds).
    - `TIMESTAMP`: Date and time (including fractional seconds).

- **Boolean**
    - `BOOLEAN`




### SQL in practice (Basic)

- **Primary keys** ensure **uniqueness** within a table, while **foreign keys** establish **relationships** between tables, maintaining data integrity and enabling effective data retrieval and manipulation through structured queries.
    - **Foreign Keys Constraints Actions** They define what should happen when a referenced primary key in a parent table is **updated or deleted**.
        - `CASCADE`
            - When specified, the corresponding rows in the child table are automatically deleted or updated when the referenced row in the parent table is deleted or updated.
            ```sql
            -- if a client is deleted, all related lawyers (lawyers that reference that client) will also be deleted.
            CREATE TABLE lawyer (
                law_firm VARCHAR(26) PRIMARY KEY,
                age INT,
                full_name VARCHAR(80),
                CONSTRAINT fk_lawyer_client 
                FOREIGN KEY (full_name) REFERENCES client (full_name)
                ON DELETE CASCADE
                ON UPDATE CASCADE
            );
            ```
        - `SET NULL`
            - When specified, the foreign key columns in the child table are set to NULL if the corresponding row in the parent table is deleted or updated, allowing the child row to remain in the table even though it no longer references a valid parent row.
        - `SET DEFAULT`
            - When specified, the foreign key columns in the child table are set to their default value if the corresponding row in the parent table is deleted or updated.
            - The default value must be defined in the child table schema.
            ```sql
            CREATE TABLE lawyer (
                law_firm VARCHAR(26) PRIMARY KEY,
                age INT,
                client_name VARCHAR(80) DEFAULT 'none',
                CONSTRAINT fk_client 
                FOREIGN KEY (full_name) REFERENCES client (full_name)
                ON DELETE SET DEFAULT
                ON UPDATE SET DEFAULT
            );
        - `NO ACTION` / `RESTRICT`
            - Prevents the delete or update operation if it would result in orphaned rows in the child table. (error)
        - `NO ACTION`
            - This is the **default** behavior if no action is specified.
            - If an attempt is made to delete or update a row to which existing foreign key references point, the operation is rejected (error).
            - The check is deferred until the end of the transaction, meaning the actual enforcement of the constraint happens when the transaction is about to commit.
            ```sql
            -- example
            BEGIN;
            DELETE FROM client where full_name = 'Jimothy Halpert';                                          
            COMMIT;

            ERROR:  update or delete on table "client" violates foreign key constraint "fk_lawyer_client" on table "lawyer"
            DETAIL:  Key (full_name)=(Jimothy Halpert) is still referenced from table "lawyer".
            ROLLBACK
            ```
        - `RESTRICT`
            - Similar to `NO ACTION`, but the check is immediate. The operation is prevented if it would violate the foreign key constraint.

- `CHECK`
    - constraint used to specify a condition that must be met for each row in a table.
    - the **CHECK** constraint is limited to evaluating conditions within a single table; it cannot directly reference columns from other tables.
    ```sql
    alter table client
    add constraint check_above_age
    check (age > 17)
    ```

- `ALTER`
    - allows renaming the table, a column or a restriction
    - allows adding/removing columns, restrictions
    - allows editing columns (data type, restrictions)
    
    ```sql
    ALTER TABLE client
    RENAME COLUMN birth TO birthdate,
    ADD balance INT,
    ADD CONSTRAINT check_real_age CHECK (age < 150),
    DROP age,
    ALTER age TYPE FLOAT;

    ALTER TABLE client RENAME TO allClients;

    ALTER TABLE client ALTER COLUMN balance SET DEFAULT 500;
    ```



- `COPY TO`
    - export data from a table to a file in various formats
    ```sql
    COPY tab [ ( col [, ...] ) ]
    TO { 'file' | PROGRAM 'command' | STDOUT } 
    [ ( option [, ...] ) ];

    -- example
    COPY client TO '/path/to/clients.csv' WITH CSV HEADER;
    ```

- `COPY FROM`
    - import data from a file into a table
    ```sql
    COPY tab [ ( col [, ...] ) ]
    FROM { 'file' | PROGRAM 'command' | STDIN }
    [ ( option [, ...] ) ];

    -- example
    COPY client (full_name, birth, age) FROM '/path/to/clients.csv' WITH CSV HEADER;
    ```

- **SQL Operators**
    - SQL has the usual operators for **arithmetic**, **comparison** and **logic**.
        - Note that the `!=` operator can be written as `<>`
    - Other operators
        - [NOT] **BETWEEN** x AND y
        - **IS** [NOT] NULL / TRUE / FALSE / UNKNOWN
        - <expression> [NOT] **IN** (value, ...)
        - <expression> <operator> **ANY** / SOME / ALL (array)
        - <string> [NOT] **LIKE** <pattern> 

        ```sql
        -- examples
        SELECT * FROM client WHERE age BETWEEN 20 AND 30;
        SELECT * FROM client WHERE full_name NOT IN ('Jim Halpert', 'Michael Scott');
        SELECT * FROM client WHERE full_name LIKE 'J%';
        SELECT * FROM client WHERE balance IS NULL;
        SELECT * FROM client WHERE balance = ANY (ARRAY[1000, 1500, 2000]);
        ```
    - Functions
        ```sql
        -- Aggregate Functions
        SELECT COUNT(*) FROM client;
        SELECT SUM(balance) FROM client;
        SELECT AVG(balance) FROM client;
        SELECT MIN(balance) FROM client;
        SELECT MAX(balance) FROM client;

        -- String Functions
        SELECT CONCAT(full_name, ' ', phone) AS contact FROM client;
        SELECT UPPER(full_name) FROM client;
        SELECT LOWER(full_name) FROM client;
        SELECT SUBSTRING(full_name, 1, 3) FROM client;
        SELECT LENGTH(full_name) FROM client;

        -- Date and Time Functions
        SELECT NOW();

        -- Numeric Functions
        SELECT ROUND(salary, 2) FROM Employees;
        SELECT CEIL(salary) FROM Employees;
        SELECT FLOOR(salary) FROM Employees;
        SELECT ABS(salary) FROM Employees;
        ```

- `LIKE` **\<pattern\>**
    - `%`: Represents zero, one, or multiple characters.
        - `LIKE 'a%'`: matches any string that starts with 'a'.
        - `LIKE '%at'`: matches any string that ends with 'at'.
        - `LIKE '%or%'`: matches any string that contains 'or' anywhere in the string.
    - `_`: Represents a single character.
        - `LIKE '_at'`: matches any three-character string that ends with 'at'.


> [!NOTE]
> In PostgreSQL, **strings must use single-quotes (')**, since double-quotes (") are used to refer to database objects (tables, columns, etc.).

<br>
<br>

Consider the following **bank database**, which will be used for the next examples.

![bank-model](media/bank.png)

- `SELECT`
    - a query refers to a request for data from a database, and `SELECT` Queries are the most common type used for data retrieval.

    ```sql
    [WITH with_query [, ...]]
    SELECT [ALL | DISTINCT [ON (expression [, ...])]]
    [* | expression [[AS] output_name] [, ...]]
    [FROM from_item [, ...]]
    [WHERE condition]
    [GROUP BY [ALL | DISTINCT] grouping_element [, ...]]
    [HAVING condition]
    [{UNION | INTERSECT | EXCEPT} [ALL | DISTINCT] select]
    [ORDER BY expression [ASC | DESC | USING operator]
    [NULLS { FIRST | LAST}] [, ...]]
    [LIMIT {count | ALL}]
    ```

- **Subquery**
    - Subqueries can impact performance, especially if not optimized. Consider alternative approaches like **joins** or **common table expressions** (CTEs) for complex scenarios.
    ```sql
    -- extremely inefficient example to select customers who have both a loan and a loan
    SELECT DISTINCT customer_name FROM customer WHERE customer_name IN (SELECT customer_name FROM borrower WHERE customer_name IN (SELECT customer_name FROM depositor));

    -- best alternative
    SELECT DISTINCT b.customer_name FROM borrower b JOIN depositor d ON b.customer_name = d.customer_name;
    ```

- `GROUP BY`
    - **GROUP BY** is a clause in SQL used to group rows that have the same values into summary rows.
    - Is often used in conjunction with aggregate functions (such as SUM, COUNT, AVG, MIN, MAX) to perform calculations on these groups.
    - Collects all rows that share the same value(s) in specified columns and then applies aggregate functions to each group.


    1. How many customers are there in each city
        ```sql
        SELECT customer_city AS city, COUNT(*) AS total_customers FROM customer GROUP BY customer_city;
        ```

        city           | total_customers 
        ---------------|-----------------
        Braga          |               2
        Castelo Branco |               1
        Lisbon         |               2
        Coimbra        |               2
        Oporto         |               4
        Faro           |               1
        Vila Real      |               1
        Aveiro         |               1
        Cascais        |               1

    2. How many customers, above one, are there in each city
        ```sql
        SELECT customer_city AS city, count(*) AS total_customers FROM customer GROUP BY customer_city HAVING count(*) > 1;
        ```

        city    | total_customers 
        --------|-----------------
        Braga   |               2
        Lisbon  |               2
        Coimbra |               2
        Oporto  |               4


- `ORDER BY`
    - **ORDER BY** is a clause in SQL used to sort the result set of a query based on one or more columns. 
    
    1. List Branches Ordered by Assets in Descending Order

        ```sql
        SELECT branch_name AS branch, assets
        FROM branch
        ORDER BY assets DESC;
        ```

        |   branch     |    assets    |
        |--------------|--------------|
        |Round Hill    | 8000000.0000 |
        |University    | 7200000.0000 |
        |Wine Celar    | 4002800.0000 |
        |Clerigos      | 3900000.0000 |
        |Central       | 2100000.0000 |
        |Downtown      | 1900000.0000 |
        |Uptown        | 1700000.0000 |
        |Metro         |  400200.0000 |
        |Ship Terminal |  400000.0000 |


![joins](media/sql-join.png)

- `JOIN`
    - used to combine rows from two or more tables based on a related column between them.

    1. `LEFT JOIN`

        ```sql
        -- List all customers and their account balances (if any)

        SELECT c.customer_name AS customer, a.balance
        FROM customer c
        LEFT JOIN depositor d ON c.customer_name = d.customer_name
        LEFT JOIN account a ON d.account_number = a.account_number;
        ```

        | customer | balance |  
        |----------|---------|
        | Cook     | 700.0000|
        | Johnson  | 500.0000|
        | Cook     | 500.0000|
        | Johnson  | 900.0000|
        | Brown    | 600.0000|
        | Iacocca  | 650.0000|
        | Evans    | 550.0000|
        | Flores   | 800.0000|
        | Oliver   | 750.0000|
        | Brown    | 850.0000|
        | Lopez    |         |
        | Nguyen   |         |
        | Adams    |         |
        | Parker   |         |
        | King     |         |
        | Davis    |         |
        | Martin   |         |
        | Gonzalez |         |

    2. `NATURAL JOIN`
        - Type of join that automatically joins tables based on columns with the same name and data types.
        - Be careful when using it, as it may behave unexpectedly. `NATURAL JOIN` performs a join on all columns with the same name, so if we have two columns with the same name but not related, we will obtain incorrect data.

        ```sql
        SELECT customer_name AS customers_in_debt, amount AS debt
        FROM borrower
        NATURAL JOIN loan;
        ```

        customers_in_debt |   debt    
        ------------------|-----------
        Brown             | 6000.0000
        Nguyen            | 4000.0000
        Cook              | 3000.0000
        Iacocca           | 7000.0000
        Gonzalez          | 1000.0000
        Iacocca           | 1000.0000
        Parker            | 8000.0000
        Brown             | 9000.0000
        Brown             | 2000.0000
        Davis             | 5000.0000


- **SET OPERATORS**
    1. `UNION`

        ```sql
        -- list all customers who have either a deposit or a loan

        SELECT customer_name FROM depositor
        UNION
        SELECT customer_name FROM borrower;
        ```

        |customer_name |
        |--------------|
        | Cook         |
        | Johnson      |
        | Oliver       |
        | Davis        |
        | Brown        |
        | Flores       |
        | Nguyen       |
        | Evans        |
        | Parker       |
        | Gonzalez     |
        | Iacocca      |
    
    2. `INTERSECT`

        ```sql
        -- list all customers who have both a deposit and a loan

        SELECT customer_name FROM depositor
        INTERSECT
        SELECT customer_name FROM borrower;
        ```

        |customer_name |
        |--------------|
        | Cook         |
        | Brown        |
        | Iacocca      |

    3. `EXCEPT`

        ```sql
        -- list all customers who have a deposit byt do not have a loan

        SELECT customer_name FROM depositor
        UNION
        SELECT customer_name FROM borrower;
        ```

        |customer_name |
        |--------------|
        | Johnson      |
        | Oliver       |
        | Flores       |
        | Evans        |



### SQL in practice (Advanced)

