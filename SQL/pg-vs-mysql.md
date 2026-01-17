# PostgreSQL vs MySQL - Complete Comparison Guide

> A comprehensive comparison of PostgreSQL and MySQL for tech interviews. This guide covers syntax differences, feature comparisons, and when to use each database.

---

## Quick Overview

```
+------------------------------------------------------------------+
|              PostgreSQL vs MySQL AT A GLANCE                      |
+------------------------------------------------------------------+
|                                                                   |
|   PostgreSQL                       MySQL                          |
|   ┌─────────────────────┐         ┌─────────────────────┐        |
|   │ "The World's Most   │         │ "The World's Most   │        |
|   │  Advanced Open      │         │  Popular Open       │        |
|   │  Source Database"   │         │  Source Database"   │        |
|   └─────────────────────┘         └─────────────────────┘        |
|                                                                   |
|   • ORDBMS (Object-Relational)    • RDBMS (Relational)           |
|   • Standards compliant           • Speed-focused                |
|   • Feature-rich                  • Simple and fast              |
|   • Complex queries               • Read-heavy workloads         |
|                                                                   |
+------------------------------------------------------------------+
```

| Aspect | PostgreSQL | MySQL |
|--------|------------|-------|
| **Type** | ORDBMS (Object-Relational) | RDBMS (Relational) |
| **Philosophy** | Standards compliance, extensibility | Speed, simplicity |
| **Best For** | Complex queries, data integrity | Web apps, read-heavy workloads |
| **License** | PostgreSQL License (permissive) | GPL (with commercial options) |
| **Default Port** | 5432 | 3306 |
| **Case Sensitivity** | Identifiers lowercase by default | Depends on OS/collation |

---

## Database & Connection Commands

### Database Management

| Operation | PostgreSQL | MySQL |
|-----------|------------|-------|
| Create database | `CREATE DATABASE dbname;` | `CREATE DATABASE dbname;` |
| Create with encoding | `CREATE DATABASE dbname WITH ENCODING 'UTF8';` | `CREATE DATABASE dbname CHARACTER SET utf8mb4;` |
| Drop database | `DROP DATABASE dbname;` | `DROP DATABASE dbname;` |
| List databases | `\l` or `SELECT datname FROM pg_database;` | `SHOW DATABASES;` |
| Connect to database | `\c dbname` | `USE dbname;` |
| Current database | `SELECT current_database();` | `SELECT DATABASE();` |
| List tables | `\dt` or query `information_schema.tables` | `SHOW TABLES;` |
| Describe table | `\d tablename` | `DESCRIBE tablename;` or `SHOW COLUMNS FROM tablename;` |

### Connection Examples

```sql
-- PostgreSQL (psql)
psql -h localhost -U username -d database_name
\c another_database  -- Switch database

-- MySQL (mysql client)
mysql -h localhost -u username -p database_name
USE another_database;  -- Switch database
```

---

## Data Types Comparison

### Auto-Increment / Serial

| PostgreSQL | MySQL |
|------------|-------|
| `SERIAL` | `AUTO_INCREMENT` |
| `BIGSERIAL` | `BIGINT AUTO_INCREMENT` |
| `SMALLSERIAL` | `SMALLINT AUTO_INCREMENT` |
| `GENERATED AS IDENTITY` (SQL standard) | Not supported |

```sql
-- PostgreSQL
CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100)
);

-- Or SQL-standard way (PostgreSQL 10+)
CREATE TABLE users (
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    name VARCHAR(100)
);

-- MySQL
CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100)
);
```

### String Types

| Type | PostgreSQL | MySQL |
|------|------------|-------|
| Fixed-length | `CHAR(n)` | `CHAR(n)` |
| Variable-length | `VARCHAR(n)` | `VARCHAR(n)` |
| Unlimited text | `TEXT` (no performance penalty) | `TEXT` (up to 65KB), `MEDIUMTEXT`, `LONGTEXT` |
| Binary | `BYTEA` | `BLOB`, `MEDIUMBLOB`, `LONGBLOB` |

**Key Difference:** In PostgreSQL, `TEXT` has no performance penalty vs `VARCHAR`. In MySQL, `TEXT` types have limitations and can't have default values.

```sql
-- PostgreSQL: TEXT and VARCHAR are essentially the same
CREATE TABLE posts (
    content TEXT  -- Recommended for variable-length strings
);

-- MySQL: TEXT types have limitations
CREATE TABLE posts (
    content LONGTEXT  -- Up to 4GB, but can't have default value
);
```

