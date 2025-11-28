-- RDBMS -> Relational Database Management System, example: MySQL, Oracle, SQL Server, etc.
-- ORDBMS -> Object-Relational Database Management System, example: PostgreSQL, etc.
-- NoSQL databases example: MongoDB, CouchBase, etc.
-- SQL is a language, MySQL is a RDBMS software, it is used to create, modify and query the database.

CREATE DATABASE temp;
USE temp;
CREATE TABLE students(
id INT PRIMARY KEY,
name VARCHAR(255)
);
SHOW DATABASES;
SHOW TABLES;
INSERT INTO students(id, name) VALUES(1, 'aniket');
INSERT INTO students(id, name) VALUES(2, 'rahul');
SELECT \* FROM students;

-- Datatypes in SQL:
-- CHAR: string -- fixed length string (0-255)
-- VARCHAR: string -- variable length string (0-255)
-- TEXT: string -- large text data
-- BLOB: Binary Large Object
-- TINYINT: 1 byte integer
-- SMALLINT: 2 byte integer
-- INT: 4 byte integer
-- BIGINT: 8 byte integer
-- FLOAT: floating point number
-- DOUBLE: double precision number
-- DATE: date (YYYY-MM-DD)
-- TIME: time (HH:MM:SS)
-- DATETIME: date and time
-- TIMESTAMP: date and time (with timezone)
-- DECIMAL: exact numeric value with fixed precision
-- ENUM: enumeration of predefined values
-- SET: set of predefined values
-- BOOLEAN: true/false value
-- BIT: bit field
-- signed vs unsigned: signed allows negative values, unsigned only allows positive values

-- Advanced datatypes:
-- JSON: JSON data
-- XML: XML data
-- GEOSPATIAL: geographic/spatial data
-- BINARY: binary data

-- Types of commands in SQL:
-- DDL: Data Definition Language:
-- CREATE, ALTER, DROP, TRUNCATE, RENAME, etc.
-- DRL/DQL: Data Retrieval/Query Language:
-- SELECT, etc.
-- DML: Data Manipulation Language:
-- INSERT, UPDATE, DELETE, etc.
-- DCL: Data Control Language:
-- GRANT, REVOKE, etc.
-- TCL: Transaction Control Language:
-- COMMIT, ROLLBACK, SAVEPOINT, etc.

-- Managing databases:
CREATE DATABASE <database_name>;
USE <database_name>;
SHOW DATABASES;
SHOW TABLES;
DROP DATABASE <database_name>;
DROP TABLE <table_name>;
TRUNCATE TABLE <table_name>;
RENAME TABLE <old_name> TO <new_name>;

-- Managing tables:
CREATE TABLE <table_name>(
<column_name> <datatype>,
<column_name> <datatype>,
<column_name> <datatype>
);
SHOW TABLES;
INSERT INTO <table_name> VALUES(<value1>, <value2>, ...);
SELECT \* FROM <table_name>;
UPDATE <table_name> SET <column_name> = <value> WHERE <condition>;
DELETE FROM <table_name> WHERE <condition>;

-- Managing data:
INSERT INTO <table_name> VALUES(<value1>, <value2>, ...);
SELECT \* FROM <table_name>;
UPDATE <table_name> SET <column_name> = <value> WHERE <condition>;
DELETE FROM <table_name> WHERE <condition>;

-- Querying data:
SELECT <column_name> FROM <table_name>;
SELECT <column_name> FROM <table_name> WHERE <condition>;
SELECT <column_name> FROM <table_name> WHERE <condition> ORDER BY <column_name>;
SELECT <column_name> FROM <table_name> WHERE <condition> ORDER BY <column_name> LIMIT <number>;
SELECT \* FROM <table_name>;
UPDATE <table_name> SET <column_name> = <value> WHERE <condition>;
DELETE FROM <table_name> WHERE <condition>;
-- Creating Worker table
-- PRIMARY KEY: uniquely identifies each record in the table, cannot be NULL, automatically creates an index
-- VARCHAR(n): variable-length string with maximum length n
-- DECIMAL(p, s): fixed-point number with p total digits and s digits after decimal point
-- DATE: stores date values in 'YYYY-MM-DD' format
CREATE TABLE Worker (
worker_id INT PRIMARY KEY,
first_name VARCHAR(50),
last_name VARCHAR(50),
salary DECIMAL(10, 2),
dept VARCHAR(50),
joining_date DATE
);

-- Creating Bonus table with foreign key reference to Worker
-- FOREIGN KEY: establishes a link between two tables, ensures referential integrity
-- REFERENCES: specifies which table and column the foreign key refers to
-- ON DELETE CASCADE: automatically deletes related records in this table when the referenced record is deleted
CREATE TABLE Bonus (
worker_ref_id INT,
bonus_amount DECIMAL(10, 2),
bonus_date DATE,
FOREIGN KEY (worker_ref_id) REFERENCES Worker(worker_id) ON DELETE CASCADE
);

-- Creating Title table with foreign key reference to Worker
-- Multiple foreign keys can reference the same parent table
-- Each foreign key constraint can have its own ON DELETE/ON UPDATE rules
CREATE TABLE Title (
worker_ref_id INT,
title VARCHAR(50),
affected_from DATE,
FOREIGN KEY (worker_ref_id) REFERENCES Worker(worker_id) ON DELETE CASCADE
);

in dql: firstly we have select statement
//concept of dual table
-select 4+5;
select now();
select lcase('aniket');
se
