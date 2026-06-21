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

CRUD represents the four fundamental operations that form the basis of all database interactions. Every application that works with persistent data uses these operations.

| Operation  | SQL Command | Purpose                    | Analogy                    |
| ---------- | ----------- | -------------------------- | -------------------------- |
| **C**reate | `INSERT`    | Add new records to a table | Adding a new row in Excel  |
| **R**ead   | `SELECT`    | Retrieve data from tables  | Looking up data            |
| **U**pdate | `UPDATE`    | Modify existing records    | Editing a cell in Excel    |
| **D**elete | `DELETE`    | Remove records from tables | Deleting a row in Excel    |

### CRUD Visual Example

```sql
-- Starting with an empty table
CREATE TABLE products (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100),
    price NUMERIC(10,2)
);

-- Table: products (empty)
-- +----+------+-------+
-- | id | name | price |
-- +----+------+-------+
-- (0 rows)

-- CREATE: Insert a new product
INSERT INTO products (name, price) VALUES ('Laptop', 999.99);

-- Table: products
-- +----+--------+--------+
-- | id | name   | price  |
-- +----+--------+--------+
-- | 1  | Laptop | 999.99 |
-- +----+--------+--------+

-- READ: Query the data
SELECT * FROM products WHERE price > 500;

-- Result:
-- +----+--------+--------+
-- | id | name   | price  |
-- +----+--------+--------+
-- | 1  | Laptop | 999.99 |
-- +----+--------+--------+

-- UPDATE: Modify existing data
UPDATE products SET price = 899.99 WHERE id = 1;

-- Table: products (after update)
-- +----+--------+--------+
-- | id | name   | price  |
-- +----+--------+--------+
-- | 1  | Laptop | 899.99 |  <-- Price changed!
-- +----+--------+--------+

-- DELETE: Remove data
DELETE FROM products WHERE id = 1;

-- Table: products (after delete)
-- +----+------+-------+
-- | id | name | price |
-- +----+------+-------+
-- (0 rows)
```

> **Interview Insight:** Understanding CRUD is fundamental. Be prepared to explain how each operation maps to HTTP methods in REST APIs: CREATE→POST, READ→GET, UPDATE→PUT/PATCH, DELETE→DELETE.

---

## SQL Command Categories

SQL commands are divided into five categories based on their purpose. Understanding these categories helps you reason about what each command does and its impact on the database.

```
+------------------------------------------------------------------+
|                    SQL COMMAND CATEGORIES                         |
+------------------------------------------------------------------+
|                                                                   |
|   DDL (Structure)    DQL (Read)     DML (Modify)                 |
|   ┌──────────────┐   ┌──────────┐   ┌──────────────┐             |
|   │ CREATE       │   │ SELECT   │   │ INSERT       │             |
|   │ ALTER        │   │          │   │ UPDATE       │             |
|   │ DROP         │   └──────────┘   │ DELETE       │             |
|   │ TRUNCATE     │                  └──────────────┘             |
|   └──────────────┘                                               |
|                                                                   |
|   DCL (Permissions)           TCL (Transactions)                 |
|   ┌──────────────┐            ┌──────────────────┐               |
|   │ GRANT        │            │ BEGIN/START      │               |
|   │ REVOKE       │            │ COMMIT           │               |
|   └──────────────┘            │ ROLLBACK         │               |
|                               │ SAVEPOINT        │               |
|                               └──────────────────┘               |
+------------------------------------------------------------------+
```

### 1. DDL (Data Definition Language) - Defines Structure

DDL commands define and modify the **structure** of database objects. These commands are **auto-committed** (changes are permanent immediately).

| Command    | Purpose                              | PostgreSQL Example                          |
| ---------- | ------------------------------------ | ------------------------------------------- |
| `CREATE`   | Create tables, databases, views      | `CREATE TABLE users (id SERIAL PRIMARY KEY);` |
| `ALTER`    | Modify table structure               | `ALTER TABLE users ADD COLUMN email VARCHAR(255);` |
| `DROP`     | Delete tables, databases entirely    | `DROP TABLE users;`                         |
| `TRUNCATE` | Remove ALL rows (faster than DELETE) | `TRUNCATE TABLE users RESTART IDENTITY;`    |

**Visual Example - CREATE and ALTER:**

```sql
-- Step 1: Create initial table
CREATE TABLE employees (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100)
);

-- Table structure after CREATE:
--           Table "public.employees"
-- +--------+-----------------------+-----------+
-- | Column | Type                  | Modifiers |
-- +--------+-----------------------+-----------+
-- | id     | integer               | NOT NULL  |
-- | name   | character varying(100)|           |
-- +--------+-----------------------+-----------+
-- Indexes: "employees_pkey" PRIMARY KEY (id)

-- Step 2: Add a column
ALTER TABLE employees ADD COLUMN salary NUMERIC(10,2);

-- Table structure after ALTER:
--           Table "public.employees"
-- +--------+-----------------------+-----------+
-- | Column | Type                  | Modifiers |
-- +--------+-----------------------+-----------+
-- | id     | integer               | NOT NULL  |
-- | name   | character varying(100)|           |
-- | salary | numeric(10,2)         |           |  <-- New column!
-- +--------+-----------------------+-----------+
```

### 2. DQL/DRL (Data Query Language) - Reads Data

DQL is used to **retrieve** data from the database. It consists of the SELECT statement and its clauses.

```sql
SELECT column1, column2   -- Columns to retrieve (use * for all)
FROM table_name           -- Source table
WHERE condition;          -- Filter criteria (optional)
```

**Visual Example - SELECT with different clauses:**

```sql
-- Sample table: employees
-- +----+---------+--------+------------+
-- | id | name    | salary | department |
-- +----+---------+--------+------------+
-- | 1  | Alice   | 75000  | Engineering|
-- | 2  | Bob     | 55000  | Sales      |
-- | 3  | Charlie | 80000  | Engineering|
-- | 4  | Diana   | 60000  | Marketing  |
-- +----+---------+--------+------------+

-- Query 1: Select all
SELECT * FROM employees;
-- Returns all 4 rows with all columns

-- Query 2: Select specific columns with filter
SELECT name, salary FROM employees WHERE department = 'Engineering';

-- Result:
-- +---------+--------+
-- | name    | salary |
-- +---------+--------+
-- | Alice   | 75000  |
-- | Charlie | 80000  |
-- +---------+--------+
-- (2 rows)
```

### 3. DML (Data Manipulation Language) - Modifies Data

DML commands **modify the data** inside tables (not the structure). These operations can be rolled back if within a transaction.

| Command  | Purpose              |
| -------- | -------------------- |
| `INSERT` | Add new rows         |
| `UPDATE` | Change existing rows |
| `DELETE` | Remove specific rows |

**Visual Example - DML Operations:**

```sql
-- Starting table: products
-- +----+--------+-------+
-- | id | name   | stock |
-- +----+--------+-------+
-- | 1  | Apple  | 100   |
-- | 2  | Banana | 50    |
-- +----+--------+-------+

-- INSERT: Add new row
INSERT INTO products (name, stock) VALUES ('Orange', 75);

-- After INSERT:
-- +----+--------+-------+
-- | id | name   | stock |
-- +----+--------+-------+
-- | 1  | Apple  | 100   |
-- | 2  | Banana | 50    |
-- | 3  | Orange | 75    |  <-- New row added
-- +----+--------+-------+

-- UPDATE: Modify existing row
UPDATE products SET stock = 120 WHERE name = 'Apple';

-- After UPDATE:
-- +----+--------+-------+
-- | id | name   | stock |
-- +----+--------+-------+
-- | 1  | Apple  | 120   |  <-- Stock updated from 100 to 120
-- | 2  | Banana | 50    |
-- | 3  | Orange | 75    |
-- +----+--------+-------+

-- DELETE: Remove specific row
DELETE FROM products WHERE name = 'Banana';

-- After DELETE:
-- +----+--------+-------+
-- | id | name   | stock |
-- +----+--------+-------+
-- | 1  | Apple  | 120   |
-- | 3  | Orange | 75    |  <-- Banana row removed, id 2 is gone
-- +----+--------+-------+
```

### 4. DCL (Data Control Language) - Manages Permissions

DCL commands control **access permissions** to database objects. They define who can do what in the database.

```sql
-- Grant specific permissions to a user/role
GRANT SELECT, INSERT ON employees TO user_john;

-- Visual representation of permissions:
-- +----------+--------+--------+--------+--------+
-- | User     | SELECT | INSERT | UPDATE | DELETE |
-- +----------+--------+--------+--------+--------+
-- | user_john| ✓      | ✓      | ✗      | ✗      |
-- +----------+--------+--------+--------+--------+

-- Remove permissions
REVOKE INSERT ON employees FROM user_john;

-- After REVOKE:
-- +----------+--------+--------+--------+--------+
-- | User     | SELECT | INSERT | UPDATE | DELETE |
-- +----------+--------+--------+--------+--------+
-- | user_john| ✓      | ✗      | ✗      | ✗      |
-- +----------+--------+--------+--------+--------+

-- Grant all privileges
GRANT ALL PRIVILEGES ON employees TO admin_role;

-- Grant with option to re-grant
GRANT SELECT ON employees TO user_john WITH GRANT OPTION;
-- user_john can now grant SELECT to other users
```

### 5. TCL (Transaction Control Language) - Manages Transactions

TCL commands manage **transactions** - groups of operations that should succeed or fail together. This ensures data integrity.

**Why Transactions Matter:**

```
Without Transaction:              With Transaction:
┌─────────────────────┐          ┌─────────────────────┐
│ Debit Account A     │          │ BEGIN               │
│      ↓              │          │   Debit Account A   │
│ [SYSTEM CRASH!]     │          │   [SYSTEM CRASH!]   │
│      ↓              │          │   (auto ROLLBACK)   │
│ Credit Account B    │          │ END                 │
│   (never happens)   │          │                     │
└─────────────────────┘          └─────────────────────┘
Result: Money lost!              Result: No change, safe!
```

```sql
BEGIN;  -- Start a transaction (or START TRANSACTION)
    UPDATE accounts SET balance = balance - 100 WHERE id = 1;
    UPDATE accounts SET balance = balance + 100 WHERE id = 2;
COMMIT;  -- Make ALL changes permanent
-- OR use ROLLBACK; to undo ALL changes
```

**Visual Example - Transaction with ROLLBACK:**

```sql
-- Starting table: accounts
-- +----+-------+---------+
-- | id | name  | balance |
-- +----+-------+---------+
-- | 1  | Alice | 1000    |
-- | 2  | Bob   | 500     |
-- +----+-------+---------+

BEGIN;
    UPDATE accounts SET balance = balance - 200 WHERE id = 1;

    -- Table state IN TRANSACTION:
    -- +----+-------+---------+
    -- | id | name  | balance |
    -- +----+-------+---------+
    -- | 1  | Alice | 800     |  <-- Changed (uncommitted)
    -- | 2  | Bob   | 500     |
    -- +----+-------+---------+

    -- Oops, wrong amount! Let's undo
ROLLBACK;

-- Table state AFTER ROLLBACK (unchanged):
-- +----+-------+---------+
-- | id | name  | balance |
-- +----+-------+---------+
-- | 1  | Alice | 1000    |  <-- Back to original!
-- | 2  | Bob   | 500     |
-- +----+-------+---------+
```

**PostgreSQL-Specific Transaction Features:**

```sql
-- Savepoints for partial rollback
BEGIN;
    INSERT INTO orders (product_id, quantity) VALUES (1, 10);

    -- After first INSERT:
    -- orders: [(1, product_id=1, quantity=10)]

    SAVEPOINT my_savepoint;

    INSERT INTO orders (product_id, quantity) VALUES (2, 5);

    -- After second INSERT:
    -- orders: [(1, product_id=1, quantity=10), (2, product_id=2, quantity=5)]

    -- Oops, wrong data for second insert
    ROLLBACK TO SAVEPOINT my_savepoint;

    -- After ROLLBACK TO SAVEPOINT:
    -- orders: [(1, product_id=1, quantity=10)]  <-- Second insert undone!

    INSERT INTO orders (product_id, quantity) VALUES (2, 15);  -- Correct data
COMMIT;

-- Final result:
-- orders: [(1, product_id=1, quantity=10), (2, product_id=2, quantity=15)]

-- Transaction isolation levels
BEGIN TRANSACTION ISOLATION LEVEL SERIALIZABLE;
    -- Your queries here - strictest isolation
COMMIT;
```

**Transaction Isolation Levels:**

| Level            | Dirty Read | Non-repeatable Read | Phantom Read | Use Case                    |
| ---------------- | ---------- | ------------------- | ------------ | --------------------------- |
| READ UNCOMMITTED | Possible   | Possible            | Possible     | Rarely used in PostgreSQL   |
| READ COMMITTED   | No         | Possible            | Possible     | PostgreSQL default          |
| REPEATABLE READ  | No         | No                  | Possible     | Financial reports           |
| SERIALIZABLE     | No         | No                  | No           | Critical operations         |

---

## PostgreSQL Data Types

PostgreSQL has a rich type system that goes beyond standard SQL. Choosing the right data type is crucial for:
- **Performance**: Smaller types = faster queries
- **Storage**: Appropriate types save disk space
- **Data Integrity**: Types enforce valid data

```
+------------------------------------------------------------------+
|                  PostgreSQL TYPE CATEGORIES                       |
+------------------------------------------------------------------+
|                                                                   |
|   Numeric          String           Date/Time                     |
|   ┌──────────┐     ┌──────────┐     ┌─────────────────┐          |
|   │ INTEGER  │     │ CHAR(n)  │     │ DATE            │          |
|   │ BIGINT   │     │ VARCHAR  │     │ TIME            │          |
|   │ NUMERIC  │     │ TEXT     │     │ TIMESTAMP       │          |
|   │ REAL     │     └──────────┘     │ INTERVAL        │          |
|   └──────────┘                      └─────────────────┘          |
|                                                                   |
|   Special (PostgreSQL-Specific)                                   |
|   ┌─────────────────────────────────────────────┐                |
|   │ BOOLEAN │ UUID │ JSON/JSONB │ ARRAY │ ENUM  │                |
|   └─────────────────────────────────────────────┘                |
+------------------------------------------------------------------+
```

### String Types

| Type                  | Size                         | When to Use                                            |
| --------------------- | ---------------------------- | ------------------------------------------------------ |
| `CHAR(n)`             | Fixed n characters           | When all values have same length (e.g., country codes) |
| `VARCHAR(n)`          | Variable, up to n characters | **Most common choice** - names, emails, etc.           |
| `TEXT`                | Unlimited variable length    | Long text content (no performance penalty in PostgreSQL) |

**Visual Comparison of String Types:**

```sql
-- CHAR(5): Fixed length, pads with spaces
INSERT INTO test_char VALUES ('AB');
-- Stored as: 'AB   ' (padded to 5 characters)

-- Visual representation:
-- +---+---+---+---+---+
-- | A | B |   |   |   |  <-- Always 5 characters
-- +---+---+---+---+---+

-- VARCHAR(5): Variable length, no padding
INSERT INTO test_varchar VALUES ('AB');
-- Stored as: 'AB' (exactly what you inserted)

-- Visual representation:
-- +---+---+
-- | A | B |  <-- Only stores actual characters
-- +---+---+

-- TEXT: No length limit
INSERT INTO test_text VALUES ('This can be as long as you want...');
```