### Numeric Types

| Purpose | PostgreSQL | MySQL |
|---------|------------|-------|
| Small integer | `SMALLINT` | `SMALLINT` or `TINYINT` |
| Integer | `INTEGER` or `INT` | `INT` |
| Big integer | `BIGINT` | `BIGINT` |
| Decimal (exact) | `NUMERIC(p,s)` or `DECIMAL(p,s)` | `DECIMAL(p,s)` |
| Float | `REAL` | `FLOAT` |
| Double | `DOUBLE PRECISION` | `DOUBLE` |

**MySQL-only types:**
- `TINYINT` (-128 to 127)
- `MEDIUMINT` (-8M to 8M)
- `UNSIGNED` modifier (e.g., `INT UNSIGNED`)

```sql
-- PostgreSQL: No unsigned types, use CHECK constraint instead
CREATE TABLE products (
    quantity INTEGER CHECK (quantity >= 0)
);

-- MySQL: Has unsigned types
CREATE TABLE products (
    quantity INT UNSIGNED
);
```

### Boolean Type

| PostgreSQL | MySQL |
|------------|-------|
| `BOOLEAN` (true/false/null) | `BOOLEAN` (alias for `TINYINT(1)`) |
| Accepts: `TRUE`, `FALSE`, `'yes'`, `'no'`, `'t'`, `'f'` | Accepts: `1`, `0`, `TRUE`, `FALSE` |

```sql
-- PostgreSQL
INSERT INTO users (is_active) VALUES (TRUE);
INSERT INTO users (is_active) VALUES ('yes');
SELECT * FROM users WHERE is_active = TRUE;

-- MySQL
INSERT INTO users (is_active) VALUES (1);
INSERT INTO users (is_active) VALUES (TRUE);  -- Stored as 1
SELECT * FROM users WHERE is_active = 1;
```

### Date/Time Types

| Purpose | PostgreSQL | MySQL |
|---------|------------|-------|
| Date only | `DATE` | `DATE` |
| Time only | `TIME` | `TIME` |
| Time with timezone | `TIME WITH TIME ZONE` | Not supported |
| Timestamp | `TIMESTAMP` | `DATETIME` |
| Timestamp with TZ | `TIMESTAMP WITH TIME ZONE` (TIMESTAMPTZ) | `TIMESTAMP` (stores in UTC) |
| Time interval | `INTERVAL` | Not supported |

```sql
-- PostgreSQL: Rich date/time support
SELECT NOW() + INTERVAL '1 day';
SELECT NOW() - INTERVAL '2 hours 30 minutes';
SELECT age(TIMESTAMP '2024-01-15', TIMESTAMP '1990-06-15');
SELECT CURRENT_TIMESTAMP AT TIME ZONE 'America/New_York';

-- MySQL: More limited
SELECT NOW() + INTERVAL 1 DAY;
SELECT DATE_ADD(NOW(), INTERVAL 2 HOUR);
SELECT TIMESTAMPDIFF(YEAR, '1990-06-15', '2024-01-15');
SELECT CONVERT_TZ(NOW(), 'UTC', 'America/New_York');
```

### Special PostgreSQL Types (Not in MySQL)

| Type | Description | Example |
|------|-------------|---------|
| `UUID` | Universally Unique Identifier | `gen_random_uuid()` |
| `JSONB` | Binary JSON (indexable) | `'{"key": "value"}'::jsonb` |
| `ARRAY` | Array of any type | `ARRAY[1, 2, 3]` |
| `HSTORE` | Key-value pairs | `'key1=>value1'` |
| `INET` | IPv4/IPv6 address | `'192.168.1.1'` |
| `CIDR` | IP network | `'192.168.1.0/24'` |
| `MACADDR` | MAC address | `'08:00:2b:01:02:03'` |
| `TSRANGE` | Timestamp range | `'[2024-01-01, 2024-12-31)'` |
| `POINT`, `LINE`, `POLYGON` | Geometric types | `POINT(1, 2)` |

