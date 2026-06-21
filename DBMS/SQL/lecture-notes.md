# **LEC-9: SQL in 1-Video**

### **1\.**

**SQL**: Structured Query Language, used to access and manipulate data.

### **2\.**

SQL used CRUD operations to communicate with DB.

1. **CREATE** \- execute INSERT statements to insert new tuple into the relation.  
2. **READ** \- Read data already in the relations.  
3. **UPDATE** \- Modify already inserted data in the relation.  
4. **DELETE** \- Delete specific data point/tuple/row or multiple rows.

### **3\.**

SQL is not DB, is a query language.

### **4\. What is RDBMS? (Relational Database Management System)**

1. Software that enable us to implement designed relational model.  
2. e.g., MySQL, MS SQL, Oracle, IBM etc.  
3. Table/Relation is the simplest form of data storage object in R-DB.  
4. MySQL is open-source RDBMS, and it uses SQL for all CRUD operations  
5. MySQL used client-server model, where client is CLI or frontend that used services provided by MySQL server.

### **6\. Difference between SQL and MySQL**

1. SQL is Structured Query language used to perform CRUD operations in R-DB, while MySQL is a RDBMS used to store, manage and administrate DB (provided by itself) using SQL.

### **SQL DATA TYPES ES**

(Ref: https://www.w3schools.com/sql/sql\_datatypes.asp)

1. In SQL DB, data is stored in the form of tables.  
2. Data can be of different types, like INT, CHAR etc.

| DATATYPE | Description |
| :---- | :---- |
| **CHAR** | string(0-255), string with size $=(0,255\]$, e.g., CHAR(251) |
| **VARCHAR** | string(0-255) |
| **TINYTEXT** | String(0-255) |
| **TEXT** | string(0-65535) |
| **BLOB** | string(0-65535) |
| **MEDIUMTEXT** | string(0-16777215) |
| **MEDIUMBLOB** | string(0-16777215) |
| **LONGTEXT** | string(0-4294967295) |
| **LONGBLOB** | string(0-4294967295) |
| **TINYINT** | integer(-128 to 127\) |
| **SMALLINT** | integer(-32768 to 32767\) |
| **MEDIUMINT** | integer(-8388608 to 8388607\) |
| **INT** | integer(-2147483648 to 2147483647\) |
| **BIGINT** | integer (-9223372036854775808 to 9223372036854775807\) |
| **FLOAT** | Decimal with precision to 23 digits |
| **DOUBLE** | Decimal with 24 to 53 digits |
| **DECIMAL** | Double stored as string |
| **DATE** | YYYY-MM-DD |
| **DATETIME** | YYYY-MM-DD HH:MM:SS |
| **TIMESTAMP** | YYYYMMDDHHMMSS |
| **TIME** | HH:MM:SS |
| **ENUM** | One of the preset values |
| **SET** | One or many of the preset values |
| **BOOLEAN** | $0/1$ |
| **BIT** | e.g., BIT(n), n upto 64, store values in bits. |

3. Size: TINY \< SMALL \< MEDIUM \< INT \< BIGINT.  
4. Variable length Data types e.g., VARCHAR, are better to use as they occupy space equal to the actual data size.  
5. Values can also be unsigned e.g., INT UNSIGNED.

### **6\. Types of SQL commands:**

1. **DDL (data definition language)**: defining relation schema.  
   1. **CREATE**: create table, DB, view.  
   2. **ALTER TABLE**: modification in table structure. e.g, change column datatype or add/remove columns.  
   3. **DROP**: delete table, DB, view.  
   4. **TRUNCATE**: remove all the tuples from the table.  
   5. **RENAME**: rename DB name, table name, column name etc.  
2. **DRL/DQL (data retrieval language / data query language)**: retrieve data from the tables.  
   1. **SELECT**  
3. **DML (data modification language)**: use to perform modifications in the DB  
   1. **INSERT**: insert data into a relation  
   2. **UPDATE**: update relation data.  
   3. **DELETE**: delete row(s) from the relation.  
4. **DCL (Data Control language)**: grant or revoke authorities from user.  
   1. **GRANT**: access privileges to the DB  
   2. **REVOKE**: revoke user access privileges.  
5. **TCL (Transaction control language)**: to manage transactions done in the DB  
   1. **START TRANSACTION**: begin a transaction  
   2. **COMMIT**: apply all the changes and end transaction  
   3. **ROLLBACK**: discard changes and end transaction  
   4. **SAVEPOINT**: checkout within the group of transactions in which to rollback.

### **MANAGING DB (DDL)**

1. **Creation of DB**  
   1. CREATE DATABASE IF NOT EXISTS db-name;  
   2. USE db-name; //need to execute to choose on which DB CREATE TABLE etc commands will be executed.  
      //make switching between DBs possible.  
   3. DROP DATABASE IF EXISTS db-name; //dropping database.  
   4. SHOW DATABASES; //list all the DBs in the server.  
   5. SHOW TABLES; //list tables in the selected DB.

### **DATA RETRIEVAL LANGUAGE (DRL)**

1. Syntax: SELECT \<set of column names\> FROM \<table\_name\>;  
2. Order of execution from RIGHT to LEFT.  
3. **Q. Can we use SELECT keyword without using FROM clause?**  
   1. Yes, using DUAL Tables.  
   2. Dual tables are dummy tables created by MySQL, help users to do certain obvious actions without referring to user defined tables.  
   3. e.g., SELECT 55 \+ 11;  
      SELECT now();  
      SELECT ucase(); etc.  
4. **WHERE**  
   1. Reduce rows based on given conditions.  
   2. E.g., SELECT \* FROM customer WHERE age \> 18;  
5. **BETWEEN**  
   1. SELECT \* FROM customer WHERE age between 0 AND 100;  
   2. In the above e.g., 0 and 100 are inclusive.  
6. **IN**  
   1. Reduces OR conditions;  
   2. e.g., SELECT \* FROM officers WHERE officer\_name IN ('Lakshay', 'Maharana Pratap', 'Deepika');  
7. **AND/OR/NOT**  
   1. AND: WHERE cond1 AND cond2  
   2. OR: WHERE cond1 OR cond2  
   3. NOT: WHERE col\_name NOT IN (1,2,3,4);  
8. **IS NULL**  
   1. e.g., SELECT \* FROM customer WHERE prime\_status is NULL;  
9. **Pattern Searching / Wildcard ('%', '\_')**  
   1. '%', any number of character from 0 to n. Similar to '\*' asterisk in regex.  
   2. '\_', only one character.  
   3. SELECT \* FROM customer WHERE name LIKE '%p\_';  
10. **ORDER BY**  
    1. Sorting the data retrieved using WHERE clause.  
    2. ORDER BY \<column-name\> DESC;  
    3. DESC \= Descending and ASC \= Ascending  
    4. e.g., SELECT \* FROM customer ORDER BY name DESC;  
11. **GROUP BY**  
    1. GROUP BY Clause is used to collect data from multiple records and group the result by one or more column. It is generally used in a SELECT statement.  
    2. Groups into category based on column given.  
    3. SELECT C1, C2, c3 FROM sample\_table WHERE cond GROUP BY c1, c2, c3.  
    4. All the column names mentioned after SELECT statement shall be repeated in GROUP BY, in order to successfully execute the query.  
    5. Used with aggregation functions to perform various actions.  
       1. COUNT()  
       2. SUM()  
       3. AVG()  
       4. MIN()  
       5. MAX()  
12. **DISTINCT**  
    1. Find distinct values in the table.  
    2. SELECT DISTINCT(col\_name) FROM table\_name;  
    3. GROUP BY can also be used for the same  
       1. "Select col\_name from table GROUP BY col\_name;" same output as above DISTINCT query.  
       2. SQL is smart enough to realise that if you are using GROUP BY and not using any aggregation function, then you mean "DISTINCT".  
13. **GROUP BY HAVING**  
    1. Out of the categories made by GROUP BY, we would like to know only particular thing (cond).  
    2. Similar to WHERE.  
    3. Select COUNT(cust\_id), country from customer GROUP BY country HAVING COUNT(cust\_id) \> 50;  
    4. **WHERE vs HAVING**  
       1. Both have same function of filtering the row base on certain conditions.  
       2. WHERE clause is used to filter the rows from the table based on specified condition  
       3. HAVING clause is used to filter the rows from the groups based on the specified condition.  
       4. HAVING is used after GROUP BY while WHERE is used before GROUP BY clause.  
       5. If you are using HAVING, GROUP BY is necessary.  
       6. WHERE can be used with SELECT, UPDATE & DELETE keywords while GROUP BY used with SELECT.

### **CONSTRAINTS (DDL)**

1. **Primary Key**  
   CREATE TABLE Customer  
   (  
     id INT PRIMARY KEY,  
     branch.id INT,  
     Firstname CHAR(5),  
     Lastname "",  
     DOB DATE,  
     Gender CHAR(5),  
     PRIMARY KEY (id) \-- Choose one of the two ways  
   );

   1. PK is not null, unique and only one per table.  
2. **Foreign Key**  
   1. FK refers to PK of other table.  
   2. Each relation can having any number of FK.

CREATE TABLE ORDER (  
  id INT PRIMARY KEY,  
  delivery\_date DATE,  
  order\_placed\_date DATE,  
  cust\_id INT,  
  FOREIGN KEY (cust\_id) REFERENCES customer(id)  
);

3. **UNIQUE**  
   1. Unique, can be null, table can have multiple unique attributes.

CREATE TABLE customer (  
  email VARCHAR(1024) UNIQUE,  
);

4. CHECK  
   1\.  
   CREATE TABLE customer (  
     CONSTRAINT age\_check CHECK (age\>12),  
   );

   2. "age\_check", can also avoid this, MySQL generates name of constraint automatically.  
5. **DEFAULT**  
   1. Set default value of the column.

CREATE TABLE account (  
  saving-rate DOUBLE NOT NULL DEFAULT 4.25,  
);

6. An attribute can be PK and FK both in a table.  
7. **ALTER OPERATIONS**  
   1. Changes schema  
   2. **ADD**  
      1. Add new column.  
      2. ALTER TABLE table\_name ADD new\_col\_name datatype ADD new\_col\_name\_2 datatype;  
      3. e.g., ALTER TABLE customer ADD age INT NOT NULL;  
   3. **MODIFY**  
      1. Change datatype of an attribute.  
      2. ALTER TABLE table-name MODIFY col-name col-datatype;  
      3. E.g., VARCHAR TO CHAR  
         ALTER TABLE customer MODIFY name CHAR(1024);  
   4. **CHANGE COLUMN**  
      1. Rename column name.  
      2. ALTER TABLE table-name CHANGE COLUMN old-col-name new-col-name new-col-datatype;  
      3. e.g., ALTER TABLE customer CHANGE COLUMN name customer-name VARCHAR(1024);  
   5. **DROP COLUMN**  
      1. Drop a column completely.  
      2. ALTER TABLE table-name DROP COLUMN col-name;  
      3. e.g., ALTER TABLE customer DROP COLUMN middle-name;  
   6. **RENAME**  
      1. Rename table name itself.  
      2. ALTER TABLE table-name RENAME TO new-table-name;  
      3. e.g., ALTER TABLE customer RENAME TO customer-details;

### **DATA MANIPULATION LANGUAGE (DML)**

1. **INSERT**  
   1. INSERT INTO table-name(col1, col2, col3) VALUES (v1, v2, v3), (val1, val2, val3);  
2. **UPDATE**  
   1. UPDATE table-name SET col1 \= 1, col2 \= 'abc' WHERE id \= 1;  
   2. Update multiple rows e.g.,  
      1. UPDATE student SET standard \= standard \+ 1  
   3. **ON UPDATE CASCADE**  
      1. Can be added to the table while creating constraints. Suppose there is a situation where we have two tables such that primary key of one table is the foreign key for another table. if we update the primary key of the first table then using the ON UPDATE CASCADE foreign key of the second table automatically get updated.  
3. **DELETE**  
   1. DELETE FROM table-name WHERE id \= 1;  
   2. DELETE FROM table-name; //all rows will be deleted.  
   3. **DELETE CASCADE** \- (to overcome DELETE constraint of Referential constraints)  
      1. What would happen to child entry if parent table's entry is deleted?

   CREATE TABLE ORDER (  
           order\_id int PRIMARY KEY,  
           delivery\_date DATE,  
           cust\_id INT,  
           FOREIGN KEY(cust\_id) REFERENCES customer(id) ON DELETE CASCADE  
         );

   4. ON DELETE NULL \- (can FK have null values?)  
      1\.  
      CREATE TABLE ORDER (  
        order\_id int PRIMARY KEY,  
        delivery\_date DATE,  
        cust\_id INT,  
        FOREIGN KEY(cust\_id) REFERENCES customer(id) ON DELETE SET NULL  
      );

4. **REPLACE**  
   1. Primarily used for already present tuple in a table.  
   2. As UPDATE, using REPLACE with the help of WHERE clause in PK, then that row will be replaced.  
   3. As INSERT, if there is no duplicate data new tuple will be inserted.  
   4. REPLACE INTO student (id, class) VALUES (4, 3);  
   5. REPLACE INTO table SET col1=val1, col2=val2

### **JOINING TABLES**

1. All RDBMS are relational in nature, we refer to other tables to get meaningful outcomes.  
2. FK are used to do reference to other table.  
3. **INNER JOIN**  
   1. Returns a resultant table that has matching values from both the tables or all the tables.  
   2. SELECT column-list FROM table1 INNER JOIN table2 ON condition1  
      INNER JOIN table3 ON condition2  
   3. **Alias in MySQL (AS)**  
      1. Aliases in MySQL is used to give a temporary name to a table or a column in a table for the purpose of a particular query. It works as a nickname for expressing the tables or column names. It makes the query short and neat.  
      2. SELECT col\_name AS alias\_name FROM table\_name;  
      3. SELECT col\_name1, col\_name2,... FROM table\_name AS alias\_name;  
4. **OUTER JOIN**  
   1. **LEFT JOIN**  
      1. This returns a resulting table that all the data from left table and the matched data from the right table.  
      2. SELECT columns FROM table LEFT JOIN table2 ON Join\_Condition;  
   2. **RIGHT JOIN**  
      1. This returns a resulting table that all the data from right table and the matched data from the left table.  
      2. SELECT columns FROM table RIGHT JOIN table2 ON join\_cond;  
   3. **FULL JOIN**  
      1. This returns a resulting table that contains all data when there is a match on left or right table data.  
      2. Emulated in MySQL using LEFT and RIGHT JOIN.  
      3. LEFT JOIN UNION RIGHT JOIN.  
      4. SELECT columns FROM table1 as t1 LEFT JOIN table2 as t2 ON t1.id=t2.id  
         UNION  
         SELECT columns FROM table1 as t1 RIGHT JOIN table2 as t2 ON t1.id \= t2.id;  
      5. UNION ALL, can also be used this will duplicate values as well while UNION gives unique values.  
5. **CROSS JOIN**  
   1. This returns all the cartesian products of the data present in both tables. Hence, all possible variations are reflected in the output.  
   2. Used rarely in practical purpose.  
   3. Table-1 has 10 rows and table-2 has 5, then resultant would have 50 rows.  
   4. SELECT column-lists FROM table1 CROSS JOIN table2;  
6. **SELF JOIN**  
   1. It is used to get the output from a particular table when the same table is joined to itself.  
   2. Used very less.  
   3. Emulated using INNER JOIN.  
   4. SELECT columns FROM table as t1 INNER JOIN table as t2 ON t1.id=t2.id  
7. **Join without using join keywords.**  
   1. SELECT \* FROM table1, table2 WHERE condition;  
   2. e.g., SELECT artist\_name, album\_name, year\_recorded FROM artist, album WHERE artist.id \= album.artist\_id;

### **SET OPERATIONS**

1. Used to combine multiple select statements.  
2. Always gives distinct rows.

| JOIN | SET Operations |
| :---- | :---- |
| Combines multiple tables based on matching condition. Column wise combination. Data types of two tables can be different. | Combination is resulting set from two or more SELECT statements. Row wise combination. Datatypes of corresponding columns from each table should be the same. |
| Can generate both distinct or duplicate rows. | Generate distinct rows. |
| The number of column(s) selected may or may not be the same from each table. | The number of column(s) selected must be the same from each table. |
| Combines results horizontally. | Combines results vertically. |

3. **UNION**  
   1. Combines two or more SELECT statements.  
   2. SELECT \* FROM table1 UNION SELECT \* FROM table2;  
   3. Number of column, order of column must be same for table1 and table2.  
4. **INTERSECT**  
   1. Returns common values of the tables.  
   2. Emulated.  
   3. SELECT DISTINCT column-list FROM table-1 INNER JOIN table-2 USING(join\_cond);  
   4. SELECT DISTINCT \* FROM table1 INNER JOIN table2 ON USING(id);  
5. **MINUS**  
   1. This operator returns the distinct row from the first table that does not occur in the second table.  
   2. Emulated.  
   3. SELECT column\_list FROM table1 LEFT JOIN table2 ON condition WHERE table2.column\_name IS NULL;  
   4. e.g., SELECT id FROM table-1 LEFT JOIN table-2 USING(id) WHERE table-2.id IS NULL;

### **SUB QUERIES**

1. Outer query depends on inner query.  
2. Alternative to joins.  
3. Nested queries.  
4. SELECT column\_list (s) FROM table\_name WHERE column\_name OPERATOR (SELECT column\_list (s) FROM table\_name \[WHERE\]);  
5. e.g., SELECT \* FROM table1 WHERE col1 IN (SELECT col1 FROM table1);  
6. Sub queries exist mainly in 3 clauses  
   1. Inside a WHERE clause.  
   2. Inside a FROM clause.  
   3. Inside a SELECT clause.  
7. **Subquery using FROM clause**  
   1. SELECT MAX(rating) FROM (SELECT \* FROM movie WHERE country \='India') as temp;  
8. **Subquery using SELECT**  
   1. SELECT (SELECT column\_list(s) FROM T\_name WHERE condition), columnList(s) FROM T2\_name WHERE condition;  
9. **Derived Subquery**  
   1. SELECT columnLists(s) FROM (SELECT columnLists(s) FROM table\_name WHERE \[condition\]) as new\_table\_name;  
10. **Co-related sub-queries**  
    1. With a normal nested subquery, the inner SELECT query runs first and executes once, returning values to be used by the main query. A correlated subquery, however, executes once for each candidate row considered by the outer query. In other words, the inner query is driven by the outer query.

SELECT column1, column2, ...  
FROM table1 as outer  
WHERE column1 operator  
(SELECT column1, column2  
FROM table2  
WHERE expr1 \= outer.expr2);

### **JOIN VS SUB-QUERIES**

| JOINS | SUBQUERIES |
| :---- | :---- |
| Faster | Slower |
| Joins maximise calculation burden on DBMS | Keeps responsibility of calculation on user. |
| Complex, difficult to understand and implement | Comparatively easy to understand and implement. |
| Choosing optimal join for optimal use case is difficult | Easy. |

### **MySQL VIEWS**

1. A view is a database object that has no values. Its contents are based on the base table. It contains rows and columns similar to the real table.  
2. In MySQL, the View is a virtual table created by a query by joining one or more tables. It is operated similarly to the base table but does not contain any data of its own.  
3. The View and table have one main difference that the views are definitions built on top of other tables (or views). If any changes occur in the underlying table, the same changes reflected in the View also.  
4. CREATE VIEW view\_name AS SELECT columns FROM tables \[WHERE conditions\];  
5. ALTER VIEW view\_name AS SELECT columns FROM table WHERE conditions;  
6. DROP VIEW IF EXISTS view\_name;  
7. CREATE VIEW Trainer AS SELECT c.course\_name, c.trainer, t.email FROM courses c, contact t WHERE c.id=t.id; (View using Join clause).

NOTE: We can also import/export table schema from files (.csv or json).