> **PostgreSQL Note:** Unlike MySQL, `TEXT` has no performance penalty vs `VARCHAR` in PostgreSQL. Use `TEXT` freely for variable-length strings.

### Numeric Types

Choosing the right numeric type depends on your precision needs and value range.

| Type              | Range                      | Use Case                                        |
| ----------------- | -------------------------- | ----------------------------------------------- |
| `SMALLINT`        | -32,768 to 32,767          | Small counts                                    |
| `INTEGER` / `INT` | -2.1 billion to 2.1 billion | Most numeric data                               |
| `BIGINT`          | Huge numbers               | IDs for large systems                           |
| `REAL`            | 6 decimal digits precision | Approximate decimals                            |
| `DOUBLE PRECISION`| 15 decimal digits          | Scientific calculations                         |
| `NUMERIC(p,s)`    | Exact precision            | **Money** - always use this for currency!       |
| `DECIMAL(p,s)`    | Same as NUMERIC            | Alias for NUMERIC                               |

**Understanding NUMERIC(precision, scale):**

```sql
-- NUMERIC(10, 2) means:
-- - Total 10 digits
-- - 2 digits after decimal point
-- - So max value: 99999999.99

-- Visual:
-- NUMERIC(10, 2)
-- +--+--+--+--+--+--+--+--+--+--+
-- | 9| 9| 9| 9| 9| 9| 9| 9| .| 9| 9|
-- +--+--+--+--+--+--+--+--+--+--+
--  |<-- 8 digits -->|   |<-2->|
--     (before .)        (after)

-- Example:
CREATE TABLE prices (
    amount NUMERIC(10, 2)
);

INSERT INTO prices VALUES (1234.56);    -- OK: 1234.56
INSERT INTO prices VALUES (12345678.99); -- OK: 12345678.99
INSERT INTO prices VALUES (123456789.00); -- ERROR! Too many digits
```

**Why NUMERIC for Money (Not FLOAT)?**

```sql
-- FLOAT has precision issues:
SELECT 0.1 + 0.2;  -- Returns: 0.30000000000000004 (unexpected!)

-- NUMERIC is exact:
SELECT 0.1::NUMERIC + 0.2::NUMERIC;  -- Returns: 0.3 (correct!)

-- For financial calculations, always use NUMERIC!
```

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

JSONB stores JSON in a binary format that's faster to query and can be indexed. This is one of PostgreSQL's most powerful features.

```sql
-- Create table with JSONB
CREATE TABLE events (
    id SERIAL PRIMARY KEY,
    data JSONB
);

-- Insert JSON data
INSERT INTO events (data) VALUES ('{"name": "Click", "page": "/home", "user_id": 123}');
INSERT INTO events (data) VALUES ('{"name": "View", "page": "/products", "user_id": 456}');

-- Table contents:
-- +----+------------------------------------------------+
-- | id | data                                           |
-- +----+------------------------------------------------+
-- | 1  | {"name": "Click", "page": "/home", "user_id": 123} |
-- | 2  | {"name": "View", "page": "/products", "user_id": 456} |
-- +----+------------------------------------------------+

-- Query JSON data with different operators:

-- ->> : Get as TEXT
SELECT data->>'name' AS event_name FROM events;
-- Result:
-- +------------+
-- | event_name |
-- +------------+
-- | Click      |
-- | View       |
-- +------------+

-- -> : Get as JSON (preserves type)
SELECT data->'user_id' AS user_id FROM events;
-- Result:
-- +---------+
-- | user_id |
-- +---------+
-- | 123     |  <-- This is JSON number, not text
-- | 456     |
-- +---------+

-- Filter using JSON
SELECT * FROM events WHERE data->>'name' = 'Click';
-- Result:
-- +----+------------------------------------------------+
-- | id | data                                           |
-- +----+------------------------------------------------+
-- | 1  | {"name": "Click", "page": "/home", "user_id": 123} |
-- +----+------------------------------------------------+

-- @> : Contains operator (very fast with GIN index!)
SELECT * FROM events WHERE data @> '{"name": "Click"}';
-- Same result as above, but can use index

-- JSONB path query (PostgreSQL 12+)
SELECT jsonb_path_query(data, '$.name') FROM events;
```

**JSON vs JSONB:**

| Feature      | JSON          | JSONB                |
| ------------ | ------------- | -------------------- |
| Storage      | Text          | Binary               |
| Query Speed  | Slower        | **Faster**           |
| Indexable    | No            | **Yes (GIN index)**  |
| Preserves    | Key order     | Sorts keys           |
| Duplicates   | Keeps all     | Last value wins      |

> **Best Practice:** Always use JSONB unless you specifically need JSON text preservation.

**Array Examples:**

PostgreSQL supports arrays of any data type. Arrays are 1-indexed (not 0-indexed like most programming languages).

```sql
-- Create table with array column
CREATE TABLE students (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100),
    scores INTEGER[]
);

-- Insert array data (two syntax options)
INSERT INTO students (name, scores) VALUES ('Alice', ARRAY[85, 90, 92]);
INSERT INTO students (name, scores) VALUES ('Bob', '{78, 82, 88}');
INSERT INTO students (name, scores) VALUES ('Charlie', ARRAY[95, 88, 91, 87]);

-- Table contents:
-- +----+---------+----------------+
-- | id | name    | scores         |
-- +----+---------+----------------+
-- | 1  | Alice   | {85,90,92}     |
-- | 2  | Bob     | {78,82,88}     |
-- | 3  | Charlie | {95,88,91,87}  |
-- +----+---------+----------------+

-- Access by index (1-based!)
SELECT name, scores[1] AS first_score FROM students;
-- Result:
-- +---------+-------------+
-- | name    | first_score |
-- +---------+-------------+
-- | Alice   | 85          |
-- | Bob     | 78          |
-- | Charlie | 95          |
-- +---------+-------------+

-- ANY: Check if array contains value
SELECT * FROM students WHERE 90 = ANY(scores);
-- Result:
-- +----+-------+------------+
-- | id | name  | scores     |
-- +----+-------+------------+
-- | 1  | Alice | {85,90,92} |
-- +----+-------+------------+

-- @>: Array contains another array
SELECT * FROM students WHERE scores @> ARRAY[85, 92];
-- Result: Alice (her scores contain both 85 AND 92)

-- Array length
SELECT name, array_length(scores, 1) AS num_scores FROM students;
-- Result:
-- +---------+------------+
-- | name    | num_scores |
-- +---------+------------+
-- | Alice   | 3          |
-- | Bob     | 3          |
-- | Charlie | 4          |
-- +---------+------------+

-- Unnest: Expand array to rows
SELECT name, unnest(scores) AS score FROM students WHERE name = 'Alice';
-- Result:
-- +-------+-------+
-- | name  | score |
-- +-------+-------+
-- | Alice | 85    |
-- | Alice | 90    |
-- | Alice | 92    |
-- +-------+-------+

-- Array aggregate: Combine values into array
SELECT department, ARRAY_AGG(name) AS employees
FROM employees
GROUP BY department;
-- Result:
-- +-------------+---------------------+
-- | department  | employees           |
-- +-------------+---------------------+
-- | Engineering | {Alice,Bob,Charlie} |
-- | Sales       | {Diana,Eve}         |
-- +-------------+---------------------+
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

Constraints are rules enforced by the database to ensure data validity and consistency. They prevent invalid data from being inserted and maintain relationships between tables.

```
+------------------------------------------------------------------+
|                      CONSTRAINT TYPES                             |
+------------------------------------------------------------------+
|                                                                   |
|  PRIMARY KEY  ─────>  Uniquely identifies each row               |
|                       (NOT NULL + UNIQUE combined)                |
|                                                                   |
|  FOREIGN KEY  ─────>  Links to another table's PRIMARY KEY       |
|                       (Maintains referential integrity)           |
|                                                                   |
|  UNIQUE       ─────>  No duplicate values allowed                |
|                       (NULL values are allowed and not checked)   |
|                                                                   |
|  NOT NULL     ─────>  Column cannot contain NULL                 |
|                                                                   |
|  CHECK        ─────>  Custom validation rule                     |
|                       (e.g., age >= 18, price > 0)                |
|                                                                   |
|  EXCLUDE      ─────>  Prevent overlapping values (PostgreSQL)    |
|                       (e.g., no overlapping date ranges)          |
+------------------------------------------------------------------+
```

### Primary Key

A PRIMARY KEY uniquely identifies each row. It's automatically NOT NULL and UNIQUE.

```sql
-- Method 1: Inline definition
CREATE TABLE customers (
    id SERIAL PRIMARY KEY,  -- SERIAL auto-increments
    name VARCHAR(100)
);

-- After inserting 3 customers:
-- +----+---------+
-- | id | name    |  <-- id is PRIMARY KEY: unique, never NULL
-- +----+---------+
-- | 1  | Alice   |
-- | 2  | Bob     |
-- | 3  | Charlie |
-- +----+---------+

-- Try to insert duplicate id:
INSERT INTO customers (id, name) VALUES (1, 'Diana');
-- ERROR: duplicate key value violates unique constraint "customers_pkey"

-- Try to insert NULL id:
INSERT INTO customers (id, name) VALUES (NULL, 'Diana');
-- ERROR: null value in column "id" violates not-null constraint

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
    PRIMARY KEY (order_id, product_id)  -- Both columns together must be unique
);

-- Valid composite key data:
-- +----------+------------+----------+
-- | order_id | product_id | quantity |
-- +----------+------------+----------+
-- | 1        | 100        | 2        |  -- OK
-- | 1        | 101        | 1        |  -- OK (same order_id, different product_id)
-- | 2        | 100        | 3        |  -- OK (different order_id, same product_id)
-- | 1        | 100        | 5        |  -- ERROR! (1,100) already exists
-- +----------+------------+----------+
```

### Foreign Key

A FOREIGN KEY creates a link between two tables, ensuring referential integrity.

```sql
-- Parent table
CREATE TABLE departments (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100)
);

-- Child table with foreign key
CREATE TABLE employees (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100),
    dept_id INTEGER,
    FOREIGN KEY (dept_id) REFERENCES departments(id)
);

-- Sample data:
-- departments:               employees:
-- +----+-------------+       +----+-------+---------+
-- | id | name        |       | id | name  | dept_id |
-- +----+-------------+       +----+-------+---------+
-- | 1  | Engineering |       | 1  | Alice | 1       | --> Points to Engineering
-- | 2  | Sales       |       | 2  | Bob   | 2       | --> Points to Sales
-- +----+-------------+       | 3  | Charlie| 1      | --> Points to Engineering
--                            +----+-------+---------+

-- Try to insert invalid foreign key:
INSERT INTO employees (name, dept_id) VALUES ('Diana', 99);
-- ERROR: insert or update on table "employees" violates foreign key constraint
-- Detail: Key (dept_id)=(99) is not present in table "departments"
```

**Cascade Options Explained:**

```sql
CREATE TABLE orders (
    order_id SERIAL PRIMARY KEY,
    customer_id INTEGER,
    FOREIGN KEY (customer_id) REFERENCES customers(id)
        ON DELETE CASCADE    -- What happens when parent is deleted?
        ON UPDATE CASCADE    -- What happens when parent id changes?
);
```

| Option         | ON DELETE                             | ON UPDATE                             |
| -------------- | ------------------------------------- | ------------------------------------- |
| `CASCADE`      | Delete child rows when parent deleted | Update FK when parent PK changes      |
| `SET NULL`     | Set FK to NULL when parent deleted    | Set FK to NULL when parent PK changes |
| `SET DEFAULT`  | Set FK to default value               | Set FK to default value               |
| `RESTRICT`     | Prevent deletion if children exist    | Prevent update if children exist      |
| `NO ACTION`    | Same as RESTRICT (default)            | Same as RESTRICT (default)            |

**Visual Example - ON DELETE CASCADE:**

```sql
-- Before delete:
-- customers:          orders:
-- +----+-------+      +----+-------------+
-- | id | name  |      | id | customer_id |
-- +----+-------+      +----+-------------+
-- | 1  | Alice |      | 1  | 1           | --> Alice's order
-- | 2  | Bob   |      | 2  | 1           | --> Alice's order
-- +----+-------+      | 3  | 2           | --> Bob's order
--                     +----+-------------+

DELETE FROM customers WHERE id = 1;

-- After delete (CASCADE):
-- customers:          orders:
-- +----+-------+      +----+-------------+
-- | id | name  |      | id | customer_id |
-- +----+-------+      +----+-------------+
-- | 2  | Bob   |      | 3  | 2           |  <-- Alice's orders deleted!
-- +----+-------+      +----+-------------+
```

### CHECK Constraint (Always Enforced in PostgreSQL)

CHECK constraints enforce custom validation rules.

```sql
CREATE TABLE employees (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100),
    age INTEGER,
    salary NUMERIC(10,2),

    -- Named constraint (recommended)
    CONSTRAINT chk_age CHECK (age >= 18 AND age <= 100),

    -- Unnamed constraint
    CHECK (salary > 0)
);

-- Valid insertions:
INSERT INTO employees (name, age, salary) VALUES ('Alice', 25, 50000);  -- OK
INSERT INTO employees (name, age, salary) VALUES ('Bob', 18, 35000);    -- OK

-- Invalid insertions:
INSERT INTO employees (name, age, salary) VALUES ('Teen', 16, 20000);
-- ERROR: new row violates check constraint "chk_age"

INSERT INTO employees (name, age, salary) VALUES ('Free', 25, 0);
-- ERROR: new row violates check constraint "employees_salary_check"

INSERT INTO employees (name, age, salary) VALUES ('Negative', 30, -100);
-- ERROR: new row violates check constraint "employees_salary_check"
```

### NOT NULL with DEFAULT

```sql
CREATE TABLE products (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,         -- Must provide
    price NUMERIC(10,2) NOT NULL DEFAULT 0.00,  -- Has default if not provided
    stock INTEGER NOT NULL DEFAULT 0
);

-- Insert without optional fields - uses defaults:
INSERT INTO products (name) VALUES ('Widget');

-- Result:
-- +----+--------+-------+-------+
-- | id | name   | price | stock |
-- +----+--------+-------+-------+
-- | 1  | Widget | 0.00  | 0     |  <-- Defaults applied
-- +----+--------+-------+-------+

-- Insert without required field - fails:
INSERT INTO products (price) VALUES (9.99);
-- ERROR: null value in column "name" violates not-null constraint
```

### EXCLUDE Constraint (PostgreSQL-Specific)

Prevents overlapping ranges or values - perfect for booking systems.

```sql
-- Requires btree_gist extension
CREATE EXTENSION IF NOT EXISTS btree_gist;

CREATE TABLE room_reservations (
    id SERIAL PRIMARY KEY,
    room_id INTEGER,
    during TSRANGE  -- Time range type
    EXCLUDE USING GIST (room_id WITH =, during WITH &&)
);
-- This prevents: same room with overlapping time ranges

-- Insert first reservation:
INSERT INTO room_reservations (room_id, during)
VALUES (1, '[2024-01-15 09:00, 2024-01-15 11:00)');
-- OK: Room 1 booked from 9am to 11am