```sql
-- PostgreSQL ARRAY example
CREATE TABLE students (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100),
    scores INTEGER[]
);
INSERT INTO students (name, scores) VALUES ('Alice', ARRAY[85, 90, 92]);
SELECT * FROM students WHERE 90 = ANY(scores);

-- PostgreSQL JSONB example
CREATE TABLE events (
    id SERIAL PRIMARY KEY,
    data JSONB
);
INSERT INTO events (data) VALUES ('{"type": "click", "page": "/home"}');
SELECT * FROM events WHERE data @> '{"type": "click"}';

-- MySQL alternative: JSON type (less powerful)
CREATE TABLE events (
    id INT AUTO_INCREMENT PRIMARY KEY,
    data JSON
);
INSERT INTO events (data) VALUES ('{"type": "click", "page": "/home"}');
SELECT * FROM events WHERE JSON_CONTAINS(data, '"click"', '$.type');
```

---

## Syntax Differences

### String Concatenation

```sql
-- PostgreSQL: || operator or CONCAT()
SELECT first_name || ' ' || last_name AS full_name FROM users;
SELECT CONCAT(first_name, ' ', last_name) AS full_name FROM users;

-- MySQL: CONCAT() only (|| is logical OR by default)
SELECT CONCAT(first_name, ' ', last_name) AS full_name FROM users;
```

### Case-Insensitive Search

```sql
-- PostgreSQL: ILIKE (case-insensitive LIKE)
SELECT * FROM users WHERE name ILIKE '%john%';
SELECT * FROM users WHERE name ~* 'john';  -- Case-insensitive regex

-- MySQL: LIKE is case-insensitive by default (depends on collation)
SELECT * FROM users WHERE name LIKE '%john%';
-- Or explicit:
SELECT * FROM users WHERE LOWER(name) LIKE '%john%';
```

### Type Casting

```sql
-- PostgreSQL: :: operator or CAST()
SELECT '123'::INTEGER;
SELECT CAST('123' AS INTEGER);
SELECT '2024-01-15'::DATE;

-- MySQL: CAST() only
SELECT CAST('123' AS SIGNED);
SELECT CAST('2024-01-15' AS DATE);
```

### LIMIT and OFFSET

```sql
-- PostgreSQL
SELECT * FROM users LIMIT 10 OFFSET 20;

-- MySQL (both syntaxes work)
SELECT * FROM users LIMIT 10 OFFSET 20;
SELECT * FROM users LIMIT 20, 10;  -- LIMIT offset, count (confusing order!)
```

### NULLS FIRST / NULLS LAST

```sql
-- PostgreSQL: Native support
SELECT * FROM users ORDER BY name NULLS FIRST;
SELECT * FROM users ORDER BY name DESC NULLS LAST;

-- MySQL: Workaround needed
SELECT * FROM users ORDER BY name IS NULL DESC, name;
SELECT * FROM users ORDER BY CASE WHEN name IS NULL THEN 0 ELSE 1 END, name;
```

### Regular Expressions

```sql
-- PostgreSQL: POSIX regex operators
SELECT * FROM users WHERE name ~ '^[A-Z]';      -- Starts with uppercase
SELECT * FROM users WHERE name ~* '^john';      -- Case-insensitive
SELECT * FROM users WHERE name !~ '[0-9]';      -- Doesn't contain digit

-- Also: SIMILAR TO (SQL standard regex)
SELECT * FROM users WHERE name SIMILAR TO '(John|Jane)%';

-- MySQL: REGEXP operator
SELECT * FROM users WHERE name REGEXP '^[A-Z]';
SELECT * FROM users WHERE name REGEXP '^john';  -- Case-insensitive by default
SELECT * FROM users WHERE name NOT REGEXP '[0-9]';
```

### Current Date/Time Functions

| Purpose | PostgreSQL | MySQL |
|---------|------------|-------|
| Current timestamp | `NOW()` or `CURRENT_TIMESTAMP` | `NOW()` or `CURRENT_TIMESTAMP` |
| Current date | `CURRENT_DATE` | `CURDATE()` or `CURRENT_DATE` |
| Current time | `CURRENT_TIME` | `CURTIME()` or `CURRENT_TIME` |
| Uppercase | `UPPER()` | `UPPER()` or `UCASE()` |
| Lowercase | `LOWER()` | `LOWER()` or `LCASE()` |

---

## INSERT, UPDATE, DELETE Differences

### INSERT with RETURNING

```sql
-- PostgreSQL: RETURNING clause (very useful!)
INSERT INTO users (name, email) VALUES ('John', 'john@test.com')
RETURNING id, name, created_at;
-- Returns the inserted row(s) including generated values

-- MySQL: No RETURNING, use LAST_INSERT_ID()
INSERT INTO users (name, email) VALUES ('John', 'john@test.com');
SELECT LAST_INSERT_ID();
-- Only gets the auto-increment ID, not other generated values
```

