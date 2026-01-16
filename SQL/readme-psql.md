# PostgreSQL Complete Guide for Tech Interviews

> A comprehensive, interview-focused PostgreSQL reference guide. This document covers PostgreSQL-specific syntax and features, highlighting differences from MySQL.

---

## What is PostgreSQL?

**PostgreSQL** (often called "Postgres") is a powerful, open-source Object-Relational Database Management System (ORDBMS). It extends SQL with additional features like table inheritance, custom types, and advanced indexing.

**Key Insight for Interviews:** PostgreSQL is known for its standards compliance, extensibility, and ACID compliance. It's often preferred for complex applications requiring advanced data types and integrity.

```
+----------------------------------------------------------+
|  You (Client)  ->  SQL Commands  ->  PostgreSQL Server   |
|                                           |              |
|                                      Database Tables     |
+----------------------------------------------------------+
```

### Database Types Quick Reference

| Type       | Description                                                                         | Examples                              |
| ---------- | ----------------------------------------------------------------------------------- | ------------------------------------- |
| **RDBMS**  | Relational Database Management System - stores data in tables with rows and columns | MySQL, PostgreSQL, Oracle, SQL Server |
| **ORDBMS** | Object-Relational DBMS - extends RDBMS with object-oriented features                | **PostgreSQL**                        |
| **NoSQL**  | Non-relational databases for unstructured data                                      | MongoDB, CouchBase, Redis             |

---

## CRUD Operations - The Foundation

| Operation  | SQL Command | Purpose                    |
| ---------- | ----------- | -------------------------- |
| **C**reate | `INSERT`    | Add new records to a table |
| **R**ead   | `SELECT`    | Retrieve data from tables  |
| **U**pdate | `UPDATE`    | Modify existing records    |
| **D**elete | `DELETE`    | Remove records from tables |

---

## SQL Command Categories

### 1. DDL (Data Definition Language) - Defines Structure

| Command    | Purpose                              | PostgreSQL Example                          |
| ---------- | ------------------------------------ | ------------------------------------------- |
| `CREATE`   | Create tables, databases, views      | `CREATE TABLE users (id SERIAL PRIMARY KEY);` |
| `ALTER`    | Modify table structure               | `ALTER TABLE users ADD COLUMN email VARCHAR(255);` |
| `DROP`     | Delete tables, databases entirely    | `DROP TABLE users;`                         |
| `TRUNCATE` | Remove ALL rows (faster than DELETE) | `TRUNCATE TABLE users RESTART IDENTITY;`    |

### 2. DQL/DRL (Data Query Language) - Reads Data

```sql
SELECT column1, column2   -- Columns to retrieve (use * for all)
FROM table_name           -- Source table
WHERE condition;          -- Filter criteria (optional)
```

### 3. DML (Data Manipulation Language) - Modifies Data

| Command  | Purpose              |
| -------- | -------------------- |
| `INSERT` | Add new rows         |
| `UPDATE` | Change existing rows |
| `DELETE` | Remove specific rows |

### 4. DCL (Data Control Language) - Manages Permissions

```sql
-- Grant specific permissions to a user/role
GRANT SELECT, INSERT ON employees TO user_john;

-- Remove permissions
REVOKE INSERT ON employees FROM user_john;

-- Grant all privileges
GRANT ALL PRIVILEGES ON employees TO admin_role;

-- Grant with option to re-grant
GRANT SELECT ON employees TO user_john WITH GRANT OPTION;
```

### 5. TCL (Transaction Control Language) - Manages Transactions

```sql
BEGIN;  -- Start a transaction (or START TRANSACTION)
    UPDATE accounts SET balance = balance - 100 WHERE id = 1;
    UPDATE accounts SET balance = balance + 100 WHERE id = 2;
COMMIT;  -- Make ALL changes permanent
-- OR use ROLLBACK; to undo ALL changes
```

**PostgreSQL-Specific Transaction Features:**

```sql
-- Savepoints for partial rollback
BEGIN;
    INSERT INTO orders (product_id, quantity) VALUES (1, 10);
    SAVEPOINT my_savepoint;
    INSERT INTO orders (product_id, quantity) VALUES (2, 5);
    -- Oops, wrong data
    ROLLBACK TO SAVEPOINT my_savepoint;
    INSERT INTO orders (product_id, quantity) VALUES (2, 15);
COMMIT;

-- Transaction isolation levels
BEGIN TRANSACTION ISOLATION LEVEL SERIALIZABLE;
    -- Your queries here
COMMIT;
```

---

## PostgreSQL Data Types

### String Types

| Type                  | Size                         | When to Use                                            |
| --------------------- | ---------------------------- | ------------------------------------------------------ |
| `CHAR(n)`             | Fixed n characters           | When all values have same length (e.g., country codes) |
| `VARCHAR(n)`          | Variable, up to n characters | **Most common choice** - names, emails, etc.           |
| `TEXT`                | Unlimited variable length    | Long text content (no performance penalty in PostgreSQL) |

> **PostgreSQL Note:** Unlike MySQL, `TEXT` has no performance penalty vs `VARCHAR` in PostgreSQL. Use `TEXT` freely for variable-length strings.

### Numeric Types

| Type              | Range                      | Use Case                                        |
| ----------------- | -------------------------- | ----------------------------------------------- |
| `SMALLINT`        | -32,768 to 32,767          | Small counts                                    |
| `INTEGER` / `INT` | -2.1 billion to 2.1 billion | Most numeric data                               |
| `BIGINT`          | Huge numbers               | IDs for large systems                           |
| `REAL`            | 6 decimal digits precision | Approximate decimals                            |
| `DOUBLE PRECISION`| 15 decimal digits          | Scientific calculations                         |
| `NUMERIC(p,s)`    | Exact precision            | **Money** - always use this for currency!       |
| `DECIMAL(p,s)`    | Same as NUMERIC            | Alias for NUMERIC                               |

**Auto-Incrementing Types (PostgreSQL-Specific):**