-- Try overlapping reservation:
INSERT INTO room_reservations (room_id, during)
VALUES (1, '[2024-01-15 10:00, 2024-01-15 12:00)');
-- ERROR: conflicting key value violates exclusion constraint
-- (overlaps with existing 9am-11am booking)

-- Non-overlapping reservation works:
INSERT INTO room_reservations (room_id, during)
VALUES (1, '[2024-01-15 14:00, 2024-01-15 16:00)');
-- OK: Room 1 booked from 2pm to 4pm (no overlap)

-- Different room at same time works:
INSERT INTO room_reservations (room_id, during)
VALUES (2, '[2024-01-15 10:00, 2024-01-15 12:00)');
-- OK: Room 2 booked (different room, overlap allowed)
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

ORDER BY sorts the result set. Without ORDER BY, SQL doesn't guarantee any specific order.

```sql
-- Sample data:
-- +----+---------+--------+-------------+
-- | id | name    | salary | department  |
-- +----+---------+--------+-------------+
-- | 1  | Alice   | 80000  | Engineering |
-- | 2  | Bob     | 55000  | Sales       |
-- | 3  | Charlie | 70000  | Engineering |
-- | 4  | Diana   | 60000  | Marketing   |
-- | 5  | Eve     | NULL   | Sales       |
-- +----+---------+--------+-------------+

-- Sort ascending (default)
SELECT * FROM employees ORDER BY salary;
SELECT * FROM employees ORDER BY salary ASC;  -- Same as above

-- Result (ASC - smallest first):
-- +----+---------+--------+-------------+
-- | id | name    | salary | department  |
-- +----+---------+--------+-------------+
-- | 5  | Eve     | NULL   |             |  <-- NULLs first by default in ASC
-- | 2  | Bob     | 55000  | Sales       |
-- | 4  | Diana   | 60000  | Marketing   |
-- | 3  | Charlie | 70000  | Engineering |
-- | 1  | Alice   | 80000  | Engineering |
-- +----+---------+--------+-------------+

-- Sort descending
SELECT * FROM employees ORDER BY salary DESC;

-- Result (DESC - largest first):
-- +----+---------+--------+-------------+
-- | id | name    | salary | department  |
-- +----+---------+--------+-------------+
-- | 1  | Alice   | 80000  | Engineering |
-- | 3  | Charlie | 70000  | Engineering |
-- | 4  | Diana   | 60000  | Marketing   |
-- | 2  | Bob     | 55000  | Sales       |
-- | 5  | Eve     | NULL   |             |  <-- NULLs last in DESC
-- +----+---------+--------+-------------+

-- Multiple sort criteria
SELECT * FROM employees ORDER BY department ASC, salary DESC;

-- Result (first by department, then by salary within department):
-- +----+---------+--------+-------------+
-- | id | name    | salary | department  |
-- +----+---------+--------+-------------+
-- | 1  | Alice   | 80000  | Engineering |  <-- Engineering first
-- | 3  | Charlie | 70000  | Engineering |  <-- Higher salary first within Eng
-- | 4  | Diana   | 60000  | Marketing   |  <-- Marketing second
-- | 2  | Bob     | 55000  | Sales       |  <-- Sales third
-- | 5  | Eve     | NULL   | Sales       |
-- +----+---------+--------+-------------+

-- Sort by column position
SELECT first_name, salary FROM employees ORDER BY 2 DESC;  -- Sort by salary

-- NULLS FIRST / NULLS LAST (PostgreSQL-specific)
SELECT * FROM employees ORDER BY salary NULLS FIRST;
SELECT * FROM employees ORDER BY salary DESC NULLS LAST;
```

### GROUP BY - Aggregating Data

GROUP BY collapses multiple rows into summary rows based on column values.

```sql
-- Sample data:
-- +----+---------+--------+-------------+
-- | id | name    | salary | department  |
-- +----+---------+--------+-------------+
-- | 1  | Alice   | 80000  | Engineering |
-- | 2  | Bob     | 55000  | Sales       |
-- | 3  | Charlie | 70000  | Engineering |
-- | 4  | Diana   | 60000  | Sales       |
-- | 5  | Eve     | 75000  | Engineering |
-- +----+---------+--------+-------------+

-- Count employees per department
SELECT department, COUNT(*) as employee_count
FROM employees
GROUP BY department;

-- Step-by-step visualization:

-- Step 1: Group rows by department
-- Engineering group: [Alice, Charlie, Eve]
-- Sales group:       [Bob, Diana]

-- Step 2: Apply aggregate function to each group
-- Engineering: COUNT(*) = 3
-- Sales:       COUNT(*) = 2

-- Result:
-- +-------------+----------------+
-- | department  | employee_count |
-- +-------------+----------------+
-- | Engineering | 3              |
-- | Sales       | 2              |
-- +-------------+----------------+
```

**Aggregate Functions:**

| Function        | Purpose                          | NULL handling           |
| --------------- | -------------------------------- | ----------------------- |
| `COUNT(*)`      | Number of rows                   | Counts all rows         |
| `COUNT(column)` | Number of non-NULL values        | Ignores NULLs           |
| `SUM()`         | Total of numeric column          | Ignores NULLs           |
| `AVG()`         | Average value                    | Ignores NULLs           |
| `MIN()`         | Smallest value                   | Ignores NULLs           |
| `MAX()`         | Largest value                    | Ignores NULLs           |
| `STRING_AGG()`  | Concatenate strings (PostgreSQL) | Ignores NULLs           |
| `ARRAY_AGG()`   | Aggregate into array (PostgreSQL)| Includes NULLs          |

```sql
-- Multiple aggregate functions
SELECT
    department,
    COUNT(*) as headcount,
    SUM(salary) as total_payroll,
    ROUND(AVG(salary), 2) as avg_salary,
    MIN(salary) as lowest_salary,
    MAX(salary) as highest_salary,
    STRING_AGG(name, ', ' ORDER BY name) as employee_names,
    ARRAY_AGG(name ORDER BY salary DESC) as employee_array
FROM employees
GROUP BY department;

-- Result:
-- +-------------+-----------+---------------+------------+---------------+----------------+-------------------+----------------------------+
-- | department  | headcount | total_payroll | avg_salary | lowest_salary | highest_salary | employee_names    | employee_array             |
-- +-------------+-----------+---------------+------------+---------------+----------------+-------------------+----------------------------+
-- | Engineering | 3         | 225000        | 75000.00   | 70000         | 80000          | Alice, Charlie, Eve| {Alice,Eve,Charlie}        |
-- | Sales       | 2         | 115000        | 57500.00   | 55000         | 60000          | Bob, Diana        | {Diana,Bob}                |
-- +-------------+-----------+---------------+------------+---------------+----------------+-------------------+----------------------------+
```

### HAVING - Filtering Groups

HAVING filters groups **after** aggregation (unlike WHERE which filters rows before).

```sql
-- Departments with more than 2 employees
SELECT department, COUNT(*) as count
FROM employees
GROUP BY department
HAVING COUNT(*) > 2;

-- Step-by-step:
-- Step 1: GROUP BY creates groups
--         Engineering: 3 employees
--         Sales: 2 employees
-- Step 2: HAVING filters groups
--         Engineering: 3 > 2? YES → Keep
--         Sales: 2 > 2? NO → Remove

-- Result:
-- +-------------+-------+
-- | department  | count |
-- +-------------+-------+
-- | Engineering | 3     |
-- +-------------+-------+

-- Departments with average salary above 60000
SELECT department, ROUND(AVG(salary), 2) as avg_sal
FROM employees
GROUP BY department
HAVING AVG(salary) > 60000;

-- Result:
-- +-------------+----------+
-- | department  | avg_sal  |
-- +-------------+----------+
-- | Engineering | 75000.00 |  <-- 75000 > 60000 ✓
-- +-------------+----------+
-- Sales (57500) excluded because 57500 < 60000
```

### WHERE vs HAVING

```
+------------------------------------------------------------------+
|           WHERE                    |         HAVING               |
+------------------------------------------------------------------+
| Filters individual ROWS           | Filters GROUPS               |
| Before GROUP BY                   | After GROUP BY               |
| Cannot use aggregate functions    | Can use aggregate functions  |
| Faster (fewer rows to process)    | Slower (processes all first) |
+------------------------------------------------------------------+

Execution Order:
1. FROM      → Get all rows
2. WHERE     → Filter rows        ← WHERE works here
3. GROUP BY  → Create groups
4. HAVING    → Filter groups      ← HAVING works here
5. SELECT    → Choose columns
6. ORDER BY  → Sort results
```

```sql
-- Both in action
SELECT department, ROUND(AVG(salary), 2) as avg_sal
FROM employees
WHERE hire_date > '2020-01-01'    -- Filter ROWS first (before grouping)
GROUP BY department
HAVING AVG(salary) > 50000;       -- Filter GROUPS after (after grouping)

-- Step-by-step:
-- Step 1: FROM employees (5 rows)
-- Step 2: WHERE hire_date > '2020-01-01' (filters to 4 rows)
-- Step 3: GROUP BY department (creates 2 groups)
-- Step 4: HAVING AVG(salary) > 50000 (keeps groups with avg > 50000)
-- Step 5: SELECT columns
```

### DISTINCT - Removing Duplicates

```sql
-- Sample data:
-- +-------------+
-- | department  |
-- +-------------+
-- | Engineering |
-- | Sales       |
-- | Engineering |  <-- Duplicate
-- | Marketing   |
-- | Sales       |  <-- Duplicate
-- +-------------+

-- Get unique departments
SELECT DISTINCT department FROM employees;

-- Result:
-- +-------------+
-- | department  |
-- +-------------+
-- | Engineering |
-- | Sales       |
-- | Marketing   |
-- +-------------+

-- DISTINCT ON (PostgreSQL-specific) - first row per group
SELECT DISTINCT ON (department) department, name, salary
FROM employees
ORDER BY department, salary DESC;

-- Result: Returns highest-paid employee per department
-- +-------------+---------+--------+
-- | department  | name    | salary |
-- +-------------+---------+--------+
-- | Engineering | Alice   | 80000  |  <-- Highest in Engineering
-- | Marketing   | Diana   | 60000  |  <-- Only one in Marketing
-- | Sales       | Diana   | 60000  |  <-- Highest in Sales
-- +-------------+---------+--------+

-- Note: DISTINCT ON requires ORDER BY to start with the same columns
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

JOINs combine rows from two or more tables based on related columns. Understanding JOINs is crucial for interviews as they're the foundation of relational database queries.

```
+------------------------------------------------------------------+
|                         JOIN TYPES                                |
+------------------------------------------------------------------+
|                                                                   |
|   INNER JOIN     LEFT JOIN       RIGHT JOIN     FULL OUTER JOIN  |
|   ┌─────────┐    ┌─────────┐     ┌─────────┐    ┌─────────┐      |
|   │  ████   │    │████     │     │     ████│    │█████████│      |
|   │  ████   │    │████ ░░░░│     │░░░░ ████│    │█████████│      |
|   │  ████   │    │████     │     │     ████│    │█████████│      |
|   └─────────┘    └─────────┘     └─────────┘    └─────────┘      |
|   Only matching  All left +      All right +   All from both     |
|   rows           matching right  matching left                   |
|                                                                   |
|   CROSS JOIN                     SELF JOIN                        |
|   ┌─────────┐                    ┌─────────┐                     |
|   │ A x B   │                    │ Table   │                     |
|   │ Every   │                    │ joined  │                     |
|   │ combo   │                    │ to self │                     |
|   └─────────┘                    └─────────┘                     |
+------------------------------------------------------------------+
```

### Sample Data for Examples

```sql
-- Create and populate sample tables:
CREATE TABLE employees (
    id SERIAL PRIMARY KEY,
    name VARCHAR(50),
    dept_id INTEGER
);

CREATE TABLE departments (
    id SERIAL PRIMARY KEY,
    dept_name VARCHAR(50)
);

INSERT INTO employees VALUES (1, 'Alice', 1), (2, 'Bob', 2), (3, 'Charlie', NULL);
INSERT INTO departments VALUES (1, 'Engineering'), (2, 'Sales'), (3, 'HR');
```

```
employees                          departments
+----+----------+---------+        +----+-------------+
| id | name     | dept_id |        | id | dept_name   |
+----+----------+---------+        +----+-------------+
| 1  | Alice    | 1       |        | 1  | Engineering |
| 2  | Bob      | 2       |        | 2  | Sales       |
| 3  | Charlie  | NULL    |        | 3  | HR          |
+----+----------+---------+        +----+-------------+

Key observations:
- Alice (dept_id=1) → matches Engineering
- Bob (dept_id=2) → matches Sales
- Charlie (dept_id=NULL) → no department assigned
- HR department (id=3) → no employees
```

### INNER JOIN

Returns only rows that have matching values in **both** tables.

```sql
SELECT e.name, d.dept_name
FROM employees e
INNER JOIN departments d ON e.dept_id = d.id;
```

**Step-by-step execution:**

```
Step 1: Take each employee row
Step 2: Find matching department (where e.dept_id = d.id)
Step 3: If match found, include the combined row

Employee Alice (dept_id=1):  Match with Engineering (id=1) ✓ Include
Employee Bob (dept_id=2):    Match with Sales (id=2)       ✓ Include
Employee Charlie (dept_id=NULL): No match                  ✗ Exclude

Result:
+-------+-------------+
| name  | dept_name   |
+-------+-------------+
| Alice | Engineering |
| Bob   | Sales       |
+-------+-------------+
(2 rows)

Note: Charlie excluded (no dept_id)
      HR excluded (no matching employee)
```

### LEFT JOIN (Left Outer Join)

Returns **all rows from the left table**, and matching rows from the right table. If no match, NULL values are returned for right table columns.

```sql
SELECT e.name, d.dept_name
FROM employees e
LEFT JOIN departments d ON e.dept_id = d.id;
```

**Step-by-step execution:**

```
Step 1: Include ALL employees (left table)
Step 2: For each, find matching department
Step 3: If no match, put NULL for department columns

Employee Alice (dept_id=1):      Match with Engineering ✓
Employee Bob (dept_id=2):        Match with Sales       ✓
Employee Charlie (dept_id=NULL): No match → NULL        ✓ (Still included!)

Result:
+---------+-------------+
| name    | dept_name   |
+---------+-------------+
| Alice   | Engineering |
| Bob     | Sales       |
| Charlie | NULL        |  <-- Charlie included with NULL dept_name
+---------+-------------+
(3 rows)

Note: All employees included, HR department not shown
```

### RIGHT JOIN (Right Outer Join)

Returns **all rows from the right table**, and matching rows from the left table.

```sql
SELECT e.name, d.dept_name
FROM employees e
RIGHT JOIN departments d ON e.dept_id = d.id;
```

**Step-by-step execution:**

```
Step 1: Include ALL departments (right table)
Step 2: For each, find matching employees
Step 3: If no match, put NULL for employee columns

Department Engineering (id=1): Match with Alice   ✓
Department Sales (id=2):       Match with Bob     ✓
Department HR (id=3):          No match → NULL    ✓ (Still included!)

Result:
+-------+-------------+
| name  | dept_name   |
+-------+-------------+
| Alice | Engineering |
| Bob   | Sales       |
| NULL  | HR          |  <-- HR included with NULL name
+-------+-------------+
(3 rows)