### UPDATE with RETURNING

```sql
-- PostgreSQL: RETURNING after UPDATE
UPDATE users SET salary = salary * 1.1 WHERE department = 'Sales'
RETURNING id, name, salary;
-- Returns all updated rows

-- MySQL: No equivalent (use SELECT before/after or stored procedure)
UPDATE users SET salary = salary * 1.1 WHERE department = 'Sales';
SELECT * FROM users WHERE department = 'Sales';
```

### UPDATE with JOIN

```sql
-- PostgreSQL: UPDATE ... FROM
UPDATE employees e
SET salary = e.salary * d.bonus_multiplier
FROM departments d
WHERE e.department_id = d.id;

-- MySQL: UPDATE ... JOIN
UPDATE employees e
JOIN departments d ON e.department_id = d.id
SET e.salary = e.salary * d.bonus_multiplier;
```

### DELETE with JOIN

```sql
-- PostgreSQL: DELETE ... USING
DELETE FROM employees e
USING departments d
WHERE e.department_id = d.id AND d.name = 'Closed';

-- MySQL: DELETE ... JOIN
DELETE e FROM employees e
JOIN departments d ON e.department_id = d.id
WHERE d.name = 'Closed';
```

### UPSERT (Insert or Update)

```sql
-- PostgreSQL: ON CONFLICT
INSERT INTO users (id, name, email)
VALUES (1, 'John', 'john@test.com')
ON CONFLICT (id) DO UPDATE SET
    name = EXCLUDED.name,
    email = EXCLUDED.email;

-- With DO NOTHING
INSERT INTO users (id, name, email)
VALUES (1, 'John', 'john@test.com')
ON CONFLICT (id) DO NOTHING;

-- MySQL: ON DUPLICATE KEY UPDATE
INSERT INTO users (id, name, email)
VALUES (1, 'John', 'john@test.com')
ON DUPLICATE KEY UPDATE
    name = VALUES(name),
    email = VALUES(email);

-- MySQL: REPLACE (deletes then inserts)
REPLACE INTO users (id, name, email)
VALUES (1, 'John', 'john@test.com');
```

### TRUNCATE

```sql
-- PostgreSQL: Can be rolled back, has RESTART IDENTITY
BEGIN;
TRUNCATE TABLE users RESTART IDENTITY;  -- Reset serial counter
ROLLBACK;  -- Works! Table is restored

TRUNCATE TABLE users, orders CASCADE;  -- Cascade to dependent tables

-- MySQL: Cannot be rolled back
TRUNCATE TABLE users;  -- Auto-increment resets automatically
-- No CASCADE option, must truncate in order
```

---

## JOIN Differences

### FULL OUTER JOIN

```sql
-- PostgreSQL: Native support
SELECT e.name, d.dept_name
FROM employees e
FULL OUTER JOIN departments d ON e.dept_id = d.id;

-- MySQL: Must emulate with UNION
SELECT e.name, d.dept_name
FROM employees e
LEFT JOIN departments d ON e.dept_id = d.id
UNION
SELECT e.name, d.dept_name
FROM employees e
RIGHT JOIN departments d ON e.dept_id = d.id;
```

### LATERAL JOIN

```sql
-- PostgreSQL: Native LATERAL support (like correlated subquery in FROM)
SELECT c.name, top_orders.order_date, top_orders.amount
FROM customers c
LEFT JOIN LATERAL (
    SELECT order_date, amount
    FROM orders
    WHERE customer_id = c.id  -- References outer query!
    ORDER BY amount DESC
    LIMIT 3
) top_orders ON TRUE;

-- MySQL 8.0+: LATERAL supported
SELECT c.name, top_orders.order_date, top_orders.amount
FROM customers c
LEFT JOIN LATERAL (
    SELECT order_date, amount
    FROM orders
    WHERE customer_id = c.id
    ORDER BY amount DESC
    LIMIT 3
) top_orders ON TRUE;

-- MySQL < 8.0: Must use correlated subquery or other workarounds
```

### NATURAL JOIN

```sql
-- Both support NATURAL JOIN (joins on matching column names)
-- PostgreSQL
SELECT * FROM employees NATURAL JOIN departments;

-- MySQL
SELECT * FROM employees NATURAL JOIN departments;

-- Warning: Avoid in production - fragile if schema changes
```

---

## SET Operations

### INTERSECT and EXCEPT