| Type                      | Description                                  |
| ------------------------- | -------------------------------------------- |
| `SERIAL`                  | Auto-incrementing 4-byte integer (1 to 2.1B) |
| `BIGSERIAL`               | Auto-incrementing 8-byte integer             |
| `SMALLSERIAL`             | Auto-incrementing 2-byte integer             |
| `GENERATED AS IDENTITY`   | SQL-standard way (PostgreSQL 10+)            |

```sql
-- Using SERIAL (PostgreSQL way)
CREATE TABLE products (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100)
);

-- Using GENERATED AS IDENTITY (SQL standard, recommended)
CREATE TABLE products (
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    name VARCHAR(100)
);

-- Allow manual override
CREATE TABLE products (
    id INTEGER GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    name VARCHAR(100)
);
```

### Date/Time Types

| Type                        | Format                    | Example                                     |
| --------------------------- | ------------------------- | ------------------------------------------- |
| `DATE`                      | YYYY-MM-DD                | '2024-01-15'                                |
| `TIME`                      | HH:MM:SS                  | '14:30:00'                                  |
| `TIME WITH TIME ZONE`       | HH:MM:SS+TZ               | '14:30:00+05:30'                            |
| `TIMESTAMP`                 | YYYY-MM-DD HH:MM:SS       | '2024-01-15 14:30:00'                       |
| `TIMESTAMP WITH TIME ZONE`  | With timezone (TIMESTAMPTZ)| '2024-01-15 14:30:00+05:30'                |
| `INTERVAL`                  | Time duration             | '1 year 2 months 3 days'                    |

```sql
-- Interval examples
SELECT NOW() + INTERVAL '1 day';
SELECT NOW() - INTERVAL '2 hours 30 minutes';
SELECT age(TIMESTAMP '2024-01-15', TIMESTAMP '1990-06-15');
```

### Special PostgreSQL Types

| Type       | Description                                    | Example                                      |
| ---------- | ---------------------------------------------- | -------------------------------------------- |
| `BOOLEAN`  | true/false/null (not 0/1)                      | `TRUE`, `FALSE`, `'yes'`, `'no'`             |
| `UUID`     | Universally Unique Identifier                  | `gen_random_uuid()`                          |
| `JSON`     | JSON data (stored as text)                     | `'{"key": "value"}'`                         |
| `JSONB`    | Binary JSON (faster queries, indexable)        | `'{"key": "value"}'::jsonb`                  |
| `ARRAY`    | Array of any type                              | `ARRAY[1, 2, 3]` or `'{1,2,3}'`              |
| `HSTORE`   | Key-value pairs                                | `'key1=>value1, key2=>value2'`               |
| `INET`     | IPv4 or IPv6 address                           | `'192.168.1.1'`                              |
| `CIDR`     | IPv4 or IPv6 network                           | `'192.168.1.0/24'`                           |
| `MACADDR`  | MAC address                                    | `'08:00:2b:01:02:03'`                        |

**JSONB Examples:**

```sql
-- Create table with JSONB
CREATE TABLE events (
    id SERIAL PRIMARY KEY,
    data JSONB
);

-- Insert JSON data
INSERT INTO events (data) VALUES ('{"name": "Click", "page": "/home"}');

-- Query JSON data
SELECT data->>'name' FROM events;           -- Get as text
SELECT data->'page' FROM events;            -- Get as JSON
SELECT * FROM events WHERE data->>'name' = 'Click';
SELECT * FROM events WHERE data @> '{"name": "Click"}';  -- Contains
```

**Array Examples:**

```sql
-- Create table with array column
CREATE TABLE students (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100),
    scores INTEGER[]
);

-- Insert array data
INSERT INTO students (name, scores) VALUES ('Alice', ARRAY[85, 90, 92]);
INSERT INTO students (name, scores) VALUES ('Bob', '{78, 82, 88}');

-- Query arrays
SELECT * FROM students WHERE 90 = ANY(scores);      -- Contains 90
SELECT * FROM students WHERE scores @> ARRAY[85];   -- Contains 85
SELECT name, scores[1] AS first_score FROM students; -- Access by index (1-based!)
SELECT name, array_length(scores, 1) AS num_scores FROM students;
```

### ENUM Type

```sql
-- Create enum type
CREATE TYPE mood AS ENUM ('sad', 'ok', 'happy');

-- Use in table
CREATE TABLE person (
    name VARCHAR(100),
    current_mood mood
);

INSERT INTO person VALUES ('Alice', 'happy');
```

---

## Database & Table Management (DDL)

### Creating and Managing Databases

```sql
-- Create a new database
CREATE DATABASE company_db;

-- Create with specific encoding and owner
CREATE DATABASE company_db
    WITH OWNER = postgres
    ENCODING = 'UTF8'
    LC_COLLATE = 'en_US.UTF-8'
    LC_CTYPE = 'en_US.UTF-8';

-- Delete a database (must be disconnected from it)
DROP DATABASE IF EXISTS company_db;

-- List all databases (psql command)
\l
-- Or using SQL:
SELECT datname FROM pg_database;

-- Connect to a database (psql command)
\c company_db

-- Show current database
SELECT current_database();

-- List all tables in current schema
\dt
-- Or using SQL:
SELECT table_name FROM information_schema.tables 
WHERE table_schema = 'public';
```

### Schema Management (PostgreSQL Feature)

```sql
-- Create a schema (namespace for tables)
CREATE SCHEMA sales;

-- Create table in specific schema
CREATE TABLE sales.orders (
    id SERIAL PRIMARY KEY,
    amount NUMERIC(10,2)
);

-- Query from schema
SELECT * FROM sales.orders;

-- Set search path
SET search_path TO sales, public;

-- Drop schema
DROP SCHEMA sales CASCADE;  -- CASCADE drops all objects in schema
```

### Creating Tables