Note: All departments included, Charlie not shown
```

### FULL OUTER JOIN (Native in PostgreSQL!)

Returns **all rows from both tables**. Unlike MySQL, PostgreSQL supports this natively.

```sql
SELECT e.name, d.dept_name
FROM employees e
FULL OUTER JOIN departments d ON e.dept_id = d.id;
```

**Step-by-step execution:**

```
Step 1: Include ALL rows from BOTH tables
Step 2: Match where possible
Step 3: NULL for non-matching sides

Result:
+---------+-------------+
| name    | dept_name   |
+---------+-------------+
| Alice   | Engineering |  <-- Matched
| Bob     | Sales       |  <-- Matched
| Charlie | NULL        |  <-- Left only (no department)
| NULL    | HR          |  <-- Right only (no employees)
+---------+-------------+
(4 rows)

Complete picture: Shows all employees AND all departments
```

### CROSS JOIN

Returns the **Cartesian product** - every combination of rows from both tables.

```sql
SELECT e.name, d.dept_name
FROM employees e
CROSS JOIN departments d;
```

**Step-by-step execution:**

```
3 employees × 3 departments = 9 rows

+----------+-------------+
| name     | dept_name   |
+----------+-------------+
| Alice    | Engineering |
| Alice    | Sales       |
| Alice    | HR          |
| Bob      | Engineering |
| Bob      | Sales       |
| Bob      | HR          |
| Charlie  | Engineering |
| Charlie  | Sales       |
| Charlie  | HR          |
+----------+-------------+
(9 rows)

Use case: Generating all possible combinations (e.g., product variants)
Warning: Can produce very large result sets (m × n rows)
```

### SELF JOIN

Joins a table to itself. Useful for hierarchical data.

```sql
-- Updated employees table with manager_id:
-- +----+---------+------------+
-- | id | name    | manager_id |
-- +----+---------+------------+
-- | 1  | CEO     | NULL       |
-- | 2  | Alice   | 1          |
-- | 3  | Bob     | 1          |
-- | 4  | Charlie | 2          |
-- +----+---------+------------+

-- Find employees and their managers
SELECT
    e.name as employee,
    m.name as manager
FROM employees e
LEFT JOIN employees m ON e.manager_id = m.id;

-- Result:
-- +----------+---------+
-- | employee | manager |
-- +----------+---------+
-- | CEO      | NULL    |  <-- CEO has no manager
-- | Alice    | CEO     |
-- | Bob      | CEO     |
-- | Charlie  | Alice   |
-- +----------+---------+
```

### LATERAL JOIN (PostgreSQL-Specific!)

Allows the right-hand subquery to reference columns from the left. Think of it as a "for each" loop in SQL.

```sql
-- Get top 2 orders for each customer
SELECT c.name, o.order_date, o.amount
FROM customers c
LEFT JOIN LATERAL (
    SELECT order_date, amount
    FROM orders
    WHERE customer_id = c.id    -- References 'c' from outer query!
    ORDER BY amount DESC
    LIMIT 2
) o ON TRUE;

-- Sample data:
-- customers:            orders:
-- +----+-------+        +----+-------------+--------+--------+
-- | id | name  |        | id | customer_id | amount | date   |
-- +----+-------+        +----+-------------+--------+--------+
-- | 1  | Alice |        | 1  | 1           | 100    | Jan 1  |
-- | 2  | Bob   |        | 2  | 1           | 300    | Jan 2  |
--                       | 3  | 1           | 200    | Jan 3  |
--                       | 4  | 2           | 150    | Jan 1  |

-- Result:
-- +-------+------------+--------+
-- | name  | order_date | amount |
-- +-------+------------+--------+
-- | Alice | Jan 2      | 300    |  <-- Top 2 for Alice
-- | Alice | Jan 3      | 200    |
-- | Bob   | Jan 1      | 150    |  <-- Top 1 for Bob (only 1 order)
-- +-------+------------+--------+
```

### USING Clause

Shorter syntax when column names match:

```sql
-- Instead of:
SELECT e.name, d.dept_name
FROM employees e
INNER JOIN departments d ON e.department_id = d.department_id;

-- Use:
SELECT e.name, d.dept_name
FROM employees e
INNER JOIN departments d USING (department_id);
-- Column must have same name in both tables
```

### NATURAL JOIN

Automatically joins on all matching column names:

```sql
SELECT e.name, d.dept_name
FROM employees e
NATURAL JOIN departments d;
-- Joins on all columns with same name

-- Caution: Can be unpredictable if column names change!
-- Prefer explicit JOIN conditions in production code.
```

### Interview Tip: JOIN Decision Tree

```
Q: Do you need ALL rows from the LEFT table?
   |
   +-- YES --> LEFT JOIN
   |
   +-- NO --> Q: Do you need ALL rows from the RIGHT table?
                |
                +-- YES --> RIGHT JOIN
                |
                +-- NO --> Q: Do you need ALL rows from BOTH tables?
                            |
                            +-- YES --> FULL OUTER JOIN
                            |
                            +-- NO --> INNER JOIN (most common)
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

A subquery is a query nested inside another query. Subqueries allow you to build complex queries step by step, using the result of one query as input for another.

```
+------------------------------------------------------------------+
|                      SUBQUERY TYPES                               |
+------------------------------------------------------------------+
|                                                                   |
|   WHERE Subquery          FROM Subquery (Derived Table)          |
|   ┌─────────────┐         ┌─────────────────────────┐            |
|   │ SELECT ...  │         │ SELECT * FROM           │            |
|   │ WHERE col   │         │   (SELECT ... ) AS sub  │            |
|   │   IN (...)  │         └─────────────────────────┘            |
|   └─────────────┘                                                 |
|                                                                   |
|   SELECT Subquery         Correlated Subquery                     |
|   ┌─────────────┐         ┌─────────────────────────┐            |
|   │ SELECT      │         │ References outer query  │            |
|   │  (SELECT..) │         │ Re-executes for each    │            |
|   │ FROM ...    │         │ row in outer query      │            |
|   └─────────────┘         └─────────────────────────┘            |
+------------------------------------------------------------------+
```

### Subquery in WHERE Clause

The most common use: filter rows based on a subquery result.

```sql
-- Sample data:
-- employees:
-- +----+---------+--------+-------------+
-- | id | name    | salary | department  |
-- +----+---------+--------+-------------+
-- | 1  | Alice   | 80000  | Engineering |
-- | 2  | Bob     | 55000  | Sales       |
-- | 3  | Charlie | 70000  | Engineering |
-- | 4  | Diana   | 60000  | Marketing   |
-- +----+---------+--------+-------------+
-- Average salary = (80000+55000+70000+60000)/4 = 66250

-- Find employees earning more than average
SELECT name, salary
FROM employees
WHERE salary > (SELECT AVG(salary) FROM employees);

-- Step-by-step execution:
-- Step 1: Execute inner query
--         SELECT AVG(salary) FROM employees → 66250
--
-- Step 2: Replace subquery with result
--         SELECT name, salary FROM employees WHERE salary > 66250
--
-- Step 3: Execute outer query

-- Result:
-- +---------+--------+
-- | name    | salary |
-- +---------+--------+
-- | Alice   | 80000  |  <-- 80000 > 66250 ✓
-- | Charlie | 70000  |  <-- 70000 > 66250 ✓
-- +---------+--------+
```

**Using IN with subquery:**

```sql
-- departments table:
-- +----+-------------+
-- | id | dept_name   |
-- +----+-------------+
-- | 1  | Engineering |
-- | 2  | Sales       |
-- +----+-------------+

-- Find employees in Engineering department
SELECT name FROM employees
WHERE dept_id IN (
    SELECT id FROM departments WHERE dept_name = 'Engineering'
);

-- Step-by-step:
-- Step 1: Inner query returns: (1)
-- Step 2: Outer becomes: SELECT name FROM employees WHERE dept_id IN (1)

-- Result:
-- +---------+
-- | name    |
-- +---------+
-- | Alice   |
-- | Charlie |
-- +---------+
```

### Subquery in FROM Clause (Derived Table)

Creates a temporary table from the subquery result.

```sql
-- Find the highest rating among Indian movies
SELECT MAX(rating) as highest_rating
FROM (
    SELECT id, title, rating
    FROM movies
    WHERE country = 'India'
) AS indian_movies;

-- Step-by-step:
-- Sample movies:
-- +----+----------------+--------+---------+
-- | id | title          | rating | country |
-- +----+----------------+--------+---------+
-- | 1  | Inception      | 8.8    | USA     |
-- | 2  | 3 Idiots       | 8.4    | India   |
-- | 3  | PK             | 8.1    | India   |
-- | 4  | Dangal         | 8.6    | India   |
-- +----+----------------+--------+---------+

-- Step 1: Inner query creates derived table "indian_movies":
-- +----+----------------+--------+
-- | id | title          | rating |
-- +----+----------------+--------+
-- | 2  | 3 Idiots       | 8.4    |
-- | 3  | PK             | 8.1    |
-- | 4  | Dangal         | 8.6    |
-- +----+----------------+--------+

-- Step 2: Outer query finds MAX(rating) from derived table:
-- +----------------+
-- | highest_rating |
-- +----------------+
-- | 8.6            |
-- +----------------+
```

### Subquery in SELECT Clause

Returns a single value for each row.

```sql
SELECT
    name,
    salary,
    (SELECT AVG(salary) FROM employees) as company_avg,
    salary - (SELECT AVG(salary) FROM employees) as diff_from_avg
FROM employees;

-- Result:
-- +---------+--------+-------------+---------------+
-- | name    | salary | company_avg | diff_from_avg |
-- +---------+--------+-------------+---------------+
-- | Alice   | 80000  | 66250       | 13750         |
-- | Bob     | 55000  | 66250       | -11250        |
-- | Charlie | 70000  | 66250       | 3750          |
-- | Diana   | 60000  | 66250       | -6250         |
-- +---------+--------+-------------+---------------+
```

### Correlated Subqueries

A correlated subquery references columns from the outer query. It re-executes for each row in the outer query.

```sql
-- Find employees earning more than their department's average
SELECT e1.name, e1.salary, e1.department
FROM employees e1
WHERE e1.salary > (
    SELECT AVG(e2.salary)
    FROM employees e2
    WHERE e2.department = e1.department  -- References outer query!
);

-- Step-by-step for each row:

-- Row 1: Alice (Engineering)
--   Inner query: AVG where department = 'Engineering'
--   Engineering employees: Alice(80000), Charlie(70000) → AVG = 75000
--   Is 80000 > 75000? YES → Include Alice

-- Row 2: Bob (Sales)
--   Inner query: AVG where department = 'Sales'
--   Sales employees: Bob(55000) → AVG = 55000
--   Is 55000 > 55000? NO → Exclude Bob

-- Row 3: Charlie (Engineering)
--   Inner query: AVG where department = 'Engineering'
--   Engineering employees: Alice(80000), Charlie(70000) → AVG = 75000
--   Is 70000 > 75000? NO → Exclude Charlie

-- Row 4: Diana (Marketing)
--   Inner query: AVG where department = 'Marketing'
--   Marketing employees: Diana(60000) → AVG = 60000
--   Is 60000 > 60000? NO → Exclude Diana

-- Result:
-- +-------+--------+-------------+
-- | name  | salary | department  |
-- +-------+--------+-------------+
-- | Alice | 80000  | Engineering |  <-- Only Alice earns above her dept avg
-- +-------+--------+-------------+
```

### EXISTS Subquery

EXISTS returns TRUE if the subquery returns any rows. Often more efficient than IN for large datasets.

```sql
-- Find departments that have employees
SELECT d.dept_name
FROM departments d
WHERE EXISTS (
    SELECT 1 FROM employees e WHERE e.dept_id = d.id
);

-- Step-by-step:
-- departments:              employees:
-- +----+-------------+      +----+---------+---------+
-- | id | dept_name   |      | id | name    | dept_id |
-- +----+-------------+      +----+---------+---------+
-- | 1  | Engineering |      | 1  | Alice   | 1       |
-- | 2  | Sales       |      | 2  | Bob     | 2       |
-- | 3  | HR          |      | 3  | Charlie | NULL    |
-- +----+-------------+      +----+---------+---------+

-- Check Engineering (id=1): Any employee with dept_id=1? YES → Include
-- Check Sales (id=2): Any employee with dept_id=2? YES → Include
-- Check HR (id=3): Any employee with dept_id=3? NO → Exclude

-- Result:
-- +-------------+
-- | dept_name   |
-- +-------------+
-- | Engineering |
-- | Sales       |
-- +-------------+

-- Find departments WITHOUT employees (NOT EXISTS)
SELECT d.dept_name
FROM departments d
WHERE NOT EXISTS (
    SELECT 1 FROM employees e WHERE e.dept_id = d.id
);

-- Result:
-- +-------------+
-- | dept_name   |
-- +-------------+
-- | HR          |  <-- No employees in HR
-- +-------------+
```

**EXISTS vs IN Performance:**

```
+------------------------------------------------------------------+
|           EXISTS                    |         IN                  |
+------------------------------------------------------------------+
| Stops at first match               | Evaluates all matches       |
| Better for large subquery results  | Better for small lists      |
| Can use index on outer table       | Materializes subquery first |
| Preferred for correlated queries   | Simpler syntax              |
+------------------------------------------------------------------+
```

---

## Common Table Expressions (CTEs)

CTEs (using the `WITH` clause) make complex queries more readable by breaking them into named, reusable subqueries. Think of them as temporary named result sets.

```
+------------------------------------------------------------------+
|                      CTE BENEFITS                                 |
+------------------------------------------------------------------+
|                                                                   |
|  1. Readability    - Break complex queries into logical steps    |
|  2. Reusability    - Reference the same subquery multiple times  |
|  3. Recursion      - Handle hierarchical/tree data               |
|  4. Maintainability - Easier to debug and modify                 |
|                                                                   |
+------------------------------------------------------------------+

   Without CTE:                    With CTE:
   ┌─────────────────────┐        ┌─────────────────────┐
   │ SELECT * FROM       │        │ WITH subquery AS (  │
   │   (SELECT * FROM    │        │   SELECT * FROM t1  │
   │     (SELECT ...     │  →     │ )                   │
   │     ) AS sub1       │        │ SELECT * FROM       │
   │   ) AS sub2         │        │   subquery          │
   └─────────────────────┘        └─────────────────────┘
   Hard to read                    Clear and organized
```

### Basic CTE

```sql
-- Sample data:
-- employees:
-- +----+---------+--------+-------------+
-- | id | name    | salary | department  |
-- +----+---------+--------+-------------+
-- | 1  | Alice   | 90000  | Engineering |
-- | 2  | Bob     | 55000  | Sales       |
-- | 3  | Charlie | 85000  | Engineering |
-- | 4  | Diana   | 60000  | Sales       |
-- | 5  | Eve     | 95000  | Engineering |
-- +----+---------+--------+-------------+

WITH high_earners AS (
    SELECT name, salary, department
    FROM employees
    WHERE salary > 80000
)
SELECT * FROM high_earners WHERE department = 'Engineering';

-- Step-by-step execution:

-- Step 1: CTE "high_earners" is created:
-- +-------+--------+-------------+
-- | name  | salary | department  |
-- +-------+--------+-------------+
-- | Alice | 90000  | Engineering |
-- | Charlie| 85000 | Engineering |
-- | Eve   | 95000  | Engineering |
-- +-------+--------+-------------+

-- Step 2: Main query filters CTE result:
-- +-------+--------+-------------+
-- | name  | salary | department  |
-- +-------+--------+-------------+
-- | Alice | 90000  | Engineering |  <-- All already Engineering!
-- | Charlie| 85000 | Engineering |
-- | Eve   | 95000  | Engineering |
-- +-------+--------+-------------+
```