```sql
-- PostgreSQL: Native support for all set operations
SELECT name FROM employees_usa
INTERSECT
SELECT name FROM employees_india;

SELECT name FROM employees_usa
EXCEPT
SELECT name FROM employees_india;

-- With ALL to keep duplicates
SELECT name FROM employees_usa
EXCEPT ALL
SELECT name FROM employees_india;

-- MySQL: No native INTERSECT/EXCEPT (must emulate)

-- INTERSECT emulation
SELECT DISTINCT a.name
FROM employees_usa a
INNER JOIN employees_india b ON a.name = b.name;

-- EXCEPT/MINUS emulation
SELECT a.name
FROM employees_usa a
LEFT JOIN employees_india b ON a.name = b.name
WHERE b.name IS NULL;
```

---

## Window Functions

Both PostgreSQL and MySQL (8.0+) support window functions, but PostgreSQL has more features:

### DISTINCT ON (PostgreSQL-only)

```sql
-- PostgreSQL: Get first row per group
SELECT DISTINCT ON (department) department, name, salary
FROM employees
ORDER BY department, salary DESC;
-- Returns highest-paid employee per department

-- MySQL: Must use subquery with ROW_NUMBER
SELECT department, name, salary FROM (
    SELECT department, name, salary,
        ROW_NUMBER() OVER (PARTITION BY department ORDER BY salary DESC) as rn
    FROM employees
) ranked
WHERE rn = 1;
```

### Window Frame Differences

```sql
-- Both support basic window functions
SELECT name, salary,
    ROW_NUMBER() OVER (ORDER BY salary DESC) as rank,
    SUM(salary) OVER (ORDER BY hire_date) as running_total
FROM employees;

-- PostgreSQL: More frame options
SELECT name, salary,
    AVG(salary) OVER (
        ORDER BY hire_date
        ROWS BETWEEN 2 PRECEDING AND CURRENT ROW
    ) as moving_avg
FROM employees;
```

---

## Transactions

### Transaction Syntax

```sql
-- PostgreSQL
BEGIN;  -- or START TRANSACTION
    UPDATE accounts SET balance = balance - 100 WHERE id = 1;
    UPDATE accounts SET balance = balance + 100 WHERE id = 2;
COMMIT;
-- or ROLLBACK;

-- MySQL
START TRANSACTION;  -- or BEGIN
    UPDATE accounts SET balance = balance - 100 WHERE id = 1;
    UPDATE accounts SET balance = balance + 100 WHERE id = 2;
COMMIT;
-- or ROLLBACK;
```

### Savepoints

```sql
-- Both support savepoints
BEGIN;
    INSERT INTO orders VALUES (1, 'Order 1');
    SAVEPOINT my_savepoint;
    INSERT INTO orders VALUES (2, 'Order 2');
    ROLLBACK TO SAVEPOINT my_savepoint;  -- Undo Order 2
    INSERT INTO orders VALUES (3, 'Order 3');
COMMIT;
-- Result: Orders 1 and 3 are committed
```

### Transaction Isolation Levels

| Level | PostgreSQL | MySQL |
|-------|------------|-------|
| Read Uncommitted | Treated as Read Committed | Supported |
| Read Committed | **Default** | Supported |
| Repeatable Read | Supported | **Default** (InnoDB) |
| Serializable | Supported | Supported |

```sql
-- PostgreSQL
BEGIN TRANSACTION ISOLATION LEVEL SERIALIZABLE;
-- or
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;

-- MySQL
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;
START TRANSACTION;
```

### DDL in Transactions

```sql
-- PostgreSQL: DDL is transactional!
BEGIN;
    CREATE TABLE temp_users (id INT);
    INSERT INTO temp_users VALUES (1);
ROLLBACK;
-- Table does NOT exist after rollback

-- MySQL: DDL causes implicit commit
START TRANSACTION;
    INSERT INTO users VALUES (1, 'John');
    CREATE TABLE temp_users (id INT);  -- Implicit COMMIT happens here!
ROLLBACK;
-- Previous INSERT is already committed, can't roll back
```

---

## Indexes

### Basic Index Syntax

```sql
-- Both
CREATE INDEX idx_name ON users(last_name);
CREATE UNIQUE INDEX idx_email ON users(email);
DROP INDEX idx_name;  -- PostgreSQL
DROP INDEX idx_name ON users;  -- MySQL (requires table name)
```

### PostgreSQL-Specific Index Features