```sql
CREATE TABLE employees (
    -- SERIAL: PostgreSQL auto-increment (creates sequence automatically)
    id SERIAL PRIMARY KEY,
    
    -- NOT NULL: These fields MUST have a value
    first_name VARCHAR(50) NOT NULL,
    last_name VARCHAR(50) NOT NULL,
    
    -- UNIQUE: No two employees can have the same email
    email VARCHAR(100) UNIQUE,
    
    -- NUMERIC(10,2): Exact precision for money
    salary NUMERIC(10, 2) DEFAULT 0.00,
    
    -- Foreign key reference
    department_id INTEGER,
    
    -- DATE type
    hire_date DATE DEFAULT CURRENT_DATE,
    
    -- BOOLEAN: true/false (not 0/1 like MySQL)
    is_active BOOLEAN DEFAULT TRUE,
    
    -- Timestamps with automatic updates
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    
    -- Named foreign key constraint
    CONSTRAINT fk_department
        FOREIGN KEY (department_id) REFERENCES departments(id)
);

-- Describe table structure (psql)
\d employees
```

---

## Constraints - Rules for Data Integrity

### Primary Key

```sql
-- Method 1: Inline definition
CREATE TABLE customers (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100)
);

-- Method 2: Constraint at the end
CREATE TABLE customers (
    id INTEGER,
    name VARCHAR(100),
    PRIMARY KEY (id)
);

-- Method 3: Composite Primary Key
CREATE TABLE order_items (
    order_id INTEGER,
    product_id INTEGER,
    quantity INTEGER,
    PRIMARY KEY (order_id, product_id)
);
```

### Foreign Key

```sql
CREATE TABLE orders (
    order_id SERIAL PRIMARY KEY,
    order_date DATE,
    customer_id INTEGER,
    
    FOREIGN KEY (customer_id) REFERENCES customers(id)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);
```

**Cascade Options:**

| Option         | ON DELETE                             | ON UPDATE                             |
| -------------- | ------------------------------------- | ------------------------------------- |
| `CASCADE`      | Delete child rows when parent deleted | Update FK when parent PK changes      |
| `SET NULL`     | Set FK to NULL when parent deleted    | Set FK to NULL when parent PK changes |
| `SET DEFAULT`  | Set FK to default value               | Set FK to default value               |
| `RESTRICT`     | Prevent deletion if children exist    | Prevent update if children exist      |
| `NO ACTION`    | Same as RESTRICT (default)            | Same as RESTRICT (default)            |

### CHECK Constraint (Always Enforced in PostgreSQL)

```sql
CREATE TABLE employees (
    id SERIAL PRIMARY KEY,
    age INTEGER,
    salary NUMERIC(10,2),
    
    -- Named constraint
    CONSTRAINT chk_age CHECK (age >= 18 AND age <= 100),
    
    -- Unnamed constraint
    CHECK (salary > 0)
);
```

### NOT NULL with DEFAULT

```sql
CREATE TABLE products (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    price NUMERIC(10,2) NOT NULL DEFAULT 0.00,
    stock INTEGER NOT NULL DEFAULT 0
);
```

### EXCLUDE Constraint (PostgreSQL-Specific)

Prevents overlapping ranges or values:

```sql
-- Requires btree_gist extension
CREATE EXTENSION IF NOT EXISTS btree_gist;

CREATE TABLE room_reservations (
    room_id INTEGER,
    during TSRANGE,
    EXCLUDE USING GIST (room_id WITH =, during WITH &&)
);
-- Prevents double-booking: same room with overlapping time ranges
```

---

## ALTER Operations - Modifying Tables

```sql
-- ADD: Insert a new column
ALTER TABLE employees ADD COLUMN phone VARCHAR(20);

-- ADD multiple columns
ALTER TABLE employees
    ADD COLUMN middle_name VARCHAR(50),
    ADD COLUMN birth_date DATE;

-- ALTER COLUMN: Change data type
ALTER TABLE employees ALTER COLUMN phone TYPE CHAR(15);

-- ALTER COLUMN: Set/drop default
ALTER TABLE employees ALTER COLUMN salary SET DEFAULT 50000;
ALTER TABLE employees ALTER COLUMN salary DROP DEFAULT;

-- ALTER COLUMN: Set/drop NOT NULL
ALTER TABLE employees ALTER COLUMN phone SET NOT NULL;
ALTER TABLE employees ALTER COLUMN phone DROP NOT NULL;

-- RENAME COLUMN
ALTER TABLE employees RENAME COLUMN phone TO contact_number;

-- DROP COLUMN
ALTER TABLE employees DROP COLUMN middle_name;

-- RENAME TABLE
ALTER TABLE employees RENAME TO staff;

-- Add constraint
ALTER TABLE employees ADD CONSTRAINT chk_salary CHECK (salary > 0);

-- Drop constraint
ALTER TABLE employees DROP CONSTRAINT chk_salary;
```

---

## SELECT - The Heart of SQL

### Basic Syntax

```sql
-- Select specific columns
SELECT column1, column2 FROM table_name;

-- Get ALL columns
SELECT * FROM employees;

-- Get specific columns
SELECT first_name, last_name, salary FROM employees;
```

### SELECT Without a Table

```sql
-- Mathematical calculations
SELECT 55 + 11;              -- Returns: 66
SELECT 100 * 0.15;           -- Returns: 15.00

-- Built-in functions
SELECT NOW();                -- Current timestamp with timezone
SELECT CURRENT_DATE;         -- Current date
SELECT CURRENT_TIME;         -- Current time
SELECT CURRENT_TIMESTAMP;    -- Current timestamp

-- String functions
SELECT UPPER('hello');       -- Returns: HELLO
SELECT LOWER('WORLD');       -- Returns: world
SELECT LENGTH('test');       -- Returns: 4
SELECT CONCAT('Hello', ' ', 'World');  -- Returns: Hello World
SELECT 'Hello' || ' ' || 'World';      -- String concatenation operator
```

### WHERE - Filtering Rows