### Multiple CTEs

You can define multiple CTEs, each building on previous ones.

```sql
WITH
    -- CTE 1: Calculate department statistics
    dept_stats AS (
        SELECT
            department,
            AVG(salary) as avg_salary,
            COUNT(*) as emp_count
        FROM employees
        GROUP BY department
    ),
    -- CTE 2: Find high-paying departments (uses CTE 1!)
    high_paying_depts AS (
        SELECT department
        FROM dept_stats
        WHERE avg_salary > 60000
    )
-- Main query: Get employees in high-paying departments
SELECT e.name, e.salary, e.department
FROM employees e
JOIN high_paying_depts h ON e.department = h.department;

-- Step-by-step:

-- Step 1: dept_stats CTE:
-- +-------------+------------+-----------+
-- | department  | avg_salary | emp_count |
-- +-------------+------------+-----------+
-- | Engineering | 90000      | 3         |
-- | Sales       | 57500      | 2         |
-- +-------------+------------+-----------+

-- Step 2: high_paying_depts CTE (filters dept_stats):
-- +-------------+
-- | department  |
-- +-------------+
-- | Engineering |  <-- Only Engineering has avg > 60000
-- +-------------+

-- Step 3: Main query joins employees with high_paying_depts:
-- +---------+--------+-------------+
-- | name    | salary | department  |
-- +---------+--------+-------------+
-- | Alice   | 90000  | Engineering |
-- | Charlie | 85000  | Engineering |
-- | Eve     | 95000  | Engineering |
-- +---------+--------+-------------+
```

### Recursive CTE (PostgreSQL-Specific!)

Recursive CTEs can reference themselves - essential for hierarchical data like org charts, file systems, or graphs.

```sql
-- Generate a number series (simple recursion example)
WITH RECURSIVE numbers AS (
    -- Base case (anchor): Starting point
    SELECT 1 as n

    UNION ALL

    -- Recursive case: References itself
    SELECT n + 1 FROM numbers WHERE n < 5
)
SELECT * FROM numbers;

-- Execution trace:

-- Iteration 0 (base case):
-- +---+
-- | n |
-- +---+
-- | 1 |
-- +---+

-- Iteration 1: n=1, 1<5? YES, add n+1=2
-- +---+
-- | n |
-- +---+
-- | 1 |
-- | 2 |
-- +---+

-- Iteration 2: n=2, 2<5? YES, add n+1=3
-- ... continues...

-- Iteration 4: n=4, 4<5? YES, add n+1=5
-- +---+
-- | n |
-- +---+
-- | 1 |
-- | 2 |
-- | 3 |
-- | 4 |
-- | 5 |
-- +---+

-- Iteration 5: n=5, 5<5? NO, stop recursion

-- Final Result:
-- +---+
-- | n |
-- +---+
-- | 1 |
-- | 2 |
-- | 3 |
-- | 4 |
-- | 5 |
-- +---+
```

**Employee Hierarchy Example:**

```sql
-- Sample org chart:
-- +----+---------+------------+
-- | id | name    | manager_id |
-- +----+---------+------------+
-- | 1  | CEO     | NULL       |  <-- Top level
-- | 2  | CTO     | 1          |  <-- Reports to CEO
-- | 3  | CFO     | 1          |  <-- Reports to CEO
-- | 4  | DevLead | 2          |  <-- Reports to CTO
-- | 5  | Dev1    | 4          |  <-- Reports to DevLead
-- | 6  | Dev2    | 4          |  <-- Reports to DevLead
-- +----+---------+------------+

-- Visual hierarchy:
--         CEO (1)
--        /    \
--     CTO(2)  CFO(3)
--       |
--   DevLead(4)
--     /    \
--  Dev1(5) Dev2(6)

WITH RECURSIVE employee_hierarchy AS (
    -- Base case: Find top-level employees (no manager)
    SELECT
        id,
        name,
        manager_id,
        1 as level,
        name::TEXT as path
    FROM employees
    WHERE manager_id IS NULL

    UNION ALL

    -- Recursive case: Find employees who report to previous level
    SELECT
        e.id,
        e.name,
        e.manager_id,
        h.level + 1,
        h.path || ' > ' || e.name
    FROM employees e
    JOIN employee_hierarchy h ON e.manager_id = h.id
)
SELECT * FROM employee_hierarchy ORDER BY level, name;

-- Step-by-step execution:

-- Iteration 0 (base case): manager_id IS NULL
-- +----+------+------------+-------+------+
-- | id | name | manager_id | level | path |
-- +----+------+------------+-------+------+
-- | 1  | CEO  | NULL       | 1     | CEO  |
-- +----+------+------------+-------+------+

-- Iteration 1: employees where manager_id = 1
-- +----+------+------------+-------+-----------+
-- | id | name | manager_id | level | path      |
-- +----+------+------------+-------+-----------+
-- | 1  | CEO  | NULL       | 1     | CEO       |
-- | 2  | CTO  | 1          | 2     | CEO > CTO |
-- | 3  | CFO  | 1          | 2     | CEO > CFO |
-- +----+------+------------+-------+-----------+

-- Iteration 2: employees where manager_id IN (2, 3)
-- +----+---------+------------+-------+-------------------+
-- | id | name    | manager_id | level | path              |
-- +----+---------+------------+-------+-------------------+
-- | 1  | CEO     | NULL       | 1     | CEO               |
-- | 2  | CTO     | 1          | 2     | CEO > CTO         |
-- | 3  | CFO     | 1          | 2     | CEO > CFO         |
-- | 4  | DevLead | 2          | 3     | CEO > CTO > DevLead|
-- +----+---------+------------+-------+-------------------+

-- Final Result:
-- +----+---------+------------+-------+---------------------------+
-- | id | name    | manager_id | level | path                      |
-- +----+---------+------------+-------+---------------------------+
-- | 1  | CEO     | NULL       | 1     | CEO                       |
-- | 3  | CFO     | 1          | 2     | CEO > CFO                 |
-- | 2  | CTO     | 1          | 2     | CEO > CTO                 |
-- | 4  | DevLead | 2          | 3     | CEO > CTO > DevLead       |
-- | 6  | Dev2    | 4          | 4     | CEO > CTO > DevLead > Dev2|
-- | 5  | Dev1    | 4          | 4     | CEO > CTO > DevLead > Dev1|
-- +----+---------+------------+-------+---------------------------+
```

### CTE vs Subquery

```
+------------------------------------------------------------------+
|           CTE                       |      Subquery              |
+------------------------------------------------------------------+
| Named, can reference multiple times | Anonymous, one-time use    |
| Can be recursive                    | Cannot be recursive        |
| More readable for complex queries   | Fine for simple cases      |
| Defined before main query           | Embedded in main query     |
+------------------------------------------------------------------+
```

```sql
-- Same query, two styles:

-- CTE style (clearer):
WITH active_users AS (
    SELECT * FROM users WHERE active = true
)
SELECT * FROM active_users WHERE created_at > '2024-01-01';

-- Subquery style:
SELECT * FROM (
    SELECT * FROM users WHERE active = true
) AS active_users
WHERE created_at > '2024-01-01';
```

---

## Window Functions (Analytics)

Window functions perform calculations across a set of rows related to the current row, without collapsing rows like GROUP BY. They're powerful for analytics, rankings, and running calculations.

```
+------------------------------------------------------------------+
|              WINDOW FUNCTION ANATOMY                              |
+------------------------------------------------------------------+
|                                                                   |
|   function_name() OVER (                                          |
|       PARTITION BY col    -- Divide into groups (optional)        |
|       ORDER BY col        -- Sort within partition                |
|       ROWS/RANGE BETWEEN  -- Define frame (optional)              |
|   )                                                               |
|                                                                   |
+------------------------------------------------------------------+
|                                                                   |
|   GROUP BY vs WINDOW:                                             |
|   +----------------------+    +---------------------------+       |
|   |  GROUP BY            |    |  WINDOW FUNCTION          |       |
|   +----------------------+    +---------------------------+       |
|   |  ■ ■ ■  → [sum]      |    |  ■ → ■ + running_sum      |       |
|   |  ■ ■    → [sum]      |    |  ■ → ■ + running_sum      |       |
|   +----------------------+    |  ■ → ■ + running_sum      |       |
|   Collapses rows to 1        |  ■ → ■ + running_sum      |       |
|                              +---------------------------+       |
|                               Keeps all rows!                     |
+------------------------------------------------------------------+
```

### Sample Data

```sql
-- employees table for examples:
-- +----+---------+--------+-------------+------------+
-- | id | name    | salary | department  | hire_date  |
-- +----+---------+--------+-------------+------------+
-- | 1  | Alice   | 80000  | Engineering | 2020-01-15 |
-- | 2  | Bob     | 55000  | Sales       | 2020-03-01 |
-- | 3  | Charlie | 70000  | Engineering | 2020-06-01 |
-- | 4  | Diana   | 60000  | Sales       | 2020-09-15 |
-- | 5  | Eve     | 75000  | Engineering | 2021-01-10 |
-- +----+---------+--------+-------------+------------+
```

### Basic Window Functions

**ROW_NUMBER: Unique sequential number for each row**

```sql
SELECT name, salary,
    ROW_NUMBER() OVER (ORDER BY salary DESC) as rank
FROM employees;

-- Step-by-step:
-- 1. Sort all employees by salary DESC
-- 2. Assign row numbers 1, 2, 3...

-- Result:
-- +---------+--------+------+
-- | name    | salary | rank |
-- +---------+--------+------+
-- | Alice   | 80000  | 1    |  <-- Highest salary
-- | Eve     | 75000  | 2    |
-- | Charlie | 70000  | 3    |
-- | Diana   | 60000  | 4    |
-- | Bob     | 55000  | 5    |  <-- Lowest salary
-- +---------+--------+------+
```

**RANK vs DENSE_RANK: Handling ties**

```sql
-- Modified data with tie:
-- Alice: 80000, Eve: 75000, Charlie: 75000 (tie!), Diana: 60000, Bob: 55000

SELECT name, salary,
    RANK() OVER (ORDER BY salary DESC) as rank,
    DENSE_RANK() OVER (ORDER BY salary DESC) as dense_rank
FROM employees;

-- Result:
-- +---------+--------+------+------------+
-- | name    | salary | rank | dense_rank |
-- +---------+--------+------+------------+
-- | Alice   | 80000  | 1    | 1          |
-- | Eve     | 75000  | 2    | 2          |  <-- Tied for 2nd
-- | Charlie | 75000  | 2    | 2          |  <-- Tied for 2nd
-- | Diana   | 60000  | 4    | 3          |  <-- RANK skips 3, DENSE_RANK continues
-- | Bob     | 55000  | 5    | 4          |
-- +---------+--------+------+------------+
--                     ↑          ↑
--              RANK: 1,2,2,4,5   DENSE_RANK: 1,2,2,3,4
--              (skips 3)        (no gaps)
```

**NTILE: Divide into buckets**

```sql
SELECT name, salary,
    NTILE(4) OVER (ORDER BY salary DESC) as quartile
FROM employees;

-- Divides 5 employees into 4 groups (as evenly as possible):
-- Result:
-- +---------+--------+----------+
-- | name    | salary | quartile |
-- +---------+--------+----------+
-- | Alice   | 80000  | 1        |  <-- Top 25%
-- | Eve     | 75000  | 1        |
-- | Charlie | 70000  | 2        |  <-- Second quartile
-- | Diana   | 60000  | 3        |  <-- Third quartile
-- | Bob     | 55000  | 4        |  <-- Bottom 25%
-- +---------+--------+----------+
```

### PARTITION BY - Window Within Groups

PARTITION BY creates separate windows for each group, like GROUP BY but keeps all rows.

```sql
-- Rank within each department
SELECT name, department, salary,
    RANK() OVER (PARTITION BY department ORDER BY salary DESC) as dept_rank
FROM employees;

-- Step-by-step:
-- 1. Partition by department (Engineering, Sales)
-- 2. Within each partition, rank by salary DESC

-- Result:
-- +---------+-------------+--------+-----------+
-- | name    | department  | salary | dept_rank |
-- +---------+-------------+--------+-----------+
-- | Alice   | Engineering | 80000  | 1         |  <-- #1 in Engineering
-- | Eve     | Engineering | 75000  | 2         |  <-- #2 in Engineering
-- | Charlie | Engineering | 70000  | 3         |  <-- #3 in Engineering
-- | Diana   | Sales       | 60000  | 1         |  <-- #1 in Sales (new partition!)
-- | Bob     | Sales       | 55000  | 2         |  <-- #2 in Sales
-- +---------+-------------+--------+-----------+
```

**Running total per department:**

```sql
SELECT name, department, salary,
    SUM(salary) OVER (
        PARTITION BY department
        ORDER BY hire_date
    ) as running_total
FROM employees;

-- Result:
-- +---------+-------------+--------+---------------+
-- | name    | department  | salary | running_total |
-- +---------+-------------+--------+---------------+
-- | Alice   | Engineering | 80000  | 80000         |  <-- First in Eng
-- | Charlie | Engineering | 70000  | 150000        |  <-- 80000+70000
-- | Eve     | Engineering | 75000  | 225000        |  <-- 80000+70000+75000
-- | Bob     | Sales       | 55000  | 55000         |  <-- First in Sales (resets!)
-- | Diana   | Sales       | 60000  | 115000        |  <-- 55000+60000
-- +---------+-------------+--------+---------------+
```

### Frame Specification

Control exactly which rows are included in the window calculation.

```
+------------------------------------------------------------------+
|                    FRAME BOUNDARIES                               |
+------------------------------------------------------------------+
|                                                                   |
|   UNBOUNDED PRECEDING  ─────────┐                                |
|   n PRECEDING          ─────────┼─── Before current row          |
|   CURRENT ROW          ─────────┼─── The current row             |
|   n FOLLOWING          ─────────┼─── After current row           |
|   UNBOUNDED FOLLOWING  ─────────┘                                |
|                                                                   |
|   Example: ROWS BETWEEN 2 PRECEDING AND CURRENT ROW              |
|                                                                   |
|      [ row -2 ]  ─┐                                              |
|      [ row -1 ]   ├─── These 3 rows are in the frame             |
|   →  [ current]  ─┘                                              |
|      [ row +1 ]  ← Not included                                  |
|                                                                   |
+------------------------------------------------------------------+
```

**Moving average (last 3 rows):**