```sql
-- Partial Index (index subset of rows)
CREATE INDEX idx_active_users ON users(email)
WHERE is_active = TRUE;

-- Expression Index (index on computed value)
CREATE INDEX idx_lower_email ON users(LOWER(email));

-- GIN Index (for JSONB, arrays, full-text)
CREATE INDEX idx_tags ON posts USING GIN(tags);
CREATE INDEX idx_data ON events USING GIN(data);

-- BRIN Index (for large, naturally ordered tables)
CREATE INDEX idx_created ON logs USING BRIN(created_at);

-- Concurrent Index (no table lock)
CREATE INDEX CONCURRENTLY idx_name ON users(name);
```

### Index Types Comparison

| Index Type | PostgreSQL | MySQL |
|------------|------------|-------|
| B-tree | Default | Default |
| Hash | Supported | Supported (InnoDB) |
| GIN (inverted) | Supported | Not available |
| GiST | Supported | Not available |
| BRIN | Supported | Not available |
| Full-text | `tsvector` + GIN/GiST | `FULLTEXT` index |
| Spatial | PostGIS extension | `SPATIAL` index |

---

## Views

### Regular Views

```sql
-- Both: Same syntax
CREATE VIEW high_earners AS
SELECT name, salary, department
FROM employees
WHERE salary > 80000;

CREATE OR REPLACE VIEW high_earners AS
SELECT name, salary, department
FROM employees
WHERE salary > 90000;

DROP VIEW high_earners;
```

### Materialized Views

```sql
-- PostgreSQL: Native support
CREATE MATERIALIZED VIEW sales_summary AS
SELECT
    date_trunc('month', order_date) as month,
    SUM(amount) as total
FROM orders
GROUP BY 1;

-- Refresh data
REFRESH MATERIALIZED VIEW sales_summary;

-- Concurrent refresh (requires unique index)
CREATE UNIQUE INDEX ON sales_summary(month);
REFRESH MATERIALIZED VIEW CONCURRENTLY sales_summary;

-- MySQL: No materialized views
-- Workaround: Use regular table + scheduled refresh
CREATE TABLE sales_summary AS
SELECT
    DATE_FORMAT(order_date, '%Y-%m-01') as month,
    SUM(amount) as total
FROM orders
GROUP BY 1;

-- Manual refresh
TRUNCATE TABLE sales_summary;
INSERT INTO sales_summary SELECT ...;
```

---

## Schemas (Namespaces)

```sql
-- PostgreSQL: Schemas are namespaces within a database
CREATE SCHEMA sales;
CREATE TABLE sales.orders (id SERIAL PRIMARY KEY);
SELECT * FROM sales.orders;

SET search_path TO sales, public;

DROP SCHEMA sales CASCADE;

-- MySQL: "Schema" is synonymous with "Database"
CREATE DATABASE sales;  -- Same as CREATE SCHEMA
USE sales;
CREATE TABLE orders (id INT AUTO_INCREMENT PRIMARY KEY);
```

---

## Stored Procedures & Functions

### Function Syntax

```sql
-- PostgreSQL
CREATE OR REPLACE FUNCTION get_employee_count(dept_name TEXT)
RETURNS INTEGER AS $$
BEGIN
    RETURN (SELECT COUNT(*) FROM employees WHERE department = dept_name);
END;
$$ LANGUAGE plpgsql;

SELECT get_employee_count('Engineering');

-- MySQL
DELIMITER //
CREATE FUNCTION get_employee_count(dept_name VARCHAR(100))
RETURNS INT
DETERMINISTIC
BEGIN
    DECLARE emp_count INT;
    SELECT COUNT(*) INTO emp_count FROM employees WHERE department = dept_name;
    RETURN emp_count;
END //
DELIMITER ;

SELECT get_employee_count('Engineering');
```

### Procedure Syntax

```sql
-- PostgreSQL (PostgreSQL 11+)
CREATE OR REPLACE PROCEDURE transfer_funds(
    from_account INT,
    to_account INT,
    amount NUMERIC
)
LANGUAGE plpgsql
AS $$
BEGIN
    UPDATE accounts SET balance = balance - amount WHERE id = from_account;
    UPDATE accounts SET balance = balance + amount WHERE id = to_account;
    COMMIT;
END;
$$;

CALL transfer_funds(1, 2, 100.00);

-- MySQL
DELIMITER //
CREATE PROCEDURE transfer_funds(
    IN from_account INT,
    IN to_account INT,
    IN amount DECIMAL(10,2)
)
BEGIN
    UPDATE accounts SET balance = balance - amount WHERE id = from_account;
    UPDATE accounts SET balance = balance + amount WHERE id = to_account;
END //
DELIMITER ;

CALL transfer_funds(1, 2, 100.00);
```