```sql
-- Basic comparison
SELECT * FROM employees WHERE salary > 50000;

-- Multiple conditions with AND
SELECT * FROM employees
WHERE department = 'Engineering' AND salary > 60000;

-- Multiple conditions with OR
SELECT * FROM employees
WHERE department = 'Sales' OR department = 'Marketing';

-- NOT operator
SELECT * FROM employees
WHERE NOT department = 'HR';
-- Or: WHERE department != 'HR'
-- Or: WHERE department <> 'HR'
```

### BETWEEN - Range Queries

```sql
-- BETWEEN is inclusive on both ends
SELECT * FROM employees
WHERE salary BETWEEN 40000 AND 80000;
-- Same as: WHERE salary >= 40000 AND salary <= 80000

-- Works with dates
SELECT * FROM orders
WHERE order_date BETWEEN '2024-01-01' AND '2024-12-31';
```

### IN - Multiple Value Matching

```sql
-- With IN
SELECT * FROM employees
WHERE department IN ('Sales', 'Marketing', 'HR');

-- NOT IN
SELECT * FROM employees
WHERE department NOT IN ('Accounting', 'Legal');
```

### IS NULL - Handling Missing Data

```sql
-- Find rows with NULL
SELECT * FROM employees WHERE manager_id IS NULL;

-- Find rows without NULL
SELECT * FROM employees WHERE manager_id IS NOT NULL;
```

> **Important:** Never use `= NULL` or `!= NULL`. Always use `IS NULL` or `IS NOT NULL`.

### Pattern Matching with LIKE and ILIKE

| Wildcard | Meaning                                |
| -------- | -------------------------------------- |
| `%`      | Any sequence of characters (0 or more) |
| `_`      | Exactly one character                  |

```sql
-- Case-sensitive LIKE
SELECT * FROM employees WHERE first_name LIKE 'A%';      -- Starts with 'A'
SELECT * FROM employees WHERE last_name LIKE '%son';     -- Ends with 'son'
SELECT * FROM employees WHERE first_name LIKE '%mar%';   -- Contains 'mar'
SELECT * FROM employees WHERE first_name LIKE '_a%';     -- Second char is 'a'

-- Case-INSENSITIVE ILIKE (PostgreSQL-specific!)
SELECT * FROM employees WHERE first_name ILIKE 'a%';     -- Starts with 'a' or 'A'
SELECT * FROM employees WHERE email ILIKE '%@gmail.com'; -- Case-insensitive email match
```

### SIMILAR TO and Regular Expressions (PostgreSQL-Specific)

```sql
-- SIMILAR TO (SQL regex)
SELECT * FROM employees WHERE first_name SIMILAR TO '(John|Jane|Joe)%';

-- POSIX Regular Expressions
SELECT * FROM employees WHERE first_name ~ '^[A-Z]';     -- Starts with uppercase
SELECT * FROM employees WHERE first_name ~* '^john';     -- Case-insensitive regex
SELECT * FROM employees WHERE first_name !~ '^[0-9]';    -- Doesn't start with digit
```

---

## Sorting and Grouping

### ORDER BY - Sorting Results

```sql
-- Sort ascending (default)
SELECT * FROM employees ORDER BY salary;
SELECT * FROM employees ORDER BY salary ASC;

-- Sort descending
SELECT * FROM employees ORDER BY salary DESC;

-- Multiple sort criteria
SELECT * FROM employees ORDER BY department ASC, salary DESC;

-- Sort by column position
SELECT first_name, salary FROM employees ORDER BY 2 DESC;  -- Sort by salary

-- NULLS FIRST / NULLS LAST (PostgreSQL-specific)
SELECT * FROM employees ORDER BY manager_id NULLS FIRST;
SELECT * FROM employees ORDER BY manager_id DESC NULLS LAST;
```

### GROUP BY - Aggregating Data

```sql
-- Count employees per department
SELECT department, COUNT(*) as employee_count
FROM employees
GROUP BY department;
```

**Aggregate Functions:**

| Function        | Purpose                          |
| --------------- | -------------------------------- |
| `COUNT(*)`      | Number of rows                   |
| `COUNT(column)` | Number of non-NULL values        |
| `SUM()`         | Total of numeric column          |
| `AVG()`         | Average value                    |
| `MIN()`         | Smallest value                   |
| `MAX()`         | Largest value                    |
| `STRING_AGG()`  | Concatenate strings (PostgreSQL) |
| `ARRAY_AGG()`   | Aggregate into array (PostgreSQL)|

```sql
-- Multiple aggregate functions
SELECT
    department,
    COUNT(*) as headcount,
    SUM(salary) as total_payroll,
    AVG(salary) as avg_salary,
    MIN(salary) as lowest_salary,
    MAX(salary) as highest_salary,
    STRING_AGG(first_name, ', ') as employee_names,
    ARRAY_AGG(first_name) as employee_array
FROM employees
GROUP BY department;
```

### HAVING - Filtering Groups

```sql
-- Departments with more than 10 employees
SELECT department, COUNT(*) as count
FROM employees
GROUP BY department
HAVING COUNT(*) > 10;

-- Departments with average salary above 60000
SELECT department, AVG(salary) as avg_sal
FROM employees
GROUP BY department
HAVING AVG(salary) > 60000;
```

### WHERE vs HAVING

| Aspect     | WHERE                           | HAVING                      |
| ---------- | ------------------------------- | --------------------------- |
| Filters    | Individual rows                 | Grouped rows                |
| Position   | Before GROUP BY                 | After GROUP BY              |
| Aggregates | Cannot use SUM(), COUNT(), etc. | Can use aggregate functions |
| Timing     | Filters BEFORE grouping         | Filters AFTER grouping      |

```sql
-- Both in action
SELECT department, AVG(salary) as avg_sal
FROM employees
WHERE hire_date > '2020-01-01'    -- Filter rows BEFORE grouping
GROUP BY department
HAVING AVG(salary) > 50000;       -- Filter groups AFTER grouping
```

### DISTINCT - Removing Duplicates