```sql
SELECT name, salary, hire_date,
    AVG(salary) OVER (
        ORDER BY hire_date
        ROWS BETWEEN 2 PRECEDING AND CURRENT ROW
    ) as moving_avg
FROM employees;

-- Step-by-step (ordered by hire_date):
-- Alice (2020-01-15):   Frame = [Alice]                → AVG = 80000
-- Bob (2020-03-01):     Frame = [Alice, Bob]           → AVG = 67500
-- Charlie (2020-06-01): Frame = [Alice, Bob, Charlie]  → AVG = 68333
-- Diana (2020-09-15):   Frame = [Bob, Charlie, Diana]  → AVG = 61667
-- Eve (2021-01-10):     Frame = [Charlie, Diana, Eve]  → AVG = 68333

-- Result:
-- +---------+--------+------------+------------+
-- | name    | salary | hire_date  | moving_avg |
-- +---------+--------+------------+------------+
-- | Alice   | 80000  | 2020-01-15 | 80000.00   |
-- | Bob     | 55000  | 2020-03-01 | 67500.00   |
-- | Charlie | 70000  | 2020-06-01 | 68333.33   |
-- | Diana   | 60000  | 2020-09-15 | 61666.67   |
-- | Eve     | 75000  | 2021-01-10 | 68333.33   |
-- +---------+--------+------------+------------+
```

**Cumulative sum (running total from beginning):**

```sql
SELECT name, salary,
    SUM(salary) OVER (
        ORDER BY hire_date
        ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW
    ) as cumulative_sum
FROM employees;

-- Result:
-- +---------+--------+----------------+
-- | name    | salary | cumulative_sum |
-- +---------+--------+----------------+
-- | Alice   | 80000  | 80000          |  <-- Just Alice
-- | Bob     | 55000  | 135000         |  <-- Alice + Bob
-- | Charlie | 70000  | 205000         |  <-- Alice + Bob + Charlie
-- | Diana   | 60000  | 265000         |  <-- + Diana
-- | Eve     | 75000  | 340000         |  <-- + Eve (total of all)
-- +---------+--------+----------------+
```

### LAG and LEAD

Access data from previous or next rows without a self-join.

```sql
SELECT name, salary, hire_date,
    LAG(salary) OVER (ORDER BY hire_date) as prev_salary,
    salary - LAG(salary) OVER (ORDER BY hire_date) as salary_change
FROM employees;

-- LAG looks at the previous row in the ordered sequence:

-- Result:
-- +---------+--------+------------+-------------+---------------+
-- | name    | salary | hire_date  | prev_salary | salary_change |
-- +---------+--------+------------+-------------+---------------+
-- | Alice   | 80000  | 2020-01-15 | NULL        | NULL          |  <-- No previous
-- | Bob     | 55000  | 2020-03-01 | 80000       | -25000        |
-- | Charlie | 70000  | 2020-06-01 | 55000       | +15000        |
-- | Diana   | 60000  | 2020-09-15 | 70000       | -10000        |
-- | Eve     | 75000  | 2021-01-10 | 60000       | +15000        |
-- +---------+--------+------------+-------------+---------------+

-- LEAD looks at the next row:
SELECT name, salary,
    LEAD(salary) OVER (ORDER BY hire_date) as next_salary
FROM employees;

-- Result:
-- +---------+--------+-------------+
-- | name    | salary | next_salary |
-- +---------+--------+-------------+
-- | Alice   | 80000  | 55000       |  <-- Bob's salary
-- | Bob     | 55000  | 70000       |  <-- Charlie's salary
-- | Charlie | 70000  | 60000       |
-- | Diana   | 60000  | 75000       |
-- | Eve     | 75000  | NULL        |  <-- No next row
-- +---------+--------+-------------+

-- LAG/LEAD with offset and default:
SELECT name, salary,
    LAG(salary, 2, 0) OVER (ORDER BY hire_date) as salary_2_rows_ago
FROM employees;
-- LAG(column, offset, default_value)
-- Looks 2 rows back, returns 0 if not found
```

### FIRST_VALUE and LAST_VALUE

Get the first or last value in the window.

```sql
SELECT name, department, salary,
    FIRST_VALUE(name) OVER (
        PARTITION BY department
        ORDER BY salary DESC
    ) as highest_earner,
    LAST_VALUE(name) OVER (
        PARTITION BY department
        ORDER BY salary DESC
        ROWS BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING
    ) as lowest_earner
FROM employees;

-- Result:
-- +---------+-------------+--------+----------------+---------------+
-- | name    | department  | salary | highest_earner | lowest_earner |
-- +---------+-------------+--------+----------------+---------------+
-- | Alice   | Engineering | 80000  | Alice          | Charlie       |
-- | Eve     | Engineering | 75000  | Alice          | Charlie       |
-- | Charlie | Engineering | 70000  | Alice          | Charlie       |
-- | Diana   | Sales       | 60000  | Diana          | Bob           |
-- | Bob     | Sales       | 55000  | Diana          | Bob           |
-- +---------+-------------+--------+----------------+---------------+

-- Note: LAST_VALUE needs the full frame specification to work correctly!
```

### Common Window Function Interview Patterns

```sql
-- Pattern 1: Percentage of total
SELECT name, salary,
    ROUND(100.0 * salary / SUM(salary) OVER (), 2) as pct_of_total
FROM employees;

-- Pattern 2: Difference from group average
SELECT name, department, salary,
    salary - AVG(salary) OVER (PARTITION BY department) as diff_from_dept_avg
FROM employees;

-- Pattern 3: Year-over-year comparison
SELECT month, revenue,
    revenue - LAG(revenue, 12) OVER (ORDER BY month) as yoy_change
FROM monthly_sales;
```

### Advanced Window Functions for Data Science

These window functions are essential for data scientists performing statistical analysis, building features for ML models, and conducting cohort/funnel analysis.

#### PERCENT_RANK and CUME_DIST (Percentile Calculations)

```sql
-- PERCENT_RANK: Relative rank as a percentage (0 to 1)
-- Formula: (rank - 1) / (total_rows - 1)

SELECT name, salary,
    PERCENT_RANK() OVER (ORDER BY salary) as percentile_rank,
    CUME_DIST() OVER (ORDER BY salary) as cumulative_dist
FROM employees;

-- With sample data:
-- +----+---------+--------+
-- | id | name    | salary |
-- +----+---------+--------+
-- | 1  | Bob     | 55000  |
-- | 2  | Diana   | 60000  |
-- | 3  | Charlie | 70000  |
-- | 4  | Eve     | 75000  |
-- | 5  | Alice   | 80000  |
-- +----+---------+--------+

-- Result:
-- +---------+--------+-----------------+------------------+
-- | name    | salary | percentile_rank | cumulative_dist  |
-- +---------+--------+-----------------+------------------+
-- | Bob     | 55000  | 0.00            | 0.20             |  <-- Lowest (0th percentile)
-- | Diana   | 60000  | 0.25            | 0.40             |  <-- 25th percentile
-- | Charlie | 70000  | 0.50            | 0.60             |  <-- 50th percentile (median position)
-- | Eve     | 75000  | 0.75            | 0.80             |  <-- 75th percentile
-- | Alice   | 80000  | 1.00            | 1.00             |  <-- 100th percentile (highest)
-- +---------+--------+-----------------+------------------+

-- PERCENT_RANK vs CUME_DIST:
-- +------------------------------------------------------------------+
-- |  PERCENT_RANK: What % of values are BELOW this row?              |
-- |  CUME_DIST:    What % of values are AT OR BELOW this row?        |
-- +------------------------------------------------------------------+
-- |                                                                   |
-- |  Example with 5 rows (rank 1-5):                                 |
-- |  Row 1: PERCENT_RANK = (1-1)/(5-1) = 0.00                        |
-- |         CUME_DIST = 1/5 = 0.20                                   |
-- |  Row 3: PERCENT_RANK = (3-1)/(5-1) = 0.50                        |
-- |         CUME_DIST = 3/5 = 0.60                                   |
-- |                                                                   |
-- +------------------------------------------------------------------+
```

**Use Case: Identifying outliers based on percentile**

```sql
-- Find employees with salary in top 10% or bottom 10%
SELECT name, salary, percentile_rank,
    CASE 
        WHEN percentile_rank >= 0.90 THEN 'Top 10%'
        WHEN percentile_rank <= 0.10 THEN 'Bottom 10%'
        ELSE 'Middle 80%'
    END as salary_tier
FROM (
    SELECT name, salary,
        PERCENT_RANK() OVER (ORDER BY salary) as percentile_rank
    FROM employees
) ranked;
```

#### NTH_VALUE - Get Specific Row Value

```sql
-- NTH_VALUE returns the value at the nth row in the window

SELECT name, department, salary,
    NTH_VALUE(name, 1) OVER (
        PARTITION BY department 
        ORDER BY salary DESC
        ROWS BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING
    ) as top_earner,
    NTH_VALUE(name, 2) OVER (
        PARTITION BY department 
        ORDER BY salary DESC
        ROWS BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING
    ) as second_earner
FROM employees;

-- Result:
-- +---------+-------------+--------+------------+---------------+
-- | name    | department  | salary | top_earner | second_earner |
-- +---------+-------------+--------+------------+---------------+
-- | Alice   | Engineering | 80000  | Alice      | Eve           |
-- | Eve     | Engineering | 75000  | Alice      | Eve           |
-- | Charlie | Engineering | 70000  | Alice      | Eve           |
-- | Diana   | Sales       | 60000  | Diana      | Bob           |
-- | Bob     | Sales       | 55000  | Diana      | Bob           |
-- +---------+-------------+--------+------------+---------------+

-- Note: Like LAST_VALUE, NTH_VALUE needs the full frame specification!
```

#### Named Windows (WINDOW Clause)

When using the same window definition multiple times, define it once with the WINDOW clause:

```sql
-- Without WINDOW clause (repetitive):
SELECT name, salary, department,
    ROW_NUMBER() OVER (PARTITION BY department ORDER BY salary DESC) as rn,
    RANK() OVER (PARTITION BY department ORDER BY salary DESC) as rnk,
    SUM(salary) OVER (PARTITION BY department ORDER BY salary DESC) as running_sum
FROM employees;

-- With WINDOW clause (cleaner):
SELECT name, salary, department,
    ROW_NUMBER() OVER w as rn,
    RANK() OVER w as rnk,
    SUM(salary) OVER w as running_sum
FROM employees
WINDOW w AS (PARTITION BY department ORDER BY salary DESC);

-- Multiple named windows:
SELECT name, salary, department,
    RANK() OVER by_dept_salary as dept_rank,
    RANK() OVER by_salary as overall_rank,
    AVG(salary) OVER by_dept as dept_avg
FROM employees
WINDOW 
    by_dept_salary AS (PARTITION BY department ORDER BY salary DESC),
    by_salary AS (ORDER BY salary DESC),
    by_dept AS (PARTITION BY department);
```

### Data Science Window Function Patterns

#### Pattern 1: Cumulative Distribution for Feature Engineering

```sql
-- Create percentile features for ML models
SELECT 
    user_id,
    total_purchases,
    PERCENT_RANK() OVER (ORDER BY total_purchases) as purchase_percentile,
    NTILE(10) OVER (ORDER BY total_purchases) as purchase_decile,
    CASE 
        WHEN PERCENT_RANK() OVER (ORDER BY total_purchases) >= 0.8 THEN 'high_value'
        WHEN PERCENT_RANK() OVER (ORDER BY total_purchases) >= 0.5 THEN 'medium_value'
        ELSE 'low_value'
    END as customer_segment
FROM customer_summary;
```

#### Pattern 2: Rolling Statistics (Moving Averages, Std Dev)

```sql
-- 7-day and 30-day rolling metrics for time series analysis
SELECT 
    date,
    daily_revenue,
    AVG(daily_revenue) OVER (
        ORDER BY date 
        ROWS BETWEEN 6 PRECEDING AND CURRENT ROW
    ) as rolling_7day_avg,
    AVG(daily_revenue) OVER (
        ORDER BY date 
        ROWS BETWEEN 29 PRECEDING AND CURRENT ROW
    ) as rolling_30day_avg,
    STDDEV(daily_revenue) OVER (
        ORDER BY date 
        ROWS BETWEEN 29 PRECEDING AND CURRENT ROW
    ) as rolling_30day_stddev
FROM daily_sales;

-- Result helps identify trends and anomalies:
-- +------------+---------------+------------------+-------------------+--------------------+
-- | date       | daily_revenue | rolling_7day_avg | rolling_30day_avg | rolling_30day_stddev|
-- +------------+---------------+------------------+-------------------+--------------------+
-- | 2024-01-01 | 10000         | 10000.00         | 10000.00          | NULL               |
-- | 2024-01-02 | 12000         | 11000.00         | 11000.00          | 1414.21            |
-- | 2024-01-03 | 8000          | 10000.00         | 10000.00          | 2000.00            |
-- | ...        | ...           | ...              | ...               | ...                |
-- +------------+---------------+------------------+-------------------+--------------------+
```

#### Pattern 3: Sessionization (Gap Analysis)

```sql
-- Identify user sessions based on inactivity gaps (30 minutes)
WITH events_with_gaps AS (
    SELECT 
        user_id,
        event_time,
        event_type,
        LAG(event_time) OVER (PARTITION BY user_id ORDER BY event_time) as prev_event_time,
        EXTRACT(EPOCH FROM (
            event_time - LAG(event_time) OVER (PARTITION BY user_id ORDER BY event_time)
        )) / 60 as minutes_since_last
    FROM user_events
),
sessions_marked AS (
    SELECT *,
        CASE WHEN minutes_since_last IS NULL OR minutes_since_last > 30 THEN 1 ELSE 0 END as new_session
    FROM events_with_gaps
)
SELECT 
    user_id,
    event_time,
    event_type,
    SUM(new_session) OVER (PARTITION BY user_id ORDER BY event_time) as session_id
FROM sessions_marked;

-- Result:
-- +---------+---------------------+------------+------------+
-- | user_id | event_time          | event_type | session_id |
-- +---------+---------------------+------------+------------+
-- | 1       | 2024-01-01 10:00:00 | pageview   | 1          |  <-- Session 1 starts
-- | 1       | 2024-01-01 10:05:00 | click      | 1          |
-- | 1       | 2024-01-01 10:10:00 | purchase   | 1          |
-- | 1       | 2024-01-01 14:00:00 | pageview   | 2          |  <-- Session 2 (>30 min gap)
-- | 1       | 2024-01-01 14:02:00 | click      | 2          |
-- +---------+---------------------+------------+------------+
```

#### Pattern 4: Cohort Analysis (Retention)

