# 📘 Database Normalization — The Complete Guide

> **One-stop beginner-friendly reference for exams and tech interviews.**
> Every concept is explained in simple language with multiple table examples.

---

## Table of Contents

1. [What is Normalization?](#1-what-is-normalization)
2. [Why Do We Need It? — The Anomaly Problem](#2-why-do-we-need-it--the-anomaly-problem)
3. [Key Terminology](#3-key-terminology)
4. [Types of Functional Dependencies](#4-types-of-functional-dependencies)
5. [First Normal Form (1NF)](#5-first-normal-form-1nf)
6. [Second Normal Form (2NF)](#6-second-normal-form-2nf)
7. [Third Normal Form (3NF)](#7-third-normal-form-3nf)
8. [Boyce-Codd Normal Form (BCNF / 3.5NF)](#8-boyce-codd-normal-form-bcnf--35nf)
9. [Fourth Normal Form (4NF)](#9-fourth-normal-form-4nf)
10. [Fifth Normal Form (5NF / PJNF)](#10-fifth-normal-form-5nf--pjnf)
11. [Domain-Key Normal Form (DKNF)](#11-domain-key-normal-form-dknf)
12. [Denormalization](#12-denormalization)
13. [Lossless Join and Dependency Preservation](#13-lossless-join-and-dependency-preservation)
14. [Canonical Cover and Minimal Cover](#14-canonical-cover-and-minimal-cover)
15. [Finding Candidate Keys from FDs](#15-finding-candidate-keys-from-fds)
16. [Attribute Closure](#16-attribute-closure)
17. [Decomposition Techniques](#17-decomposition-techniques)
18. [Normal Forms — Quick Comparison Table](#18-normal-forms--quick-comparison-table)
19. [Common Interview Questions](#19-common-interview-questions)
20. [Cheat Sheet — Remember This!](#20-cheat-sheet--remember-this)

---

## 1. What is Normalization?

**Normalization** is the process of organizing data in a database to:

- **Eliminate redundancy** (duplicate data)
- **Eliminate anomalies** (insertion, update, deletion problems)
- **Ensure data integrity** (data stays correct and consistent)

Think of it like organizing a messy closet — you separate things into the right drawers so nothing gets lost or duplicated.

### The Core Idea

```
Badly designed table (one giant table with everything)
        |
        |  Apply normalization rules step by step
        v
Multiple well-structured tables linked by keys
```

### Normalization Hierarchy

```
UNF (Unnormalized Form)
 |
 v
1NF  ──  Atomic values, no repeating groups
 |
 v
2NF  ──  Remove partial dependencies
 |
 v
3NF  ──  Remove transitive dependencies
 |
 v
BCNF ──  Every determinant is a candidate key
 |
 v
4NF  ──  Remove multi-valued dependencies
 |
 v
5NF  ──  Remove join dependencies
 |
 v
DKNF ──  Theoretical ideal (all constraints from domain + keys)
```

---

## 2. Why Do We Need It? — The Anomaly Problem

Consider this **un-normalized** student-course table:

| StudentID | StudentName | CourseID | CourseName      | Instructor   | InstructorPhone |
|-----------|-------------|----------|-----------------|--------------|-----------------|
| 101       | Alice       | CS101    | Databases       | Prof. Smith  | 9999-1111       |
| 101       | Alice       | CS102    | Operating Sys   | Prof. Jones  | 9999-2222       |
| 102       | Bob         | CS101    | Databases       | Prof. Smith  | 9999-1111       |
| 103       | Charlie     | CS103    | Networking      | Prof. Smith  | 9999-1111       |

### Three Types of Anomalies

#### Insertion Anomaly

> **Problem:** You CANNOT add a new course unless a student enrolls in it.

Want to add "CS104 — Machine Learning taught by Prof. Lee"?
You can't — because StudentID is part of the key, and you don't have a student yet!

#### Update Anomaly

> **Problem:** Changing one fact requires updating MULTIPLE rows.

Prof. Smith changes phone number?
You must update it in rows 1, 3, AND 4. Miss one? Data is now inconsistent!

#### Deletion Anomaly

> **Problem:** Deleting data causes UNINTENDED loss of other data.

Charlie (103) drops CS103?
Delete that row and you LOSE the information that CS103 (Networking) exists and that Prof. Smith teaches it!

### The Fix

Normalization breaks this one big table into smaller, well-structured tables where each fact is stored exactly ONCE.

---

## 3. Key Terminology

### Relation (Table)

A table with rows (tuples) and columns (attributes).

### Tuple (Row)

A single record in a table.

### Attribute (Column)

A property or field in a table.

### Super Key

Any set of attributes that can **uniquely identify** a tuple.

```
Table: Student(StudentID, Name, Email, Phone)

Super Keys:
  {StudentID}
  {Email}
  {StudentID, Name}
  {StudentID, Email}
  {StudentID, Name, Email, Phone}   (all attributes — always a super key)
  ... any superset of a candidate key
```

### Candidate Key

A **minimal** super key — remove any attribute and it stops being unique.

```
Candidate Keys for Student:
  {StudentID}    -- minimal, uniquely identifies
  {Email}        -- minimal, uniquely identifies (assuming unique emails)

NOT a candidate key:
  {StudentID, Name}  -- not minimal (StudentID alone is enough)
```

### Primary Key

One candidate key chosen as the main identifier. Usually underlined in schemas.

```
Student(StudentID, Name, Email, Phone)
         ^^^^^^^^
       Primary Key
```

### Foreign Key

An attribute in one table that references the primary key of another table.

```
Enrollment(StudentID, CourseID)
             |           |
             v           v
         Student.PK   Course.PK
```

### Prime Attribute

An attribute that is **part of ANY candidate key**.

### Non-Prime Attribute

An attribute that is **NOT part of any candidate key**.

```
Table: Enrollment(StudentID, CourseID, Grade, EnrollDate)

Candidate Key: {StudentID, CourseID}

Prime attributes:     StudentID, CourseID
Non-prime attributes: Grade, EnrollDate
```

### Functional Dependency (FD)

If knowing the value of attribute A **uniquely determines** the value of attribute B, we write:

```
A --> B    (A functionally determines B)
```

**Example:** StudentID --> StudentName

If you know StudentID = 101, you ALWAYS get the same StudentName (Alice).
StudentID **determines** StudentName.

**NOT a dependency:** StudentName --> StudentID (two students could share a name).

---

## 4. Types of Functional Dependencies

### Trivial FD

A dependency where the right side is a subset of the left side.

```
{StudentID, Name} --> {Name}       -- trivial (Name is on both sides)
{A, B} --> {A}                      -- trivial
```

These are always true and uninteresting.

### Non-Trivial FD

The right side is NOT a subset of the left side.

```
StudentID --> Name                  -- non-trivial
{StudentID, CourseID} --> Grade     -- non-trivial
```

### Full Functional Dependency

B is **fully functionally dependent** on A if:
- A --> B
- No proper subset of A determines B

```
{StudentID, CourseID} --> Grade

Is it full?
  StudentID  --> Grade?   NO (a student has many grades)
  CourseID   --> Grade?   NO (a course has many grades)

YES, it's a full dependency. You need BOTH attributes.
```

### Partial Functional Dependency

B depends on A, but also depends on a **proper subset** of A.

```
{StudentID, CourseID} --> StudentName

But: StudentID --> StudentName  (just StudentID is enough!)

This is a PARTIAL dependency — a subset of the key determines the attribute.
```

**This is what 2NF eliminates!**

### Transitive Functional Dependency

A --> B and B --> C, therefore A --> C (but B is not a candidate key).

```
StudentID --> DeptID --> DeptName

StudentID determines DeptID
DeptID determines DeptName

So: StudentID --> DeptName (transitively, through DeptID)
```

**This is what 3NF eliminates!**

### Multi-Valued Dependency (MVD)

A -->> B means: for a given A, the set of B values is **independent** of all other attributes.

```
Employee -->> Skill
Employee -->> Language

An employee's skills have NOTHING to do with their languages.
These are independent multi-valued facts.
```

**This is what 4NF eliminates!**

---

## 5. First Normal Form (1NF)

### Rule

> **A table is in 1NF if:**
> 1. Every column contains only **atomic (indivisible) values**
> 2. There are **no repeating groups** or arrays
> 3. Each row is **unique** (has a primary key)

### Example 1: Fixing Multi-Valued Cells

**VIOLATES 1NF** — Phone has multiple values in one cell:

| StudentID | Name    | Phone                  |
|-----------|---------|------------------------|
| 101       | Alice   | 9876543210, 9123456789 |
| 102       | Bob     | 9111222333             |

**IN 1NF** — Each cell has ONE atomic value:

| StudentID | Name    | Phone      |
|-----------|---------|------------|
| 101       | Alice   | 9876543210 |
| 101       | Alice   | 9123456789 |
| 102       | Bob     | 9111222333 |

Primary Key: {StudentID, Phone}

### Example 2: Fixing Repeating Groups

**VIOLATES 1NF** — Courses stored as repeating columns:

| StudentID | Name  | Course1   | Course2      | Course3    |
|-----------|-------|-----------|--------------|------------|
| 101       | Alice | Databases | Operating Sys| Networking |
| 102       | Bob   | Databases |              |            |

Problems:
- What if a student takes 10 courses? Add 10 columns?
- Empty cells waste space
- Hard to query "find all students in Databases"

**IN 1NF** — One row per student-course combination:

| StudentID | Name  | Course        |
|-----------|-------|---------------|
| 101       | Alice | Databases     |
| 101       | Alice | Operating Sys |
| 101       | Alice | Networking    |
| 102       | Bob   | Databases     |

### Example 3: Table with Composite Data

**VIOLATES 1NF** — Address is not atomic:

| EmpID | Name  | Address                      |
|-------|-------|------------------------------|
| 1     | Alice | 42 MG Road, Delhi, 110001    |
| 2     | Bob   | 7 Park Street, Kolkata, 700016 |

**IN 1NF** — Split into atomic columns:

| EmpID | Name  | Street        | City    | PinCode |
|-------|-------|---------------|---------|---------|
| 1     | Alice | 42 MG Road    | Delhi   | 110001  |
| 2     | Bob   | 7 Park Street | Kolkata | 700016  |

### Key Takeaway

```
1NF = "One value per cell, one fact per column"
```

---

## 6. Second Normal Form (2NF)

### Rule

> **A table is in 2NF if:**
> 1. It is in **1NF**
> 2. Every non-prime attribute is **fully functionally dependent** on the **entire** candidate key
>    (i.e., NO partial dependencies)

**Note:** 2NF only matters when the candidate key is **composite** (has 2+ attributes).
If the primary key is a single column, 1NF automatically satisfies 2NF.

### Example 1: Student-Course Table

**IN 1NF but VIOLATES 2NF:**

| StudentID | CourseID | StudentName | CourseName  | Grade |
|-----------|----------|-------------|-------------|-------|
| 101       | CS101    | Alice       | Databases   | A     |
| 101       | CS102    | Alice       | OS          | B     |
| 102       | CS101    | Bob         | Databases   | A+    |

Candidate Key: {StudentID, CourseID}

Let's check the dependencies:

```
{StudentID, CourseID} --> Grade         FULL dependency (need both)
{StudentID, CourseID} --> StudentName   PARTIAL! (StudentID alone determines it)
{StudentID, CourseID} --> CourseName    PARTIAL! (CourseID alone determines it)

Partial Dependencies Found:
  StudentID --> StudentName
  CourseID  --> CourseName
```

**CONVERTED TO 2NF** — Remove partial dependencies into separate tables:

**Table 1: Student**

| StudentID | StudentName |
|-----------|-------------|
| 101       | Alice       |
| 102       | Bob         |

**Table 2: Course**

| CourseID | CourseName |
|----------|------------|
| CS101    | Databases  |
| CS102    | OS         |

**Table 3: Enrollment**

| StudentID | CourseID | Grade |
|-----------|----------|-------|
| 101       | CS101    | A     |
| 101       | CS102    | B     |
| 102       | CS101    | A+    |

### Example 2: Order-Product Table

**VIOLATES 2NF:**

| OrderID | ProductID | ProductName | Quantity | UnitPrice |
|---------|-----------|-------------|----------|-----------|
| O1      | P1        | Laptop      | 2        | 50000     |
| O1      | P2        | Mouse       | 5        | 500       |
| O2      | P1        | Laptop      | 1        | 50000     |

Candidate Key: {OrderID, ProductID}

```
Partial Dependencies:
  ProductID --> ProductName
  ProductID --> UnitPrice
  (These depend only on ProductID, not on the full key)
```

**CONVERTED TO 2NF:**

**Products:**

| ProductID | ProductName | UnitPrice |
|-----------|-------------|-----------|
| P1        | Laptop      | 50000     |
| P2        | Mouse       | 500       |

**OrderDetails:**

| OrderID | ProductID | Quantity |
|---------|-----------|----------|
| O1      | P1        | 2        |
| O1      | P2        | 5        |
| O2      | P1        | 1        |

### How to Convert to 2NF — Step by Step

```
Step 1: Identify the composite candidate key
Step 2: Find all non-prime attributes
Step 3: Check if any non-prime attribute depends on just PART of the key
Step 4: If yes, move that attribute (with the part of the key it depends on)
        into a NEW table
Step 5: Keep only full dependencies in the original table
```

### Key Takeaway

```
2NF = 1NF + "The WHOLE key, nothing but the whole key"
       (No attribute depends on just a PART of the composite key)
```

---

## 7. Third Normal Form (3NF)

### Rule

> **A table is in 3NF if:**
> 1. It is in **2NF**
> 2. No non-prime attribute is **transitively dependent** on the candidate key
>
> **Equivalently:** For every FD X --> A:
> - X is a super key, **OR**
> - A is a prime attribute (part of some candidate key)

### Example 1: Employee-Department

**IN 2NF but VIOLATES 3NF:**

| EmpID | EmpName | DeptID | DeptName    | DeptHead   |
|-------|---------|--------|-------------|------------|
| 101   | Alice   | D1     | Engineering | Prof. Roy  |
| 102   | Bob     | D1     | Engineering | Prof. Roy  |
| 103   | Charlie | D2     | Marketing   | Prof. Sen  |

Candidate Key: {EmpID}

```
Functional Dependencies:
  EmpID  --> EmpName, DeptID, DeptName, DeptHead
  DeptID --> DeptName, DeptHead

Transitive Dependencies:
  EmpID --> DeptID --> DeptName    (transitive!)
  EmpID --> DeptID --> DeptHead    (transitive!)
  
  DeptName and DeptHead depend on EmpID
  BUT only THROUGH DeptID (not directly)
```

Problems with this design:
- DeptName "Engineering" is stored twice (redundancy)
- Change DeptHead? Must update ALL rows for that dept (update anomaly)
- Delete all employees in Marketing? Lose the department info (deletion anomaly)

**CONVERTED TO 3NF:**

**Employee:**

| EmpID | EmpName | DeptID |
|-------|---------|--------|
| 101   | Alice   | D1     |
| 102   | Bob     | D1     |
| 103   | Charlie | D2     |

**Department:**

| DeptID | DeptName    | DeptHead  |
|--------|-------------|-----------|
| D1     | Engineering | Prof. Roy |
| D2     | Marketing   | Prof. Sen |

### Example 2: Book Table

**VIOLATES 3NF:**

| BookID | Title       | AuthorID | AuthorName | AuthorCountry |
|--------|-------------|----------|------------|---------------|
| B1     | DBMS Guide  | A1       | Navathe    | USA           |
| B2     | OS Concepts | A2       | Silberschatz| USA          |
| B3     | Networks    | A1       | Navathe    | USA           |

Candidate Key: {BookID}

```
Transitive dependency:
  BookID --> AuthorID --> AuthorName
  BookID --> AuthorID --> AuthorCountry
```

**CONVERTED TO 3NF:**

**Book:**

| BookID | Title       | AuthorID |
|--------|-------------|----------|
| B1     | DBMS Guide  | A1       |
| B2     | OS Concepts | A2       |
| B3     | Networks    | A1       |

**Author:**

| AuthorID | AuthorName   | AuthorCountry |
|----------|--------------|---------------|
| A1       | Navathe      | USA           |
| A2       | Silberschatz | USA           |

### Example 3: Tournament Winners

**VIOLATES 3NF:**

| Tournament        | Year | Winner  | WinnerDOB  |
|-------------------|------|---------|------------|
| Cricket World Cup | 2023 | India   | -          |
| FIFA World Cup    | 2022 | Argentina| -         |
| Cricket World Cup | 2019 | England | -          |

Candidate Key: {Tournament, Year}

```
{Tournament, Year} --> Winner --> WinnerDOB

Transitive dependency:
  Winner --> WinnerDOB  (Winner is not a super key)
```

**CONVERTED TO 3NF:**

**TournamentWinner:**

| Tournament        | Year | Winner    |
|-------------------|------|-----------|
| Cricket World Cup | 2023 | India     |
| FIFA World Cup    | 2022 | Argentina |
| Cricket World Cup | 2019 | England   |

**WinnerInfo:**

| Winner    | WinnerDOB |
|-----------|-----------|
| India     | -         |
| Argentina | -         |
| England   | -         |

### How to Identify Transitive Dependencies

```
Look for this pattern:

  CandidateKey --> A --> B
  where A is NOT a candidate key

If you find it:
  - A --> B is a transitive dependency
  - Move A and B into a new table with A as the key
  - Keep A in the original table as a foreign key
```

### Key Takeaway

```
3NF = 2NF + "Nothing depends on non-key attributes"
      (No non-prime attribute determines another non-prime attribute)

The Classic Mnemonic:
  "Every non-key attribute must depend on
   THE KEY, THE WHOLE KEY, and NOTHING BUT THE KEY"
   
   1NF = the key (unique rows)
   2NF = the whole key (no partial deps)
   3NF = nothing but the key (no transitive deps)
```

---

## 8. Boyce-Codd Normal Form (BCNF / 3.5NF)

### Rule

> **A table is in BCNF if:**
> For every non-trivial functional dependency X --> Y,
> **X must be a super key**.
>
> (No exceptions — even prime attributes can't violate this!)

### Difference Between 3NF and BCNF

```
3NF says: For X --> A, either:
  - X is a super key, OR
  - A is a prime attribute      <-- this exception exists in 3NF

BCNF says: For X --> A:
  - X MUST be a super key       <-- NO exceptions!
```

**BCNF is stricter than 3NF.**
Every BCNF table is in 3NF, but NOT every 3NF table is in BCNF.

### When Does 3NF ≠ BCNF?

This happens when:
- There are **multiple overlapping candidate keys**
- A **prime attribute** functionally depends on something that isn't a super key

### Example 1: Student-Subject-Professor

Consider a university rule: Each professor teaches only ONE subject, but each subject can be taught by multiple professors.

**IN 3NF but VIOLATES BCNF:**

| Student | Subject   | Professor  |
|---------|-----------|------------|
| Alice   | Databases | Prof. Smith|
| Alice   | Networks  | Prof. Lee  |
| Bob     | Databases | Prof. Smith|
| Charlie | Databases | Prof. Roy  |

Candidate Keys:
- {Student, Subject} — a student takes each subject once
- {Student, Professor} — a student has one subject per professor

```
Functional Dependencies:
  {Student, Subject}   --> Professor
  {Student, Professor} --> Subject
  Professor --> Subject    <-- PROBLEM! Professor is NOT a super key!

Professor is a prime attribute (part of candidate key {Student, Professor})
3NF allows this (A is prime), but BCNF does NOT.
```

**CONVERTED TO BCNF:**

**ProfessorSubject:**

| Professor  | Subject   |
|------------|-----------|
| Prof. Smith| Databases |
| Prof. Lee  | Networks  |
| Prof. Roy  | Databases |

**StudentProfessor:**

| Student | Professor  |
|---------|------------|
| Alice   | Prof. Smith|
| Alice   | Prof. Lee  |
| Bob     | Prof. Smith|
| Charlie | Prof. Roy  |

### Example 2: Court Booking

Rules: Each court has a fixed rate per hour. Members can book courts.

**VIOLATES BCNF:**

| Court  | StartTime | EndTime | Rate | MemberID |
|--------|-----------|---------|------|----------|
| Court1 | 09:00     | 10:00   | 500  | M1       |
| Court1 | 10:00     | 11:00   | 500  | M2       |
| Court2 | 09:00     | 10:00   | 800  | M1       |

Candidate Key: {Court, StartTime}

```
FD: Court --> Rate

Court is NOT a super key (it's just part of the key)
This violates BCNF.
```

**CONVERTED TO BCNF:**

**CourtRate:**

| Court  | Rate |
|--------|------|
| Court1 | 500  |
| Court2 | 800  |

**Booking:**

| Court  | StartTime | EndTime | MemberID |
|--------|-----------|---------|----------|
| Court1 | 09:00     | 10:00   | M1       |
| Court1 | 10:00     | 11:00   | M2       |
| Court2 | 09:00     | 10:00   | M1       |

### Important Warning About BCNF

```
BCNF decomposition may NOT always preserve all functional dependencies!

3NF decomposition ALWAYS preserves dependencies (guaranteed).
BCNF decomposition guarantees LOSSLESS JOIN but may lose some FDs.

This is a classic exam/interview question!
```

### Key Takeaway

```
BCNF = "For EVERY dependency X --> Y, X must be a super key"
       (Even prime attributes must follow this rule — no exceptions!)
```

---

## 9. Fourth Normal Form (4NF)

### Rule

> **A table is in 4NF if:**
> 1. It is in **BCNF**
> 2. It has **no non-trivial multi-valued dependencies**
>    (unless the left side is a super key)

### What is a Multi-Valued Dependency (MVD)?

X -->> Y means: for each value of X, the set of Y values is **independent** of all other attributes in the table.

### Example 1: Employee Skills and Languages

Suppose employees have skills and languages, and these are **independent** of each other.

**VIOLATES 4NF:**

| EmpID | Skill       | Language |
|-------|-------------|----------|
| 101   | Java        | English  |
| 101   | Java        | Hindi    |
| 101   | Python      | English  |
| 101   | Python      | Hindi    |
| 102   | C++         | French   |

For EmpID = 101:
- Skills = {Java, Python}
- Languages = {English, Hindi}

These are INDEPENDENT — knowing the skill tells you nothing about the language.
But to represent all combinations, we need 2 x 2 = 4 rows for employee 101!

```
Multi-Valued Dependencies:
  EmpID -->> Skill
  EmpID -->> Language

These are independent multi-valued facts about the same key.
```

Why this is bad:
- **Redundancy:** Each skill is repeated for each language
- **Update Anomaly:** Add "Spanish" for emp 101? Must add it with EVERY skill
- **Row explosion:** 5 skills x 4 languages = 20 rows for one employee!

**CONVERTED TO 4NF:**

**EmpSkill:**

| EmpID | Skill  |
|-------|--------|
| 101   | Java   |
| 101   | Python |
| 102   | C++    |

**EmpLanguage:**

| EmpID | Language |
|-------|----------|
| 101   | English  |
| 101   | Hindi    |
| 102   | French   |

Now adding "Spanish" for emp 101 = just 1 new row in EmpLanguage!

### Example 2: Student-Sport-Instrument

A student can play multiple sports and multiple instruments independently.

**VIOLATES 4NF:**

| StudentID | Sport    | Instrument |
|-----------|----------|------------|
| 1         | Cricket  | Guitar     |
| 1         | Cricket  | Piano      |
| 1         | Football | Guitar     |
| 1         | Football | Piano      |
| 2         | Tennis   | Violin     |

```
MVDs:
  StudentID -->> Sport
  StudentID -->> Instrument
```

**CONVERTED TO 4NF:**

**StudentSport:**

| StudentID | Sport    |
|-----------|----------|
| 1         | Cricket  |
| 1         | Football |
| 2         | Tennis   |

**StudentInstrument:**

| StudentID | Instrument |
|-----------|------------|
| 1         | Guitar     |
| 1         | Piano      |
| 2         | Violin     |

### How to Detect MVD Violations

```
Ask yourself: "Are these two multi-valued attributes about the same key
              INDEPENDENT of each other?"

If YES --> they should be in SEPARATE tables.
If NO  --> they are genuinely related and can stay together.
```

### Key Takeaway

```
4NF = BCNF + "No independent multi-valued facts in the same table"
      Separate independent 1-to-many relationships into different tables.
```

---

## 10. Fifth Normal Form (5NF / PJNF)

### Rule

> **A table is in 5NF (Project-Join Normal Form) if:**
> 1. It is in **4NF**
> 2. It cannot be **losslessly decomposed** into smaller tables
>    that, when joined back, reconstruct the original
>    (no non-trivial join dependencies)

### What is a Join Dependency?

A join dependency exists when a table can be recreated by joining three or more smaller tables, but NOT by joining just two.

### Example: Supplier-Part-Project

A supplier can supply parts to projects. But the relationship is THREE-WAY — knowing any two pairs doesn't tell you the third.

**Consider this table:**

| Supplier | Part   | Project   |
|----------|--------|-----------|
| S1       | Bolt   | Bridge    |
| S1       | Nut    | Building  |
| S1       | Bolt   | Building  |
| S2       | Bolt   | Bridge    |

**Key Question:** If S1 supplies Bolt, and S1 works on Building, and Bolt is used in Building... does S1 supply Bolt to Building?

**Case A — If the answer is ALWAYS YES (cyclic constraint):**

This means the three-way table can be decomposed into three binary tables:

**SupplierPart:**

| Supplier | Part |
|----------|------|
| S1       | Bolt |
| S1       | Nut  |
| S2       | Bolt |

**SupplierProject:**

| Supplier | Project  |
|----------|----------|
| S1       | Bridge   |
| S1       | Building |
| S2       | Bridge   |

**PartProject:**

| Part | Project  |
|------|----------|
| Bolt | Bridge   |
| Bolt | Building |
| Nut  | Building |

Joining all three back gives the original table (no spurious tuples if the cyclic constraint holds).

**Case B — If the answer is NOT necessarily yes:**

Then the three-way relationship is GENUINE and cannot be decomposed. The table is already in 5NF.

### When Does 5NF Matter?

```
5NF violations are RARE in practice.
They occur when:
  - A table represents a 3+ way relationship
  - A cyclic constraint allows decomposition into binary relationships
  - The decomposition is lossless

Most real-world databases stop at BCNF or 3NF.
```

### Key Takeaway

```
5NF = 4NF + "No hidden join dependencies"
      A table that cannot be further split without losing information.
```

---

## 11. Domain-Key Normal Form (DKNF)

### Rule

> **A table is in DKNF if:**
> Every constraint on the table is a logical consequence of
> **domain constraints** and **key constraints** alone.

### What Are Domain and Key Constraints?

- **Domain constraint:** The allowed values for an attribute (e.g., Age must be 0-150, Status must be 'Active' or 'Inactive')
- **Key constraint:** Uniqueness enforced by candidate keys

### Why DKNF Matters (Theoretically)

```
If a table is in DKNF:
  - It is automatically free of ALL anomalies
  - No update, insertion, or deletion anomalies possible
  - It is the "ultimate" normal form

BUT:
  - It is mostly THEORETICAL
  - No general algorithm exists to convert to DKNF
  - It is rarely used in practice
  - You just need to know what it is for exams
```

### Simple Example

**Table: Employee**

| EmpID | Name  | Salary | Tax   |
|-------|-------|--------|-------|
| 1     | Alice | 50000  | 10000 |
| 2     | Bob   | 30000  | 6000  |

Constraint: Tax = 20% of Salary

This constraint is NOT a domain constraint (it's a computation rule) and NOT a key constraint. So this table is NOT in DKNF.

**Fix:** Remove the Tax column (it's derivable from Salary using a domain rule).

| EmpID | Name  | Salary |
|-------|-------|--------|
| 1     | Alice | 50000  |
| 2     | Bob   | 30000  |

Now Tax can be computed, and all remaining constraints are domain + key based. The table is in DKNF.

### Key Takeaway

```
DKNF = The theoretical ideal where ALL constraints come from domains and keys.
       Rarely achieved in practice, but important to understand conceptually.
```

---

## 12. Denormalization

### What Is It?

**Denormalization** = Intentionally introducing redundancy back into a normalized database for **performance reasons**.

### Why Denormalize?

```
Normalization:
  + Eliminates redundancy
  + Prevents anomalies
  - Requires many JOINs for queries (SLOW for large data)
  
Denormalization:
  + Fewer JOINs = FASTER reads
  + Better query performance
  - Redundant data (uses more storage)
  - Risk of anomalies (must handle in application code)
```

### Example

**Normalized (3NF):**

```
Order(OrderID, CustomerID, OrderDate)
Customer(CustomerID, CustomerName, City)

Query: "Show all orders with customer name and city"
  --> Requires a JOIN between Order and Customer
```

**Denormalized:**

```
OrderDenormalized(OrderID, CustomerID, CustomerName, City, OrderDate)

Same query: No JOIN needed, directly read one table
  --> Much faster for read-heavy applications
```

### When to Denormalize

```
Denormalize when:
  - Read operations far outnumber writes
  - Query performance is critical (dashboards, analytics)
  - Data warehouses and reporting databases
  - Caching frequently accessed joined data

Do NOT denormalize when:
  - Write operations are frequent (high risk of inconsistency)
  - Data integrity is paramount (banking, healthcare)
  - The database is small (JOINs are fast enough)
```

---

## 13. Lossless Join and Dependency Preservation

These are the **two properties** a good decomposition must have.

### Lossless Join Decomposition

> When you decompose R into R1 and R2, joining them back must give you
> EXACTLY the original data — no extra rows (spurious tuples) and no missing rows.

**Condition for Lossless Join (Binary Decomposition):**

```
Decompose R into R1 and R2.

The decomposition is lossless if and only if:

  R1 ∩ R2 --> R1    (the common attributes determine all of R1)
  OR
  R1 ∩ R2 --> R2    (the common attributes determine all of R2)

In other words: The common attributes must be a super key
                of at least one of the resulting tables.
```

**Example — Lossless:**

```
R(A, B, C) with FD: A --> B

Decompose into:
  R1(A, B)     -- A is a key for R1 (since A --> B)
  R2(A, C)

Common attributes: {A}
A is a super key of R1? YES (A --> B means A --> AB)

Result: LOSSLESS join decomposition
```

**Example — Lossy (BAD):**

```
R(A, B, C) with FD: A --> B

Decompose into:
  R1(A, B)
  R2(B, C)

Common attributes: {B}
Is B a key of R1? NO.
Is B a key of R2? NO.

Result: LOSSY — joining may create spurious tuples!
```

**Illustration of Spurious Tuples:**

Original table R:

| A  | B  | C  |
|----|----|----|
| a1 | b1 | c1 |
| a2 | b1 | c2 |

R1(A, B):

| A  | B  |
|----|----|
| a1 | b1 |
| a2 | b1 |

R2(B, C):

| B  | C  |
|----|----|
| b1 | c1 |
| b1 | c2 |

R1 JOIN R2 on B:

| A  | B  | C  |
|----|----|----|
| a1 | b1 | c1 |
| a1 | b1 | c2 |
| a2 | b1 | c1 |
| a2 | b1 | c2 |

We got 4 rows instead of the original 2! The extra rows are **spurious tuples**.

### Dependency Preservation

> After decomposition, we should be able to check ALL original FDs
> using only the individual decomposed tables (without needing to join).

```
Original: R(A, B, C) with FDs: A --> B, B --> C

Decompose into:
  R1(A, B) — can check A --> B here
  R2(B, C) — can check B --> C here

ALL original FDs can be checked in individual tables.
Result: DEPENDENCY PRESERVING
```

**Example of NON-preserving decomposition:**

```
Original: R(A, B, C) with FDs: A --> B, B --> C, C --> A

Decompose into:
  R1(A, C)
  R2(B, C)

Can we check A --> B? 
  A is only in R1, B is only in R2. 
  We need to JOIN to check this FD.
  NOT dependency preserving!
```

### Key Takeaway

```
Good Decomposition = Lossless Join + Dependency Preservation

  - 3NF decomposition: ALWAYS achieves BOTH (guaranteed!)
  - BCNF decomposition: ALWAYS achieves lossless join
                         but may NOT preserve all dependencies
```

---

## 14. Canonical Cover and Minimal Cover

### What Is It?

A **canonical cover** (or **minimal cover**) of a set of FDs is the **smallest equivalent set** — same meaning, fewer FDs, simpler form.

### Why Do We Need It?

- Simplifies the set of FDs before decomposition
- Removes redundant and unnecessary dependencies
- Used in algorithms for finding 3NF and BCNF decompositions

### Steps to Find the Canonical Cover

```
Given a set F of functional dependencies:

Step 1: DECOMPOSE the right side
        A --> BC  becomes  A --> B, A --> C
        (Split right-hand side into single attributes)

Step 2: REMOVE extraneous attributes from the LEFT side
        If AB --> C, check: does B --> C hold? (using closures)
          If yes, AB --> C can be simplified to B --> C
        
Step 3: REMOVE redundant FDs
        For each FD, check if it can be derived from the remaining FDs
        If yes, remove it.

Step 4: COMBINE FDs with the same left side
        A --> B, A --> C  becomes  A --> BC
```

### Full Worked Example

```
Given FDs:
  F = { A --> BC,  B --> C,  AB --> D,  D --> A }

Step 1: Decompose right side
  A --> B
  A --> C
  B --> C
  AB --> D
  D --> A

Step 2: Remove extraneous left-side attributes
  Check AB --> D:
    Can A alone determine D?
    A+ (closure of A) = {A, B, C, D}  (A-->B, A-->C, then AB-->D so A-->D)
    YES! A --> D is valid
    
    Simplify AB --> D  to  A --> D

  Updated set:
    A --> B
    A --> C
    B --> C
    A --> D
    D --> A

Step 3: Remove redundant FDs
  Check A --> C:
    Remove it. Can we still derive A --> C from {A-->B, B-->C, A-->D, D-->A}?
    A --> B --> C.  YES!
    
    REMOVE A --> C (it's redundant)

  Updated set:
    A --> B
    B --> C
    A --> D
    D --> A

Step 4: Combine same left side
    A --> B,  A --> D  becomes  A --> BD

CANONICAL COVER:
  Fc = { A --> BD,  B --> C,  D --> A }
```

---

## 15. Finding Candidate Keys from FDs

### Attribute Closure Method

The **closure** of a set of attributes X, written X+, is the set of ALL attributes that can be determined from X using the given FDs.

### How to Compute Closure

```
Algorithm for computing X+ (closure of X):

  1. Start: result = X
  2. Repeat:
       For each FD  A --> B in the set:
         If A is a subset of result:
           Add B to result
     Until no change
  3. X+ = result
```

### Worked Example

```
Relation: R(A, B, C, D, E)
FDs: { A --> B,  BC --> D,  D --> E,  E --> A }

Find closure of {A, C}:

  Start: {A, C}
  A --> B:   A is in {A,C}?  YES.  Add B.  Result = {A, B, C}
  BC --> D:  BC in {A,B,C}?  YES.  Add D.  Result = {A, B, C, D}
  D --> E:   D in {A,B,C,D}? YES.  Add E.  Result = {A, B, C, D, E}
  
  {A, C}+ = {A, B, C, D, E} = ALL attributes

  So {A, C} is a SUPER KEY!
  Is it minimal? (i.e., is it a candidate key?)
    {A}+ = {A, B}  -- not all attributes
    {C}+ = {C}     -- not all attributes
  Neither subset determines everything, so {A, C} IS a candidate key!
```

### Strategy to Find ALL Candidate Keys

```
Step 1: Identify attributes that appear:
  - ONLY on the LEFT side of FDs   --> MUST be in every key
  - ONLY on the RIGHT side of FDs  --> NEVER in any key
  - On BOTH sides                  --> MAY be in some keys
  - In NO FD at all                --> MUST be in every key

Step 2: Start with the "must be in every key" attributes
Step 3: Compute their closure
Step 4: If closure = all attributes, that's a candidate key
Step 5: If not, try adding each "may be" attribute one by one
```

### Full Example

```
R(A, B, C, D, E, F)
FDs: { AB --> C,  C --> D,  D --> E,  E --> A }

Step 1: Attribute analysis:
  Left only:  B, F  (B appears left in AB-->C, F appears nowhere)
  Right only: (none that aren't also on the left)
  Both sides: A, C, D, E
  No FD:      F

  MUST be in every key: {B, F}

Step 2: Compute {B, F}+
  {B, F}+ = {B, F}  (no FD has just B or F on left)
  Not all attributes.

Step 3: Try adding each "both sides" attribute:
  {A, B, F}+ :
    AB --> C:  AB in {A,B,F}? YES. Add C. {A,B,C,F}
    C --> D:   Add D. {A,B,C,D,F}
    D --> E:   Add E. {A,B,C,D,E,F} = ALL!
  
  {A, B, F} is a super key. Minimal? 
    We already showed {B,F}+ doesn't cover all.
    {A,F}+ = {A,F} -- No.
    {A,B}+ = {A,B,C,D,E} -- missing F. No.
    So {A,B,F} is a CANDIDATE KEY.

  {C, B, F}+ :
    C --> D. {B,C,D,F}
    D --> E. {B,C,D,E,F}
    E --> A. {A,B,C,D,E,F} = ALL!
    
    {C, B, F} is a candidate key.

  {D, B, F}+ :
    D --> E. {B,D,E,F}
    E --> A. {A,B,D,E,F}
    AB --> C. {A,B,C,D,E,F} = ALL!
    
    {D, B, F} is a candidate key.

  {E, B, F}+ :
    E --> A. {A,B,E,F}
    AB --> C. {A,B,C,E,F}
    C --> D. {A,B,C,D,E,F} = ALL!
    
    {E, B, F} is a candidate key.

Candidate Keys: {ABF}, {BCF}, {BDF}, {BEF}
```

---

## 16. Attribute Closure

This section provides additional practice with closures (essential for exams!).

### Quick Reference

```
To check if X is a super key:
  Compute X+
  If X+ contains ALL attributes of R, then X is a super key.
  If X is also MINIMAL (no subset of X is a super key), then X is a candidate key.

To check if X --> Y holds:
  Compute X+
  If Y is a subset of X+, then X --> Y holds.
```

### Practice Problem

```
R(A, B, C, D, E)
FDs: { A --> BC,  CD --> E,  B --> D,  E --> A }

Q1: Is {A} a candidate key?
  {A}+ = {A} --> add BC = {A,B,C} --> B-->D, add D = {A,B,C,D} 
       --> CD-->E, add E = {A,B,C,D,E} = ALL
  YES, {A} is a super key. It's a single attribute, so it's minimal.
  {A} IS a candidate key.

Q2: What about {E}?
  {E}+ = {E} --> E-->A, add A = {A,E} --> A-->BC = {A,B,C,E} 
       --> B-->D = {A,B,C,D,E} = ALL
  {E} is also a candidate key!

Q3: What about {B}?
  {B}+ = {B} --> B-->D = {B,D}
  NOT all attributes. {B} is NOT a super key.

Q4: Does CD --> AB hold?
  {C,D}+ = {C,D} --> CD-->E = {C,D,E} --> E-->A = {A,C,D,E} 
         --> A-->BC = {A,B,C,D,E}
  Is {A,B} a subset of {A,B,C,D,E}? YES!
  So CD --> AB DOES hold.

Candidate Keys: {A}, {E}, {B,C} (verify {B,C}+ = all)
  {B,C}+ = {B,C} --> A-->BC? No A. B-->D = {B,C,D} --> CD-->E = {B,C,D,E}
         --> E-->A = {A,B,C,D,E} = ALL. YES!
  {B,C} is also a candidate key.
```

---

## 17. Decomposition Techniques

### 3NF Decomposition Algorithm (Synthesis Method)

This algorithm **guarantees** both lossless join AND dependency preservation.

```
Input: Relation R with FD set F

Step 1: Find the canonical cover Fc of F

Step 2: For each FD  X --> Y  in Fc:
          Create a table with attributes (X ∪ Y)
          The key of this table is X

Step 3: If no table's attributes contain a candidate key of R:
          Add a table with attributes of one candidate key

Step 4: Remove any table whose attributes are a subset
        of another table's attributes
```

### Worked Example

```
R(A, B, C, D) with FDs: { A --> B,  B --> C,  A --> D }

Step 1: Canonical cover
  A --> B,  B --> C,  A --> D
  Combine: A --> BD,  B --> C
  Fc = { A --> BD,  B --> C }

Step 2: Create tables
  From A --> BD:  R1(A, B, D)  key = A
  From B --> C:   R2(B, C)     key = B

Step 3: Does any table contain a candidate key?
  Candidate key of R = {A}  (since {A}+ = {A,B,C,D})
  R1 contains A. YES.

Step 4: No subset tables to remove.

Result: R1(A, B, D) and R2(B, C)
```

### BCNF Decomposition Algorithm

This algorithm **guarantees** lossless join but may NOT preserve dependencies.

```
Input: Relation R with FD set F

Step 1: Find an FD X --> Y that violates BCNF
        (X is not a super key of the current table)

Step 2: Decompose into:
          R1 = X ∪ Y          (with key X)
          R2 = R - Y + X      (original minus Y, keep X)

Step 3: Repeat for R1 and R2 until all tables are in BCNF
```

### Worked Example

```
R(A, B, C, D) with FDs: { A --> B,  B --> C }
Candidate key: {A, D}

Check BCNF:
  A --> B:  Is A a super key? {A}+ = {A,B,C}. No D. NOT a super key.
  VIOLATION!

Decompose on A --> B:
  R1(A, B)     -- contains A --> B (A is key here)
  R2(A, C, D)  -- remaining attributes with A

Check R2: Does B --> C affect R2? 
  B is not in R2, so we need to project FDs.
  FDs on R2(A, C, D): A --> C (derived: A-->B-->C)
  Is A a super key of R2? {A}+ in R2 = {A,C}. No D. NOT a super key.
  VIOLATION!

Decompose R2 on A --> C:
  R3(A, C)     -- A is key
  R4(A, D)     -- A is key? {A}+ in R4 = {A}. 
                  Actually key is {A,D} reduced... 
                  No FD determines D from A alone.
                  Key of R4 = {A, D}

Final tables: R1(A, B), R3(A, C), R4(A, D)
All in BCNF.

But: B --> C cannot be checked without joining R1 and R3!
  NOT dependency preserving.
```

---

## 18. Normal Forms — Quick Comparison Table

| Normal Form | Requirement | Eliminates | Key Rule |
|-------------|-------------|------------|----------|
| **1NF** | Atomic values, no repeating groups | Multi-valued cells, repeating columns | Each cell = one value |
| **2NF** | 1NF + No partial dependencies | Attributes depending on part of composite key | Full dependency on whole key |
| **3NF** | 2NF + No transitive dependencies | Non-key attribute depending on another non-key | Non-key depends only on key |
| **BCNF** | Every determinant is a super key | Prime attribute dependencies on non-super-keys | Every X --> Y, X is super key |
| **4NF** | BCNF + No multi-valued dependencies | Independent multi-valued facts | Separate independent MVDs |
| **5NF** | 4NF + No join dependencies | Cyclic join dependencies | Cannot split further losslessly |
| **DKNF** | All constraints from domain + keys | All possible anomalies | Theoretical ideal |

### Hierarchy of Strictness

```
1NF  ⊇  2NF  ⊇  3NF  ⊇  BCNF  ⊇  4NF  ⊇  5NF  ⊇  DKNF

Every BCNF table is in 3NF (but not vice versa)
Every 4NF table is in BCNF (but not vice versa)
... and so on
```

### Practical Usage

```
Most real-world databases: 3NF or BCNF
Data warehouses: Often denormalized (1NF or 2NF)
Exam questions: Usually test up to BCNF, sometimes 4NF
Interview questions: 1NF through BCNF are most common
```

---

## 19. Common Interview Questions

### Q1: What is normalization? Why is it needed?

**Answer:** Normalization is the process of organizing a relational database to reduce redundancy and eliminate anomalies (insertion, update, deletion). It involves decomposing tables into smaller tables following specific rules (normal forms) while maintaining relationships through foreign keys.

### Q2: Explain 1NF, 2NF, 3NF with examples.

**Answer:** Use the mnemonic: **"The key, the whole key, and nothing but the key."**
- **1NF:** Each column has atomic values (the key — unique rows)
- **2NF:** No partial dependencies on composite key (the whole key)
- **3NF:** No transitive dependencies (nothing but the key)

### Q3: Difference between 3NF and BCNF?

**Answer:**
- 3NF allows X --> A when A is a prime attribute, even if X isn't a super key
- BCNF requires X to be a super key for ALL dependencies, no exceptions
- 3NF decomposition preserves dependencies; BCNF may not

### Q4: Can a table be in 3NF but not BCNF?

**Answer:** Yes! This happens when a prime attribute is functionally dependent on a non-super-key attribute. Example: Student-Subject-Professor where Professor --> Subject and Professor is prime (part of candidate key {Student, Professor}).

### Q5: What is denormalization? When would you use it?

**Answer:** Intentionally adding redundancy to a normalized database for performance. Used in read-heavy applications, data warehouses, and reporting systems where JOIN operations are too slow.

### Q6: What is a lossless join decomposition?

**Answer:** A decomposition where joining the resulting tables back together produces EXACTLY the original table — no extra or missing rows. The condition is that the common attributes between any two decomposed tables must be a super key of at least one of them.

### Q7: Difference between partial and transitive dependency?

**Answer:**
- **Partial:** A non-prime attribute depends on PART of a composite candidate key
  (e.g., {StudentID, CourseID} is key, but StudentID --> StudentName)
- **Transitive:** A non-prime attribute depends on the key THROUGH another non-prime attribute
  (e.g., EmpID --> DeptID --> DeptName)

### Q8: What is a multi-valued dependency?

**Answer:** X -->> Y means for each X, the set of Y values is independent of all other attributes. Example: Employee -->> Skill and Employee -->> Language where skills and languages are unrelated. Resolved by 4NF (separate into independent tables).

### Q9: Why might BCNF decomposition lose dependency preservation?

**Answer:** BCNF requires every determinant to be a super key. Sometimes the only way to achieve this is to split a table such that an FD's attributes end up in different tables, making it impossible to check that FD without joining. 3NF avoids this by allowing one exception (prime attribute on the right side).

### Q10: How do you find candidate keys from functional dependencies?

**Answer:**
1. Find attributes that appear only on the LEFT side of FDs — they must be in every key
2. Find attributes in NO FD — they must be in every key
3. Start with these "must-have" attributes, compute their closure
4. If closure = all attributes, that set is a candidate key
5. If not, try adding attributes that appear on both sides

---

## 20. Cheat Sheet — Remember This!

### The Golden Mnemonic

```
"Every non-key attribute must depend on
 THE KEY          --> 1NF (unique identification)
 THE WHOLE KEY    --> 2NF (no partial dependency)
 NOTHING BUT KEY  --> 3NF (no transitive dependency)
 ...so help me Codd" --> BCNF (every determinant is a super key)
```

### Quick Decision Flowchart

```
Is every cell atomic with no repeating groups?
  NO  --> Not even 1NF. Fix it.
  YES --> In 1NF

Does any non-prime attribute depend on PART of a composite key?
  YES --> Not 2NF. Remove partial dependencies.
  NO  --> In 2NF

Does any non-prime attribute depend on another non-prime attribute?
  YES --> Not 3NF. Remove transitive dependencies.
  NO  --> In 3NF

Is every determinant a super key?
  NO  --> Not BCNF. Decompose on the violating FD.
  YES --> In BCNF

Are there independent multi-valued dependencies?
  YES --> Not 4NF. Separate into different tables.
  NO  --> In 4NF

Can the table be recreated by joining 3+ smaller tables?
  YES --> Not 5NF. Decompose further.
  NO  --> In 5NF
```

### Anomaly Quick Reference

| Anomaly | What Goes Wrong | Solution |
|---------|----------------|----------|
| Insertion | Can't add data without unrelated data | Separate independent entities |
| Update | Must change same fact in multiple rows | Store each fact once |
| Deletion | Removing data loses unrelated info | Separate independent entities |

### Dependency Types at a Glance

| Dependency Type | Pattern | Fixed By |
|----------------|---------|----------|
| Partial | Part-of-key --> Non-prime | 2NF |
| Transitive | Key --> Non-prime --> Non-prime | 3NF |
| Non-super-key determinant | Non-super-key --> Prime | BCNF |
| Multi-valued | Key -->> A, Key -->> B (independent) | 4NF |
| Join | 3-way decomposable relationship | 5NF |

### Properties of Decomposition

| Property | 3NF Decomposition | BCNF Decomposition |
|----------|-------------------|-------------------|
| Lossless Join | Always guaranteed | Always guaranteed |
| Dependency Preservation | Always guaranteed | NOT always guaranteed |
| Algorithm | Synthesis (from canonical cover) | Repeated decomposition |

### Armstrong's Axioms (For Deriving FDs)

```
1. Reflexivity:   If Y ⊆ X, then X --> Y           (trivial)
2. Augmentation:  If X --> Y, then XZ --> YZ         (add same to both sides)
3. Transitivity:  If X --> Y and Y --> Z, then X --> Z

Derived Rules:
4. Union:         If X --> Y and X --> Z, then X --> YZ
5. Decomposition: If X --> YZ, then X --> Y and X --> Z
6. Pseudotransitivity: If X --> Y and WY --> Z, then WX --> Z
```

---

## End-to-End Walkthrough: Normalizing a Table from UNF to BCNF

Let's normalize a real-world example step by step.

### Starting Point: University Database (Unnormalized)

| StudentID | StudentName | Courses                                             |
|-----------|-------------|-----------------------------------------------------|
| 101       | Alice       | CS101-Databases-Prof.Smith, CS102-OS-Prof.Jones     |
| 102       | Bob         | CS101-Databases-Prof.Smith                          |
| 103       | Charlie     | CS102-OS-Prof.Jones, CS103-Networks-Prof.Smith      |

### Step 1: Convert to 1NF

Remove multi-valued cells and repeating groups.

| StudentID | StudentName | CourseID | CourseName | Professor  |
|-----------|-------------|----------|------------|------------|
| 101       | Alice       | CS101    | Databases  | Prof.Smith |
| 101       | Alice       | CS102    | OS         | Prof.Jones |
| 102       | Bob         | CS101    | Databases  | Prof.Smith |
| 103       | Charlie     | CS102    | OS         | Prof.Jones |
| 103       | Charlie     | CS103    | Networks   | Prof.Smith |

Now in 1NF. Candidate Key: {StudentID, CourseID}

### Step 2: Convert to 2NF

Check for partial dependencies:

```
StudentID --> StudentName        (partial: depends on part of key)
CourseID  --> CourseName          (partial: depends on part of key)
CourseID  --> Professor           (partial: depends on part of key)
```

Decompose:

**Student:**

| StudentID | StudentName |
|-----------|-------------|
| 101       | Alice       |
| 102       | Bob         |
| 103       | Charlie     |

**Course:**

| CourseID | CourseName | Professor  |
|----------|------------|------------|
| CS101    | Databases  | Prof.Smith |
| CS102    | OS         | Prof.Jones |
| CS103    | Networks   | Prof.Smith |

**Enrollment:**

| StudentID | CourseID |
|-----------|----------|
| 101       | CS101    |
| 101       | CS102    |
| 102       | CS101    |
| 103       | CS102    |
| 103       | CS103    |

Now in 2NF.

### Step 3: Convert to 3NF

Check for transitive dependencies:

- **Student table:** StudentID --> StudentName. No transitive dependency. Already in 3NF.
- **Course table:** CourseID --> Professor --> ? No, Professor doesn't determine anything else here.
- **Enrollment table:** No non-prime attributes at all. Already in 3NF.

All tables are in 3NF!

### Step 4: Check for BCNF

For every FD X --> Y, is X a super key?

- **Student:** StudentID --> StudentName. StudentID is the key. In BCNF.
- **Course:** CourseID --> CourseName, CourseID --> Professor. CourseID is the key. In BCNF.
- **Enrollment:** No non-trivial FDs beyond the key. In BCNF.

All tables are in BCNF!

### Final Schema

```
Student(StudentID, StudentName)
  PK: StudentID

Course(CourseID, CourseName, Professor)
  PK: CourseID

Enrollment(StudentID, CourseID)
  PK: {StudentID, CourseID}
  FK: StudentID --> Student.StudentID
  FK: CourseID  --> Course.CourseID
```

```
Relationship Diagram:

  Student ──────< Enrollment >────── Course
  (1:M)                                (1:M)

  One student can enroll in many courses.
  One course can have many students.
  This is a Many-to-Many relationship resolved by the Enrollment junction table.
```

---

> **Remember:** Normalization is a systematic, step-by-step process. Master the three
> anomalies, understand each dependency type, and practice computing closures.
> For interviews, focus on 1NF through BCNF with clear examples.
> For exams, also know 4NF, 5NF, canonical covers, and decomposition algorithms.

---