---

## Common Table Expressions (CTEs)

### Basic CTE

```sql
-- Both: Same syntax
WITH high_earners AS (
    SELECT name, salary, department
    FROM employees
    WHERE salary > 80000
)
SELECT * FROM high_earners WHERE department = 'Engineering';
```

### Recursive CTE

```sql
-- PostgreSQL
WITH RECURSIVE employee_tree AS (
    SELECT id, name, manager_id, 1 as level
    FROM employees
    WHERE manager_id IS NULL

    UNION ALL

    SELECT e.id, e.name, e.manager_id, t.level + 1
    FROM employees e
    JOIN employee_tree t ON e.manager_id = t.id
)
SELECT * FROM employee_tree;

-- MySQL (8.0+): Same syntax
WITH RECURSIVE employee_tree AS (
    SELECT id, name, manager_id, 1 as level
    FROM employees
    WHERE manager_id IS NULL

    UNION ALL

    SELECT e.id, e.name, e.manager_id, t.level + 1
    FROM employees e
    JOIN employee_tree t ON e.manager_id = t.id
)
SELECT * FROM employee_tree;
```

---

## JSON Support

### JSON Data Type

| Feature | PostgreSQL | MySQL |
|---------|------------|-------|
| JSON type | `JSON` (text) | `JSON` |
| Binary JSON | `JSONB` (indexable!) | Not available |
| JSON indexing | GIN index on JSONB | Generated columns + index |

### JSON Operators

```sql
-- PostgreSQL JSONB operators
SELECT data->'name' FROM events;           -- Get as JSON
SELECT data->>'name' FROM events;          -- Get as TEXT
SELECT data#>'{user,name}' FROM events;    -- Path access
SELECT data#>>'{user,name}' FROM events;   -- Path access as text
SELECT data @> '{"type":"click"}' FROM events;  -- Contains
SELECT data ? 'name' FROM events;          -- Has key

-- MySQL JSON functions
SELECT JSON_EXTRACT(data, '$.name') FROM events;
SELECT data->'$.name' FROM events;         -- Shorthand (MySQL 5.7+)
SELECT JSON_UNQUOTE(data->'$.name') FROM events;  -- As text
SELECT JSON_CONTAINS(data, '"click"', '$.type') FROM events;
SELECT JSON_CONTAINS_PATH(data, 'one', '$.name') FROM events;
```

---

## Full-Text Search

```sql
-- PostgreSQL: tsvector and tsquery
CREATE TABLE articles (
    id SERIAL PRIMARY KEY,
    title TEXT,
    body TEXT,
    search_vector TSVECTOR
);

CREATE INDEX idx_search ON articles USING GIN(search_vector);

UPDATE articles SET search_vector =
    to_tsvector('english', title || ' ' || body);

SELECT * FROM articles
WHERE search_vector @@ to_tsquery('english', 'database & postgresql');

-- MySQL: FULLTEXT index
CREATE TABLE articles (
    id INT AUTO_INCREMENT PRIMARY KEY,
    title VARCHAR(255),
    body TEXT,
    FULLTEXT KEY idx_search (title, body)
);

SELECT * FROM articles
WHERE MATCH(title, body) AGAINST('database postgresql' IN NATURAL LANGUAGE MODE);
```

---

## Performance Features

### EXPLAIN

```sql
-- PostgreSQL
EXPLAIN SELECT * FROM users WHERE email = 'test@test.com';
EXPLAIN ANALYZE SELECT * FROM users WHERE email = 'test@test.com';
EXPLAIN (ANALYZE, BUFFERS, FORMAT JSON) SELECT ...;

-- MySQL
EXPLAIN SELECT * FROM users WHERE email = 'test@test.com';
EXPLAIN ANALYZE SELECT * FROM users WHERE email = 'test@test.com';  -- MySQL 8.0.18+
EXPLAIN FORMAT=JSON SELECT ...;
```

### Table Statistics

```sql
-- PostgreSQL
ANALYZE users;  -- Update statistics
VACUUM users;   -- Reclaim space
VACUUM ANALYZE users;  -- Both

-- MySQL
ANALYZE TABLE users;  -- Update statistics
OPTIMIZE TABLE users;  -- Reclaim space and rebuild
```

---

## Quick Syntax Reference Table

