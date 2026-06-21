# 📘 Relational Model — Complete Concepts Guide

> **Covers everything from the Relational Model and Normalization syllabus EXCEPT normalization itself.**
> Designed for GATE, UGC-NET, and tech interview preparation.
> Every concept explained in simple language with multiple illustrated examples.

---

## Table of Contents

1. [Intension and Extension](#1-intension-and-extension)
2. [Keys in DBMS](#2-keys-in-dbms)
3. [Functional Dependency](#3-functional-dependency)
4. [Armstrong's Axioms](#4-armstrongs-axioms)
5. [Inference Rules (Derived from Armstrong's Axioms)](#5-inference-rules-derived-from-armstrongs-axioms)
6. [Closure in Functional Dependencies](#6-closure-in-functional-dependencies)
7. [Denormalization](#7-denormalization)
8. [Equivalence of FD Sets](#8-equivalence-of-fd-sets)
9. [Canonical Cover (Minimal Cover)](#9-canonical-cover-minimal-cover)
10. [Lossless Join Decomposition](#10-lossless-join-decomposition)
11. [Dependency Preservation](#11-dependency-preservation)
12. [GATE Previous Year Patterns](#12-gate-previous-year-patterns)
13. [Quick Revision Cheat Sheet](#13-quick-revision-cheat-sheet)

---

## 1. Intension and Extension

These two terms describe **what** a database table looks like vs. **what data** it currently holds.

### 1.1 Intension (Schema)

> **Intension** = The **structure / definition** of a relation (table).
> It is the **logical design** — column names, data types, constraints.
> It **rarely changes** once defined.

Think of it as the **blueprint** of a house — it tells you the layout, but not who lives there.

**Example:**

```
Student(StudentID: INT, Name: VARCHAR(50), Age: INT, Branch: VARCHAR(20))
         ^^^^^^^^       ^^^^               ^^^       ^^^^^^
         Attribute      Attribute          Attr      Attribute
         
Domain constraints:
  - StudentID: positive integer, unique
  - Name: string up to 50 chars
  - Age: integer between 17 and 60
  - Branch: one of {CSE, ECE, ME, CE, EE}
```

The intension includes:
- **Relation name:** Student
- **Attribute names:** StudentID, Name, Age, Branch
- **Attribute domains:** INT, VARCHAR, etc.
- **Constraints:** Primary key, NOT NULL, CHECK, etc.
- **Degree:** Number of attributes (here, 4)

### 1.2 Extension (Instance / State)

> **Extension** = The **actual data** (set of tuples/rows) in the table at any given moment.
> It **changes frequently** as data is inserted, updated, or deleted.

Think of it as the **people currently living** in the house.

**Example:**

| StudentID | Name    | Age | Branch |
|-----------|---------|-----|--------|
| 101       | Alice   | 20  | CSE    |
| 102       | Bob     | 21  | ECE    |
| 103       | Charlie | 19  | CSE    |

This is the **extension** (or instance/state) of the Student relation at this moment.

Tomorrow, if Charlie is deleted and a new student David is added, the extension changes but the intension stays the same.

### 1.3 Comparison Table

| Property          | Intension (Schema)                    | Extension (Instance)                 |
|-------------------|---------------------------------------|--------------------------------------|
| What is it?       | Structure / definition of the table   | Actual data / rows in the table      |
| Changes?          | Rarely (only on ALTER TABLE)          | Frequently (INSERT, UPDATE, DELETE)  |
| Also called       | Schema, type, intention               | State, instance, snapshot            |
| Defined by        | DDL (CREATE TABLE, ALTER TABLE)       | DML (INSERT, UPDATE, DELETE)         |
| Analogy           | Blueprint of a house                  | People living in the house           |
| Time-dependent?   | No (relatively static)                | Yes (changes over time)             |
| Degree            | Number of attributes (columns)        | N/A                                  |
| Cardinality       | N/A                                   | Number of tuples (rows)              |

### 1.4 Degree vs. Cardinality

```
Relation: Student

Degree = Number of columns (attributes) = 4
         (StudentID, Name, Age, Branch)
         This is part of the INTENSION.

Cardinality = Number of rows (tuples) = 3
              (Alice, Bob, Charlie)
              This is part of the EXTENSION.
```

| StudentID | Name    | Age | Branch |    <-- 4 columns = Degree 4
|-----------|---------|-----|--------|
| 101       | Alice   | 20  | CSE    |  |
| 102       | Bob     | 21  | ECE    |  |-- 3 rows = Cardinality 3
| 103       | Charlie | 19  | CSE    |  |

### 1.5 GATE-Style Question

**Q:** The number of tuples in a relation is called ___, and the number of attributes is called ___.

**A:** Cardinality; Degree

**Q:** Which changes more frequently — intension or extension?

**A:** Extension (data changes with every INSERT/UPDATE/DELETE)

---

## 2. Keys in DBMS

Keys are attributes (or sets of attributes) that uniquely identify rows and establish relationships between tables. Understanding keys is **fundamental** — almost every GATE/interview question on relational model involves keys.

### 2.1 Super Key

> A **super key** is any set of one or more attributes that can **uniquely identify** every tuple in a relation.

**Rule:** Any superset of a candidate key is a super key.

**Example:**

| EmpID | Name  | Email            | Phone      |
|-------|-------|------------------|------------|
| 1     | Alice | alice@mail.com   | 9876543210 |
| 2     | Bob   | bob@mail.com     | 9123456789 |
| 3     | Alice | alice2@mail.com  | 9111222333 |

```
Super Keys (assuming EmpID and Email are unique):

  {EmpID}                    -- uniquely identifies (minimal)
  {Email}                    -- uniquely identifies (minimal)
  {EmpID, Name}              -- still unique (superset of {EmpID})
  {EmpID, Email}             -- still unique (superset)
  {EmpID, Name, Email}       -- still unique (superset)
  {EmpID, Name, Email, Phone} -- always a super key (all attributes)
  {Email, Phone}             -- still unique (superset of {Email})

NOT a Super Key:
  {Name}                     -- Alice appears twice! Not unique.
  {Phone}                    -- could be unique here, depends on constraint
```

**Important:** If a relation has `n` attributes and `{A}` is a candidate key, then there are `2^(n-1)` super keys containing `A`.

### 2.2 Candidate Key

> A **candidate key** is a **minimal** super key — remove any attribute from it, and it's no longer a super key.

```
From the above example:

  {EmpID}       -- minimal super key (remove nothing, still single attr)
  {Email}       -- minimal super key
  
  {EmpID, Name} -- NOT a candidate key (EmpID alone is enough)
```

**Properties of Candidate Keys:**
- **Uniqueness:** No two tuples have the same value
- **Minimality:** No proper subset is also a super key
- **NOT NULL:** No attribute in a candidate key can be NULL
- A relation can have **multiple** candidate keys

### 2.3 Primary Key

> The **primary key** is the candidate key **chosen by the database designer** as the main identifier.

```
Convention: Underline the primary key in schemas.

  Employee(EmpID, Name, Email, Phone)
           ^^^^^
         Primary Key

Rules:
  - Only ONE primary key per table
  - Cannot be NULL
  - Must be unique
  - Should be stable (not change frequently)
  - Preferably short and simple
```

**Why not choose Email as primary key?**
- Emails can change (people switch providers)
- Emails are long strings (slower for indexing)
- EmpID is more stable and efficient

### 2.4 Alternate Key

> Any candidate key that is **NOT chosen** as the primary key.

```
Candidate Keys: {EmpID}, {Email}
Primary Key:    {EmpID}
Alternate Key:  {Email}
```

### 2.5 Foreign Key

> A **foreign key** is an attribute in one table that **references the primary key** of another table.
> It creates a **link** between two tables.

**Example:**

```
Department(DeptID, DeptName)            -- Parent / Referenced table
           ^^^^^^
           Primary Key

Employee(EmpID, Name, DeptID)           -- Child / Referencing table
         ^^^^^        ^^^^^^
         PK           FK --> Department.DeptID
```

**Department (Parent):**

| DeptID | DeptName    |
|--------|-------------|
| D1     | Engineering |
| D2     | Marketing   |

**Employee (Child):**

| EmpID | Name    | DeptID |
|-------|---------|--------|
| 1     | Alice   | D1     |
| 2     | Bob     | D2     |
| 3     | Charlie | D1     |

```
Rules of Foreign Keys:
  1. FK value must either match a PK value in the parent table OR be NULL
     (this is called REFERENTIAL INTEGRITY)
  2. FK can have duplicate values (many employees in same dept)
  3. FK can be NULL (employee not yet assigned to a dept)
  4. FK can reference the SAME table (self-referencing FK)
     Example: Employee.ManagerID --> Employee.EmpID
```

### 2.6 Composite Key

> A key made up of **two or more attributes** together.

**Example:**

| StudentID | CourseID | Semester | Grade |
|-----------|----------|----------|-------|
| 101       | CS101    | Fall24   | A     |
| 101       | CS102    | Fall24   | B     |
| 102       | CS101    | Fall24   | A+    |
| 101       | CS101    | Spr25    | A     |

```
No single attribute uniquely identifies a row:
  - StudentID 101 appears 3 times
  - CourseID CS101 appears 3 times
  - Semester Fall24 appears 3 times

Composite Key: {StudentID, CourseID, Semester}
  This combination is unique for every row.
```

### 2.7 Surrogate Key

> An **artificially generated** key (usually auto-increment integer or UUID) that has no business meaning.

```
OrderID (auto-increment: 1, 2, 3, ...)
UUID    (e.g., "a1b2c3d4-e5f6-7890-abcd-ef1234567890")

Why use surrogate keys?
  - Natural keys can change (name, email, phone)
  - Natural keys can be long or complex (composite keys)
  - Surrogate keys are simple, short, and immutable
  - Better performance for JOINs and indexing
```

### 2.8 Natural Key

> A key formed from **real-world data** that naturally exists in the domain.

```
Examples:
  - Social Security Number (SSN) for US citizens
  - Aadhaar Number for Indian citizens
  - ISBN for books
  - PAN Card Number for tax purposes

Pros: Meaningful, no extra storage
Cons: Can change, may have privacy issues, might be long
```

### 2.9 All Keys at a Glance

```
                        Super Key
                       /    |    \
                      /     |     \
            Candidate Key  ...   Other super keys
            /         \             (non-minimal)
           /           \
     Primary Key    Alternate Key(s)


  Foreign Key:  Lives in another table, points to a Primary Key
  Composite Key: A key with 2+ attributes
  Surrogate Key: Artificial, system-generated
  Natural Key:   Comes from real-world data
```

### 2.10 Counting Super Keys and Candidate Keys (GATE Favourite)

**Formula:** If a candidate key has `k` attributes and the relation has `n` attributes total, then the number of super keys containing that candidate key = `2^(n-k)`.

**Example 1:**

```
R(A, B, C, D, E)  -- n = 5 attributes
Candidate Key: {A, B}  -- k = 2 attributes

Super keys containing {A, B} = 2^(5-2) = 2^3 = 8

They are:
  {A,B}, {A,B,C}, {A,B,D}, {A,B,E},
  {A,B,C,D}, {A,B,C,E}, {A,B,D,E}, {A,B,C,D,E}
```

**Example 2: Multiple Candidate Keys**

```
R(A, B, C, D)  -- n = 4
Candidate Keys: {A} and {B, C}

Super keys from {A} = 2^(4-1) = 8
Super keys from {B,C} = 2^(4-2) = 4

But some super keys contain BOTH {A} and {B,C}:
  Super keys from {A,B,C} = 2^(4-3) = 2

By Inclusion-Exclusion:
  Total unique super keys = 8 + 4 - 2 = 10
```

### 2.11 Prime vs Non-Prime Attributes

```
R(A, B, C, D, E)
Candidate Keys: {A, B} and {C, D}

Prime attributes: A, B, C, D
  (attributes that appear in ANY candidate key)

Non-prime attributes: E
  (attributes that are NOT part of ANY candidate key)
```

This distinction is critical for 2NF and 3NF checks!

### 2.12 GATE-Style Questions

**Q1:** A relation R has 6 attributes and candidate keys {A, B} and {C}. How many super keys does R have?

```
From {A,B}: 2^(6-2) = 16
From {C}:   2^(6-1) = 32
From {A,B,C}: 2^(6-3) = 8

Total = 16 + 32 - 8 = 40
```

**Q2:** If a relation has no NULL values and all rows are distinct, is every attribute combination a super key?

**A:** No. Distinctness of rows means the set of ALL attributes is a super key. But not every subset is — e.g., a single column could have duplicate values.

**Q3:** Can a foreign key be part of the primary key?

**A:** Yes! This commonly happens in junction/bridge tables for many-to-many relationships.

```
Enrollment(StudentID, CourseID, Grade)
            ^^^^^^^^   ^^^^^^^^
            FK (to     FK (to
            Student)   Course)
            
Primary Key = {StudentID, CourseID}
Both attributes are foreign keys AND together form the primary key.
```

---

## 3. Functional Dependency

Functional Dependencies (FDs) are the mathematical foundation of normalization and relational design. They describe **constraints** on the data — how one attribute's value determines another.

### 3.1 Definition

> **X --> Y** (X functionally determines Y) means:
> If two tuples have the **same value of X**, they MUST have the **same value of Y**.

```
Formally:
  For any tuples t1 and t2 in relation R:
    If t1[X] = t2[X], then t1[Y] = t2[Y]
```

### 3.2 Examples of FDs

**Example 1: Employee Table**

| EmpID | Name    | DeptID | DeptName    | Salary |
|-------|---------|--------|-------------|--------|
| 1     | Alice   | D1     | Engineering | 80000  |
| 2     | Bob     | D2     | Marketing   | 60000  |
| 3     | Charlie | D1     | Engineering | 75000  |
| 4     | Diana   | D2     | Marketing   | 65000  |

```
Valid FDs:
  EmpID  --> Name        (each EmpID has exactly one Name)
  EmpID  --> DeptID      (each EmpID has exactly one DeptID)
  EmpID  --> Salary      (each EmpID has exactly one Salary)
  DeptID --> DeptName    (each DeptID has exactly one DeptName)

Invalid FDs:
  Name --> EmpID         (two people could share the same name)
  DeptID --> Salary      (D1 has 80000 and 75000 — NOT unique)
  DeptName --> Salary    (Engineering has 80000 and 75000)
  DeptID --> EmpID       (D1 maps to both 1 and 3)
```

**Example 2: Student Registration**

| RollNo | Name  | Course | Professor  |
|--------|-------|--------|------------|
| 1      | Alice | DBMS   | Dr. Gupta  |
| 2      | Bob   | OS     | Dr. Singh  |
| 3      | Alice | CN     | Dr. Verma  |
| 1      | Alice | OS     | Dr. Singh  |

```
FDs:
  RollNo --> Name              (same RollNo always has same Name)
  Course --> Professor         (each course has one professor)
  {RollNo, Course} --> all     (this is the candidate key)

Non-FDs:
  Name --> RollNo              (Alice has RollNo 1 and 3? No, only 1)
                               (Actually Alice always has RollNo 1, but
                                in general names aren't unique)
  RollNo --> Course            (RollNo 1 has DBMS and OS — NOT a valid FD)
```

### 3.3 Trivial vs Non-Trivial FDs

#### Trivial FD

> X --> Y is **trivial** if Y is a **subset** of X.

```
{A, B} --> A       trivial (A is part of {A, B})
{A, B} --> B       trivial
{A, B} --> {A, B}  trivial
{Name, Age} --> Name   trivial
```

Trivial FDs are always true by definition — they give us no new information.

#### Non-Trivial FD

> X --> Y is **non-trivial** if Y is **NOT** a subset of X.

```
EmpID --> Name               non-trivial
{A, B} --> C                 non-trivial (C is not in {A, B})
{A, B} --> {B, C}            non-trivial (C is not in {A, B})
```

#### Completely Non-Trivial FD

> X --> Y is **completely non-trivial** if X and Y have **no attributes in common**.

```
{A, B} --> {C, D}    completely non-trivial (no overlap)
{A, B} --> {B, C}    non-trivial but NOT completely (B is common)
EmpID --> Name        completely non-trivial
```

### 3.4 Types of FDs Visualized

```
                  Functional Dependencies
                 /          |            \
           Trivial     Non-Trivial    Multi-Valued
           (Y ⊆ X)    (Y ⊄ X)        (X -->> Y)
                        /    \
                    Full     Partial
                             (part of key
                              determines Y)
                              
          Also: Transitive FD
                (X --> Y --> Z, Y not a key)
```

### 3.5 Full vs Partial FD (Detailed)

#### Full FD

> X --> Y is a **full FD** if no proper subset of X determines Y.

```
R(StudentID, CourseID, Grade)

{StudentID, CourseID} --> Grade

Is it full?
  StudentID --> Grade?   NO (one student has many grades)
  CourseID --> Grade?    NO (one course has many grades)
  
  Neither subset works, so it IS a full FD.
```

#### Partial FD

> X --> Y is a **partial FD** if some proper subset of X also determines Y.

```
R(StudentID, CourseID, StudentName, Grade)
Key = {StudentID, CourseID}

{StudentID, CourseID} --> StudentName

But: StudentID --> StudentName   (just part of the key works!)

This is a PARTIAL FD.
```

**Why it matters:** Partial FDs violate 2NF.

### 3.6 Transitive FD (Detailed)

> X --> Y and Y --> Z (where Y is not a super key and Y does not determine X) implies X --> Z transitively.

```
Employee Table:
  EmpID --> DeptID     (direct dependency)
  DeptID --> DeptName  (direct dependency, DeptID is NOT a super key)
  
  Therefore: EmpID --> DeptName  (transitive dependency)
  
  Chain: EmpID --> DeptID --> DeptName
```

**Visualized:**

```
  EmpID ────directly───> DeptID ────directly───> DeptName
    |                                               ^
    |___________transitively________________________|
```

**Why it matters:** Transitive FDs violate 3NF.

### 3.7 How to Identify FDs from a Table

```
Method: For each pair of attribute sets (X, Y):
  1. Group rows by the values of X
  2. Within each group, check if Y has the same value
  3. If YES for all groups, then X --> Y holds
  4. If any group has different Y values, X --> Y does NOT hold
```

**Example:**

| A | B | C |
|---|---|---|
| 1 | 2 | 3 |
| 1 | 2 | 3 |
| 2 | 3 | 4 |
| 2 | 3 | 5 |

```
Does A --> B?
  Group A=1: B = {2, 2} = same. OK.
  Group A=2: B = {3, 3} = same. OK.
  YES, A --> B holds.

Does A --> C?
  Group A=1: C = {3, 3} = same. OK.
  Group A=2: C = {4, 5} = DIFFERENT!
  NO, A --> C does NOT hold.

Does B --> C?
  Group B=2: C = {3, 3} = same. OK.
  Group B=3: C = {4, 5} = DIFFERENT!
  NO, B --> C does NOT hold.

Does {A, B} --> C?
  Group (1,2): C = {3, 3} = same. OK.
  Group (2,3): C = {4, 5} = DIFFERENT!
  NO, {A,B} --> C does NOT hold.
```

**Important GATE Trap:** An FD must hold for ALL possible valid instances, not just the current data. When a question gives you a specific table, you can only **disprove** an FD (by finding a counterexample), not conclusively prove it holds for all future data.

### 3.8 FD Set and Closure (F+)

> **F+** (the closure of a set of FDs) is the set of **ALL** FDs that can be **derived** from F using inference rules.

```
If F = { A --> B, B --> C }

Then F+ includes:
  A --> B       (given)
  B --> C       (given)
  A --> C       (derived by transitivity)
  A --> BC      (derived by union)
  A --> A       (trivial, reflexivity)
  AB --> C      (derived by augmentation)
  AB --> AC     (derived by augmentation)
  ... and many more trivial/derived FDs
```

F+ can be very large. That's why we use **attribute closure** (a much simpler computation) instead.

---

## 4. Armstrong's Axioms

Armstrong's Axioms are the **three fundamental rules** for deriving new FDs from existing ones. They are **sound** (never produce wrong FDs) and **complete** (can derive ALL valid FDs).

### 4.1 The Three Axioms

#### Axiom 1: Reflexivity (Trivial FD)

> If Y is a subset of X, then X --> Y.

```
{A, B, C} --> {A}       (A is a subset of {A,B,C})
{A, B, C} --> {A, B}    (AB is a subset of {A,B,C})
{A, B, C} --> {A, B, C} (trivially, any set determines itself)
{Name, Age} --> {Name}   (Name is a subset)
```

**Intuition:** If you know a student's name AND age, you obviously know their name.

#### Axiom 2: Augmentation

> If X --> Y, then XZ --> YZ (for any attribute set Z).

```
Given: A --> B
Then:  AC --> BC      (augmented both sides with C)
       AD --> BD      (augmented both sides with D)
       ACD --> BCD    (augmented both sides with CD)
```

**Intuition:** If EmpID determines Name, then {EmpID, DeptID} determines {Name, DeptID}. Adding extra known info doesn't break the dependency.

**Important variant:** XZ --> Y also holds (since Y is a subset of YZ).

```
Given: A --> B
Then:  AC --> B       (we can drop Z from the right side by reflexivity)
```

#### Axiom 3: Transitivity

> If X --> Y and Y --> Z, then X --> Z.

```
Given: A --> B and B --> C
Then:  A --> C

Given: EmpID --> DeptID and DeptID --> DeptName
Then:  EmpID --> DeptName
```

**Intuition:** If knowing EmpID gives you DeptID, and knowing DeptID gives you DeptName, then knowing EmpID gives you DeptName.

### 4.2 Why Only Three Axioms?

```
These three axioms are:

SOUND:    Every FD derived using them is correct (never gives a wrong FD)
COMPLETE: Every FD that is logically implied can be derived using them

Together they form a SOUND AND COMPLETE inference system.
No other axioms are needed — everything can be derived from these three.
```

### 4.3 Proof of Correctness (Exam Tip)

**Proving Reflexivity is sound:**
- If Y ⊆ X, then any two tuples with same X values must have same Y values (Y is part of X).

**Proving Augmentation is sound:**
- Given X --> Y. Take two tuples t1, t2 with t1[XZ] = t2[XZ].
- Since XZ values match, X values match, so Y values match (by X --> Y).
- Z values match (given). So YZ values match. Hence XZ --> YZ.

**Proving Transitivity is sound:**
- Given X --> Y and Y --> Z. Take t1, t2 with t1[X] = t2[X].
- By X --> Y: t1[Y] = t2[Y].
- By Y --> Z: t1[Z] = t2[Z].
- Hence X --> Z.

### 4.4 Applying Armstrong's Axioms — Worked Example

```
Given FDs on R(A, B, C, D, E):
  F = { A --> B, B --> C, CD --> E }

Prove: AD --> E

Step 1: A --> B                   (given)
Step 2: AD --> BD                 (augmentation of step 1 with D)
Step 3: B --> C                   (given)
Step 4: BD --> CD                 (augmentation of step 3 with D)
Step 5: AD --> CD                 (transitivity of step 2 and step 4)
Step 6: CD --> E                  (given)
Step 7: AD --> E                  (transitivity of step 5 and step 6)

Proved!
```

```
Another Example:
  F = { A --> BC, D --> E }
  
Prove: AD --> BCE

Step 1: A --> BC                  (given)
Step 2: AD --> BCD                (augmentation with D)
Step 3: D --> E                   (given)
Step 4: BCD --> BCE               (augmentation with BC... 
                                   actually: D-->E, so BCD-->BCE)
  
  Wait, let me redo this properly:
  
Step 1: A --> BC                  (given)
Step 2: AD --> BCD                (augmentation of step 1 with D)
Step 3: D --> E                   (given)
Step 4: BCD --> BCDE              (augmentation of step 3 with BC... 
                                   wait, augment D-->E with BCD:
                                   BCD --> BCE? 
                                   
  Let me use a cleaner approach:
  
Step 1: A --> BC                  (given)
Step 2: AD --> BCD                (augmentation of step 1 with D)
Step 3: D --> E                   (given)  
Step 4: AD --> E                  (augmentation of D-->E: AD-->AE,
                                   then by reflexivity AD-->D, 
                                   then transitivity... 
                                   
  Simpler: AD contains D, and D-->E, so AD-->E by augmentation)
  
Step 5: AD --> BCD                (from step 2)
Step 6: AD --> E                  (from step 4)
Step 7: AD --> BCDE               (union of step 5 and step 6)
Step 8: AD --> BCE                (decomposition from step 7)

Proved!
```

---

## 5. Inference Rules (Derived from Armstrong's Axioms)

These are **additional rules** that can all be proven from the three axioms. They make derivations faster and easier.

### 5.1 Union Rule

> If X --> Y and X --> Z, then X --> YZ.

```
Given: A --> B and A --> C
Then:  A --> BC

Proof using axioms:
  A --> B                       (given)
  A --> AB                      (augmentation with A: trivially A ⊆ A)
  Wait, better: 
  A --> B                       (given)
  A --> C                       (given)
  AA --> AC                     (augmentation of A-->C with A, i.e., A --> AC)
  Since AA = A: A --> AC
  A --> B                       (given)
  AC --> BC                     (augmentation of A-->B with C)
  A --> BC                      (transitivity: A-->AC and AC-->BC)
```

**Intuition:** If you know EmpID determines both Name and Salary, then EmpID determines {Name, Salary}.

### 5.2 Decomposition Rule

> If X --> YZ, then X --> Y and X --> Z.

```
Given: A --> BC
Then:  A --> B and A --> C

Proof:
  A --> BC          (given)
  BC --> B          (reflexivity, since B ⊆ BC)
  A --> B           (transitivity: A-->BC and BC-->B)
  Similarly: A --> C
```

**Intuition:** If EmpID determines {Name, Salary}, it obviously determines Name alone and Salary alone.

### 5.3 Pseudo-Transitivity

> If X --> Y and WY --> Z, then WX --> Z.

```
Given: A --> B and CB --> D
Then:  CA --> D

Proof:
  A --> B           (given)
  CA --> CB         (augmentation with C)
  CB --> D          (given)
  CA --> D          (transitivity: CA-->CB and CB-->D)
```

### 5.4 Self-Determination

> X --> X (every attribute set determines itself).

```
This is a special case of reflexivity where Y = X.
  A --> A
  {A, B} --> {A, B}
```

### 5.5 Composition Rule

> If X --> Y and A --> B, then XA --> YB.

```
Given: P --> Q and R --> S
Then:  PR --> QS

Proof:
  P --> Q           (given)
  PR --> QR         (augmentation with R)
  R --> S           (given)
  QR --> QS         (augmentation with Q)
  PR --> QS         (transitivity: PR-->QR and QR-->QS)
```

### 5.6 All Rules Summary Table

| Rule | Statement | When to Use |
|------|-----------|-------------|
| Reflexivity | Y ⊆ X implies X --> Y | Trivial FDs |
| Augmentation | X --> Y implies XZ --> YZ | Adding attributes to both sides |
| Transitivity | X --> Y, Y --> Z implies X --> Z | Chaining dependencies |
| Union | X --> Y, X --> Z implies X --> YZ | Combining right sides |
| Decomposition | X --> YZ implies X --> Y, X --> Z | Splitting right sides |
| Pseudo-transitivity | X --> Y, WY --> Z implies WX --> Z | Indirect chains |
| Composition | X --> Y, A --> B implies XA --> YB | Combining two FDs |

### 5.7 Practice: Derive FDs

```
Given: F = { A --> B, B --> C, C --> D }

Derive as many FDs as possible:

  A --> B         (given)
  B --> C         (given)
  C --> D         (given)
  A --> C         (transitivity: A-->B, B-->C)
  A --> D         (transitivity: A-->C, C-->D)
  B --> D         (transitivity: B-->C, C-->D)
  A --> BCD       (union: A-->B, A-->C, A-->D)
  B --> CD        (union: B-->C, B-->D)
  A --> BC        (union)
  A --> BD        (union)
  AB --> CD       (augmentation of A-->CD with B... 
                   or more directly: AB contains A, A-->BCD, so AB-->BCD)
  ... and all trivial FDs and augmented versions
```

---

## 6. Closure in Functional Dependencies

Closure is the **most important computational tool** for working with FDs. It tells you everything that a set of attributes can determine.

### 6.1 Attribute Closure (X+)

> **X+** (closure of attribute set X under FD set F) = the set of ALL attributes functionally determined by X.

### 6.2 Algorithm to Compute X+

```
ALGORITHM: ComputeClosure(X, F)
  Input:  X = set of attributes
          F = set of functional dependencies
  Output: X+ = closure of X

  1. result = X
  2. REPEAT:
       changed = false
       FOR EACH FD (A --> B) in F:
         IF A ⊆ result:
           IF B ⊄ result:
             result = result ∪ B
             changed = true
     UNTIL changed = false
  3. RETURN result
```

### 6.3 Worked Example 1

```
R(A, B, C, D, E, F)
F = { A --> B, BC --> D, D --> E, CF --> B }

Find {A, C}+

Iteration 1:
  result = {A, C}
  
  A --> B:    A ⊆ {A,C}? YES. Add B.    result = {A, B, C}
  BC --> D:   BC ⊆ {A,B,C}? YES. Add D. result = {A, B, C, D}
  D --> E:    D ⊆ {A,B,C,D}? YES. Add E. result = {A, B, C, D, E}
  CF --> B:   CF ⊆ {A,B,C,D,E}? NO (F missing). Skip.

Iteration 2:
  A --> B:    B already in result. Skip.
  BC --> D:   D already in result. Skip.
  D --> E:    E already in result. Skip.
  CF --> B:   Still no F. Skip.
  
  No changes. STOP.

{A, C}+ = {A, B, C, D, E}
```

### 6.4 Worked Example 2

```
R(A, B, C, D, E)
F = { AB --> C, C --> D, BD --> E, A --> B }

Find {A}+

result = {A}

Iteration 1:
  AB --> C:  AB ⊆ {A}? NO (B missing). Skip.
  C --> D:   C ⊆ {A}? NO. Skip.
  BD --> E:  BD ⊆ {A}? NO. Skip.
  A --> B:   A ⊆ {A}? YES. Add B.  result = {A, B}

Iteration 2:
  AB --> C:  AB ⊆ {A,B}? YES. Add C.  result = {A, B, C}
  C --> D:   C ⊆ {A,B,C}? YES. Add D. result = {A, B, C, D}
  BD --> E:  BD ⊆ {A,B,C,D}? YES. Add E. result = {A, B, C, D, E}
  A --> B:   B already in result. Skip.

Iteration 3:
  No changes. STOP.

{A}+ = {A, B, C, D, E} = ALL attributes!

Therefore A is a CANDIDATE KEY (and a super key).
```

### 6.5 Worked Example 3

```
R(A, B, C, D)
F = { A --> B, B --> C }

Find {B}+

result = {B}

  A --> B: A ⊆ {B}? NO. Skip.
  B --> C: B ⊆ {B}? YES. Add C. result = {B, C}
  
  No more changes.

{B}+ = {B, C}

B is NOT a super key (doesn't determine A or D).
```

### 6.6 Uses of Attribute Closure

| Task | How to Use Closure |
|------|-------------------|
| **Check if X is a super key** | Compute X+. If X+ = all attributes, YES. |
| **Check if X is a candidate key** | X is a super key AND no proper subset of X is a super key. |
| **Check if X --> Y holds** | Compute X+. If Y ⊆ X+, then YES. |
| **Find all candidate keys** | Systematically compute closures starting from minimal attribute sets. |
| **Check equivalence of FD sets** | Two sets F and G are equivalent if F+ = G+ (check both directions). |

### 6.7 Finding ALL Candidate Keys — Systematic Method

```
Step 1: Categorize each attribute:
  L    = appears ONLY on the LEFT side of FDs
  R    = appears ONLY on the RIGHT side of FDs
  LR   = appears on BOTH sides
  NONE = appears in NO FD

Step 2: 
  - L and NONE attributes MUST be in every candidate key
  - R attributes are NEVER in any candidate key
  - LR attributes MAY or MAY NOT be in a candidate key

Step 3:
  Start = L ∪ NONE
  If Start+ = all attributes, Start is the only candidate key
  If not, try adding each LR attribute one at a time
  Continue until you find all minimal sets whose closure = all attributes
```

### 6.8 Finding Candidate Keys — Full Example

```
R(A, B, C, D, E, F)
FDs = { AB --> C, BC --> AD, D --> E, CF --> B }

Step 1: Categorize attributes
  Left sides:  AB, BC, D, CF  --> attributes: A, B, C, D, F
  Right sides: C, AD, E, B    --> attributes: A, B, C, D, E

  L (left only):    F     (F never appears on right)
  R (right only):   E     (E never appears on left)
  LR (both):        A, B, C, D
  NONE:             (none)

Step 2:
  MUST be in every key: {F}
  NEVER in any key: {E}
  May be in key: {A, B, C, D}

Step 3: Start with {F}
  {F}+ = {F}  (no FD has F alone on left)
  Not all attributes. Need to add from {A, B, C, D}.

  Try {A, F}:
    {A,F}+ = {A,F} -- no FD fires. Not enough.

  Try {A, B, F}:
    AB --> C: YES. {A,B,C,F}
    BC --> AD: YES. {A,B,C,D,F}
    D --> E: YES. {A,B,C,D,E,F} = ALL!
    
    {A,B,F} is a super key. Is it minimal?
      {A,F}+ = {A,F} -- No.
      {B,F}+ = {B,F} -- No.
    YES, {A,B,F} is a candidate key.

  Try {C, F}:
    CF --> B: YES. {B,C,F}
    BC --> AD: YES. {A,B,C,D,F}
    D --> E: YES. {A,B,C,D,E,F} = ALL!
    
    {C,F} is a super key. Is it minimal?
      {C}+ = {C} -- No.
      {F}+ = {F} -- No.
    YES, {C,F} is a candidate key!

  Try {D, F}:
    D --> E: YES. {D,E,F}
    No more FDs fire. Not enough.
    
    Try {A,D,F}: 
      D-->E. {A,D,E,F}. No more. Not enough.
    
    Try {B,D,F}:
      D-->E. {B,D,E,F}. No more. Not enough.
    
  Try {A,C,F}:
    CF-->B. {A,B,C,F}
    AB-->C: already have C. BC-->AD. {A,B,C,D,F}
    D-->E. ALL!
    But {C,F} ⊆ {A,C,F} and {C,F} is already a candidate key.
    So {A,C,F} is a super key but NOT a candidate key (not minimal).

Candidate Keys: {A,B,F} and {C,F}
```

### 6.9 Closure of FD Set (F+) vs Attribute Closure (X+)

| Concept | Notation | What It Is | Size |
|---------|----------|-----------|------|
| Attribute Closure | X+ | All attributes determined by X | Small (subset of attributes) |
| FD Set Closure | F+ | All FDs logically implied by F | Can be HUGE (exponential) |

```
F+ is rarely computed directly. Instead, use X+ to answer questions about F+.

To check if X --> Y is in F+:
  Compute X+ using F.
  If Y ⊆ X+, then X --> Y is in F+.
```

---

## 7. Denormalization

### 7.1 What Is Denormalization?

> **Denormalization** is the intentional process of **adding redundancy** back into a normalized database to **improve read performance**.

It is the **opposite** of normalization — you're deliberately violating normal forms for practical benefits.

### 7.2 Why Denormalize?

```
Normalized Database:
  + No redundancy
  + No anomalies
  + Easier to maintain consistency
  - Complex queries require many JOINs
  - JOINs on large tables are SLOW
  - Poor read performance for reporting/analytics

Denormalized Database:
  + Fewer JOINs = FASTER queries
  + Better read performance
  + Simpler queries
  - Data redundancy (wastes storage)
  - Risk of inconsistencies
  - More complex INSERT/UPDATE logic
```

### 7.3 Common Denormalization Techniques

#### Technique 1: Adding Redundant Columns

**Normalized:**

```
Order(OrderID, CustomerID, OrderDate)
Customer(CustomerID, CustomerName, City)

Query: SELECT OrderID, CustomerName, City FROM Order JOIN Customer...
  --> Requires a JOIN every time
```

**Denormalized:**

```
Order(OrderID, CustomerID, CustomerName, City, OrderDate)
  --> CustomerName and City are redundantly stored
  --> No JOIN needed for the query
```

#### Technique 2: Pre-computed Aggregates

**Normalized:**

```
OrderItem(OrderID, ProductID, Quantity, Price)

Query: SELECT OrderID, SUM(Quantity * Price) AS Total FROM OrderItem GROUP BY OrderID
  --> Must compute the sum every time
```

**Denormalized:**

```
Order(OrderID, ..., TotalAmount)
  --> TotalAmount is pre-computed and stored
  --> No SUM computation needed at query time
```

#### Technique 3: Materialized Views

```
A materialized view is a pre-computed result of a complex query
stored as a physical table. It combines data from multiple tables
and is refreshed periodically.

Example: SalesSummary view that pre-joins Sales, Product, and Region tables.
```

#### Technique 4: Storing Derived Data

```
Employee(EmpID, Name, DOB, Age)
  --> Age can be derived from DOB
  --> But computing age from DOB in every query is expensive
  --> Store Age as a denormalized column, update periodically
```

### 7.4 When to Denormalize vs. Normalize

| Scenario | Recommendation |
|----------|---------------|
| OLTP (Online Transaction Processing) | Normalize (3NF/BCNF) — many writes, data integrity critical |
| OLAP (Online Analytical Processing) | Denormalize — few writes, fast reads for analytics |
| Data Warehousing | Denormalize — star/snowflake schemas with fact and dimension tables |
| Small database | Normalize — JOINs are fast enough |
| Very large database with complex reporting | Denormalize — JOINs become expensive |
| Real-time systems | Denormalize — response time is critical |
| Banking / Healthcare | Normalize — data consistency is non-negotiable |

### 7.5 Star Schema (Denormalized for Data Warehousing)

```
                    DimProduct
                        |
                        |
  DimTime -------- FactSales -------- DimCustomer
                        |
                        |
                    DimStore

  FactSales = central fact table (denormalized, contains foreign keys + measures)
  Dim* = dimension tables (relatively normalized)
```

**FactSales:**

| SaleID | ProductID | CustomerID | StoreID | TimeID | Quantity | Revenue |
|--------|-----------|------------|---------|--------|----------|---------|
| 1      | P1        | C1         | S1      | T1     | 5        | 2500    |

The fact table often includes denormalized columns from dimension tables for faster queries.

### 7.6 GATE-Style Questions

**Q1:** Denormalization is typically used in ___ systems.

**A:** OLAP / Data Warehousing / Decision Support Systems

**Q2:** What is the trade-off of denormalization?

**A:** Faster read performance at the cost of data redundancy and potential inconsistencies.

**Q3:** Is denormalization always bad?

**A:** No. It's a deliberate design choice when read performance matters more than write efficiency and data redundancy.

---

## 8. Equivalence of FD Sets

### 8.1 Definition

> Two sets of FDs, F and G, are **equivalent** (F ≡ G) if they have the same closure: **F+ = G+**.

This means every FD derivable from F can also be derived from G, and vice versa.

### 8.2 How to Check Equivalence

```
To check if F ≡ G:

  Step 1: Check if F covers G
    For each FD X --> Y in G:
      Compute X+ using F
      Check if Y ⊆ X+
    If all pass, F covers G (every FD in G is implied by F)

  Step 2: Check if G covers F
    For each FD X --> Y in F:
      Compute X+ using G
      Check if Y ⊆ X+
    If all pass, G covers F

  Step 3: If both F covers G AND G covers F, then F ≡ G
```

### 8.3 Example

```
F = { A --> B, B --> C, A --> C }
G = { A --> B, B --> C }

Check: Does F cover G?
  A --> B:  Using F: {A}+ = {A,B,C}. B ⊆ {A,B,C}? YES.
  B --> C:  Using F: {B}+ = {B,C}. C ⊆ {B,C}? YES.
  F covers G. ✓

Check: Does G cover F?
  A --> B:  Using G: {A}+ = {A,B,C}. B ⊆ {A,B,C}? YES.
  B --> C:  Using G: {B}+ = {B,C}. C ⊆ {B,C}? YES.
  A --> C:  Using G: {A}+ = {A,B,C}. C ⊆ {A,B,C}? YES.
  G covers F. ✓

F ≡ G (they are equivalent!)
A --> C in F is REDUNDANT — it can be derived from A-->B and B-->C.
```

---

## 9. Canonical Cover (Minimal Cover)

### 9.1 What Is It?

> A **canonical cover** Fc of F is the **simplest equivalent** set of FDs — same meaning, minimal form.

### 9.2 Properties of Canonical Cover

```
Fc must satisfy:
  1. F ≡ Fc (equivalent to original)
  2. No redundant FDs (removing any FD changes the closure)
  3. No extraneous attributes on the left side
  4. Each right side has a single attribute
     (some textbooks allow multiple, but the standard form uses single)
```

### 9.3 Algorithm to Find Canonical Cover

```
ALGORITHM: CanonicalCover(F)

Step 1: DECOMPOSE right sides
  Replace every FD  X --> A1 A2 ... An  with
    X --> A1
    X --> A2
    ...
    X --> An

Step 2: REMOVE extraneous LEFT-SIDE attributes
  For each FD  XA --> B  (where X has 2+ attributes):
    For each attribute A in the left side:
      Compute (X - A)+ using the current FD set
      If B ∈ (X - A)+:
        Replace XA --> B  with  (X - A) --> B
        (A was extraneous — not needed)

Step 3: REMOVE redundant FDs
  For each FD  X --> A:
    Compute X+ using (F - {X --> A})  [all FDs except this one]
    If A ∈ X+:
      Remove X --> A  (it's derivable from the others)

Step 4: COMBINE FDs with same left side
  If X --> A and X --> B exist, combine to X --> AB
  (Optional step, depends on convention)
```

### 9.4 Complete Worked Example

```
Given: F = { A --> BC,  B --> C,  AB --> D,  D --> B }

--- Step 1: Decompose right sides ---
  A --> B
  A --> C
  B --> C
  AB --> D
  D --> B

--- Step 2: Remove extraneous left-side attributes ---
  Check AB --> D:
    Can we remove A?  {B}+ = {B, C}. D ∈ {B,C}? NO. Keep A.
    Can we remove B?  {A}+ = {A, B, C} (A-->B, A-->C).
                      With {A}+ = {A,B,C}: 
                      Now check AB-->D with just A:
                      {A}+ using all FDs = {A} --> B, C, then AB-->D: D, then D-->B.
                      {A}+ = {A,B,C,D}. D ∈ {A,B,C,D}? YES!
    
    Remove B from left side: AB --> D becomes A --> D
  
  Updated:
    A --> B
    A --> C
    B --> C
    A --> D
    D --> B

--- Step 3: Remove redundant FDs ---
  Check A --> B:
    Remove it. F' = {A-->C, B-->C, A-->D, D-->B}
    {A}+ using F' = {A} --> C, D (from A-->C, A-->D)
                          --> B (from D-->B)
                   {A}+ = {A,B,C,D}. B ∈ {A,B,C,D}? YES.
    A --> B is redundant. REMOVE.
  
  Updated: {A-->C, B-->C, A-->D, D-->B}
  
  Check A --> C:
    Remove it. F' = {B-->C, A-->D, D-->B}
    {A}+ using F' = {A} --> D --> B --> C
    {A}+ = {A,B,C,D}. C ∈ {A,B,C,D}? YES.
    A --> C is redundant. REMOVE.
  
  Updated: {B-->C, A-->D, D-->B}
  
  Check B --> C:
    Remove it. F' = {A-->D, D-->B}
    {B}+ using F' = {B}. C ∈ {B}? NO.
    B --> C is NOT redundant. KEEP.
  
  Check A --> D:
    Remove it. F' = {B-->C, D-->B}
    {A}+ using F' = {A}. D ∈ {A}? NO.
    A --> D is NOT redundant. KEEP.
  
  Check D --> B:
    Remove it. F' = {B-->C, A-->D}
    {D}+ using F' = {D}. B ∈ {D}? NO.
    D --> B is NOT redundant. KEEP.

--- Step 4: Combine same left sides ---
  No FDs share the same left side.

CANONICAL COVER:
  Fc = { B --> C,  A --> D,  D --> B }
```

### 9.5 GATE Tips for Canonical Cover

```
Common mistakes:
  1. Forgetting to decompose right sides FIRST
  2. Not re-checking after removing an attribute/FD
     (removing one may make another redundant)
  3. Order of removal can matter — different valid canonical covers exist
  4. The canonical cover is NOT necessarily unique!
     (but all canonical covers are equivalent)
```

---

## 10. Lossless Join Decomposition

### 10.1 What Is It?

> A decomposition of R into R1 and R2 is **lossless** if joining R1 and R2 back together gives **exactly** the original R — no extra rows (spurious tuples), no missing rows.

### 10.2 Condition for Lossless Join (Binary Decomposition)

```
Decompose R into R1 and R2.

LOSSLESS if and only if:
  (R1 ∩ R2) --> (R1 - R2)     [common attrs determine all of R1's extra attrs]
  OR
  (R1 ∩ R2) --> (R2 - R1)     [common attrs determine all of R2's extra attrs]

Equivalently:
  The common attributes (R1 ∩ R2) must be a SUPER KEY of R1 or R2.
```

### 10.3 Example — Lossless Decomposition

```
R(A, B, C) with FD: A --> B

Decompose into:
  R1(A, B)    and    R2(A, C)

Common attributes: R1 ∩ R2 = {A}

Check: Is {A} a super key of R1?
  In R1(A, B): A --> B (given). So {A}+ in R1 = {A, B} = all of R1.
  YES, A is a super key of R1.

LOSSLESS DECOMPOSITION!
```

**Verification with data:**

Original R:

| A  | B  | C  |
|----|----|----|
| a1 | b1 | c1 |
| a2 | b2 | c2 |

R1(A, B):

| A  | B  |
|----|----| 
| a1 | b1 |
| a2 | b2 |

R2(A, C):

| A  | C  |
|----|----| 
| a1 | c1 |
| a2 | c2 |

R1 JOIN R2 on A:

| A  | B  | C  |
|----|----|----|
| a1 | b1 | c1 |
| a2 | b2 | c2 |

Exactly matches R. Lossless!

### 10.4 Example — Lossy Decomposition

```
R(A, B, C) with FD: A --> B

Decompose into:
  R1(A, C)    and    R2(B, C)

Common attributes: R1 ∩ R2 = {C}

Check: Is {C} a super key of R1?
  In R1(A, C): {C}+ = {C}. Not all of R1.
  NO.

Is {C} a super key of R2?
  In R2(B, C): {C}+ = {C}. Not all of R2.
  NO.

LOSSY DECOMPOSITION! (BAD)
```

**Verification with data:**

Original R:

| A  | B  | C  |
|----|----|----|
| a1 | b1 | c1 |
| a2 | b1 | c1 |

R1(A, C):

| A  | C  |
|----|----|
| a1 | c1 |
| a2 | c1 |

R2(B, C):

| B  | C  |
|----|----|
| b1 | c1 |

R1 JOIN R2 on C:

| A  | B  | C  |
|----|----|----|
| a1 | b1 | c1 |
| a2 | b1 | c1 |

Happens to match here, but consider:

Original R:

| A  | B  | C  |
|----|----|----|
| a1 | b1 | c1 |
| a2 | b2 | c1 |

R1(A, C):

| A  | C  |
|----|----|
| a1 | c1 |
| a2 | c1 |

R2(B, C):

| B  | C  |
|----|----|
| b1 | c1 |
| b2 | c1 |

R1 JOIN R2 on C:

| A  | B  | C  |
|----|----|----|
| a1 | b1 | c1 |
| a1 | b2 | c1 |
| a2 | b1 | c1 |
| a2 | b2 | c1 |

4 rows instead of 2! (a1, b2, c1) and (a2, b1, c1) are **spurious tuples**.

### 10.5 Testing Lossless Join for Multi-Way Decomposition (Chase Algorithm)

For decompositions into 3+ tables, use the **Chase Algorithm** (also called the Tableau method):

```
Given: R(A, B, C, D, E) decomposed into R1(A, B, C), R2(C, D), R3(A, D, E)
FDs: A --> B, C --> D, D --> E

Step 1: Create a tableau (table) with one row per decomposed relation:

         A     B     C     D     E
  R1:    a     b     c     d1    e1
  R2:    a2    b2    c     d     e2
  R3:    a     b3    c3    d     e

  - Use the actual symbol (a, b, c, d, e) if the attribute is IN the relation
  - Use a subscripted symbol (a2, b2, etc.) if the attribute is NOT in the relation

Step 2: Apply FDs to make rows agree:

  Apply A --> B:
    Rows with same A: R1 (a) and R3 (a)
    Make their B values agree: b3 becomes b
    
         A     B     C     D     E
  R1:    a     b     c     d1    e1
  R2:    a2    b2    c     d     e2
  R3:    a     b     c3    d     e

  Apply C --> D:
    Rows with same C: R1 (c) and R2 (c)
    Make their D values agree: d1 becomes d
    
         A     B     C     D     E
  R1:    a     b     c     d     e1
  R2:    a2    b2    c     d     e2
  R3:    a     b     c3    d     e

  Apply D --> E:
    Rows with same D: R1 (d), R2 (d), R3 (d) — all same!
    Make E values agree: e1 and e2 become e
    
         A     B     C     D     E
  R1:    a     b     c     d     e      <-- All non-subscripted!
  R2:    a2    b2    c     d     e
  R3:    a     b     c3    d     e

Step 3: Check if any row has ALL non-subscripted symbols:
  R1: a, b, c, d, e — YES! All are non-subscripted!

LOSSLESS DECOMPOSITION!
(If no row becomes all non-subscripted after applying all FDs, it's LOSSY.)
```

---

## 11. Dependency Preservation

### 11.1 What Is It?

> A decomposition **preserves dependencies** if every FD in the original set F can be **verified within a single decomposed table** (without needing to join tables).

### 11.2 Formal Check

```
For each FD X --> Y in F:
  Check if X --> Y can be derived from the FDs that hold within
  individual decomposed tables (projected FDs).
  
  Practically:
    Compute X+ using ONLY the FDs whose attributes are all within
    one of the decomposed tables.
    
    If Y ⊆ X+, the dependency is preserved.
```

### 11.3 Example — Dependency Preserved

```
R(A, B, C) with FDs: A --> B, B --> C

Decompose into:
  R1(A, B) — contains A --> B
  R2(B, C) — contains B --> C

Can we check A --> B without joining? YES (in R1).
Can we check B --> C without joining? YES (in R2).

ALL dependencies preserved!
```

### 11.4 Example — Dependency NOT Preserved

```
R(A, B, C) with FDs: A --> B, B --> C, C --> A

Decompose into:
  R1(A, B) — contains A --> B
  R2(B, C) — contains B --> C

Can we check A --> B? YES (in R1).
Can we check B --> C? YES (in R2).
Can we check C --> A? 
  C is in R2, A is in R1.
  No single table contains both C and A.
  We need to JOIN to verify this.
  NOT preserved!
```

### 11.5 3NF vs BCNF Decomposition

```
Key Fact for Exams:

  3NF Decomposition (Synthesis Algorithm):
    - ALWAYS lossless
    - ALWAYS dependency preserving
    
  BCNF Decomposition:
    - ALWAYS lossless
    - May NOT preserve all dependencies

This is a classic GATE/interview question!
```

### 11.6 Why BCNF Can Lose Dependencies

```
R(A, B, C) with FDs: AB --> C, C --> B
Candidate Keys: {A,B} and {A,C}

C --> B violates BCNF (C is not a super key).

Decompose on C --> B:
  R1(C, B)    key: C
  R2(A, C)    key: A, C

Can we check AB --> C?
  A is in R2, B is in R1. No single table has both A, B, and C.
  NOT preserved!

To check AB --> C, we'd need to join R1 and R2 — expensive.
```

---

## 12. GATE Previous Year Patterns

### Pattern 1: Find Number of Candidate Keys

```
Very frequent (almost every year).

Approach:
  1. Find L, R, LR, NONE categories
  2. Compute closures systematically
  3. Count all minimal super keys
```

### Pattern 2: Find Number of Super Keys

```
Use the formula: 2^(n-k) per candidate key
Apply inclusion-exclusion for overlapping candidate keys.
```

### Pattern 3: Check if Decomposition is Lossless

```
Check if common attributes form a super key of at least one part.
For 3+ way decomposition, use Chase algorithm.
```

### Pattern 4: Check Highest Normal Form

```
Given a relation and FDs:
  1. Is it in 1NF? (atomic values — usually assumed yes)
  2. Check for partial dependencies (2NF violation)
  3. Check for transitive dependencies (3NF violation)
  4. Check if every determinant is a super key (BCNF violation)
```

### Pattern 5: Canonical Cover

```
Find the minimal set of FDs equivalent to the given set.
Follow the 4-step algorithm precisely.
```

### Pattern 6: Attribute Closure

```
Given FDs, compute X+ for given X.
Often used as a sub-step in other questions.
```

### Commonly Tested Tricky Scenarios

```
1. Single-attribute primary key --> automatically in 2NF
   (no composite key means no partial dependency possible)

2. A relation with ALL prime attributes is in 3NF
   (no non-prime attribute exists to have transitive dependency)

3. A relation with 2 attributes is ALWAYS in BCNF
   (only possible FD is one determining the other = super key)

4. If F = {} (no FDs), the relation is in BCNF
   (no FD to violate it)

5. Every 2-attribute relation is in BCNF
   
6. If all attributes are part of some candidate key,
   the relation is in 3NF but may not be in BCNF
```

---

## 13. Quick Revision Cheat Sheet

### Key Definitions (One-Liners)

| Term | One-Line Definition |
|------|-------------------|
| Intension | Schema / structure of the table (rarely changes) |
| Extension | Actual data / rows in the table (changes frequently) |
| Degree | Number of columns (attributes) |
| Cardinality | Number of rows (tuples) |
| Super Key | Any set of attributes that uniquely identifies tuples |
| Candidate Key | Minimal super key |
| Primary Key | Chosen candidate key (one per table) |
| Alternate Key | Candidate key not chosen as primary |
| Foreign Key | Attribute referencing another table's primary key |
| Prime Attribute | Part of some candidate key |
| Non-Prime Attribute | Not part of any candidate key |
| Functional Dependency | X --> Y: same X always means same Y |
| Trivial FD | Y is a subset of X (always true) |
| Partial FD | Non-prime depends on part of composite key (violates 2NF) |
| Transitive FD | Key --> A --> B where A is not a key (violates 3NF) |
| Attribute Closure X+ | All attributes determinable from X |
| Canonical Cover | Smallest equivalent FD set (no redundancy) |
| Lossless Join | Join of parts = original (no spurious tuples) |
| Dependency Preservation | All FDs checkable in individual tables |

### Armstrong's Axioms — Quick Card

```
PRIMARY (the 3 axioms):
  1. Reflexivity:    Y ⊆ X  =>  X --> Y
  2. Augmentation:   X --> Y  =>  XZ --> YZ
  3. Transitivity:   X --> Y, Y --> Z  =>  X --> Z

DERIVED (from the 3 axioms):
  4. Union:          X --> Y, X --> Z  =>  X --> YZ
  5. Decomposition:  X --> YZ  =>  X --> Y, X --> Z
  6. Pseudo-trans:   X --> Y, WY --> Z  =>  WX --> Z
  7. Composition:    X --> Y, A --> B  =>  XA --> YB
```

### Closure Algorithm — Quick Card

```
X+ = X
REPEAT:
  FOR each FD (A --> B):
    IF A ⊆ X+: add B to X+
UNTIL no change
```

### Candidate Key Finding — Quick Card

```
1. L = left-only attrs  -->  MUST be in every key
2. R = right-only attrs -->  NEVER in any key  
3. NONE = not in any FD -->  MUST be in every key
4. Start = L ∪ NONE
5. If Start+ = all attrs  -->  DONE (only candidate key)
6. Else: try adding LR attrs one at a time, check closures
```

### Super Key Count — Quick Card

```
Single candidate key of size k, relation size n:
  Super keys = 2^(n-k)

Multiple candidate keys (CK1, CK2):
  |SK1| + |SK2| - |SK1 ∩ SK2|
  where |SK_i| = 2^(n - |CK_i|)
  and |SK1 ∩ SK2| = 2^(n - |CK1 ∪ CK2|)
```

### Decomposition Properties — Quick Card

```
Lossless Join Check (binary):
  R1 ∩ R2 must be a super key of R1 or R2

Dependency Preservation Check:
  Every FD X --> Y must be verifiable within one decomposed table

3NF synthesis:   Lossless ✓  Dependency Preserved ✓
BCNF decompose:  Lossless ✓  Dependency Preserved ✗ (maybe)
```

---

> **Pro Tip for Exams:** Most GATE questions test attribute closure, candidate key finding,
> and checking normal forms. Master the closure algorithm — it's the foundation for everything.
> Practice computing closures quickly and accurately under time pressure.

---