```sql
-- Calculate user retention by signup cohort
WITH user_activity AS (
    SELECT 
        u.user_id,
        DATE_TRUNC('month', u.signup_date) as cohort_month,
        DATE_TRUNC('month', a.activity_date) as activity_month
    FROM users u
    JOIN user_activities a ON u.user_id = a.user_id
),
cohort_size AS (
    SELECT cohort_month, COUNT(DISTINCT user_id) as cohort_users
    FROM user_activity
    GROUP BY cohort_month
),
retention AS (
    SELECT 
        ua.cohort_month,
        ua.activity_month,
        COUNT(DISTINCT ua.user_id) as active_users,
        EXTRACT(MONTH FROM AGE(ua.activity_month, ua.cohort_month)) as months_since_signup
    FROM user_activity ua
    GROUP BY ua.cohort_month, ua.activity_month
)
SELECT 
    r.cohort_month,
    r.months_since_signup,
    r.active_users,
    cs.cohort_users,
    ROUND(100.0 * r.active_users / cs.cohort_users, 2) as retention_pct
FROM retention r
JOIN cohort_size cs ON r.cohort_month = cs.cohort_month
ORDER BY r.cohort_month, r.months_since_signup;

-- Result (Cohort Retention Table):
-- +--------------+--------------------+--------------+--------------+---------------+
-- | cohort_month | months_since_signup| active_users | cohort_users | retention_pct |
-- +--------------+--------------------+--------------+--------------+---------------+
-- | 2024-01-01   | 0                  | 1000         | 1000         | 100.00        |
-- | 2024-01-01   | 1                  | 450          | 1000         | 45.00         |
-- | 2024-01-01   | 2                  | 320          | 1000         | 32.00         |
-- | 2024-02-01   | 0                  | 1200         | 1200         | 100.00        |
-- | 2024-02-01   | 1                  | 580          | 1200         | 48.33         |
-- +--------------+--------------------+--------------+--------------+---------------+
```

#### Pattern 5: Funnel Analysis with Window Functions

```sql
-- E-commerce funnel: view -> add_to_cart -> checkout -> purchase
WITH funnel_events AS (
    SELECT 
        user_id,
        session_id,
        event_type,
        event_time,
        LEAD(event_type) OVER (
            PARTITION BY user_id, session_id 
            ORDER BY event_time
        ) as next_event
    FROM events
    WHERE event_type IN ('view', 'add_to_cart', 'checkout', 'purchase')
),
funnel_steps AS (
    SELECT 
        event_type as step,
        COUNT(DISTINCT user_id || '-' || session_id) as sessions
    FROM funnel_events
    GROUP BY event_type
)
SELECT 
    step,
    sessions,
    FIRST_VALUE(sessions) OVER (ORDER BY 
        CASE step 
            WHEN 'view' THEN 1 
            WHEN 'add_to_cart' THEN 2 
            WHEN 'checkout' THEN 3 
            WHEN 'purchase' THEN 4 
        END
    ) as top_of_funnel,
    ROUND(100.0 * sessions / FIRST_VALUE(sessions) OVER (ORDER BY 
        CASE step 
            WHEN 'view' THEN 1 
            WHEN 'add_to_cart' THEN 2 
            WHEN 'checkout' THEN 3 
            WHEN 'purchase' THEN 4 
        END
    ), 2) as conversion_rate
FROM funnel_steps
ORDER BY CASE step 
    WHEN 'view' THEN 1 
    WHEN 'add_to_cart' THEN 2 
    WHEN 'checkout' THEN 3 
    WHEN 'purchase' THEN 4 
END;

-- Result:
-- +-------------+----------+---------------+-----------------+
-- | step        | sessions | top_of_funnel | conversion_rate |
-- +-------------+----------+---------------+-----------------+
-- | view        | 10000    | 10000         | 100.00          |
-- | add_to_cart | 3500     | 10000         | 35.00           |
-- | checkout    | 1200     | 10000         | 12.00           |
-- | purchase    | 800      | 10000         | 8.00            |
-- +-------------+----------+---------------+-----------------+
```

#### Pattern 6: Gap and Island Detection (Consecutive Events)

```sql
-- Find consecutive login streaks for gamification
WITH login_groups AS (
    SELECT 
        user_id,
        login_date,
        login_date - (ROW_NUMBER() OVER (
            PARTITION BY user_id 
            ORDER BY login_date
        ))::integer as group_id
    FROM daily_logins
)
SELECT 
    user_id,
    MIN(login_date) as streak_start,
    MAX(login_date) as streak_end,
    COUNT(*) as streak_length
FROM login_groups
GROUP BY user_id, group_id
HAVING COUNT(*) >= 3  -- Only streaks of 3+ days
ORDER BY user_id, streak_start;

-- How it works:
-- login_date: 2024-01-01, 2024-01-02, 2024-01-03, 2024-01-05, 2024-01-06
-- row_number:     1           2           3           4           5
-- date - rn:   2023-12-31  2023-12-31  2023-12-31  2024-01-01  2024-01-01
--              └─────────group 1─────────┘          └───group 2───┘
-- Consecutive dates get the same group_id!
```

#### Pattern 7: Time-Based Comparison (Period over Period)

```sql
-- Compare metrics across multiple time periods
SELECT 
    date,
    revenue,
    -- Day over day
    LAG(revenue, 1) OVER (ORDER BY date) as prev_day_revenue,
    ROUND(100.0 * (revenue - LAG(revenue, 1) OVER (ORDER BY date)) / 
        NULLIF(LAG(revenue, 1) OVER (ORDER BY date), 0), 2) as dod_change_pct,
    -- Week over week
    LAG(revenue, 7) OVER (ORDER BY date) as prev_week_revenue,
    ROUND(100.0 * (revenue - LAG(revenue, 7) OVER (ORDER BY date)) / 
        NULLIF(LAG(revenue, 7) OVER (ORDER BY date), 0), 2) as wow_change_pct,
    -- Month over month (approx 30 days)
    LAG(revenue, 30) OVER (ORDER BY date) as prev_month_revenue,
    ROUND(100.0 * (revenue - LAG(revenue, 30) OVER (ORDER BY date)) / 
        NULLIF(LAG(revenue, 30) OVER (ORDER BY date), 0), 2) as mom_change_pct
FROM daily_revenue
ORDER BY date DESC
LIMIT 30;
```

### Window Function Performance Tips

```sql
-- 1. Use FILTER clause with aggregates (PostgreSQL 9.4+)
SELECT 
    date,
    COUNT(*) FILTER (WHERE status = 'completed') OVER (
        ORDER BY date ROWS BETWEEN 6 PRECEDING AND CURRENT ROW
    ) as completed_last_7_days,
    COUNT(*) FILTER (WHERE status = 'failed') OVER (
        ORDER BY date ROWS BETWEEN 6 PRECEDING AND CURRENT ROW
    ) as failed_last_7_days
FROM orders;

-- 2. Avoid multiple passes with named windows
SELECT *
FROM (
    SELECT 
        id, name, salary, department,
        AVG(salary) OVER dept_window as dept_avg,
        MAX(salary) OVER dept_window as dept_max,
        COUNT(*) OVER dept_window as dept_count
    FROM employees
    WINDOW dept_window AS (PARTITION BY department)
) subq
WHERE salary > dept_avg;

-- 3. Index columns used in PARTITION BY and ORDER BY
CREATE INDEX idx_employee_dept_salary ON employees(department, salary DESC);
```

### Window Functions Quick Reference

```
+------------------------------------------------------------------+
|                    WINDOW FUNCTION QUICK REFERENCE                |
+------------------------------------------------------------------+
| RANKING FUNCTIONS:                                                |
|   ROW_NUMBER()  - Unique sequential number (1,2,3,4...)          |
|   RANK()        - Rank with gaps for ties (1,2,2,4...)           |
|   DENSE_RANK()  - Rank without gaps (1,2,2,3...)                 |
|   NTILE(n)      - Divide into n buckets                          |
|                                                                   |
| PERCENTILE FUNCTIONS:                                             |
|   PERCENT_RANK() - Relative rank as 0-1 (% below current row)    |
|   CUME_DIST()    - Cumulative distribution (% at or below)       |
|                                                                   |
| VALUE FUNCTIONS:                                                  |
|   LAG(col, n, default)   - Value from n rows before              |
|   LEAD(col, n, default)  - Value from n rows after               |
|   FIRST_VALUE(col)       - First value in window                 |
|   LAST_VALUE(col)        - Last value in window                  |
|   NTH_VALUE(col, n)      - Nth value in window                   |
|                                                                   |
| AGGREGATE FUNCTIONS (as window functions):                        |
|   SUM(), AVG(), COUNT(), MIN(), MAX(), STDDEV(), VARIANCE()      |
|                                                                   |
| FRAME SPECIFICATIONS:                                             |
|   ROWS BETWEEN x AND y                                           |
|   RANGE BETWEEN x AND y                                          |
|   GROUPS BETWEEN x AND y  (PostgreSQL 11+)                       |
|                                                                   |
|   Where x and y can be:                                          |
|   - UNBOUNDED PRECEDING                                          |
|   - n PRECEDING                                                  |
|   - CURRENT ROW                                                  |
|   - n FOLLOWING                                                  |
|   - UNBOUNDED FOLLOWING                                          |
+------------------------------------------------------------------+
```

> **Data Science Interview Tip:** Be ready to explain when to use ROWS vs RANGE vs GROUPS in frame specifications:
> - **ROWS**: Physical rows (most common, predictable)
> - **RANGE**: Logical range based on ORDER BY value (handles ties together)
> - **GROUPS**: Groups of peer rows (PostgreSQL 11+)

---

## Views - Virtual Tables

A view is a saved query that acts like a virtual table. Views don't store data themselves - they dynamically execute the underlying query when accessed.

```
+------------------------------------------------------------------+
|                    REGULAR VIEW vs MATERIALIZED VIEW              |
+------------------------------------------------------------------+
|                                                                   |
|   Regular View:                    Materialized View:             |
|   ┌─────────────┐                  ┌─────────────┐               |
|   │ Query runs  │                  │ Data stored │               |
|   │ every time  │                  │ on disk     │               |
|   │ you access  │                  │ (snapshot)  │               |
|   └─────────────┘                  └─────────────┘               |
|   ↓                                ↓                              |
|   Always up-to-date               Needs REFRESH                   |
|   Slower access                   Faster access                   |
|   No extra storage               Uses disk space                 |
|                                                                   |
+------------------------------------------------------------------+
```

### Creating Views

```sql
-- Sample employees table:
-- +----+---------+--------+-------------+
-- | id | name    | salary | department  |
-- +----+---------+--------+-------------+
-- | 1  | Alice   | 90000  | Engineering |
-- | 2  | Bob     | 55000  | Sales       |
-- | 3  | Charlie | 85000  | Engineering |
-- +----+---------+--------+-------------+

-- Create a view
CREATE VIEW high_earners AS
SELECT name, salary, department
FROM employees
WHERE salary > 80000;

-- The view is now like a virtual table:
SELECT * FROM high_earners;

-- Result (query executes dynamically):
-- +---------+--------+-------------+
-- | name    | salary | department  |
-- +---------+--------+-------------+
-- | Alice   | 90000  | Engineering |
-- | Charlie | 85000  | Engineering |
-- +---------+--------+-------------+

-- If underlying data changes, view reflects it immediately:
UPDATE employees SET salary = 95000 WHERE name = 'Bob';

SELECT * FROM high_earners;
-- Result now includes Bob (if his salary is now > 80000)

-- Create or replace (update existing view)
CREATE OR REPLACE VIEW high_earners AS
SELECT name, salary, department
FROM employees
WHERE salary > 90000;  -- Changed threshold

-- Drop view
DROP VIEW IF EXISTS high_earners;
```

**Use Cases for Views:**
- **Security**: Hide sensitive columns from certain users
- **Simplicity**: Provide a simple interface to complex joins
- **Abstraction**: Shield applications from schema changes

### Materialized Views (PostgreSQL-Specific!)

Materialized views store the query result physically - trades freshness for speed.

```sql
-- Create materialized view
CREATE MATERIALIZED VIEW sales_summary AS
SELECT
    date_trunc('month', order_date) as month,
    SUM(amount) as total_sales,
    COUNT(*) as order_count
FROM orders
GROUP BY date_trunc('month', order_date);

-- First access is instant (data is pre-computed):
SELECT * FROM sales_summary WHERE month = '2024-01-01';

-- Result (stored on disk, not computed):
-- +------------+-------------+-------------+
-- | month      | total_sales | order_count |
-- +------------+-------------+-------------+
-- | 2024-01-01 | 150000.00   | 423         |
-- +------------+-------------+-------------+

-- Problem: New orders won't appear until refresh!

-- Refresh the data (full refresh)
REFRESH MATERIALIZED VIEW sales_summary;
-- Re-runs the query, updates stored data
-- Table is LOCKED during refresh

-- Refresh concurrently (no lock, but requires unique index)
CREATE UNIQUE INDEX ON sales_summary (month);
REFRESH MATERIALIZED VIEW CONCURRENTLY sales_summary;
-- Users can query while refreshing!

-- Drop materialized view
DROP MATERIALIZED VIEW sales_summary;
```

**When to Use Materialized Views:**

| Scenario                              | Regular View | Materialized View |
| ------------------------------------- | ------------ | ----------------- |
| Data changes frequently               | ✓            |                   |
| Query is expensive                    |              | ✓                 |
| Real-time accuracy needed             | ✓            |                   |
| Query runs frequently                 |              | ✓                 |
| Can tolerate slightly stale data      |              | ✓                 |

---

## Indexes

Indexes are data structures that speed up data retrieval at the cost of additional storage and slower writes. Understanding indexes is crucial for database performance and interviews.

```
+------------------------------------------------------------------+
|                    HOW INDEXES WORK                               |
+------------------------------------------------------------------+
|                                                                   |
|   Without Index (Full Table Scan):                                |
|   +---+---+---+---+---+---+---+---+---+---+                       |
|   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10|  ← Check ALL rows    |
|   +---+---+---+---+---+---+---+---+---+---+                       |
|   O(n) - Must scan every row to find matches                      |
|                                                                   |
|   With B-Tree Index:                                              |
|              [5]                                                  |
|             /   \                                                 |
|          [2,3]  [7,8]    ← Binary search tree structure           |
|         /  |  \    \                                              |
|        [1] [2] [3] [4]...                                         |
|   O(log n) - Jump directly to relevant rows                       |
|                                                                   |
+------------------------------------------------------------------+
```

### When to Use Indexes

```
+------------------------------------------------------------------+
|        GOOD for Indexes           |    BAD for Indexes           |
+------------------------------------------------------------------+
| ✓ Columns in WHERE clauses       | ✗ Small tables               |
| ✓ Columns in JOIN conditions     | ✗ Frequently updated columns |
| ✓ Columns in ORDER BY            | ✗ Low selectivity columns    |
| ✓ Foreign key columns            |   (e.g., boolean, gender)    |
| ✓ High-selectivity columns       | ✗ Columns rarely queried     |
+------------------------------------------------------------------+
```

### Creating Indexes

```sql
-- Basic B-tree index (default)
CREATE INDEX idx_employee_name ON employees(last_name);

-- After creating index, this query is fast:
SELECT * FROM employees WHERE last_name = 'Smith';
-- Without index: Full table scan O(n)
-- With index: B-tree lookup O(log n)

-- Unique index (also enforces uniqueness)
CREATE UNIQUE INDEX idx_employee_email ON employees(email);

-- Composite index (multiple columns)
CREATE INDEX idx_emp_dept_salary ON employees(department, salary);

-- This index helps these queries:
-- ✓ WHERE department = 'Sales'
-- ✓ WHERE department = 'Sales' AND salary > 50000
-- ✗ WHERE salary > 50000  (leftmost column not used!)

-- Order matters! Think of it like a phone book:
-- (last_name, first_name) helps find "Smith, John"
-- But doesn't help find all "John"s without last name
```

### PostgreSQL-Specific Index Types

**Partial Index - Index only some rows:**