| Operation | PostgreSQL | MySQL |
|-----------|------------|-------|
| Auto-increment | `SERIAL` / `GENERATED AS IDENTITY` | `AUTO_INCREMENT` |
| String concat | `\|\|` or `CONCAT()` | `CONCAT()` |
| Case-insensitive LIKE | `ILIKE` | `LIKE` (usually case-insensitive) |
| Type cast | `::type` or `CAST()` | `CAST()` |
| Boolean | `TRUE` / `FALSE` | `1` / `0` |
| Current timestamp | `NOW()` / `CURRENT_TIMESTAMP` | `NOW()` / `CURRENT_TIMESTAMP` |
| Limit syntax | `LIMIT n OFFSET m` | `LIMIT n OFFSET m` or `LIMIT m, n` |
| Regex match | `~` / `~*` / `SIMILAR TO` | `REGEXP` / `RLIKE` |
| Upsert | `ON CONFLICT DO UPDATE` | `ON DUPLICATE KEY UPDATE` |
| Return inserted | `RETURNING *` | `LAST_INSERT_ID()` |
| Describe table | `\d table` | `DESCRIBE table` |
| List tables | `\dt` | `SHOW TABLES` |
| List databases | `\l` | `SHOW DATABASES` |
| Connect to db | `\c dbname` | `USE dbname` |
| Full outer join | Native | UNION workaround |
| Intersect | Native | JOIN workaround |
| Except | Native | LEFT JOIN workaround |
| Materialized views | Native | Not supported |
| JSONB | Native | Not supported |
| Arrays | Native | Not supported (use JSON) |

---

## When to Choose Which?

### Choose PostgreSQL When:

- **Complex queries** - Advanced JOINs, CTEs, window functions
- **Data integrity is critical** - Financial, healthcare, government
- **Need special data types** - JSONB, arrays, geometric, network types
- **Full ACID compliance** - Transactional DDL
- **Advanced indexing** - GIN, GiST, BRIN, partial/expression indexes
- **Geographic data** - PostGIS extension
- **Standards compliance** - SQL standard adherence

### Choose MySQL When:

- **Simple CRUD operations** - Basic web applications
- **Read-heavy workloads** - Content websites, blogs
- **High concurrency reads** - Many simultaneous connections
- **Simpler replication** - Master-slave setups
- **Shared hosting** - More widely available
- **Existing MySQL expertise** - Team already knows MySQL
- **WordPress/Drupal/etc.** - Apps designed for MySQL

---

## Summary Comparison Cheat Sheet

```
+------------------------------------------------------------------+
|              PostgreSQL vs MySQL SUMMARY                          |
+------------------------------------------------------------------+
|                                                                   |
| POSTGRESQL ADVANTAGES:          | MYSQL ADVANTAGES:              |
| ┌─────────────────────────────┐ | ┌─────────────────────────────┐|
| │ ✓ FULL OUTER JOIN native    │ | │ ✓ Simpler to learn          │|
| │ ✓ INTERSECT/EXCEPT native   │ | │ ✓ Faster for simple queries │|
| │ ✓ RETURNING clause          │ | │ ✓ More hosting options      │|
| │ ✓ JSONB (indexable JSON)    │ | │ ✓ Larger community (web)    │|
| │ ✓ Arrays, custom types      │ | │ ✓ Easy replication setup    │|
| │ ✓ Materialized views        │ | │ ✓ REPLACE statement         │|
| │ ✓ Transactional DDL         │ | │ ✓ UNSIGNED integers         │|
| │ ✓ Partial/expression indexes│ | │ ✓ Simpler user management   │|
| │ ✓ LATERAL joins             │ | │                             │|
| │ ✓ ILIKE (case-insensitive)  │ | │                             │|
| │ ✓ Rich regex support        │ | │                             │|
| └─────────────────────────────┘ | └─────────────────────────────┘|
|                                                                   |
| COMMON (Both Support):                                            |
| ┌─────────────────────────────────────────────────────────────┐  |
| │ • INNER/LEFT/RIGHT JOIN  • Window functions (MySQL 8+)      │  |
| │ • CTEs (MySQL 8+)        • Stored procedures/functions      │  |
| │ • Triggers               • Views                            │  |
| │ • Transactions           • Foreign keys                     │  |
| │ • JSON type              • Full-text search                 │  |
| └─────────────────────────────────────────────────────────────┘  |
+------------------------------------------------------------------+
```

---

**Use this guide for quick reference during interviews or when migrating between databases!**
