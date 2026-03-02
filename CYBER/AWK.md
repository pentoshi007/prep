# 🔥 The Ultimate AWK Guide — From Zero to Hero

> **For:** Aspiring SOC Analysts, Penetration Testers & DevOps Engineers  
> **Prerequisites:** Basic Linux terminal usage  
> **Goal:** Master AWK completely — text processing, log analysis, data extraction

---

## 📑 Table of Contents

1. [What is AWK?](#1-what-is-awk)
2. [How AWK Works — The Mental Model](#2-how-awk-works--the-mental-model)
3. [Your First AWK Command](#3-your-first-awk-command)
4. [Fields & Records — The Core Concept](#4-fields--records--the-core-concept)
5. [Field Separator (FS) & Output Field Separator (OFS)](#5-field-separator-fs--output-field-separator-ofs)
6. [Built-in Variables](#6-built-in-variables)
7. [Pattern Matching & Filtering](#7-pattern-matching--filtering)
8. [Regular Expressions in AWK](#8-regular-expressions-in-awk)
9. [Comparison & Logical Operators](#9-comparison--logical-operators)
10. [Conditional Statements (if/else)](#10-conditional-statements-ifelse)
11. [Loops (for, while, do-while)](#11-loops-for-while-do-while)
12. [Arrays (Associative Arrays)](#12-arrays-associative-arrays)
13. [Built-in String Functions](#13-built-in-string-functions)
14. [Built-in Math Functions](#14-built-in-math-functions)
15. [printf — Formatted Output](#15-printf--formatted-output)
16. [Output Redirection & Pipes](#16-output-redirection--pipes)
17. [Multiple Input Files](#17-multiple-input-files)
18. [BEGIN & END Blocks](#18-begin--end-blocks)
19. [User-Defined Functions](#19-user-defined-functions)
20. [Multi-line Records](#20-multi-line-records)
21. [getline — Reading Input Manually](#21-getline--reading-input-manually)
22. [AWK One-Liners Cheat Sheet](#22-awk-one-liners-cheat-sheet)
23. [Real-World Cybersecurity & SOC Use Cases](#23-real-world-cybersecurity--soc-use-cases)
24. [Practice Exercises](#24-practice-exercises)

---

## 1. What is AWK?

**AWK** is a powerful **text-processing language** built into every Unix/Linux system. It's named after its three creators: **A**ho, **W**einberger, and **K**ernighan.

### Why Should You Learn AWK?

| Scenario                                   | Why AWK?                                              |
| ------------------------------------------ | ----------------------------------------------------- |
| **SOC Analyst** reviewing firewall logs    | Extract source IPs, count occurrences, find anomalies |
| **Pentester** parsing Nmap output          | Pull open ports, services, OS info from scan results  |
| **DevOps** monitoring server logs          | Summarize error codes, calculate response times       |
| **CTF challenges** on TryHackMe/HackTheBox | Quick text extraction from flags, clues, outputs      |

### AWK vs Other Tools

```
grep   → FIND lines matching a pattern (searching)
sed    → FIND and REPLACE text (substitution)
awk    → FIND, EXTRACT, TRANSFORM, and COMPUTE on structured text (full processing)
```

> **Think of it this way:** `grep` is a flashlight (finds things), `sed` is a pencil (edits things), but `awk` is an entire Swiss Army knife (finds, extracts, computes, formats, and reports).

### Versions of AWK

| Version | Description                                                       |
| ------- | ----------------------------------------------------------------- |
| `awk`   | The original AWK                                                  |
| `nawk`  | "New AWK" — added features like user-defined functions            |
| `gawk`  | **GNU AWK** — the most common version on Linux, most feature-rich |
| `mawk`  | Fast, minimal AWK implementation                                  |

> 💡 On most Linux systems, `awk` is actually `gawk`. This guide uses `gawk` features, which work on virtually all Linux distros.

**Check your version:**

```bash
awk --version
# Output: GNU Awk 5.1.0, API: 3.0 (GNU MPFR 4.1.0, GNU MP 6.2.1)
```

---

## 2. How AWK Works — The Mental Model

AWK reads input **line by line** (each line is called a **record**). For each line, it splits the text into **fields** (columns) using a delimiter (default: whitespace).

### The AWK Processing Cycle

```
┌─────────────────────────────────────────────────────┐
│                   AWK PROCESSING                     │
│                                                      │
│  1. BEGIN { }    ← Runs ONCE before any input        │
│         ↓                                            │
│  2. For each line (record) in the input:             │
│     a. Split line into fields ($1, $2, $3, ...)      │
│     b. Check if line matches the PATTERN             │
│     c. If yes → execute the ACTION { }               │
│         ↓                                            │
│  3. END { }      ← Runs ONCE after all input         │
│                                                      │
└─────────────────────────────────────────────────────┘
```

### The Basic Syntax

```bash
awk 'PATTERN { ACTION }' input_file
```

| Part         | What it does                                | Required?                                        |
| ------------ | ------------------------------------------- | ------------------------------------------------ | --- |
| `PATTERN`    | A condition that decides IF the action runs | Optional (if omitted, action runs on every line) |
| `{ ACTION }` | What to DO when the pattern matches         | Optional (if omitted, prints the whole line)     |
| `input_file` | The file to process                         | Can also use piped input via `                   | `   |

### Examples to Build Intuition

```bash
# Print every line (no pattern, default action = print)
awk '{ print }' file.txt

# Print only lines containing "error" (pattern only, default action = print)
awk '/error/' file.txt

# Print 1st field of lines containing "error" (pattern + action)
awk '/error/ { print $1 }' file.txt
```

---

## 3. Your First AWK Command

Let's create a sample file to work with throughout this guide:

```bash
cat > /tmp/employees.txt << 'EOF'
John Sales 45000
Jane Engineering 72000
Bob Marketing 38000
Alice Engineering 85000
Charlie Sales 52000
Diana Marketing 41000
Eve Engineering 91000
Frank Sales 47000
EOF
```

### Print Everything

```bash
awk '{ print }' /tmp/employees.txt
```

**Output:**

```
John Sales 45000
Jane Engineering 72000
Bob Marketing 38000
Alice Engineering 85000
Charlie Sales 52000
Diana Marketing 41000
Eve Engineering 91000
Frank Sales 47000
```

> **What happened?** No pattern was given, so AWK matched EVERY line. The action `{ print }` prints the current line. `print` without arguments defaults to `print $0` (the entire line).

### Print Specific Fields

```bash
awk '{ print $1 }' /tmp/employees.txt
```

**Output:**

```
John
Jane
Bob
Alice
Charlie
Diana
Eve
Frank
```

> **What is `$1`?** AWK splits each line by whitespace. `$1` is the first field, `$2` is the second, `$3` is the third, and so on.

```bash
# Print name and salary (fields 1 and 3)
awk '{ print $1, $3 }' /tmp/employees.txt
```

**Output:**

```
John 45000
Jane 72000
Bob 38000
Alice 85000
Charlie 52000
Diana 41000
Eve 91000
Frank 47000
```

> **The comma** between `$1` and `$3` inserts the **Output Field Separator (OFS)** — by default, a single space.

### `$0` — The Entire Line

```bash
awk '{ print $0 }' /tmp/employees.txt
```

`$0` represents the **entire current line/record**. `print` and `print $0` are identical.

---

## 4. Fields & Records — The Core Concept

This is the **MOST IMPORTANT** concept in AWK. Everything else builds on this.

### Visual Breakdown

```
Input Line:     "John Sales 45000"
                  ↓     ↓     ↓
Field Number:    $1    $2    $3
                  ↓     ↓     ↓
Value:          John  Sales  45000

$0 = "John Sales 45000"   (the whole line)
NF = 3                     (number of fields in this line)
```

### Accessing the Last Field

You don't always know how many fields a line has. Use `$NF` to get the **last field**:

```bash
awk '{ print $NF }' /tmp/employees.txt
```

**Output:**

```
45000
72000
38000
85000
52000
41000
91000
47000
```

> `NF` is a variable holding the Number of Fields. `$NF` dereferences it — so if `NF=3`, then `$NF` is `$3`.

### Second-to-Last Field

```bash
awk '{ print $(NF-1) }' /tmp/employees.txt
```

**Output:**

```
Sales
Engineering
Marketing
Engineering
Sales
Marketing
Engineering
Sales
```

> **Parentheses are needed** — `$NF-1` would mean "value of `$NF` minus 1" (a math operation), but `$(NF-1)` means "the field at position NF-1".

---

## 5. Field Separator (FS) & Output Field Separator (OFS)

### Default Behavior

By default, AWK uses **any whitespace** (spaces, tabs) as the field separator.

### Custom Field Separator with `-F`

Many real-world files use different delimiters:

```bash
# Create a CSV-like file
cat > /tmp/access.csv << 'EOF'
2024-01-15,192.168.1.100,GET,/login,200
2024-01-15,10.0.0.55,POST,/login,401
2024-01-15,192.168.1.100,GET,/dashboard,200
2024-01-15,172.16.0.22,GET,/admin,403
2024-01-15,10.0.0.55,POST,/login,401
2024-01-15,10.0.0.55,POST,/login,200
EOF
```

```bash
# Use comma as field separator
awk -F',' '{ print $2, $5 }' /tmp/access.csv
```

**Output:**

```
192.168.1.100 200
10.0.0.55 401
192.168.1.100 200
172.16.0.22 403
10.0.0.55 401
10.0.0.55 200
```

> **`-F','`** tells AWK to split fields by comma instead of whitespace. You can also write `-F,` (without quotes) for simple delimiters.

### Common Delimiters

```bash
# Colon-separated (like /etc/passwd)
awk -F':' '{ print $1, $7 }' /etc/passwd

# Tab-separated
awk -F'\t' '{ print $1 }' file.tsv

# Multiple characters as separator
awk -F'::' '{ print $1 }' file.txt

# Multiple possible separators (regex!)
awk -F'[,;:]' '{ print $1, $2 }' file.txt
```

### Setting FS Inside the Script

Instead of `-F`, you can set the `FS` variable in a `BEGIN` block:

```bash
awk 'BEGIN { FS="," } { print $2, $5 }' /tmp/access.csv
```

> This is identical to using `-F','`. The `BEGIN` block runs before any input is read, so the separator is set in time.

### Output Field Separator (OFS)

By default, when you use `print $1, $2`, the comma inserts a **space** between fields. Change this with `OFS`:

```bash
awk -F',' 'BEGIN { OFS=" | " } { print $1, $2, $5 }' /tmp/access.csv
```

**Output:**

```
2024-01-15 | 192.168.1.100 | 200
2024-01-15 | 10.0.0.55 | 401
2024-01-15 | 192.168.1.100 | 200
2024-01-15 | 172.16.0.22 | 403
2024-01-15 | 10.0.0.55 | 401
2024-01-15 | 10.0.0.55 | 200
```

> **Key:** The comma in `print $1, $2` triggers OFS. Concatenation (`print $1 $2`, no comma) does NOT use OFS — it just sticks fields together with nothing in between.

### Concatenation vs OFS

```bash
# With comma → uses OFS (default: space)
awk '{ print $1, $2 }' /tmp/employees.txt
# Output: John Sales

# Without comma → concatenation (no separator)
awk '{ print $1 $2 }' /tmp/employees.txt
# Output: JohnSales

# With a literal string
awk '{ print $1 " works in " $2 }' /tmp/employees.txt
# Output: John works in Sales
```

---

## 6. Built-in Variables

AWK has several built-in variables that give you metadata about the input:

| Variable      | Meaning                                                                           | Default            |
| ------------- | --------------------------------------------------------------------------------- | ------------------ |
| `$0`          | The entire current record (line)                                                  | —                  |
| `$1, $2, ...` | Individual fields                                                                 | —                  |
| `NF`          | **N**umber of **F**ields in the current record                                    | —                  |
| `NR`          | **N**umber of **R**ecords read so far (line number, across all files)             | —                  |
| `FNR`         | **F**ile **N**umber of **R**ecords (line number in current file, resets per file) | —                  |
| `FS`          | **F**ield **S**eparator (input)                                                   | `" "` (whitespace) |
| `OFS`         | **O**utput **F**ield **S**eparator                                                | `" "` (space)      |
| `RS`          | **R**ecord **S**eparator (what separates lines)                                   | `"\n"` (newline)   |
| `ORS`         | **O**utput **R**ecord **S**eparator                                               | `"\n"` (newline)   |
| `FILENAME`    | Name of the current input file                                                    | —                  |
| `ARGC`        | Number of command-line arguments                                                  | —                  |
| `ARGV`        | Array of command-line arguments                                                   | —                  |

### NR — Line Numbers

```bash
# Print line numbers alongside content
awk '{ print NR, $0 }' /tmp/employees.txt
```

**Output:**

```
1 John Sales 45000
2 Jane Engineering 72000
3 Bob Marketing 38000
4 Alice Engineering 85000
5 Charlie Sales 52000
6 Diana Marketing 41000
7 Eve Engineering 91000
8 Frank Sales 47000
```

### NR vs FNR

```bash
# FNR resets for each file, NR keeps counting
awk '{ print "NR=" NR, "FNR=" FNR, "FILE=" FILENAME, $0 }' /tmp/employees.txt /tmp/access.csv
```

> **Use case:** `NR == FNR` is a classic idiom to process the first file differently from the second file. More on this in the advanced section.

### NF — Checking Field Count

```bash
# Only print lines that have exactly 3 fields
awk 'NF == 3 { print }' /tmp/employees.txt

# Skip empty lines (lines with 0 fields)
awk 'NF > 0 { print }' file_with_blanks.txt

# Print number of fields per line
awk '{ print NR ": " NF " fields → " $0 }' /tmp/employees.txt
```

### RS — Record Separator

By default, a "record" is one line (separated by `\n`). You can change this:

```bash
# Use blank lines as record separators (paragraph mode)
cat > /tmp/paragraphs.txt << 'EOF'
Name: John
Role: Analyst
Level: Senior

Name: Jane
Role: Engineer
Level: Junior
EOF

awk 'BEGIN { RS="" ; FS="\n" } { print "Record " NR ": " $1 }' /tmp/paragraphs.txt
```

**Output:**

```
Record 1: Name: John
Record 2: Name: Jane
```

> When `RS=""`, AWK uses **blank lines** as record separators (paragraph mode). We also set `FS="\n"` so each line within a paragraph becomes a separate field.

---

## 7. Pattern Matching & Filtering

Patterns determine WHICH lines AWK processes. No pattern = all lines.

### Types of Patterns

#### a) String/Regex Pattern

```bash
# Lines containing "Engineering"
awk '/Engineering/' /tmp/employees.txt
```

**Output:**

```
Jane Engineering 72000
Alice Engineering 85000
Eve Engineering 91000
```

> **`/Engineering/`** is a regex pattern enclosed in forward slashes. AWK checks if the pattern exists anywhere in `$0`.

#### b) Expression Pattern

```bash
# Salary greater than 50000
awk '$3 > 50000' /tmp/employees.txt
```

**Output:**

```
Jane Engineering 72000
Alice Engineering 85000
Charlie Sales 52000
Eve Engineering 91000
```

#### c) Field-Specific Pattern

```bash
# Match regex only against a specific field
awk '$2 ~ /Sales/' /tmp/employees.txt
```

**Output:**

```
John Sales 45000
Charlie Sales 52000
Frank Sales 47000
```

> **`~`** means "matches regex". **`!~`** means "does NOT match regex".

```bash
# NOT in Engineering
awk '$2 !~ /Engineering/' /tmp/employees.txt
```

**Output:**

```
John Sales 45000
Bob Marketing 38000
Charlie Sales 52000
Diana Marketing 41000
Frank Sales 47000
```

#### d) Range Pattern

```bash
# Print from line containing "Bob" to line containing "Diana"
awk '/Bob/,/Diana/' /tmp/employees.txt
```

**Output:**

```
Bob Marketing 38000
Alice Engineering 85000
Charlie Sales 52000
Diana Marketing 41000
```

> **Range patterns** (`/start/,/stop/`) print all lines from the first match of `start` to the first match of `stop`, inclusive.

#### e) BEGIN and END Patterns

```bash
awk 'BEGIN { print "=== Employee Report ===" }
     { print $1, $3 }
     END { print "=== End of Report ===" }' /tmp/employees.txt
```

**Output:**

```
=== Employee Report ===
John 45000
Jane 72000
Bob 38000
Alice 85000
Charlie 52000
Diana 41000
Eve 91000
Frank 47000
=== End of Report ===
```

#### f) Line Number Patterns

```bash
# Print only line 3
awk 'NR == 3' /tmp/employees.txt
# Output: Bob Marketing 38000

# Print lines 2 through 5
awk 'NR >= 2 && NR <= 5' /tmp/employees.txt

# Print the last line (you need END for this)
awk 'END { print }' /tmp/employees.txt
# Output: Frank Sales 47000
```

---

## 8. Regular Expressions in AWK

AWK supports **Extended Regular Expressions (ERE)** — same as `grep -E`.

### Regex Quick Reference

| Pattern  | Meaning                     | Example                                |
| -------- | --------------------------- | -------------------------------------- |
| `.`      | Any single character        | `a.c` → "abc", "a1c"                   |
| `*`      | Zero or more of previous    | `ab*c` → "ac", "abc", "abbc"           |
| `+`      | One or more of previous     | `ab+c` → "abc", "abbc" (not "ac")      |
| `?`      | Zero or one of previous     | `ab?c` → "ac", "abc"                   |
| `^`      | Start of string/line        | `^Error` → lines starting with "Error" |
| `$`      | End of string/line          | `fail$` → lines ending with "fail"     |
| `[abc]`  | Any one character in set    | `[aeiou]` → any vowel                  |
| `[^abc]` | Any character NOT in set    | `[^0-9]` → any non-digit               |
| `[a-z]`  | Character range             | `[A-Za-z]` → any letter                |
| `\|`     | OR (alternation)            | `cat\|dog` → "cat" or "dog"            |
| `()`     | Grouping                    | `(ab)+` → "ab", "abab"                 |
| `{n}`    | Exactly n repetitions       | `a{3}` → "aaa"                         |
| `{n,m}`  | Between n and m repetitions | `a{2,4}` → "aa", "aaa", "aaaa"         |

### Regex Examples

```bash
# Lines starting with a vowel
awk '/^[AEIOUaeiou]/' /tmp/employees.txt
# Output: Alice Engineering 85000
#         Eve Engineering 91000

# Lines ending with a number > 50000 (ending in digits)
awk '/[5-9][0-9]{4}$/' /tmp/employees.txt

# Lines containing exactly 3-letter names
awk '$1 ~ /^[A-Za-z]{3}$/' /tmp/employees.txt
# Output: Bob Marketing 38000
#         Eve Engineering 91000

# Match IP address pattern (basic)
awk '/[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+/' /tmp/access.csv
```

### Case-Insensitive Matching (gawk)

```bash
# In gawk, use IGNORECASE
awk 'BEGIN { IGNORECASE=1 } /engineering/' /tmp/employees.txt

# Alternative: convert to lowercase with tolower()
awk 'tolower($0) ~ /engineering/' /tmp/employees.txt
```

---

## 9. Comparison & Logical Operators

### Comparison Operators

| Operator | Meaning               | Example         |
| -------- | --------------------- | --------------- |
| `==`     | Equal to              | `$2 == "Sales"` |
| `!=`     | Not equal to          | `$2 != "Sales"` |
| `>`      | Greater than          | `$3 > 50000`    |
| `<`      | Less than             | `$3 < 50000`    |
| `>=`     | Greater than or equal | `$3 >= 50000`   |
| `<=`     | Less than or equal    | `$3 <= 50000`   |
| `~`      | Matches regex         | `$1 ~ /^A/`     |
| `!~`     | Does not match regex  | `$1 !~ /^A/`    |

### Logical Operators

| Operator | Meaning | Example                                |
| -------- | ------- | -------------------------------------- |
| `&&`     | AND     | `$2 == "Sales" && $3 > 45000`          |
| `\|\|`   | OR      | `$2 == "Sales" \|\| $2 == "Marketing"` |
| `!`      | NOT     | `!($2 == "Sales")`                     |

### Combined Examples

```bash
# Engineering AND salary > 80000
awk '$2 == "Engineering" && $3 > 80000' /tmp/employees.txt
```

**Output:**

```
Alice Engineering 85000
Eve Engineering 91000
```

```bash
# Sales OR Marketing
awk '$2 == "Sales" || $2 == "Marketing"' /tmp/employees.txt
```

**Output:**

```
John Sales 45000
Bob Marketing 38000
Charlie Sales 52000
Diana Marketing 41000
Frank Sales 47000
```

```bash
# NOT Engineering
awk '$2 != "Engineering"' /tmp/employees.txt
# Same as:
awk '!($2 == "Engineering")' /tmp/employees.txt
```

### Ternary Operator

```bash
# Classify salary as High or Low
awk '{ level = ($3 > 50000) ? "HIGH" : "LOW"; print $1, level }' /tmp/employees.txt
```

**Output:**

```
John LOW
Jane HIGH
Bob LOW
Alice HIGH
Charlie HIGH
Diana LOW
Eve HIGH
Frank LOW
```

---

---

## 10. Conditional Statements (if/else)

AWK supports full `if/else if/else` like C or JavaScript.

### Basic if

```bash
awk '{
    if ($3 > 70000)
        print $1, "is highly paid"
}' /tmp/employees.txt
```

**Output:**

```
Jane is highly paid
Alice is highly paid
Eve is highly paid
```

### if/else

```bash
awk '{
    if ($3 > 60000)
        print $1, "→ Senior"
    else
        print $1, "→ Junior"
}' /tmp/employees.txt
```

**Output:**

```
John → Junior
Jane → Senior
Bob → Junior
Alice → Senior
Charlie → Junior
Diana → Junior
Eve → Senior
Frank → Junior
```

### if/else if/else

```bash
awk '{
    if ($3 >= 80000)
        grade = "A"
    else if ($3 >= 60000)
        grade = "B"
    else if ($3 >= 40000)
        grade = "C"
    else
        grade = "D"
    print $1, $3, "Grade:" grade
}' /tmp/employees.txt
```

**Output:**

```
John 45000 Grade:C
Jane 72000 Grade:B
Bob 38000 Grade:D
Alice 85000 Grade:A
Charlie 52000 Grade:C
Diana 41000 Grade:C
Eve 91000 Grade:A
Frank 47000 Grade:C
```

### One-liner Conditional (Ternary in Action)

```bash
# Compact version for SOC log classification
awk -F',' '{
    status = ($5 >= 400) ? "⚠ ALERT" : "✓ OK"
    print $2, $4, status
}' /tmp/access.csv
```

**Output:**

```
192.168.1.100 /login ✓ OK
10.0.0.55 /login ⚠ ALERT
192.168.1.100 /dashboard ✓ OK
172.16.0.22 /admin ⚠ ALERT
10.0.0.55 /login ⚠ ALERT
10.0.0.55 /login ✓ OK
```

---

## 11. Loops (for, while, do-while)

### for Loop

```bash
# Print fields one per line
awk '{
    for (i = 1; i <= NF; i++)
        print "Field " i ": " $i
}' /tmp/employees.txt | head -9
```

**Output:**

```
Field 1: John
Field 2: Sales
Field 3: 45000
Field 1: Jane
Field 2: Engineering
Field 3: 72000
Field 1: Bob
Field 2: Marketing
Field 3: 38000
```

### while Loop

```bash
# Same as above but using while
awk '{
    i = 1
    while (i <= NF) {
        print "  $" i " = " $i
        i++
    }
    print "---"
}' /tmp/employees.txt | head -12
```

**Output:**

```
  $1 = John
  $2 = Sales
  $3 = 45000
---
  $1 = Jane
  $2 = Engineering
  $3 = 72000
---
  $1 = Bob
  $2 = Marketing
  $3 = 38000
---
```

### do-while Loop

```bash
# Runs at least once, then checks condition
awk 'BEGIN {
    i = 1
    do {
        print "Count: " i
        i++
    } while (i <= 5)
}'
```

**Output:**

```
Count: 1
Count: 2
Count: 3
Count: 4
Count: 5
```

### Loop Control: break, continue, next, exit

| Statement  | Where it works       | What it does                           |
| ---------- | -------------------- | -------------------------------------- |
| `break`    | Inside loops         | Exits the current loop                 |
| `continue` | Inside loops         | Skips to the next iteration            |
| `next`     | In the main AWK body | Skips to the next input record (line)  |
| `exit`     | Anywhere             | Stops processing, jumps to `END` block |

```bash
# next — Skip Engineering employees
awk '$2 == "Engineering" { next } { print }' /tmp/employees.txt
```

**Output:**

```
John Sales 45000
Bob Marketing 38000
Charlie Sales 52000
Diana Marketing 41000
Frank Sales 47000
```

```bash
# exit — Stop after first match
awk '/Engineering/ { print; exit }' /tmp/employees.txt
# Output: Jane Engineering 72000
```

---

## 12. Arrays (Associative Arrays)

AWK arrays are **associative** (like JavaScript objects or Python dicts). No need to declare size — they grow as needed.

### Basic Usage

```bash
awk 'BEGIN {
    fruits["apple"] = 5
    fruits["banana"] = 3
    fruits["cherry"] = 8

    print fruits["apple"]
    print fruits["banana"]
    print fruits["cherry"]
}'
```

**Output:**

```
5
3
8
```

### Counting Occurrences (THE MOST USEFUL PATTERN)

```bash
# Count employees per department
awk '{ dept_count[$2]++ }
     END {
         for (dept in dept_count)
             print dept, dept_count[dept]
     }' /tmp/employees.txt
```

**Output:**

```
Sales 3
Engineering 3
Marketing 2
```

> **How this works:**
>
> - `dept_count[$2]++` → Uses the department name (`$2`) as the array key, and increments the count.
> - Uninitialized variables in AWK default to `0`, so the first `++` makes it `1`.
> - `for (key in array)` iterates over all keys.

### Sum Values by Category

```bash
# Total salary per department
awk '{
    dept_salary[$2] += $3
    dept_count[$2]++
}
END {
    for (dept in dept_salary)
        printf "%-15s Total: $%d  Avg: $%d\n", dept, dept_salary[dept], dept_salary[dept]/dept_count[dept]
}' /tmp/employees.txt
```

**Output:**

```
Sales           Total: $144000  Avg: $48000
Engineering     Total: $248000  Avg: $82666
Marketing       Total: $79000   Avg: $39500
```

### Cybersecurity Example — Count Requests per IP

```bash
awk -F',' '{ ip_count[$2]++ }
     END {
         for (ip in ip_count)
             print ip_count[ip], ip
     }' /tmp/access.csv | sort -rn
```

**Output:**

```
3 10.0.0.55
2 192.168.1.100
1 172.16.0.22
```

> **SOC Insight:** This pattern is how you identify potential brute-force attackers — whoever has the most login attempts!

### Check if Key Exists: `in` Operator

```bash
awk 'BEGIN {
    blocked["10.0.0.55"] = 1
    blocked["172.16.0.22"] = 1
}
{
    # Note: this uses the employees file just for syntax demo
    if ("10.0.0.55" in blocked) print "IP is blocked"
}'
```

### Delete Array Elements

```bash
awk 'BEGIN {
    a["x"] = 1; a["y"] = 2; a["z"] = 3
    delete a["y"]
    for (k in a) print k, a[k]
}'
# Output: x 1
#         z 3
```

### Multi-dimensional Arrays (Simulated)

AWK doesn't have true multi-dimensional arrays, but you can simulate them using `SUBSEP`:

```bash
# Count requests per IP per endpoint
awk -F',' '{
    hits[$2, $4]++
}
END {
    for (key in hits) {
        split(key, parts, SUBSEP)
        print parts[1], parts[2], hits[key]
    }
}' /tmp/access.csv
```

> **`SUBSEP`** is a special character (default `\034`) used to join array subscripts. `hits[$2, $4]` is actually `hits[$2 SUBSEP $4]`.

---

## 13. Built-in String Functions

| Function                           | Description                                | Example                               |
| ---------------------------------- | ------------------------------------------ | ------------------------------------- |
| `length(s)`                        | Length of string                           | `length("hello")` → `5`               |
| `substr(s, start, len)`            | Extract substring                          | `substr("hello", 2, 3)` → `"ell"`     |
| `index(s, target)`                 | Find position of target in s               | `index("hello", "ll")` → `3`          |
| `split(s, arr, sep)`               | Split string into array                    | `split("a:b:c", x, ":")` → `x[1]="a"` |
| `sub(regex, replacement, target)`  | Replace FIRST match                        | `sub(/old/, "new", $0)`               |
| `gsub(regex, replacement, target)` | Replace ALL matches                        | `gsub(/old/, "new", $0)`              |
| `match(s, regex)`                  | Find regex match position                  | `match("abc123", /[0-9]+/)`           |
| `sprintf(fmt, ...)`                | Format string (like printf but returns it) | `s = sprintf("%05d", 42)`             |
| `tolower(s)`                       | Convert to lowercase                       | `tolower("HELLO")` → `"hello"`        |
| `toupper(s)`                       | Convert to uppercase                       | `toupper("hello")` → `"HELLO"`        |

### Detailed Examples

#### length()

```bash
# Print name and its character count
awk '{ print $1, "(" length($1) " chars)" }' /tmp/employees.txt
```

**Output:**

```
John (4 chars)
Jane (4 chars)
Bob (3 chars)
Alice (5 chars)
Charlie (7 chars)
Diana (5 chars)
Eve (3 chars)
Frank (5 chars)
```

#### substr()

```bash
# Extract first 3 characters of each name
awk '{ print substr($1, 1, 3) }' /tmp/employees.txt
```

**Output:**

```
Joh
Jan
Bob
Ali
Cha
Dia
Eve
Fra
```

```bash
# Extract everything after position 5 (no length = rest of string)
awk 'BEGIN { s = "Hello, World!"; print substr(s, 8) }'
# Output: World!
```

#### split()

```bash
# Split a date string
awk 'BEGIN {
    n = split("2024-01-15", parts, "-")
    print "Year:", parts[1]
    print "Month:", parts[2]
    print "Day:", parts[3]
    print "Parts:", n
}'
```

**Output:**

```
Year: 2024
Month: 01
Day: 15
Parts: 3
```

#### sub() and gsub()

```bash
# Replace first occurrence
awk '{ sub(/Sales/, "SALES"); print }' /tmp/employees.txt
# Only replaces first "Sales" in each line

# Replace ALL occurrences
awk '{ gsub(/[aeiou]/, "*"); print }' /tmp/employees.txt
# Replaces every vowel with *
```

**Output (gsub):**

```
J*hn S*l*s 45000
J*n* Eng*n**r*ng 72000
B*b M*rk*t*ng 38000
Al*c* Eng*n**r*ng 85000
Ch*rl** S*l*s 52000
D**n* M*rk*t*ng 41000
Ev* Eng*n**r*ng 91000
Fr*nk S*l*s 47000
```

#### match()

```bash
# Find where digits start in the line
awk '{
    pos = match($0, /[0-9]+/)
    print "Digits start at position", pos, "→", substr($0, pos)
}' /tmp/employees.txt
```

> `match()` sets two special variables:
>
> - `RSTART` — starting position of the match
> - `RLENGTH` — length of the match

```bash
# Extract matched text using RSTART and RLENGTH
awk '{
    match($0, /[0-9]+/)
    print "Matched:", substr($0, RSTART, RLENGTH)
}' /tmp/employees.txt
```

**Output:**

```
Matched: 45000
Matched: 72000
Matched: 38000
Matched: 85000
Matched: 52000
Matched: 41000
Matched: 91000
Matched: 47000
```

---

## 14. Built-in Math Functions

| Function      | Description               | Example               |
| ------------- | ------------------------- | --------------------- |
| `int(x)`      | Truncate to integer       | `int(3.9)` → `3`      |
| `sqrt(x)`     | Square root               | `sqrt(16)` → `4`      |
| `sin(x)`      | Sine (radians)            | `sin(3.14159)` → ~0   |
| `cos(x)`      | Cosine (radians)          | `cos(0)` → `1`        |
| `log(x)`      | Natural log               | `log(2.718)` → ~1     |
| `exp(x)`      | e to the power x          | `exp(1)` → `2.71828`  |
| `rand()`      | Random number [0, 1)      | `rand()` → `0.237788` |
| `srand(seed)` | Seed the random generator | `srand(42)`           |

### Practical Math Examples

```bash
# Calculate total and average salary
awk '{ total += $3; count++ }
     END {
         print "Total: $" total
         print "Average: $" int(total/count)
         print "Employees:", count
     }' /tmp/employees.txt
```

**Output:**

```
Total: $471000
Average: $58875
Employees: 8
```

```bash
# Find min and max salary
awk 'BEGIN { max = 0; min = 999999999 }
     {
         if ($3 > max) { max = $3; max_name = $1 }
         if ($3 < min) { min = $3; min_name = $1 }
     }
     END {
         print "Highest:", max_name, "$"max
         print "Lowest:", min_name, "$"min
     }' /tmp/employees.txt
```

**Output:**

```
Highest: Eve $91000
Lowest: Bob $38000
```

```bash
# Generate random sample (print ~50% of lines)
awk 'BEGIN { srand() } rand() < 0.5 { print }' /tmp/employees.txt
```

---

## 15. printf — Formatted Output

`printf` gives you **precise control** over output formatting, just like C's `printf` or JavaScript's template literals.

### Format Specifiers

| Specifier    | Meaning                | Example                               |
| ------------ | ---------------------- | ------------------------------------- |
| `%s`         | String                 | `printf "%s", "hello"`                |
| `%d` or `%i` | Integer (decimal)      | `printf "%d", 42`                     |
| `%f`         | Floating point         | `printf "%f", 3.14` → `3.140000`      |
| `%e`         | Scientific notation    | `printf "%e", 12345` → `1.234500e+04` |
| `%x`         | Hexadecimal            | `printf "%x", 255` → `ff`             |
| `%o`         | Octal                  | `printf "%o", 8` → `10`               |
| `%c`         | Character (from ASCII) | `printf "%c", 65` → `A`               |
| `%%`         | Literal percent sign   | `printf "100%%"` → `100%`             |

### Width and Alignment

| Modifier | Meaning                      | Example                                 |
| -------- | ---------------------------- | --------------------------------------- |
| `%10s`   | Right-align in 10-char field | `printf "%10s", "hi"` → `"        hi"`  |
| `%-10s`  | Left-align in 10-char field  | `printf "%-10s", "hi"` → `"hi        "` |
| `%05d`   | Zero-pad to 5 digits         | `printf "%05d", 42` → `00042`           |
| `%.2f`   | 2 decimal places             | `printf "%.2f", 3.14159` → `3.14`       |
| `%-15s`  | Left-align string, 15 chars  | Common for columnar output              |

### printf vs print

```bash
# print — simple, adds newline, uses OFS
awk '{ print $1, $3 }' /tmp/employees.txt

# printf — precise, NO automatic newline, YOU control everything
awk '{ printf "%-10s $%,d\n", $1, $3 }' /tmp/employees.txt
```

### Formatted Table Output

```bash
awk 'BEGIN {
    printf "%-12s %-15s %10s\n", "NAME", "DEPARTMENT", "SALARY"
    printf "%-12s %-15s %10s\n", "----", "----------", "------"
}
{
    printf "%-12s %-15s %10d\n", $1, $2, $3
}
END {
    printf "%-12s %-15s %10s\n", "----", "----------", "------"
}' /tmp/employees.txt
```

**Output:**

```
NAME         DEPARTMENT          SALARY
----         ----------          ------
John         Sales                45000
Jane         Engineering          72000
Bob          Marketing            38000
Alice        Engineering          85000
Charlie      Sales                52000
Diana        Marketing            41000
Eve          Engineering          91000
Frank        Sales                47000
----         ----------          ------
```

> **Key difference:** `printf` does NOT add a newline automatically. You must include `\n` yourself.

### Escape Sequences in printf

| Escape | Meaning              |
| ------ | -------------------- |
| `\n`   | Newline              |
| `\t`   | Tab                  |
| `\\`   | Literal backslash    |
| `\"`   | Literal double quote |

---

## 16. Output Redirection & Pipes

### Redirect to File

```bash
# Write output to a file (overwrites)
awk '{ print $1, $3 > "/tmp/salaries.txt" }' /tmp/employees.txt

# Append to a file
awk '{ print $1, $3 >> "/tmp/salaries.txt" }' /tmp/employees.txt
```

> **Important:** The filename must be in quotes. AWK keeps the file open for efficiency — it doesn't open/close for each line.

### Pipe to External Command

```bash
# Sort output by salary
awk '{ print $3, $1 }' /tmp/employees.txt | sort -n

# Pipe FROM within AWK to a command
awk '{ print $1, $3 | "sort -k2 -n" }' /tmp/employees.txt
```

### Redirect Different Data to Different Files

```bash
awk '{
    if ($2 == "Sales")
        print > "/tmp/sales_team.txt"
    else if ($2 == "Engineering")
        print > "/tmp/eng_team.txt"
    else
        print > "/tmp/other_team.txt"
}' /tmp/employees.txt
```

### Close Files/Pipes

When redirecting to many files, close them to avoid running out of file descriptors:

```bash
awk '{
    file = "/tmp/" $2 ".txt"
    print >> file
    close(file)
}' /tmp/employees.txt
```

---

## 17. Multiple Input Files

### Processing Multiple Files

```bash
awk '{ print FILENAME ":" FNR ": " $0 }' /tmp/employees.txt /tmp/access.csv
```

### NR == FNR Idiom (Two-File Processing)

This is one of the **most powerful AWK patterns**. It lets you read data from one file and use it when processing another.

```bash
# Create a blocked IPs file
cat > /tmp/blocked_ips.txt << 'EOF'
10.0.0.55
172.16.0.22
EOF

# Filter access log: only show requests from blocked IPs
awk -F',' '
    NR == FNR { blocked[$1] = 1; next }
    $2 in blocked { print "BLOCKED:", $0 }
' /tmp/blocked_ips.txt /tmp/access.csv
```

**Output:**

```
BLOCKED: 2024-01-15,10.0.0.55,POST,/login,401
BLOCKED: 2024-01-15,172.16.0.22,GET,/admin,403
BLOCKED: 2024-01-15,10.0.0.55,POST,/login,401
BLOCKED: 2024-01-15,10.0.0.55,POST,/login,200
```

> **How `NR == FNR` works:**
>
> - `NR` = total records read across ALL files
> - `FNR` = records read in the CURRENT file
> - When reading the FIRST file, `NR == FNR` is TRUE (both start at 1 and increment together)
> - When reading the SECOND file, `FNR` resets to 1 but `NR` keeps going, so `NR != FNR`
> - `next` skips to the next record, preventing the second rule from running on file 1

---

## 18. BEGIN & END Blocks

### BEGIN — Setup Before Input

`BEGIN` runs **once** before any input is read. Use it for:

- Setting variables
- Printing headers
- Initializing arrays

```bash
awk 'BEGIN {
    FS = ","
    OFS = " | "
    print "=== Access Log Analysis ==="
    print "IP Address | Endpoint | Status"
    print "----------------------------"
}
{ print $2, $4, $5 }' /tmp/access.csv
```

### END — Cleanup After Input

`END` runs **once** after all input is processed. Use it for:

- Printing summaries
- Final calculations
- Reports

```bash
awk '{ total += $3; count++ }
     END {
         print "========================="
         printf "Total Payroll: $%d\n", total
         printf "Headcount:     %d\n", count
         printf "Average:       $%.2f\n", total/count
         print "========================="
     }' /tmp/employees.txt
```

**Output:**

```
=========================
Total Payroll: $471000
Headcount:     8
Average:       $58875.00
=========================
```

### Multiple BEGIN/END Blocks

You can have multiple BEGIN and/or END blocks — they execute in order:

```bash
awk 'BEGIN { print "First BEGIN" }
     BEGIN { print "Second BEGIN" }
     { print $1 }
     END { print "First END" }
     END { print "Second END" }' /tmp/employees.txt
```

## 19. User-Defined Functions

You can create your own functions in AWK — great for reusable logic.

### Syntax

```bash
function function_name(parameters) {
    # body
    return value
}
```

### Example: Classify Severity

```bash
awk -F',' '
function severity(code) {
    if (code >= 500) return "CRITICAL"
    if (code >= 400) return "WARNING"
    if (code >= 300) return "REDIRECT"
    return "OK"
}
{ print $2, $4, $5, "→", severity($5) }
' /tmp/access.csv
```

**Output:**

```
192.168.1.100 /login 200 → OK
10.0.0.55 /login 401 → WARNING
192.168.1.100 /dashboard 200 → OK
172.16.0.22 /admin 403 → WARNING
10.0.0.55 /login 401 → WARNING
10.0.0.55 /login 200 → OK
```

### Example: Obfuscate IP Address

```bash
awk -F',' '
function mask_ip(ip) {
    n = split(ip, octets, ".")
    return octets[1] "." octets[2] ".xxx.xxx"
}
{ print mask_ip($2), $4, $5 }
' /tmp/access.csv
```

**Output:**

```
192.168.xxx.xxx /login 200
10.0.xxx.xxx /login 401
192.168.xxx.xxx /dashboard 200
172.16.xxx.xxx /admin 403
10.0.xxx.xxx /login 401
10.0.xxx.xxx /login 200
```

> **Note on scope:** All variables inside a function are **global** by default! To make local variables, add them as extra parameters:

```bash
function my_func(param1, param2,    local1, local2) {
    # local1 and local2 are local (the extra spaces before them are a convention)
    local1 = "I'm local"
    return local1
}
```

> The spaces before `local1, local2` in the parameter list are a visual convention to distinguish them from real parameters.

---

## 20. Multi-line Records

Sometimes data isn't one-record-per-line. AWK can handle multi-line records.

### Paragraph Mode (RS = "")

```bash
cat > /tmp/servers.txt << 'EOF'
Hostname: web-server-01
IP: 192.168.1.10
OS: Ubuntu 22.04
Status: Active

Hostname: db-server-01
IP: 192.168.1.20
OS: CentOS 8
Status: Active

Hostname: mail-server-01
IP: 192.168.1.30
OS: Debian 11
Status: Down
EOF

awk 'BEGIN { RS = ""; FS = "\n" }
{
    for (i = 1; i <= NF; i++) {
        split($i, kv, ": ")
        data[kv[1]] = kv[2]
    }
    if (data["Status"] == "Down")
        printf "⚠ ALERT: %s (%s) is DOWN!\n", data["Hostname"], data["IP"]
    delete data
}' /tmp/servers.txt
```

**Output:**

```
⚠ ALERT: mail-server-01 (192.168.1.30) is DOWN!
```

> **How it works:**
>
> - `RS = ""` → blank lines separate records (paragraph mode)
> - `FS = "\n"` → within each record, lines become fields
> - `$1` = first line, `$2` = second line, etc.

---

## 21. getline — Reading Input Manually

`getline` gives you manual control over reading input. It's powerful but tricky.

### Forms of getline

| Form                   | What it does                                |
| ---------------------- | ------------------------------------------- |
| `getline`              | Read next line from current input into `$0` |
| `getline var`          | Read next line into variable `var`          |
| `getline < "file"`     | Read next line from a file                  |
| `getline var < "file"` | Read next line from file into `var`         |
| `"cmd" \| getline`     | Read output from a shell command            |
| `"cmd" \| getline var` | Read command output into `var`              |

### Read from a File

```bash
# Look up IP info from a separate file while processing logs
awk -F',' '{
    while ((getline line < "/tmp/blocked_ips.txt") > 0) {
        if ($2 == line) {
            print "BLOCKED:", $0
            break
        }
    }
    close("/tmp/blocked_ips.txt")
}' /tmp/access.csv
```

### Read from a Command

```bash
# Add the current date to output
awk 'BEGIN {
    "date +%Y-%m-%d" | getline today
    close("date +%Y-%m-%d")
    print "Report generated on:", today
    print "========================"
}
{ print $0 }' /tmp/employees.txt
```

### Skip Lines (Read and Discard)

```bash
# Skip every other line (read odd lines only)
awk '{ print; getline }' /tmp/employees.txt
```

**Output:**

```
John Sales 45000
Bob Marketing 38000
Charlie Sales 52000
Eve Engineering 91000
```

> ⚠ **Warning:** `getline` modifies `$0`, `NF`, `NR`, etc. Use it carefully. The `NR == FNR` idiom is usually safer for two-file processing.

---

## 22. AWK One-Liners Cheat Sheet

These are battle-tested one-liners you'll use constantly as a SOC analyst or pentester.

### Text Manipulation

```bash
# Print lines 10-20
awk 'NR >= 10 && NR <= 20' file.txt

# Print first and last field of each line
awk '{ print $1, $NF }' file.txt

# Remove duplicate lines (preserving order)
awk '!seen[$0]++' file.txt

# Remove blank/empty lines
awk 'NF > 0' file.txt
# Or: awk '/./' file.txt

# Reverse fields in each line
awk '{ for (i = NF; i >= 1; i--) printf "%s ", $i; print "" }' file.txt

# Print lines longer than 80 characters
awk 'length > 80' file.txt

# Number non-blank lines
awk 'NF { printf "%6d  %s\n", ++n, $0 }' file.txt

# Print only unique lines
awk '!a[$0]++' file.txt

# Print the Nth line
awk 'NR == 5' file.txt

# Print everything after pattern
awk '/pattern/,0' file.txt

# Print between two patterns
awk '/START/,/END/' file.txt

# Replace a field value
awk '{ $2 = "REDACTED"; print }' file.txt

# Sum a column
awk '{ sum += $1 } END { print sum }' numbers.txt

# Count lines
awk 'END { print NR }' file.txt

# Print the longest line
awk '{ if (length > max) { max = length; line = $0 } } END { print line }' file.txt
```

### CSV/Log Processing

```bash
# Convert CSV to TSV
awk -F',' 'BEGIN { OFS="\t" } { $1=$1; print }' file.csv

# Extract specific CSV columns (2nd and 4th)
awk -F',' '{ print $2, $4 }' file.csv

# Count unique values in a column
awk -F',' '{ count[$3]++ } END { for (k in count) print count[k], k }' file.csv | sort -rn

# Group by and sum
awk -F',' '{ sum[$1] += $3 } END { for (k in sum) print k, sum[k] }' file.csv
```

### System Administration

```bash
# List all users (from /etc/passwd)
awk -F':' '{ print $1 }' /etc/passwd

# Find users with UID >= 1000 (regular users)
awk -F':' '$3 >= 1000 { print $1, $3 }' /etc/passwd

# Show users with /bin/bash shell
awk -F':' '$7 == "/bin/bash" { print $1 }' /etc/passwd

# Disk usage: find partitions > 80% full
df -h | awk 'NR > 1 && int($5) > 80 { print "WARNING:", $6, "is", $5, "full" }'

# Top 10 largest files in a directory
ls -lS | awk 'NR > 1 && NR <= 11 { printf "%10s  %s\n", $5, $NF }'

# Monitor active connections
netstat -an | awk '/ESTABLISHED/ { count++ } END { print "Active connections:", count }'
```

---

## 23. Real-World Cybersecurity & SOC Use Cases

### Create a Realistic Log File for Practice

```bash
cat > /tmp/auth.log << 'EOF'
Jan 15 08:23:01 server sshd[12345]: Accepted password for admin from 192.168.1.100 port 52340 ssh2
Jan 15 08:23:15 server sshd[12346]: Failed password for root from 10.0.0.55 port 52341 ssh2
Jan 15 08:23:16 server sshd[12347]: Failed password for root from 10.0.0.55 port 52342 ssh2
Jan 15 08:23:17 server sshd[12348]: Failed password for root from 10.0.0.55 port 52343 ssh2
Jan 15 08:23:18 server sshd[12349]: Failed password for root from 10.0.0.55 port 52344 ssh2
Jan 15 08:23:19 server sshd[12350]: Failed password for root from 10.0.0.55 port 52345 ssh2
Jan 15 08:24:01 server sshd[12351]: Accepted password for deploy from 172.16.0.10 port 52350 ssh2
Jan 15 08:25:33 server sshd[12352]: Failed password for admin from 203.0.113.42 port 52360 ssh2
Jan 15 08:25:34 server sshd[12353]: Failed password for admin from 203.0.113.42 port 52361 ssh2
Jan 15 08:30:01 server sshd[12354]: Accepted password for admin from 192.168.1.100 port 52380 ssh2
Jan 15 09:01:15 server sshd[12355]: Failed password for invalid user test from 198.51.100.5 port 52400 ssh2
Jan 15 09:01:16 server sshd[12356]: Failed password for invalid user admin from 198.51.100.5 port 52401 ssh2
Jan 15 09:01:17 server sshd[12357]: Failed password for invalid user guest from 198.51.100.5 port 52402 ssh2
EOF
```

### Use Case 1: Detect Brute Force Attacks

```bash
# Count failed login attempts per IP
awk '/Failed password/ {
    # "from" is followed by the IP address
    for (i = 1; i <= NF; i++) {
        if ($i == "from") {
            ip = $(i+1)
            failed[ip]++
            break
        }
    }
}
END {
    print "=== Failed Login Attempts ==="
    for (ip in failed) {
        alert = (failed[ip] >= 5) ? " ← BRUTE FORCE DETECTED!" : ""
        printf "%-20s %3d attempts%s\n", ip, failed[ip], alert
    }
}' /tmp/auth.log
```

**Output:**

```
=== Failed Login Attempts ===
10.0.0.55              5 attempts ← BRUTE FORCE DETECTED!
203.0.113.42           2 attempts
198.51.100.5           3 attempts
```

### Use Case 2: Identify Unauthorized Access Attempts

```bash
# Find login attempts for invalid users
awk '/invalid user/ {
    for (i = 1; i <= NF; i++) {
        if ($i == "user") { user = $(i+1) }
        if ($i == "from") { ip = $(i+1) }
    }
    printf "[%s %s %s] Invalid user '%s' from %s\n", $1, $2, $3, user, ip
}' /tmp/auth.log
```

**Output:**

```
[Jan 15 09:01:15] Invalid user 'test' from 198.51.100.5
[Jan 15 09:01:16] Invalid user 'admin' from 198.51.100.5
[Jan 15 09:01:17] Invalid user 'guest' from 198.51.100.5
```

### Use Case 3: Parse Apache/Nginx Access Logs

```bash
cat > /tmp/web_access.log << 'EOF'
192.168.1.100 - - [15/Jan/2024:08:23:01 +0000] "GET /index.html HTTP/1.1" 200 1234
10.0.0.55 - - [15/Jan/2024:08:23:15 +0000] "POST /login HTTP/1.1" 401 543
192.168.1.100 - - [15/Jan/2024:08:24:01 +0000] "GET /api/users HTTP/1.1" 200 8765
203.0.113.42 - - [15/Jan/2024:08:25:33 +0000] "GET /../../etc/passwd HTTP/1.1" 403 234
10.0.0.55 - - [15/Jan/2024:08:26:00 +0000] "GET /admin' OR '1'='1 HTTP/1.1" 400 123
172.16.0.10 - - [15/Jan/2024:08:30:01 +0000] "GET /dashboard HTTP/1.1" 200 5432
198.51.100.5 - - [15/Jan/2024:09:01:15 +0000] "GET /wp-admin HTTP/1.1" 404 234
EOF

# Detect suspicious requests (path traversal, SQLi, etc.)
awk '{
    ip = $1
    status = $9
    request = ""
    # Extract the request between quotes
    match($0, /"[^"]*"/)
    request = substr($0, RSTART+1, RLENGTH-2)

    suspicious = 0
    reason = ""

    if (request ~ /\.\.\//) { suspicious = 1; reason = "PATH TRAVERSAL" }
    if (request ~ /OR.*=/) { suspicious = 1; reason = "SQL INJECTION" }
    if (request ~ /wp-admin|wp-login/) { suspicious = 1; reason = "WORDPRESS SCAN" }
    if (status >= 400) { suspicious = 1; if (reason == "") reason = "HTTP " status }

    if (suspicious)
        printf "⚠ [%s] %s → %s | %s\n", reason, ip, request, status
}' /tmp/web_access.log
```

**Output:**

```
⚠ [HTTP 401] 10.0.0.55 → POST /login HTTP/1.1 | 401
⚠ [PATH TRAVERSAL] 203.0.113.42 → GET /../../etc/passwd HTTP/1.1 | 403
⚠ [SQL INJECTION] 10.0.0.55 → GET /admin' OR '1'='1 HTTP/1.1 | 400
⚠ [WORDPRESS SCAN] 198.51.100.5 → GET /wp-admin HTTP/1.1 | 404
```

### Use Case 4: Firewall Log Analysis

```bash
cat > /tmp/firewall.log << 'EOF'
2024-01-15T08:00:01 ACCEPT TCP 192.168.1.100 10.0.0.1 443 52340
2024-01-15T08:00:02 DROP TCP 10.0.0.55 10.0.0.1 22 52341
2024-01-15T08:00:03 DROP TCP 10.0.0.55 10.0.0.1 22 52342
2024-01-15T08:00:04 DROP TCP 10.0.0.55 10.0.0.1 22 52343
2024-01-15T08:00:05 ACCEPT TCP 172.16.0.10 10.0.0.1 80 52350
2024-01-15T08:00:06 DROP UDP 203.0.113.42 10.0.0.1 53 52360
2024-01-15T08:00:07 DROP TCP 198.51.100.5 10.0.0.1 3389 52400
2024-01-15T08:00:08 DROP TCP 198.51.100.5 10.0.0.1 445 52401
2024-01-15T08:00:09 ACCEPT TCP 192.168.1.100 10.0.0.1 80 52380
EOF

# Comprehensive firewall analysis
awk '{
    action = $2; proto = $3; src = $4; dst_port = $6

    # Count by action
    actions[action]++

    # Track dropped IPs and ports
    if (action == "DROP") {
        drop_ip[src]++
        drop_port[dst_port]++
    }
}
END {
    print "╔══════════════════════════════════════╗"
    print "║       FIREWALL LOG ANALYSIS          ║"
    print "╠══════════════════════════════════════╣"
    printf "║  ACCEPT: %-5d   DROP: %-5d        ║\n", actions["ACCEPT"], actions["DROP"]
    print "╠══════════════════════════════════════╣"
    print "║  Top Blocked Source IPs:             ║"
    for (ip in drop_ip)
        printf "║    %-20s %3d blocks  ║\n", ip, drop_ip[ip]
    print "╠══════════════════════════════════════╣"
    print "║  Targeted Ports:                     ║"
    for (port in drop_port)
        printf "║    Port %-6s           %3d blocks  ║\n", port, drop_port[port]
    print "╚══════════════════════════════════════╝"
}' /tmp/firewall.log
```

### Use Case 5: Parse Nmap Output

```bash
cat > /tmp/nmap_output.txt << 'EOF'
Nmap scan report for 192.168.1.1
PORT     STATE    SERVICE     VERSION
22/tcp   open     ssh         OpenSSH 8.9p1
80/tcp   open     http        Apache 2.4.52
443/tcp  open     https       Apache 2.4.52
3306/tcp filtered mysql
8080/tcp open     http-proxy  Squid 5.2

Nmap scan report for 192.168.1.2
PORT     STATE  SERVICE     VERSION
22/tcp   open   ssh         OpenSSH 7.6p1
80/tcp   open   http        nginx 1.18.0
3389/tcp open   ms-wbt-server Microsoft Terminal Services
EOF

# Extract open ports per host
awk '
/^Nmap scan report/ { host = $NF }
/open/ && !/STATE/ {
    split($1, port_info, "/")
    printf "%-15s  Port: %-6s  Service: %-15s  %s\n", host, port_info[1], $3, $4
}' /tmp/nmap_output.txt
```

**Output:**

```
192.168.1.1      Port: 22      Service: ssh              OpenSSH
192.168.1.1      Port: 80      Service: http             Apache
192.168.1.1      Port: 443     Service: https            Apache
192.168.1.1      Port: 8080    Service: http-proxy       Squid
192.168.1.2      Port: 22      Service: ssh              OpenSSH
192.168.1.2      Port: 80      Service: http             nginx
192.168.1.2      Port: 3389    Service: ms-wbt-server    Microsoft
```

### Use Case 6: Monitor Real-time Logs (with tail)

```bash
# Real-time failed login monitoring
# tail -f /var/log/auth.log | awk '/Failed password/ {
#     for (i=1; i<=NF; i++) if ($i == "from") ip = $(i+1)
#     failed[ip]++
#     if (failed[ip] == 5)
#         printf "\n🚨 ALERT: %s has %d failed attempts!\n", ip, failed[ip]
#     fflush()   # Important! Forces output immediately
# }'
```

> **`fflush()`** is critical when piping — without it, AWK buffers output and you won't see real-time alerts.

---

## 24. AWK Script Files

For complex AWK programs, save them to a file and run with `-f`:

### Create a Script

```bash
cat > /tmp/analyze_log.awk << 'AWKEOF'
#!/usr/bin/awk -f
# SOC Log Analysis Script
# Usage: awk -f analyze_log.awk <logfile>

BEGIN {
    print "╔════════════════════════════════════════╗"
    print "║     SOC LOG ANALYZER v1.0              ║"
    print "╚════════════════════════════════════════╝"
    print ""
}

/Failed password/ {
    for (i = 1; i <= NF; i++) {
        if ($i == "from") { src_ip = $(i+1); break }
    }
    failed[src_ip]++
    total_failed++
}

/Accepted password/ {
    for (i = 1; i <= NF; i++) {
        if ($i == "from") { src_ip = $(i+1); break }
    }
    success[src_ip]++
    total_success++
}

END {
    printf "\n📊 Summary: %d successful, %d failed logins\n\n", total_success, total_failed

    print "✅ Successful Logins:"
    for (ip in success)
        printf "   %-20s  %d logins\n", ip, success[ip]

    print "\n❌ Failed Logins:"
    for (ip in failed) {
        alert = (failed[ip] >= 5) ? " 🚨 BRUTE FORCE!" : ""
        printf "   %-20s  %d attempts%s\n", ip, failed[ip], alert
    }
}
AWKEOF

# Run it
awk -f /tmp/analyze_log.awk /tmp/auth.log
```

> **Make it executable:**
>
> ```bash
> chmod +x /tmp/analyze_log.awk
> # Then run directly:
> /tmp/analyze_log.awk /tmp/auth.log
> ```

---

## 25. Practice Exercises

Test your AWK skills with these challenges. Use the sample files created throughout this guide.

### Beginner

1. **Print only employee names** from `/tmp/employees.txt`
2. **Print employees earning more than $50,000**
3. **Count total number of lines** in `/tmp/access.csv`
4. **Print the 3rd field** of `/tmp/access.csv` (HTTP methods)

<details>
<summary>Solutions</summary>

```bash
# 1
awk '{ print $1 }' /tmp/employees.txt

# 2
awk '$3 > 50000 { print $1, $3 }' /tmp/employees.txt

# 3
awk 'END { print NR }' /tmp/access.csv

# 4
awk -F',' '{ print $3 }' /tmp/access.csv
```

</details>

### Intermediate

5. **Calculate average salary per department** from `/tmp/employees.txt`
6. **Find all unique IPs** in `/tmp/access.csv`
7. **Print lines where the HTTP status is 4xx** from `/tmp/access.csv`
8. **Find the employee with the highest salary** (print name and salary)

<details>
<summary>Solutions</summary>

```bash
# 5
awk '{ sum[$2]+=$3; count[$2]++ } END { for (d in sum) printf "%s: $%.0f\n", d, sum[d]/count[d] }' /tmp/employees.txt

# 6
awk -F',' '{ ips[$2] } END { for (ip in ips) print ip }' /tmp/access.csv

# 7
awk -F',' '$5 >= 400 && $5 < 500' /tmp/access.csv

# 8
awk '$3 > max { max = $3; name = $1 } END { print name, max }' /tmp/employees.txt
```

</details>

### Advanced

9. **Cross-reference two files**: Using `/tmp/blocked_ips.txt` and `/tmp/access.csv`, print all requests from non-blocked IPs only.
10. **Build a full report**: From `/tmp/auth.log`, generate a formatted report showing total events, breakdown by type, and top source IPs.
11. **Detect port scanning**: From `/tmp/firewall.log`, identify any IP hitting more than 2 unique destination ports.

<details>
<summary>Solutions</summary>

```bash
# 9
awk -F',' 'NR==FNR { blocked[$1]; next } !($2 in blocked)' /tmp/blocked_ips.txt /tmp/access.csv

# 10
awk '
/Failed/  { fail++; for(i=1;i<=NF;i++) if($i=="from") fail_ip[$(i+1)]++ }
/Accepted/{ ok++;   for(i=1;i<=NF;i++) if($i=="from") ok_ip[$(i+1)]++ }
END {
    printf "Total: %d events (%d success, %d failed)\n\n", ok+fail, ok, fail
    print "Top Failed IPs:"
    for (ip in fail_ip) printf "  %-20s %d\n", ip, fail_ip[ip]
    print "\nSuccessful IPs:"
    for (ip in ok_ip) printf "  %-20s %d\n", ip, ok_ip[ip]
}' /tmp/auth.log

# 11
awk '$2 == "DROP" {
    key = $4 SUBSEP $6
    if (!(key in seen)) { seen[key]; ports[$4]++ }
}
END {
    for (ip in ports)
        if (ports[ip] > 2)
            printf "⚠ Port scan detected: %s hit %d unique ports\n", ip, ports[ip]
}' /tmp/firewall.log
```

</details>

---

## 🎯 Quick Reference Card

```
┌─────────────────────────────────────────────────────────────┐
│                    AWK QUICK REFERENCE                       │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  SYNTAX:   awk [options] 'pattern { action }' file           │
│                                                              │
│  FIELDS:   $0=whole line  $1=first  $NF=last  $(NF-1)=2nd   │
│            to last                                           │
│                                                              │
│  OPTIONS:  -F sep    Set field separator                     │
│            -v var=val Set variable                            │
│            -f file   Read AWK program from file              │
│                                                              │
│  VARIABLES: NR=line#  NF=#fields  FS=input-sep  OFS=out-sep │
│             FNR=file-line#  RS=record-sep  FILENAME=file     │
│                                                              │
│  PATTERNS: /regex/  expression  BEGIN  END  /start/,/stop/   │
│                                                              │
│  OPERATORS: ==  !=  >  <  ~  !~  &&  ||  !  ++  +=          │
│                                                              │
│  STRINGS:  length  substr  index  split  sub  gsub  match    │
│            tolower  toupper  sprintf                          │
│                                                              │
│  CONTROL:  if/else  for  while  do-while  break  continue   │
│            next  exit                                         │
│                                                              │
│  OUTPUT:   print  printf  > "file"  >> "file"  | "cmd"       │
│                                                              │
│  ARRAYS:   arr[key]=val  for(k in arr)  delete arr[k]        │
│            (key in arr)                                       │
│                                                              │
│  MOST USED PATTERN:                                          │
│    awk '{ count[$1]++ } END { for(k in count) print k,      │
│    count[k] }' file                                          │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

---

> **🏆 You've completed the Ultimate AWK Guide!**
>
> As a SOC analyst and pentester, you'll use AWK daily for:
>
> - **Log analysis** — parsing auth.log, syslog, Apache logs
> - **Threat hunting** — counting IPs, detecting brute force, finding anomalies
> - **Data wrangling** — transforming CSV/TSV, joining files, generating reports
> - **CTF challenges** — quick text extraction and pattern matching
>
> **Next steps:** Practice with your own log files and TryHackMe rooms. The more you use AWK, the more natural it becomes. Combining AWK with `grep`, `sed`, `sort`, `uniq`, and `cut` will make you unstoppable on the command line.