```sql
-- Get unique departments
SELECT DISTINCT department FROM employees;

-- DISTINCT ON (PostgreSQL-specific) - first row per group
SELECT DISTINCT ON (department) department, first_name, salary
FROM employees
ORDER BY department, salary DESC;
-- Returns highest-paid employee per department
```

---

## Data Modification (DML)

### INSERT - Adding New Data

```sql
-- INSERT single row
INSERT INTO employees (first_name, last_name, email, salary)
VALUES ('John', 'Doe', 'john@company.com', 55000);

-- INSERT multiple rows
INSERT INTO employees (first_name, last_name, salary)
VALUES
    ('Jane', 'Smith', 60000),
    ('Bob', 'Johnson', 52000),
    ('Alice', 'Williams', 58000);

-- INSERT with RETURNING (PostgreSQL-specific!)
INSERT INTO employees (first_name, last_name, salary)
VALUES ('Charlie', 'Brown', 65000)
RETURNING id, first_name, salary;
-- Returns the inserted row(s) including generated id
```

### UPDATE - Modifying Existing Data

```sql
-- UPDATE single row
UPDATE employees SET salary = 65000 WHERE id = 1;

-- UPDATE multiple columns
UPDATE employees
SET salary = 70000, department = 'Senior Engineering'
WHERE id = 1;

-- UPDATE multiple rows
UPDATE employees
SET salary = salary * 1.10
WHERE department = 'Sales';

-- UPDATE with RETURNING (PostgreSQL-specific!)
UPDATE employees
SET salary = salary * 1.10
WHERE department = 'Sales'
RETURNING id, first_name, salary;
-- Returns all updated rows

-- UPDATE with FROM clause (PostgreSQL-specific!)
UPDATE employees e
SET salary = e.salary * d.bonus_multiplier
FROM departments d
WHERE e.department_id = d.id;
```

> **Always use WHERE with UPDATE unless you intentionally want to modify all rows!**

### DELETE - Removing Data

```sql
-- DELETE specific row
DELETE FROM employees WHERE id = 5;

-- DELETE multiple rows
DELETE FROM employees WHERE department = 'Temporary';

-- DELETE with RETURNING (PostgreSQL-specific!)
DELETE FROM employees
WHERE salary < 30000
RETURNING *;
-- Returns deleted rows

-- DELETE with USING (PostgreSQL-specific!)
DELETE FROM employees e
USING departments d
WHERE e.department_id = d.id AND d.name = 'Closed';
```

### UPSERT - ON CONFLICT (PostgreSQL-Specific!)

Insert or update in a single statement:

```sql
-- Insert or do nothing on conflict
INSERT INTO employees (id, first_name, last_name, email)
VALUES (1, 'John', 'Doe', 'john@company.com')
ON CONFLICT (id) DO NOTHING;

-- Insert or update on conflict
INSERT INTO employees (id, first_name, last_name, salary)
VALUES (1, 'John', 'Doe', 75000)
ON CONFLICT (id) DO UPDATE SET
    salary = EXCLUDED.salary,
    first_name = EXCLUDED.first_name;
-- EXCLUDED refers to the row that would have been inserted

-- On conflict with unique constraint
INSERT INTO employees (email, first_name, last_name)
VALUES ('john@company.com', 'Johnny', 'Doe')
ON CONFLICT ON CONSTRAINT employees_email_key DO UPDATE SET
    first_name = EXCLUDED.first_name;
```

### TRUNCATE - Fast Delete All Rows

```sql
-- Basic truncate
TRUNCATE TABLE employees;

-- Reset auto-increment (SERIAL) counter
TRUNCATE TABLE employees RESTART IDENTITY;

-- Cascade to dependent tables
TRUNCATE TABLE departments CASCADE;

-- Truncate multiple tables
TRUNCATE TABLE orders, order_items RESTART IDENTITY CASCADE;
```

**DELETE vs TRUNCATE:**

| Aspect         | DELETE                | TRUNCATE                         |
| -------------- | --------------------- | -------------------------------- |
| WHERE clause   | Yes                   | No                               |
| Speed          | Slower (row by row)   | Faster (drops and recreates)     |
| Triggers       | Fires DELETE triggers | Does not fire row triggers       |
| Transaction    | Can rollback          | Can rollback (in PostgreSQL!)    |
| SERIAL reset   | Keeps counter         | Optional: RESTART IDENTITY       |

---

## JOINs - Combining Tables

### Sample Data for Examples

```
employees                          departments
+----+----------+---------+        +----+-------------+
| id | name     | dept_id |        | id | dept_name   |
+----+----------+---------+        +----+-------------+
| 1  | Alice    | 1       |        | 1  | Engineering |
| 2  | Bob      | 2       |        | 2  | Sales       |
| 3  | Charlie  | NULL    |        | 3  | HR          |
+----+----------+---------+        +----+-------------+
```

### INNER JOIN

```sql
SELECT e.name, d.dept_name
FROM employees e
INNER JOIN departments d ON e.dept_id = d.id;
-- Result: Alice/Engineering, Bob/Sales (Charlie excluded)
```

### LEFT JOIN (Left Outer Join)

```sql
SELECT e.name, d.dept_name
FROM employees e
LEFT JOIN departments d ON e.dept_id = d.id;
-- Result: Alice/Engineering, Bob/Sales, Charlie/NULL
```

### RIGHT JOIN (Right Outer Join)

```sql
SELECT e.name, d.dept_name
FROM employees e
RIGHT JOIN departments d ON e.dept_id = d.id;
-- Result: Alice/Engineering, Bob/Sales, NULL/HR
```

### FULL OUTER JOIN (Native in PostgreSQL!)

Unlike MySQL, PostgreSQL supports FULL OUTER JOIN natively:

```sql
SELECT e.name, d.dept_name
FROM employees e
FULL OUTER JOIN departments d ON e.dept_id = d.id;
-- Result: Alice/Engineering, Bob/Sales, Charlie/NULL, NULL/HR
```