```sql
CREATE INDEX idx_active_employees ON employees(last_name)
WHERE is_active = TRUE;

-- Only indexes active employees
-- Smaller index = faster lookups for active employee queries
-- Great when you mostly query a subset of data

-- This query uses the partial index:
SELECT * FROM employees WHERE is_active = TRUE AND last_name = 'Smith';

-- This query CANNOT use the partial index:
SELECT * FROM employees WHERE is_active = FALSE AND last_name = 'Smith';
```

**Expression Index - Index on computed values:**

```sql
CREATE INDEX idx_lower_email ON employees(LOWER(email));

-- Helps this query:
SELECT * FROM employees WHERE LOWER(email) = 'john@example.com';

-- Without expression index, database can't use index because
-- LOWER(email) != email column
```

**GIN Index - For arrays, JSONB, full-text search:**

```sql
-- For JSONB columns
CREATE INDEX idx_data ON events USING GIN(data);

-- Dramatically speeds up containment queries:
SELECT * FROM events WHERE data @> '{"type": "click"}';

-- For array columns
CREATE INDEX idx_tags ON posts USING GIN(tags);

-- Speeds up:
SELECT * FROM posts WHERE tags @> ARRAY['postgresql', 'database'];
SELECT * FROM posts WHERE 'postgresql' = ANY(tags);
```

**BRIN Index - For large, naturally ordered data:**

```sql
CREATE INDEX idx_created_at ON logs USING BRIN(created_at);

-- BRIN = Block Range Index
-- Very small index size (stores min/max per block)
-- Perfect for timestamp columns where data is inserted in order
--
-- Trade-off: Less precise than B-tree, but tiny storage overhead
-- Great for: Time-series data, log tables, append-only tables
```

### Index Types Summary

| Type    | Use Case                                      | Example                              |
| ------- | --------------------------------------------- | ------------------------------------ |
| B-tree  | Default, equality and range queries           | `WHERE id = 5`, `WHERE age > 30`     |
| Hash    | Equality comparisons only                     | `WHERE email = 'x@y.com'`            |
| GIN     | Arrays, JSONB, full-text search               | `WHERE tags @> ARRAY['sql']`         |
| GiST    | Geometric data, full-text, range types        | `WHERE location && box`              |
| SP-GiST | Space-partitioned data (IP addresses, etc.)   | `WHERE ip << '192.168.0.0/16'`       |
| BRIN    | Large tables with naturally ordered data      | `WHERE created_at > '2024-01-01'`    |

### Viewing Index Usage

```sql
-- Check if query uses index (EXPLAIN)
EXPLAIN SELECT * FROM employees WHERE email = 'john@example.com';

-- Output without index:
-- Seq Scan on employees  (cost=0.00..10.50 rows=1 width=100)
--   Filter: (email = 'john@example.com'::text)

-- Output with index:
-- Index Scan using idx_employee_email on employees  (cost=0.15..8.17 rows=1 width=100)
--   Index Cond: (email = 'john@example.com'::text)

-- EXPLAIN ANALYZE for actual execution stats
EXPLAIN ANALYZE SELECT * FROM employees WHERE email = 'john@example.com';

-- List all indexes on a table
\di employees*

-- Or using SQL:
SELECT indexname, indexdef
FROM pg_indexes
WHERE tablename = 'employees';
```

### Index Best Practices

```
1. Don't over-index
   - Each index slows down INSERT/UPDATE/DELETE
   - Each index uses disk space
   - Start with indexes on: PKs (automatic), FKs, common WHERE columns

2. Monitor index usage
   SELECT schemaname, tablename, indexname, idx_scan
   FROM pg_stat_user_indexes
   ORDER BY idx_scan;
   -- Indexes with low idx_scan might be candidates for removal

3. Consider composite index order
   - Most selective column first (usually)
   - Match your WHERE clause patterns

4. Rebuild indexes periodically
   REINDEX INDEX idx_name;
   -- Or use VACUUM to clean up dead tuples
```

```sql
-- Drop index
DROP INDEX IF EXISTS idx_employee_name;

-- Drop index without locking table (PostgreSQL)
DROP INDEX CONCURRENTLY idx_employee_name;
```

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

These patterns appear frequently in SQL interviews. Master them and you'll handle most interview questions.

### Pattern 1: Find Nth Highest Salary

One of the most common interview questions. Multiple approaches exist.

```sql
-- Sample data:
-- +----+---------+--------+
-- | id | name    | salary |
-- +----+---------+--------+
-- | 1  | Alice   | 90000  |
-- | 2  | Bob     | 70000  |
-- | 3  | Charlie | 80000  |
-- | 4  | Diana   | 70000  |  <-- Tie with Bob
-- | 5  | Eve     | 60000  |
-- +----+---------+--------+
-- Sorted: 90000, 80000, 70000, 70000, 60000

-- Method 1: Using LIMIT OFFSET (find 2nd highest)
SELECT DISTINCT salary
FROM employees
ORDER BY salary DESC
LIMIT 1 OFFSET 1;   -- Skip 1, take 1

-- Step-by-step:
-- 1. Sort by salary DESC: 90000, 80000, 70000, 60000 (DISTINCT removes duplicate 70000)
-- 2. OFFSET 1: Skip first row (90000)
-- 3. LIMIT 1: Take next row (80000)

-- Result:
-- +--------+
-- | salary |
-- +--------+
-- | 80000  |
-- +--------+

-- Method 2: Using DENSE_RANK (handles ties correctly)
SELECT salary FROM (
    SELECT salary, DENSE_RANK() OVER (ORDER BY salary DESC) as rank
    FROM employees
) ranked
WHERE rank = 2;

-- Step-by-step:
-- 1. Assign ranks:
--    salary=90000 → rank=1
--    salary=80000 → rank=2
--    salary=70000 → rank=3 (appears twice, both rank 3)
--    salary=60000 → rank=4
-- 2. Filter where rank = 2

-- Result:
-- +--------+
-- | salary |
-- +--------+
-- | 80000  |
-- +--------+

-- Note: RANK would give: 1, 2, 3, 3, 5 (skips 4)
--       DENSE_RANK gives: 1, 2, 3, 3, 4 (no gaps)
```

### Pattern 2: Find Duplicates

```sql
-- Sample data:
-- +----+-------------------+
-- | id | email             |
-- +----+-------------------+
-- | 1  | alice@example.com |
-- | 2  | bob@example.com   |
-- | 3  | alice@example.com |  <-- Duplicate!
-- | 4  | charlie@test.com  |
-- | 5  | bob@example.com   |  <-- Duplicate!
-- +----+-------------------+

SELECT email, COUNT(*) as count
FROM employees
GROUP BY email
HAVING COUNT(*) > 1;

-- Step-by-step:
-- 1. GROUP BY email creates groups
-- 2. COUNT(*) counts occurrences
-- 3. HAVING filters groups with count > 1

-- Result:
-- +-------------------+-------+
-- | email             | count |
-- +-------------------+-------+
-- | alice@example.com | 2     |
-- | bob@example.com   | 2     |
-- +-------------------+-------+

-- To delete duplicates (keep lowest id):
DELETE FROM employees
WHERE id NOT IN (
    SELECT MIN(id)
    FROM employees
    GROUP BY email
);

-- After deletion:
-- +----+-------------------+
-- | id | email             |
-- +----+-------------------+
-- | 1  | alice@example.com |
-- | 2  | bob@example.com   |
-- | 4  | charlie@test.com  |
-- +----+-------------------+
```

### Pattern 3: Employee-Manager Hierarchy

```sql
-- Sample data (self-referencing table):
-- +----+---------+------------+
-- | id | name    | manager_id |
-- +----+---------+------------+
-- | 1  | CEO     | NULL       |
-- | 2  | Alice   | 1          |
-- | 3  | Bob     | 1          |
-- | 4  | Charlie | 2          |
-- +----+---------+------------+

SELECT
    e.name as employee,
    m.name as manager
FROM employees e
LEFT JOIN employees m ON e.manager_id = m.id;

-- Self-join visualization:
-- e (employees)          m (employees as managers)
-- +----+-------+---+     +----+-----+
-- | id | name  |mgr|     | id | name|
-- +----+-------+---+     +----+-----+
-- | 1  | CEO   |NULL     | 1  | CEO |
-- | 2  | Alice | 1 | --> | 1  | CEO |  (Alice's manager is CEO)
-- | 3  | Bob   | 1 | --> | 1  | CEO |  (Bob's manager is CEO)
-- | 4  | Charlie| 2| --> | 2  | Alice| (Charlie's manager is Alice)
-- +----+-------+---+     +----+-----+

-- Result:
-- +----------+---------+
-- | employee | manager |
-- +----------+---------+
-- | CEO      | NULL    |  <-- CEO has no manager
-- | Alice    | CEO     |
-- | Bob      | CEO     |
-- | Charlie  | Alice   |
-- +----------+---------+
```

### Pattern 4: Employees Without Department

Find orphan records using LEFT JOIN + NULL check.

```sql
-- employees:              departments:
-- +----+-------+---------+ +----+-------------+
-- | id | name  | dept_id | | id | name        |
-- +----+-------+---------+ +----+-------------+
-- | 1  | Alice | 1       | | 1  | Engineering |
-- | 2  | Bob   | 2       | | 2  | Sales       |
-- | 3  | Charlie| NULL   |
-- | 4  | Diana | 99      |  <-- dept_id=99 doesn't exist!
-- +----+-------+---------+

SELECT e.name
FROM employees e
LEFT JOIN departments d ON e.dept_id = d.id
WHERE d.id IS NULL;

-- Step-by-step:
-- 1. LEFT JOIN keeps all employees
-- 2. Charlie (NULL dept_id) → no match → d.id is NULL
-- 3. Diana (dept_id=99) → no match → d.id is NULL
-- 4. WHERE d.id IS NULL → filters to only unmatched

-- Result:
-- +---------+
-- | name    |
-- +---------+
-- | Charlie |
-- | Diana   |
-- +---------+
```

### Pattern 5: Running Total

```sql
-- orders:
-- +----+------------+--------+
-- | id | order_date | amount |
-- +----+------------+--------+
-- | 1  | 2024-01-01 | 100    |
-- | 2  | 2024-01-02 | 200    |
-- | 3  | 2024-01-03 | 150    |
-- | 4  | 2024-01-04 | 300    |
-- +----+------------+--------+

SELECT
    order_date,
    amount,
    SUM(amount) OVER (ORDER BY order_date) as running_total
FROM orders;

-- Step-by-step:
-- Row 1: SUM of rows up to 2024-01-01 = 100
-- Row 2: SUM of rows up to 2024-01-02 = 100 + 200 = 300
-- Row 3: SUM of rows up to 2024-01-03 = 100 + 200 + 150 = 450
-- Row 4: SUM of rows up to 2024-01-04 = 100 + 200 + 150 + 300 = 750

-- Result:
-- +------------+--------+---------------+
-- | order_date | amount | running_total |
-- +------------+--------+---------------+
-- | 2024-01-01 | 100    | 100           |
-- | 2024-01-02 | 200    | 300           |
-- | 2024-01-03 | 150    | 450           |
-- | 2024-01-04 | 300    | 750           |
-- +------------+--------+---------------+
```

### Pattern 6: Top N per Group

Get top N records within each group (e.g., top 2 salaries per department).

```sql
-- employees:
-- +----+---------+--------+-------------+
-- | id | name    | salary | department  |
-- +----+---------+--------+-------------+
-- | 1  | Alice   | 90000  | Engineering |
-- | 2  | Bob     | 55000  | Sales       |
-- | 3  | Charlie | 80000  | Engineering |
-- | 4  | Diana   | 60000  | Sales       |
-- | 5  | Eve     | 70000  | Engineering |
-- +----+---------+--------+-------------+

-- Method 1: Using window function
SELECT * FROM (
    SELECT name, department, salary,
        ROW_NUMBER() OVER (PARTITION BY department ORDER BY salary DESC) as rn
    FROM employees
) ranked
WHERE rn <= 2;

-- Step-by-step:
-- 1. PARTITION BY department: Create separate windows
-- 2. ORDER BY salary DESC: Sort within each partition
-- 3. ROW_NUMBER(): Assign 1, 2, 3... within each partition

-- Intermediate result (before filtering):
-- +---------+-------------+--------+----+
-- | name    | department  | salary | rn |
-- +---------+-------------+--------+----+
-- | Alice   | Engineering | 90000  | 1  |  <-- #1 in Engineering
-- | Charlie | Engineering | 80000  | 2  |  <-- #2 in Engineering
-- | Eve     | Engineering | 70000  | 3  |
-- | Diana   | Sales       | 60000  | 1  |  <-- #1 in Sales
-- | Bob     | Sales       | 55000  | 2  |  <-- #2 in Sales
-- +---------+-------------+--------+----+

-- Final result (WHERE rn <= 2):
-- +---------+-------------+--------+
-- | name    | department  | salary |
-- +---------+-------------+--------+
-- | Alice   | Engineering | 90000  |
-- | Charlie | Engineering | 80000  |
-- | Diana   | Sales       | 60000  |
-- | Bob     | Sales       | 55000  |
-- +---------+-------------+--------+

-- Method 2: Using LATERAL (PostgreSQL-specific, often more efficient)
SELECT d.dept_name, e.name, e.salary
FROM departments d
LEFT JOIN LATERAL (
    SELECT name, salary
    FROM employees
    WHERE department = d.dept_name
    ORDER BY salary DESC
    LIMIT 2
) e ON TRUE;

-- LATERAL allows the subquery to reference outer table (d)
-- Runs subquery once per department, getting top 2 each time
```

### Pattern 7: Consecutive Days/Gaps

Find users with activity on consecutive days.

```sql
-- user_logins:
-- +----+---------+------------+
-- | id | user_id | login_date |
-- +----+---------+------------+
-- | 1  | 1       | 2024-01-01 |
-- | 2  | 1       | 2024-01-02 |
-- | 3  | 1       | 2024-01-03 |  <-- 3 consecutive days!
-- | 4  | 1       | 2024-01-05 |
-- | 5  | 2       | 2024-01-01 |
-- +----+---------+------------+

-- Find users with 3+ consecutive login days
WITH numbered_logins AS (
    SELECT
        user_id,
        login_date,
        login_date - ROW_NUMBER() OVER (PARTITION BY user_id ORDER BY login_date) * INTERVAL '1 day' as group_date
    FROM user_logins
)
SELECT user_id, COUNT(*) as consecutive_days
FROM numbered_logins
GROUP BY user_id, group_date
HAVING COUNT(*) >= 3;

-- The trick: For consecutive dates, date - row_number gives same value

-- Example for user_id=1:
-- login_date | row_num | login_date - row_num
-- 2024-01-01 | 1       | 2024-01-01 - 1 = 2023-12-31
-- 2024-01-02 | 2       | 2024-01-02 - 2 = 2023-12-31  <-- Same!
-- 2024-01-03 | 3       | 2024-01-03 - 3 = 2023-12-31  <-- Same!
-- 2024-01-05 | 4       | 2024-01-05 - 4 = 2024-01-01  <-- Different (gap!)

-- Result:
-- +---------+------------------+
-- | user_id | consecutive_days |
-- +---------+------------------+
-- | 1       | 3                |
-- +---------+------------------+
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