### CROSS JOIN

```sql
SELECT e.name, d.dept_name
FROM employees e
CROSS JOIN departments d;
-- Returns all combinations (3 x 3 = 9 rows)
```

### SELF JOIN

```sql
-- Find employees and their managers
SELECT
    e.name as employee,
    m.name as manager
FROM employees e
LEFT JOIN employees m ON e.manager_id = m.id;
```

### LATERAL JOIN (PostgreSQL-Specific!)

Allows the right-hand side to reference columns from the left:

```sql
-- Get top 3 orders for each customer
SELECT c.name, o.order_date, o.amount
FROM customers c
LEFT JOIN LATERAL (
    SELECT order_date, amount
    FROM orders
    WHERE customer_id = c.id
    ORDER BY amount DESC
    LIMIT 3
) o ON TRUE;
```

### USING Clause

```sql
-- When column names match
SELECT e.name, d.dept_name
FROM employees e
INNER JOIN departments d USING (department_id);
```

### NATURAL JOIN

```sql
-- Automatically joins on all matching column names
SELECT e.name, d.dept_name
FROM employees e
NATURAL JOIN departments d;
-- Caution: Can be unpredictable if column names change
```

---

## SET Operations

PostgreSQL supports all standard SET operations natively!

### UNION

```sql
-- Combines and removes duplicates
SELECT name FROM employees_usa
UNION
SELECT name FROM employees_india;
```

### UNION ALL

```sql
-- Combines and keeps duplicates (faster)
SELECT name FROM employees_usa
UNION ALL
SELECT name FROM employees_india;
```

### INTERSECT (Native in PostgreSQL!)

```sql
-- Returns only common rows
SELECT name FROM employees_usa
INTERSECT
SELECT name FROM employees_india;
```

### EXCEPT (Native in PostgreSQL!)

```sql
-- Returns rows in first query but not in second
SELECT name FROM employees_usa
EXCEPT
SELECT name FROM employees_india;

-- With ALL to keep duplicates
SELECT name FROM employees_usa
EXCEPT ALL
SELECT name FROM employees_india;
```

---

## Subqueries (Nested Queries)

### Subquery in WHERE Clause

```sql
-- Find employees earning more than average
SELECT name, salary
FROM employees
WHERE salary > (SELECT AVG(salary) FROM employees);

-- Using IN with subquery
SELECT name FROM employees
WHERE dept_id IN (
    SELECT id FROM departments WHERE dept_name = 'Engineering'
);
```

### Subquery in FROM Clause (Derived Table)

```sql
SELECT MAX(rating) as highest_rating
FROM (
    SELECT * FROM movies WHERE country = 'India'
) AS indian_movies;
```

### Subquery in SELECT Clause

```sql
SELECT
    name,
    salary,
    (SELECT AVG(salary) FROM employees) as company_avg
FROM employees;
```

### Correlated Subqueries

```sql
-- Find employees earning more than their department's average
SELECT e1.name, e1.salary, e1.dept_id
FROM employees e1
WHERE e1.salary > (
    SELECT AVG(e2.salary)
    FROM employees e2
    WHERE e2.dept_id = e1.dept_id
);
```

### EXISTS Subquery

```sql
-- Find departments that have employees
SELECT d.dept_name
FROM departments d
WHERE EXISTS (
    SELECT 1 FROM employees e WHERE e.dept_id = d.id
);

-- Find departments without employees
SELECT d.dept_name
FROM departments d
WHERE NOT EXISTS (
    SELECT 1 FROM employees e WHERE e.dept_id = d.id
);
```

---

## Common Table Expressions (CTEs)

CTEs make complex queries more readable using `WITH` clause:

### Basic CTE

```sql
WITH high_earners AS (
    SELECT name, salary, department
    FROM employees
    WHERE salary > 80000
)
SELECT * FROM high_earners WHERE department = 'Engineering';
```

### Multiple CTEs

```sql
WITH 
    dept_stats AS (
        SELECT department, AVG(salary) as avg_salary
        FROM employees
        GROUP BY department
    ),
    high_paying_depts AS (
        SELECT department FROM dept_stats WHERE avg_salary > 60000
    )
SELECT e.name, e.salary
FROM employees e
JOIN high_paying_depts h ON e.department = h.department;
```

### Recursive CTE (PostgreSQL-Specific!)

```sql
-- Generate series 1 to 10
WITH RECURSIVE numbers AS (
    SELECT 1 as n
    UNION ALL
    SELECT n + 1 FROM numbers WHERE n < 10
)
SELECT * FROM numbers;

-- Employee hierarchy
WITH RECURSIVE employee_hierarchy AS (
    -- Base case: top-level employees (no manager)
    SELECT id, name, manager_id, 1 as level
    FROM employees
    WHERE manager_id IS NULL
    
    UNION ALL
    
    -- Recursive case: employees with managers
    SELECT e.id, e.name, e.manager_id, h.level + 1
    FROM employees e
    JOIN employee_hierarchy h ON e.manager_id = h.id
)
SELECT * FROM employee_hierarchy ORDER BY level, name;
```

---

## Window Functions (Analytics)

Window functions perform calculations across rows related to the current row.

### Basic Window Functions

```sql
-- ROW_NUMBER: Unique number for each row
SELECT name, salary,
    ROW_NUMBER() OVER (ORDER BY salary DESC) as rank
FROM employees;

-- RANK: Same rank for ties, gaps after ties
SELECT name, salary,
    RANK() OVER (ORDER BY salary DESC) as rank
FROM employees;

-- DENSE_RANK: Same rank for ties, no gaps
SELECT name, salary,
    DENSE_RANK() OVER (ORDER BY salary DESC) as rank
FROM employees;

-- NTILE: Divide into N buckets
SELECT name, salary,
    NTILE(4) OVER (ORDER BY salary DESC) as quartile
FROM employees;
```

### PARTITION BY - Window Within Groups

```sql
-- Rank within each department
SELECT name, department, salary,
    RANK() OVER (PARTITION BY department ORDER BY salary DESC) as dept_rank
FROM employees;

-- Running total per department
SELECT name, department, salary,
    SUM(salary) OVER (PARTITION BY department ORDER BY hire_date) as running_total
FROM employees;
```

### Frame Specification

```sql
-- Moving average (last 3 rows)
SELECT name, salary,
    AVG(salary) OVER (
        ORDER BY hire_date
        ROWS BETWEEN 2 PRECEDING AND CURRENT ROW
    ) as moving_avg
FROM employees;

-- Cumulative sum
SELECT name, salary,
    SUM(salary) OVER (
        ORDER BY hire_date
        ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW
    ) as cumulative_sum
FROM employees;
```

### LAG and LEAD

```sql
-- Compare with previous row
SELECT name, salary,
    LAG(salary) OVER (ORDER BY hire_date) as prev_salary,
    salary - LAG(salary) OVER (ORDER BY hire_date) as salary_diff
FROM employees;

-- Compare with next row
SELECT name, salary,
    LEAD(salary) OVER (ORDER BY hire_date) as next_salary
FROM employees;

-- LAG/LEAD with offset and default
SELECT name, salary,
    LAG(salary, 2, 0) OVER (ORDER BY hire_date) as salary_2_rows_ago
FROM employees;
```

### FIRST_VALUE and LAST_VALUE

```sql
SELECT name, department, salary,
    FIRST_VALUE(name) OVER (PARTITION BY department ORDER BY salary DESC) as highest_earner,
    LAST_VALUE(name) OVER (
        PARTITION BY department ORDER BY salary DESC
        ROWS BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING
    ) as lowest_earner
FROM employees;
```

---

## Views - Virtual Tables

### Creating Views

```sql
-- Create a view
CREATE VIEW high_earners AS
SELECT name, salary, department
FROM employees
WHERE salary > 80000;

-- Use like a regular table
SELECT * FROM high_earners;

-- Create or replace
CREATE OR REPLACE VIEW high_earners AS
SELECT name, salary, department
FROM employees
WHERE salary > 90000;

-- Drop view
DROP VIEW IF EXISTS high_earners;
```

### Materialized Views (PostgreSQL-Specific!)

Stores the query result physically (faster reads, needs refresh):

```sql
-- Create materialized view
CREATE MATERIALIZED VIEW sales_summary AS
SELECT
    date_trunc('month', order_date) as month,
    SUM(amount) as total_sales,
    COUNT(*) as order_count
FROM orders
GROUP BY date_trunc('month', order_date);

-- Refresh the data
REFRESH MATERIALIZED VIEW sales_summary;

-- Refresh concurrently (requires unique index)
CREATE UNIQUE INDEX ON sales_summary (month);
REFRESH MATERIALIZED VIEW CONCURRENTLY sales_summary;

-- Drop materialized view
DROP MATERIALIZED VIEW sales_summary;
```

---

## Indexes

### Creating Indexes

```sql
-- Basic index
CREATE INDEX idx_employee_name ON employees(last_name);

-- Unique index
CREATE UNIQUE INDEX idx_employee_email ON employees(email);

-- Composite index
CREATE INDEX idx_emp_dept_salary ON employees(department, salary);

-- Partial index (PostgreSQL-specific!)
CREATE INDEX idx_active_employees ON employees(last_name)
WHERE is_active = TRUE;

-- Expression index (PostgreSQL-specific!)
CREATE INDEX idx_lower_email ON employees(LOWER(email));

-- GIN index for arrays and JSONB
CREATE INDEX idx_tags ON posts USING GIN(tags);
CREATE INDEX idx_data ON events USING GIN(data);

-- GiST index for geometric and full-text search
CREATE INDEX idx_location ON places USING GIST(location);

-- Drop index
DROP INDEX IF EXISTS idx_employee_name;
```

### Index Types

| Type    | Use Case                                      |
| ------- | --------------------------------------------- |
| B-tree  | Default, equality and range queries           |
| Hash    | Equality comparisons only                     |
| GIN     | Arrays, JSONB, full-text search               |
| GiST    | Geometric data, full-text, range types        |
| SP-GiST | Space-partitioned data                        |
| BRIN    | Large tables with naturally ordered data      |

---

## Query Execution Order

```
Written Order:          Execution Order:
SELECT                  1. FROM       -> Which tables
FROM                    2. JOIN       -> Combine tables
JOIN                    3. WHERE      -> Filter rows
WHERE                   4. GROUP BY   -> Create groups
GROUP BY                5. HAVING     -> Filter groups
HAVING                  6. SELECT     -> Choose columns
ORDER BY                7. DISTINCT   -> Remove duplicates
LIMIT                   8. ORDER BY   -> Sort results
                        9. LIMIT      -> Restrict output
```

---

## Common Interview Query Patterns

### Pattern 1: Find Nth Highest Salary

```sql
-- Using LIMIT OFFSET
SELECT DISTINCT salary
FROM employees
ORDER BY salary DESC
LIMIT 1 OFFSET 1;   -- 2nd highest

-- Using window function
SELECT salary FROM (
    SELECT salary, DENSE_RANK() OVER (ORDER BY salary DESC) as rank
    FROM employees
) ranked
WHERE rank = 2;
```

### Pattern 2: Find Duplicates

```sql
SELECT email, COUNT(*) as count
FROM employees
GROUP BY email
HAVING COUNT(*) > 1;
```

### Pattern 3: Employee-Manager Hierarchy

```sql
SELECT
    e.name as employee,
    m.name as manager
FROM employees e
LEFT JOIN employees m ON e.manager_id = m.id;
```

### Pattern 4: Employees Without Department

```sql
SELECT e.name
FROM employees e
LEFT JOIN departments d ON e.dept_id = d.id
WHERE d.id IS NULL;
```

### Pattern 5: Running Total

```sql
SELECT
    order_date,
    amount,
    SUM(amount) OVER (ORDER BY order_date) as running_total
FROM orders;
```

### Pattern 6: Top N per Group

```sql
-- Using window function
SELECT * FROM (
    SELECT name, department, salary,
        ROW_NUMBER() OVER (PARTITION BY department ORDER BY salary DESC) as rn
    FROM employees
) ranked
WHERE rn <= 3;

-- Using LATERAL (PostgreSQL-specific)
SELECT d.dept_name, e.name, e.salary
FROM departments d
LEFT JOIN LATERAL (
    SELECT name, salary
    FROM employees
    WHERE dept_id = d.id
    ORDER BY salary DESC
    LIMIT 3
) e ON TRUE;
```

---

## PostgreSQL vs MySQL Quick Reference

| Feature                    | PostgreSQL                          | MySQL                              |
| -------------------------- | ----------------------------------- | ---------------------------------- |
| Auto-increment             | `SERIAL` or `GENERATED AS IDENTITY` | `AUTO_INCREMENT`                   |
| String concat              | `\|\|` or `CONCAT()`                  | `CONCAT()` only                    |
| Case-insensitive LIKE      | `ILIKE`                             | `LIKE` (case-insensitive by default) |
| Type casting               | `::type` or `CAST()`                | `CAST()` only                      |
| Boolean type               | `TRUE/FALSE`                        | `1/0` (TINYINT)                    |
| FULL OUTER JOIN            | Native support                      | UNION workaround                   |
| INTERSECT                  | Native support                      | JOIN workaround                    |
| EXCEPT                     | Native support                      | LEFT JOIN workaround               |
| UPSERT                     | `ON CONFLICT`                       | `ON DUPLICATE KEY` or `REPLACE`    |
| RETURNING clause           | Yes                                 | No (use LAST_INSERT_ID())          |
| Array type                 | Yes                                 | No (use JSON)                      |
| JSONB                      | Yes (binary, indexable)             | JSON only                          |
| Materialized Views         | Yes                                 | No                                 |
| Schemas                    | Yes                                 | Synonymous with database           |
| TRUNCATE rollback          | Yes                                 | No                                 |
| Check constraints          | Always enforced                     | Enforced (MySQL 8.0.16+)           |
| Current timestamp          | `NOW()` or `CURRENT_TIMESTAMP`      | `NOW()` or `CURRENT_TIMESTAMP`     |
| Limit syntax               | `LIMIT n OFFSET m`                  | `LIMIT m, n` or `LIMIT n OFFSET m` |

---

## psql Command Reference

| Command                  | Description                              |
| ------------------------ | ---------------------------------------- |
| `\l`                     | List all databases                       |
| `\c dbname`              | Connect to database                      |
| `\dt`                    | List tables in current schema            |
| `\dt+`                   | List tables with size info               |
| `\d tablename`           | Describe table structure                 |
| `\d+ tablename`          | Detailed table description               |
| `\di`                    | List indexes                             |
| `\dv`                    | List views                               |
| `\dm`                    | List materialized views                  |
| `\df`                    | List functions                           |
| `\dn`                    | List schemas                             |
| `\du`                    | List users/roles                         |
| `\timing`                | Toggle query timing                      |
| `\x`                     | Toggle expanded display                  |
| `\q`                     | Quit psql                                |
| `\i filename`            | Execute SQL from file                    |
| `\o filename`            | Send output to file                      |
| `\copy`                  | Client-side COPY                         |

---

## Summary Cheat Sheet

```
+------------------------------------------------------------+
|                PostgreSQL QUICK REFERENCE                   |
+------------------------------------------------------------+
| CREATE TABLE name (id SERIAL PRIMARY KEY, ...);            |
| DROP TABLE name;                                            |
| ALTER TABLE ADD/ALTER/DROP COLUMN;                          |
+------------------------------------------------------------+
| SELECT cols FROM table;                                     |
| WHERE condition;                                            |
| ORDER BY col [DESC] [NULLS FIRST/LAST];                    |
| GROUP BY col;                                               |
| HAVING condition;                                           |
| LIMIT n OFFSET m;                                           |
+------------------------------------------------------------+
| INSERT INTO table VALUES() RETURNING *;                     |
| UPDATE table SET col = val RETURNING *;                     |
| DELETE FROM table RETURNING *;                              |
| INSERT ... ON CONFLICT DO UPDATE/NOTHING;                   |
+------------------------------------------------------------+
| INNER JOIN  -- Matching rows only                           |
| LEFT JOIN   -- All left + matching right                    |
| RIGHT JOIN  -- All right + matching left                    |
| FULL OUTER JOIN  -- All from both (native!)                 |
| LATERAL JOIN  -- Correlated subquery in FROM                |
+------------------------------------------------------------+
| UNION / UNION ALL  -- Combine rows                          |
| INTERSECT  -- Common rows (native!)                         |
| EXCEPT     -- Difference (native!)                          |
+------------------------------------------------------------+
| WITH cte AS (...) SELECT ...;  -- CTE                       |
| WITH RECURSIVE ...;  -- Recursive CTE                       |
+------------------------------------------------------------+
| ROW_NUMBER() OVER (...)  -- Window functions                |
| RANK(), DENSE_RANK(), NTILE()                               |
| LAG(), LEAD(), FIRST_VALUE(), LAST_VALUE()                  |
| PARTITION BY, ORDER BY, ROWS BETWEEN                        |
+------------------------------------------------------------+
| SERIAL / BIGSERIAL / GENERATED AS IDENTITY                  |
| TEXT, JSONB, ARRAY, UUID, BOOLEAN                           |
| TIMESTAMP WITH TIME ZONE, INTERVAL                          |
+------------------------------------------------------------+
| PRIMARY KEY, FOREIGN KEY, UNIQUE, CHECK, EXCLUDE            |
| ON DELETE CASCADE/SET NULL/RESTRICT                         |
+------------------------------------------------------------+
```

---

**Good luck with your interviews!**
