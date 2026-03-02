# 🔧 The Ultimate SED Guide — From Zero to Hero

> **Stream EDitor (sed)** — Your Swiss Army Knife for Text Manipulation in Linux  
> A comprehensive, hands-on guide for aspiring SOC Analysts & Penetration Testers.

---

## 📑 Table of Contents

| #   | Section                                                                          | Topics                                           |
| --- | -------------------------------------------------------------------------------- | ------------------------------------------------ |
| 1   | [What is SED?](#1--what-is-sed)                                                  | Definition, How it works, Why learn it           |
| 2   | [SED Syntax & Basics](#2--sed-syntax--basics)                                    | Basic syntax, Input sources, Inline editing      |
| 3   | [Substitution (s command)](#3--substitution-the-s-command)                       | Find & Replace, Flags (g, i, p, w), Delimiters   |
| 4   | [Address & Line Selection](#4--address--line-selection)                          | Line numbers, Ranges, Patterns, Negation         |
| 5   | [Deletion (d command)](#5--deletion-the-d-command)                               | Delete lines, ranges, patterns                   |
| 6   | [Insertion & Appending](#6--insertion--appending)                                | Insert (i), Append (a), Change (c)               |
| 7   | [Print & Output Control](#7--print--output-control-p-command)                    | Print (p), Suppress (-n), Write (w)              |
| 8   | [Transformations](#8--transformations-the-y-command)                             | Transliterate (y)                                |
| 9   | [Multiple Commands](#9--multiple-commands)                                       | -e flag, Semicolons, Script files (-f)           |
| 10  | [Hold & Pattern Space](#10--hold--pattern-space-advanced)                        | h, H, g, G, x — Two-buffer model                 |
| 11  | [Branching & Flow Control](#11--branching--flow-control)                         | Labels, Branch (b), Test (t)                     |
| 12  | [Regex in SED](#12--regular-expressions-in-sed)                                  | BRE, ERE (-E), Character classes, Backreferences |
| 13  | [Real-World Cybersecurity Examples](#13--real-world-cybersecurity--soc-examples) | Log parsing, IOC extraction, Config hardening    |
| 14  | [SED One-Liners Cheat Sheet](#14--sed-one-liners-cheat-sheet)                    | Quick reference for daily use                    |
| 15  | [SED vs AWK vs grep](#15--sed-vs-awk-vs-grep)                                    | When to use what                                 |
| 16  | [Common Flags Reference](#16--common-flags-reference)                            | All CLI flags in one table                       |
| 17  | [Practice Exercises](#17--practice-exercises)                                    | Hands-on challenges                              |

---

## 1. 🔍 What is SED?

### Definition

**`sed`** stands for **Stream EDitor**. It reads input (a file or piped data) **line by line**, applies editing commands to each line, and writes the result to standard output. It **never modifies the original file** unless you explicitly tell it to (`-i` flag).

### How SED Works Internally

```
┌─────────────────────────────────────────────────┐
│                  SED WORKFLOW                    │
├─────────────────────────────────────────────────┤
│                                                 │
│   Input Stream (file / pipe / stdin)            │
│          │                                      │
│          ▼                                      │
│   ┌──────────────┐                              │
│   │ Read one line │──► Pattern Space (buffer)   │
│   └──────────────┘         │                    │
│                            ▼                    │
│                   Apply sed command(s)          │
│                            │                    │
│                            ▼                    │
│                   Output to stdout              │
│                            │                    │
│                            ▼                    │
│                   Read next line (repeat)       │
│                                                 │
└─────────────────────────────────────────────────┘
```

**Key concepts:**

- **Pattern Space** — A temporary buffer where `sed` holds the current line being processed.
- **Hold Space** — A secondary buffer for advanced multi-line operations (covered in Section 10).
- **Non-destructive** — By default, `sed` does NOT change the original file. It outputs to `stdout`.

### Why Learn SED? (For Cybersecurity Professionals)

| Use Case                | Example                                         |
| ----------------------- | ----------------------------------------------- |
| **Log Parsing**         | Strip timestamps, extract IPs from auth.log     |
| **Config Hardening**    | Disable root login in sshd_config automatically |
| **IOC Extraction**      | Pull domains/IPs from threat intelligence feeds |
| **Evidence Processing** | Clean and normalize forensic artifacts          |
| **Automation**          | Batch-modify configs across multiple servers    |
| **CTF / TryHackMe**     | Manipulate text-based challenges quickly        |

---

## 2. 📐 SED Syntax & Basics

### Basic Syntax

```bash
sed [OPTIONS] 'COMMAND' input_file
```

Or with piped input:

```bash
cat file.txt | sed 'COMMAND'
echo "hello world" | sed 'COMMAND'
```

### Breaking Down the Syntax

```
sed  -n  -e  's/old/new/g'  file.txt
 │    │   │   │  │   │   │    │
 │    │   │   │  │   │   │    └── Input file
 │    │   │   │  │   │   └── Flag: g = global (all occurrences)
 │    │   │   │  │   └── Replacement string
 │    │   │   │  └── Search pattern
 │    │   │   └── Command: s = substitute
 │    │   └── -e = expression (specify a command)
 │    └── -n = suppress automatic printing
 └── The sed program
```

### Your First `sed` Command

```bash
# Create a sample file
echo -e "Hello World\nGoodbye World\nHello Linux" > sample.txt

# Replace "Hello" with "Hi"
sed 's/Hello/Hi/' sample.txt
```

**Output:**

```
Hi World
Goodbye World
Hi Linux
```

> ⚠️ **Important:** The original `sample.txt` is **unchanged**. `sed` only printed the modified version to stdout.

### Inline Editing (`-i` flag) — Modify the File Directly

```bash
# This MODIFIES the file in-place
sed -i '' 's/Hello/Hi/' sample.txt      # macOS (BSD sed)
sed -i 's/Hello/Hi/' sample.txt         # Linux (GNU sed)
```

> **macOS Note:** BSD `sed` requires `''` (empty string) after `-i`. GNU `sed` on Linux does not. This is the #1 source of confusion for beginners.

### Creating a Backup Before Editing

```bash
# Creates sample.txt.bak before modifying
sed -i.bak 's/Hello/Hi/' sample.txt     # Works on both macOS and Linux
```

This creates `sample.txt.bak` (original) and modifies `sample.txt`.

---

## 3. 🔄 Substitution — The `s` Command

The **substitute** command is the most-used `sed` command. It finds a pattern and replaces it.

### Syntax

```
s/PATTERN/REPLACEMENT/FLAGS
```

| Part          | Description                            |
| ------------- | -------------------------------------- |
| `s`           | Substitute command                     |
| `/`           | Delimiter (can be changed — see below) |
| `PATTERN`     | What to search for (regex supported)   |
| `REPLACEMENT` | What to replace it with                |
| `FLAGS`       | Modify behavior (g, i, p, number, w)   |

### 3.1 Basic Substitution (First Occurrence Only)

```bash
echo "cat cat cat" | sed 's/cat/dog/'
```

**Output:**

```
dog cat cat
```

> By default, `sed` only replaces the **first occurrence** on each line.

### 3.2 Global Substitution (`g` flag)

```bash
echo "cat cat cat" | sed 's/cat/dog/g'
```

**Output:**

```
dog dog dog
```

The `g` flag tells `sed` to replace **ALL occurrences** on each line, not just the first.

### 3.3 Case-Insensitive Substitution (`I` or `i` flag)

```bash
echo "Hello HELLO hello" | sed 's/hello/Hi/gI'
```

**Output:**

```
Hi Hi Hi
```

> **Note:** The `I` flag (case-insensitive) is a **GNU sed extension**. It may not work on BSD/macOS default sed.

### 3.4 Replace Nth Occurrence (Number flag)

```bash
echo "apple apple apple apple" | sed 's/apple/ORANGE/2'
```

**Output:**

```
apple ORANGE apple apple
```

Only the **2nd occurrence** is replaced. You can combine with `g`:

```bash
# Replace from 2nd occurrence onwards
echo "apple apple apple apple" | sed 's/apple/ORANGE/2g'
```

**Output:**

```
apple ORANGE ORANGE ORANGE
```

### 3.5 Print Only Modified Lines (`p` flag + `-n`)

```bash
cat <<EOF > server.log
INFO: Server started
ERROR: Connection refused
INFO: Request processed
ERROR: Timeout occurred
INFO: Server idle
EOF

# Print only lines where substitution happened
sed -n 's/ERROR/CRITICAL/p' server.log
```

**Output:**

```
CRITICAL: Connection refused
CRITICAL: Timeout occurred
```

> **How it works:** `-n` suppresses all output. The `p` flag prints **only** the lines where a substitution was made.

### 3.6 Write Matches to a File (`w` flag)

```bash
sed -n 's/ERROR/CRITICAL/w errors.txt' server.log
cat errors.txt
```

**Output (errors.txt):**

```
CRITICAL: Connection refused
CRITICAL: Timeout occurred
```

### 3.7 Changing Delimiters

When your pattern contains `/` (like file paths), use a different delimiter:

```bash
# HARD to read — escaping slashes
sed 's/\/usr\/local\/bin/\/opt\/bin/' file.txt

# EASY to read — using | as delimiter
sed 's|/usr/local/bin|/opt/bin|' file.txt

# You can use ANY character as delimiter:
sed 's#/usr/local/bin#/opt/bin#' file.txt
sed 's@/usr/local/bin@/opt/bin@' file.txt
```

> **Pro Tip:** When working with file paths or URLs, always change the delimiter to `|`, `#`, or `@`.

### 3.8 Using `&` — Reference the Matched Pattern

The `&` symbol in the replacement represents **the entire matched text**.

```bash
echo "192.168.1.1" | sed 's/[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}/[&]/'
```

**Output:**

```
[192.168.1.1]
```

More practical example:

```bash
# Wrap every word in quotes
echo "hello world foo" | sed 's/[a-zA-Z]\+/"&"/g'
```

**Output:**

```
"hello" "world" "foo"
```

### 3.9 Backreferences — Capture Groups `\1`, `\2`, etc.

Use `\(` and `\)` to create capture groups, and `\1`, `\2` to reference them:

```bash
# Swap first and last name
echo "John Smith" | sed 's/\([A-Za-z]*\) \([A-Za-z]*\)/\2 \1/'
```

**Output:**

```
Smith John
```

```bash
# Extract date components and reformat
echo "2025-03-15" | sed 's/\([0-9]\{4\}\)-\([0-9]\{2\}\)-\([0-9]\{2\}\)/\3\/\2\/\1/'
```

**Output:**

```
15/03/2025
```

```bash
# Duplicate a word
echo "hello" | sed 's/\(hello\)/\1 \1/'
```

**Output:**

```
hello hello
```

---

## 4. 🎯 Address & Line Selection

By default, `sed` applies commands to **every line**. Addresses let you target specific lines.

### 4.1 Single Line Number

```bash
# Apply substitution ONLY to line 3
sed '3s/foo/bar/' file.txt

# Delete line 5
sed '5d' file.txt
```

### 4.2 Line Range

```bash
# Substitute on lines 2 through 5
sed '2,5s/old/new/g' file.txt

# Delete lines 10 to 20
sed '10,20d' file.txt
```

### 4.3 From Line N to End of File (`$`)

```bash
# Substitute from line 3 to end of file
sed '3,$s/old/new/g' file.txt

# Delete from line 5 to end
sed '5,$d' file.txt
```

### 4.4 Pattern Address — Match by Regex

```bash
# Substitute only on lines containing "ERROR"
sed '/ERROR/s/foo/bar/' server.log

# Delete lines containing "DEBUG"
sed '/DEBUG/d' server.log
```

### 4.5 Pattern Range

```bash
# Substitute between lines matching START and END patterns
sed '/BEGIN/,/END/s/old/new/g' file.txt

# Delete everything between two patterns (inclusive)
sed '/START/,/STOP/d' file.txt
```

### 4.6 Mixed Address (Line Number + Pattern)

```bash
# From line 1 to the first line matching "STOP"
sed '1,/STOP/s/old/new/g' file.txt
```

### 4.7 Negation (`!`) — Apply to Lines NOT Matching

```bash
# Delete all lines EXCEPT those containing "ERROR"
sed '/ERROR/!d' server.log

# Substitute on all lines EXCEPT line 5
sed '5!s/old/new/g' file.txt

# Substitute on all lines EXCEPT lines 1-3
sed '1,3!s/old/new/g' file.txt
```

### 4.8 Step Addresses (GNU sed only)

```bash
# Every 2nd line starting from line 1 (odd lines)
sed '1~2s/old/new/g' file.txt

# Every 3rd line starting from line 0 (lines 3, 6, 9, ...)
sed '0~3s/old/new/g' file.txt
```

**Syntax:** `first~step` — Start at line `first`, then every `step` lines.

### 4.9 Last Line (`$`)

```bash
# Substitute only on the last line
sed '$s/old/new/' file.txt

# Delete the last line
sed '$d' file.txt

# Append text after the last line
sed '$a\--- END OF FILE ---' file.txt
```

---

## 5. ❌ Deletion — The `d` Command

The `d` command deletes lines from the output (not from the original file unless `-i` is used).

### 5.1 Delete Specific Lines

```bash
# Delete line 1 (remove header)
sed '1d' data.csv

# Delete the last line
sed '$d' file.txt

# Delete lines 5-10
sed '5,10d' file.txt
```

### 5.2 Delete Lines Matching a Pattern

```bash
# Delete all blank/empty lines
sed '/^$/d' file.txt

# Delete lines containing "DEBUG"
sed '/DEBUG/d' server.log

# Delete lines starting with # (comments)
sed '/^#/d' config.conf

# Delete lines starting with # OR empty lines (clean config)
sed '/^#/d; /^$/d' config.conf
```

### 5.3 Delete Lines NOT Matching (Keep only matches)

```bash
# Keep only lines containing "ERROR"
sed '/ERROR/!d' server.log

# Same thing, but using -n and p (more idiomatic)
sed -n '/ERROR/p' server.log
```

### 5.4 Delete from Pattern to End of File

```bash
# Delete everything from "FOOTER" to end
sed '/FOOTER/,$d' file.txt
```

### 5.5 Delete Except Range

```bash
# Keep only lines 5-10, delete everything else
sed '5,10!d' file.txt
```

---

## 6. ✏️ Insertion & Appending

### 6.1 Insert BEFORE a Line (`i` command)

```bash
# Insert a line before line 3
sed '3i\This line was inserted before line 3' file.txt

# Insert before every line matching "ERROR"
sed '/ERROR/i\--- ALERT ---' server.log
```

### 6.2 Append AFTER a Line (`a` command)

```bash
# Append a line after line 5
sed '5a\This line was appended after line 5' file.txt

# Append after last line
sed '$a\--- END OF LOG ---' server.log

# Append after every line containing "WARNING"
sed '/WARNING/a\>>> ACTION REQUIRED <<<' server.log
```

### 6.3 Replace/Change Entire Line (`c` command)

```bash
# Replace line 3 entirely
sed '3c\This is the new line 3' file.txt

# Replace lines matching a pattern
sed '/PermitRootLogin yes/c\PermitRootLogin no' /etc/ssh/sshd_config
```

### 6.4 Practical Example: Add Header to CSV

```bash
sed '1i\ID,Name,Email,Department' data.csv
```

### 6.5 Practical Example: Add Logging Markers

```bash
# Add start/end markers to a log file
sed -e '1i\=== LOG START ===' -e '$a\=== LOG END ===' server.log
```

---

## 7. 🖨️ Print & Output Control (`p` command)

### 7.1 Print Specific Lines

```bash
# Print line 5 (appears twice — once from p, once from default output)
sed '5p' file.txt

# Print ONLY line 5 (suppress default output with -n)
sed -n '5p' file.txt

# Print lines 3-7 only
sed -n '3,7p' file.txt

# Print the first line (head -1 equivalent)
sed -n '1p' file.txt

# Print the last line (tail -1 equivalent)
sed -n '$p' file.txt
```

### 7.2 Print Lines Matching a Pattern

```bash
# Print only lines containing "ERROR"
sed -n '/ERROR/p' server.log

# Print lines containing IP addresses
sed -n '/[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}/p' server.log
```

### 7.3 Print Line Numbers (`=` command)

```bash
# Print line numbers for lines containing "ERROR"
sed -n '/ERROR/=' server.log

# Print line number AND the line itself
sed -n '/ERROR/{=;p}' server.log
```

**Output:**

```
2
ERROR: Connection refused
4
ERROR: Timeout occurred
```

### 7.4 Count Lines (Like `wc -l`)

```bash
sed -n '$=' file.txt
```

### 7.5 Print Up To a Pattern (Then Quit)

```bash
# Print from beginning until first "STOP" (inclusive)
sed '/STOP/q' file.txt

# Print first 10 lines (like head -10)
sed '10q' file.txt
```

### 7.6 The `q` Command — Quit Early

```bash
# Process only first 100 lines of a huge log (much faster than processing all)
sed '100q' huge_server.log

# Print first match and quit immediately
sed -n '/ERROR/{p;q}' server.log
```

> **Performance Tip:** Use `q` when processing large log files. If you only need the first match, `q` stops `sed` immediately instead of scanning the entire file.

---

## 8. 🔤 Transformations — The `y` Command

The `y` command performs **character-by-character transliteration** (like the `tr` command).

### Syntax

```
y/SOURCE_CHARS/DEST_CHARS/
```

Each character in `SOURCE_CHARS` is replaced with the corresponding character in `DEST_CHARS`. Both strings **must be the same length**.

### 8.1 Convert Lowercase to Uppercase

```bash
echo "hello world" | sed 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/'
```

**Output:**

```
HELLO WORLD
```

### 8.2 ROT13 Encoding (Cybersecurity - Simple Cipher)

```bash
echo "Hello World" | sed 'y/ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz/NOPQRSTUVWXYZABCDEFGHIJKLMnopqrstuvwxyzabcdefghijklm/'
```

**Output:**

```
Uryyb Jbeyq
```

### 8.3 Replace Digits with X (Data Masking)

```bash
echo "SSN: 123-45-6789" | sed 'y/0123456789/XXXXXXXXXX/'
```

**Output:**

```
SSN: XXX-XX-XXXX
```

> **Key Difference:** `y` replaces individual characters. `s` replaces strings/patterns.

---

## 9. 📋 Multiple Commands

### 9.1 Using `-e` Flag (Multiple Expressions)

```bash
sed -e 's/foo/bar/' -e 's/baz/qux/' -e '/DEBUG/d' file.txt
```

Each `-e` specifies a separate `sed` command. They are applied **in order** to each line.

### 9.2 Using Semicolons

```bash
sed 's/foo/bar/; s/baz/qux/; /DEBUG/d' file.txt
```

Same as above, but more compact. All commands separated by `;`.

### 9.3 Using a Script File (`-f` flag)

Create a file called `commands.sed`:

```
# commands.sed - SED script for log cleaning
# Remove comment lines
/^#/d
# Remove empty lines
/^$/d
# Replace ERROR with CRITICAL
s/ERROR/CRITICAL/g
# Remove DEBUG lines
/DEBUG/d
```

Run it:

```bash
sed -f commands.sed server.log
```

> **Pro Tip:** For complex transformations, always use a script file. It's reusable, version-controllable, and self-documenting.

### 9.4 Grouping Commands with Curly Braces `{}`

Apply multiple commands to lines matching an address:

```bash
# For lines containing "ERROR": replace, then print
sed -n '/ERROR/{
    s/ERROR/CRITICAL/
    s/Connection/Conn/
    p
}' server.log
```

Curly braces `{}` group commands so they **all apply** to the same address.

---

## 10. 🧠 Hold & Pattern Space (Advanced)

This is where `sed` gets **powerful**. Understanding these two buffers is key to mastering `sed`.

### The Two-Buffer Model

```
┌──────────────────────────────────────────────────┐
│              SED TWO-BUFFER MODEL                │
├──────────────────────────────────────────────────┤
│                                                  │
│  ┌──────────────────┐   ┌──────────────────┐    │
│  │   Pattern Space   │   │    Hold Space    │    │
│  │ (active buffer)   │   │ (storage buffer) │    │
│  │                   │   │                  │    │
│  │ Current line is   │   │ Empty by default │    │
│  │ loaded here for   │   │ Used to store    │    │
│  │ processing        │   │ data across      │    │
│  │                   │   │ line cycles      │    │
│  └────────┬──────────┘   └────────┬─────────┘   │
│           │                       │              │
│           ├───── h (copy PS→HS) ──►              │
│           ├───── H (append PS→HS)─►              │
│           ◄───── g (copy HS→PS) ──┤              │
│           ◄───── G (append HS→PS)─┤              │
│           ◄─────── x (swap) ──────┤              │
│                                                  │
└──────────────────────────────────────────────────┘
```

### Buffer Commands Reference

| Command | Action                | Description                                         |
| ------- | --------------------- | --------------------------------------------------- |
| `h`     | Copy Pattern → Hold   | **Overwrites** hold space with pattern space        |
| `H`     | Append Pattern → Hold | **Appends** pattern space to hold space (with `\n`) |
| `g`     | Copy Hold → Pattern   | **Overwrites** pattern space with hold space        |
| `G`     | Append Hold → Pattern | **Appends** hold space to pattern space (with `\n`) |
| `x`     | Swap                  | **Exchanges** pattern space and hold space          |

### 10.1 Example: Reverse the Order of Lines

```bash
# Reverse a file (like tac command)
sed -n '1!G;h;$p' file.txt
```

**How it works step by step:**

```
Line 1: "AAA"
  1!G  → skip (it's line 1)
  h    → Hold = "AAA"
  $p   → skip (not last line)

Line 2: "BBB"
  1!G  → Pattern = "BBB\nAAA" (append hold to pattern)
  h    → Hold = "BBB\nAAA"
  $p   → skip

Line 3 (last): "CCC"
  1!G  → Pattern = "CCC\nBBB\nAAA"
  h    → Hold = "CCC\nBBB\nAAA"
  $p   → Print: CCC\nBBB\nAAA
```

**Output:**

```
CCC
BBB
AAA
```

### 10.2 Example: Join Every Two Lines

```bash
echo -e "Name: John\nAge: 30\nName: Jane\nAge: 25" | sed 'N;s/\n/ | /'
```

**Output:**

```
Name: John | Age: 30
Name: Jane | Age: 25
```

> `N` — Reads the **Next** line and appends it to the pattern space (with `\n`).

### 10.3 Example: Print Previous Line Before Match

```bash
# Print the line BEFORE each line containing "ERROR"
sed -n '/ERROR/{x;p;d}; x' server.log
```

### 10.4 Example: Remove Duplicate Consecutive Lines (like `uniq`)

```bash
sed '$!N; /^\(.*\)\n\1$/!P; D' file.txt
```

> **Don't worry** if hold/pattern space feels complex. Most daily `sed` work uses `s`, `d`, `p`, and addresses. Hold space is for advanced scripting.

---

## 11. 🔀 Branching & Flow Control

`sed` has basic flow control with **labels**, **branches**, and **tests**. This turns `sed` into a mini programming language.

### 11.1 Labels (`:label`)

A label is a marker in your script that you can jump to:

```
:my_label
```

### 11.2 Branch (`b` command) — Unconditional Jump

```
b label_name    # Jump to :label_name
b               # Jump to end of script (skip remaining commands for this line)
```

**Example: Skip processing for certain lines**

```bash
cat <<EOF > /tmp/mixed.txt
HEADER: Report Title
data: line 1
data: line 2
FOOTER: End of Report
data: line 3
EOF

# Skip lines containing HEADER or FOOTER — only process data lines
sed '/HEADER/b; /FOOTER/b; s/data/record/' /tmp/mixed.txt
```

**Output:**

```
HEADER: Report Title
record: line 1
record: line 2
FOOTER: End of Report
record: line 3
```

> Lines matching `HEADER` or `FOOTER` branch to the end of the script (no `s` command applied).

### 11.3 Test (`t` command) — Conditional Jump

`t label` — Jump to `:label` **only if the last `s` command made a successful substitution**.

```bash
# If substitution succeeds, skip to end (don't apply further commands)
echo -e "apple\nbanana\napricot" | sed 's/^a/A/; t done; s/^b/B/; :done'
```

**Output:**

```
Apple
Banana
Apricot
```

**How it works:**

- `apple` → `s/^a/A/` succeeds → `t done` jumps to `:done` → skips `s/^b/B/`
- `banana` → `s/^a/A/` fails → `t done` does nothing → `s/^b/B/` runs → `Banana`
- `apricot` → `s/^a/A/` succeeds → `t done` jumps → skips rest

### 11.4 Test (`T` command — GNU only) — Branch if NO Substitution

`T label` — Jump **only if the last `s` command did NOT make a substitution** (opposite of `t`).

```bash
# Print a warning if no substitution was made
echo -e "error: bad input\ninfo: all good" | sed 's/error/ERROR/; T skip; s/$/  ← FIXED/; :skip'
```

**Output:**

```
ERROR: bad input  ← FIXED
info: all good
```

### 11.5 Practical Example: Simple State Machine

```bash
# Extract text between BEGIN and END markers (multi-line)
cat <<EOF > /tmp/report.txt
Some junk before
BEGIN
Important data line 1
Important data line 2
Important data line 3
END
More junk after
EOF

sed -n '/BEGIN/,/END/{
    /BEGIN/d
    /END/d
    p
}' /tmp/report.txt
```

**Output:**

```
Important data line 1
Important data line 2
Important data line 3
```

---

## 12. 🔤 Regular Expressions in SED

`sed` uses **Basic Regular Expressions (BRE)** by default. You can switch to **Extended Regular Expressions (ERE)** with `-E` (or `-r` on older GNU sed).

### 12.1 BRE vs ERE — Key Differences

| Feature            | BRE (default) | ERE (`-E` flag) |
| ------------------ | ------------- | --------------- |
| Grouping           | `\(` and `\)` | `(` and `)`     |
| Or                 | `\|`          | `\|`            |
| One or more        | `\+`          | `+`             |
| Zero or one        | `\?`          | `?`             |
| Repetition `{n,m}` | `\{n,m\}`     | `{n,m}`         |
| Character classes  | Same          | Same            |
| Anchors            | Same          | Same            |

> **Pro Tip:** Always use `sed -E` for cleaner regex syntax. The only downside is portability to very old systems.

### 12.2 Metacharacters Reference

| Metachar    | Meaning                  | Example                                               |
| ----------- | ------------------------ | ----------------------------------------------------- | ------------ |
| `.`         | Any single character     | `s/.at/dog/` matches "cat", "bat", "hat"              |
| `*`         | Zero or more of previous | `s/ab*c/X/` matches "ac", "abc", "abbc"               |
| `+` (ERE)   | One or more of previous  | `sed -E 's/ab+c/X/'` matches "abc", "abbc"            |
| `?` (ERE)   | Zero or one of previous  | `sed -E 's/colou?r/color/'` matches "color", "colour" |
| `^`         | Start of line            | `s/^#//' ` removes leading #                          |
| `$`         | End of line              | `s/$/;/` adds ; at end                                |
| `[ ]`       | Character class          | `s/[aeiou]//g` removes vowels                         |
| `[^ ]`      | Negated class            | `s/[^0-9]//g` keeps only digits                       |
| `\b`        | Word boundary            | `s/\bcat\b/dog/g` replaces "cat" not "catalog"        |
| `\|`        | OR (alternation)         | `sed -E 's/(cat                                       | dog)/pet/g'` |
| `( )` (ERE) | Capture group            | `sed -E 's/(foo)(bar)/\2\1/'` → "barfoo"              |
| `\1`        | Backreference            | Reference capture group #1                            |
| `{n}`       | Exactly n times          | `sed -E 's/[0-9]{3}/XXX/'`                            |
| `{n,}`      | n or more times          | `sed -E 's/[0-9]{3,}/XXX/'`                           |
| `{n,m}`     | Between n and m times    | `sed -E 's/[0-9]{2,4}/XX/'`                           |

### 12.3 POSIX Character Classes

These work inside `[: :]` within `[ ]`:

| Class       | Matches          | Equivalent      |
| ----------- | ---------------- | --------------- |
| `[:alpha:]` | Letters          | `[a-zA-Z]`      |
| `[:digit:]` | Digits           | `[0-9]`         |
| `[:alnum:]` | Letters + digits | `[a-zA-Z0-9]`   |
| `[:upper:]` | Uppercase        | `[A-Z]`         |
| `[:lower:]` | Lowercase        | `[a-z]`         |
| `[:space:]` | Whitespace       | `[ \t\n\r\f\v]` |
| `[:blank:]` | Space and tab    | `[ \t]`         |
| `[:punct:]` | Punctuation      |                 |
| `[:print:]` | Printable chars  |                 |

**Usage:**

```bash
# Remove all digits
echo "abc123def456" | sed 's/[[:digit:]]//g'
# Output: abcdef

# Remove all non-alphanumeric characters
echo "Hello, World! #2024" | sed 's/[^[:alnum:] ]//g'
# Output: Hello World 2024

# Remove leading whitespace
sed 's/^[[:space:]]*//' file.txt

# Remove trailing whitespace
sed 's/[[:space:]]*$//' file.txt

# Remove both leading and trailing whitespace
sed 's/^[[:space:]]*//; s/[[:space:]]*$//' file.txt
```

### 12.4 Regex Examples in SED

```bash
# Match IP addresses
sed -E 's/([0-9]{1,3}\.){3}[0-9]{1,3}/[REDACTED_IP]/g' server.log

# Match email addresses
sed -E 's/[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}/[REDACTED_EMAIL]/g' file.txt

# Match URLs
sed -E 's|https?://[^ ]+|[REDACTED_URL]|g' file.txt

# Match MAC addresses
sed -E 's/([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}/[REDACTED_MAC]/g' file.txt

# Match dates (YYYY-MM-DD)
sed -E 's/[0-9]{4}-[0-9]{2}-[0-9]{2}/[DATE]/g' file.txt

# Match credit card numbers (basic)
sed -E 's/[0-9]{4}[- ]?[0-9]{4}[- ]?[0-9]{4}[- ]?[0-9]{4}/[REDACTED_CC]/g' file.txt
```

### 12.5 Greedy vs Non-Greedy Matching

> ⚠️ **Important:** `sed` only supports **greedy** matching. There is NO non-greedy (`*?`) support.

```bash
# Greedy: matches as MUCH as possible
echo '<b>bold</b> and <i>italic</i>' | sed 's/<.*>/REPLACED/'
# Output: REPLACED
# It matched from first < to LAST >!

# Workaround: match non-angle-bracket characters
echo '<b>bold</b> and <i>italic</i>' | sed 's/<[^>]*>/REPLACED/g'
# Output: REPLACEDboldREPLACED and REPLACEDitalicREPLACED
```

> **The trick:** Instead of `.*` (which is greedy), use `[^DELIMITER]*` to match everything except the closing delimiter.

---

## 13. 🛡️ Real-World Cybersecurity & SOC Examples

### Create Sample Files for Practice

```bash
# Sample auth.log
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
EOF

# Sample Apache access log
cat > /tmp/access.log << 'EOF'
192.168.1.100 - - [15/Jan/2024:08:23:01 +0000] "GET /index.html HTTP/1.1" 200 1234
10.0.0.55 - - [15/Jan/2024:08:23:15 +0000] "POST /login HTTP/1.1" 401 543
192.168.1.100 - - [15/Jan/2024:08:24:01 +0000] "GET /api/users HTTP/1.1" 200 8765
203.0.113.42 - - [15/Jan/2024:08:25:33 +0000] "GET /../../etc/passwd HTTP/1.1" 403 234
10.0.0.55 - - [15/Jan/2024:08:26:00 +0000] "GET /admin' OR '1'='1 HTTP/1.1" 400 123
172.16.0.10 - - [15/Jan/2024:08:30:01 +0000] "GET /dashboard HTTP/1.1" 200 5432
198.51.100.5 - - [15/Jan/2024:09:01:15 +0000] "GET /wp-admin HTTP/1.1" 404 234
192.168.1.100 - - [15/Jan/2024:09:05:00 +0000] "GET /api/v2/data HTTP/1.1" 200 9876
EOF

# Sample sshd_config
cat > /tmp/sshd_config << 'EOF'
# SSH Server Configuration
Port 22
ListenAddress 0.0.0.0
PermitRootLogin yes
PasswordAuthentication yes
PubkeyAuthentication yes
MaxAuthTries 6
X11Forwarding yes
AllowTcpForwarding yes
UsePAM yes
EOF
```

### 13.1 Extract Only IP Addresses from Logs

```bash
# Extract all IPs from auth.log
sed -E 's/.*from ([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+).*/\1/' /tmp/auth.log
```

**Output:**

```
192.168.1.100
10.0.0.55
10.0.0.55
10.0.0.55
10.0.0.55
10.0.0.55
172.16.0.10
203.0.113.42
203.0.113.42
192.168.1.100
198.51.100.5
198.51.100.5
```

**Get unique IPs with sort:**

```bash
sed -E 's/.*from ([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+).*/\1/' /tmp/auth.log | sort -u
```

### 13.2 Extract Only Failed Login Entries

```bash
sed -n '/Failed password/p' /tmp/auth.log
```

### 13.3 Redact Sensitive Data in Logs (PII Removal)

```bash
# Redact IP addresses
sed -E 's/([0-9]{1,3}\.){3}[0-9]{1,3}/[REDACTED_IP]/g' /tmp/auth.log

# Redact usernames (keep structure, hide names)
sed -E 's/(for )(invalid user )?[a-zA-Z]+( from)/\1\2[REDACTED]\3/g' /tmp/auth.log

# Redact port numbers
sed -E 's/port [0-9]+/port XXXXX/g' /tmp/auth.log
```

### 13.4 SSH Config Hardening (Automated)

```bash
# Disable root login
sed 's/PermitRootLogin yes/PermitRootLogin no/' /tmp/sshd_config

# Disable password authentication
sed 's/PasswordAuthentication yes/PasswordAuthentication no/' /tmp/sshd_config

# Reduce max auth tries
sed 's/MaxAuthTries 6/MaxAuthTries 3/' /tmp/sshd_config

# Disable X11 forwarding
sed 's/X11Forwarding yes/X11Forwarding no/' /tmp/sshd_config

# ALL AT ONCE — chain with semicolons
sed -e 's/PermitRootLogin yes/PermitRootLogin no/' \
    -e 's/PasswordAuthentication yes/PasswordAuthentication no/' \
    -e 's/MaxAuthTries 6/MaxAuthTries 3/' \
    -e 's/X11Forwarding yes/X11Forwarding no/' \
    -e 's/AllowTcpForwarding yes/AllowTcpForwarding no/' \
    /tmp/sshd_config
```

**Output (hardened config):**

```
# SSH Server Configuration
Port 22
ListenAddress 0.0.0.0
PermitRootLogin no
PasswordAuthentication no
PubkeyAuthentication yes
MaxAuthTries 3
X11Forwarding no
AllowTcpForwarding no
UsePAM yes
```

### 13.5 Clean Nmap Output

```bash
cat > /tmp/nmap_raw.txt << 'EOF'
Starting Nmap 7.94 ( https://nmap.org ) at 2024-01-15 08:00 UTC
Nmap scan report for 192.168.1.1
Host is up (0.0010s latency).
Not shown: 995 closed ports
PORT     STATE SERVICE  VERSION
22/tcp   open  ssh      OpenSSH 8.9p1
80/tcp   open  http     Apache 2.4.52
443/tcp  open  ssl/http Apache 2.4.52
3306/tcp open  mysql    MySQL 8.0.31
8080/tcp open  http     Squid 5.2
Nmap done: 1 IP address (1 host up) scanned in 12.34 seconds
EOF

# Extract only open port lines
sed -n '/^[0-9].*open/p' /tmp/nmap_raw.txt

# Extract just port numbers
sed -n '/^[0-9].*open/p' /tmp/nmap_raw.txt | sed -E 's|^([0-9]+)/.*|\1|'
```

**Output (port numbers only):**

```
22
80
443
3306
8080
```

### 13.6 IOC (Indicators of Compromise) Extraction

```bash
cat > /tmp/threat_intel.txt << 'EOF'
Threat Report - APT-42 Campaign
================================
The attackers used the following infrastructure:
C2 Server: 185.234.218.42
Phishing domain: evil-login.example.com
Backup C2: 91.219.236.15
Malware hash: d41d8cd98f00b204e9800998ecf8427e
Drop server: malware-cdn.badsite.net
Secondary hash: 5d41402abc4b2a76b9719d911017c592
Exfil endpoint: https://data-collect.evil.com/upload
EOF

# Extract all IPs
echo "=== IP Addresses ==="
sed -n -E 's/.* ([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}).*/\1/p' /tmp/threat_intel.txt

# Extract domains
echo "=== Domains ==="
sed -n -E 's/.* ([a-zA-Z0-9.-]+\.(com|net|org|io)).*/\1/p' /tmp/threat_intel.txt

# Extract MD5 hashes (32 hex chars)
echo "=== MD5 Hashes ==="
sed -n -E 's/.*([a-f0-9]{32}).*/\1/p' /tmp/threat_intel.txt

# Extract URLs
echo "=== URLs ==="
sed -n -E 's/.*(https?:\/\/[^ ]+).*/\1/p' /tmp/threat_intel.txt
```

### 13.7 Clean and Normalize Log Timestamps

```bash
# Convert "Jan 15 08:23:01" to "2024-01-15 08:23:01"
sed -E '
    s/Jan/01/; s/Feb/02/; s/Mar/03/; s/Apr/04/;
    s/May/05/; s/Jun/06/; s/Jul/07/; s/Aug/08/;
    s/Sep/09/; s/Oct/10/; s/Nov/11/; s/Dec/12/;
    s/^([0-9]{2}) ([0-9]{2}) ([0-9:]+)/2024-\1-\2 \3/
' /tmp/auth.log
```

### 13.8 Remove ANSI Color Codes from Terminal Output

Sometimes tools output colored text. To clean it for log storage:

```bash
# Remove ANSI escape sequences
sed -E 's/\x1b\[[0-9;]*m//g' colored_output.txt

# Or for broader ANSI cleanup
sed -E 's/\x1b\[[0-9;]*[a-zA-Z]//g' colored_output.txt
```

### 13.9 Build a Blocklist from Logs

```bash
# Extract IPs from failed logins and format as iptables rules
sed -n '/Failed password/p' /tmp/auth.log \
    | sed -E 's/.*from ([0-9.]+).*/\1/' \
    | sort -u \
    | sed 's/.*/iptables -A INPUT -s & -j DROP/'
```

**Output:**

```
iptables -A INPUT -s 10.0.0.55 -j DROP
iptables -A INPUT -s 198.51.100.5 -j DROP
iptables -A INPUT -s 203.0.113.42 -j DROP
```

### 13.10 Parse CSV Exports from Security Tools

```bash
cat > /tmp/vulns.csv << 'EOF'
host,port,severity,cve,description
192.168.1.10,22,HIGH,CVE-2023-1234,OpenSSH vulnerability
192.168.1.10,80,MEDIUM,CVE-2023-5678,Apache version disclosure
192.168.1.20,3306,CRITICAL,CVE-2023-9999,MySQL RCE
192.168.1.30,443,LOW,CVE-2023-1111,TLS config weakness
192.168.1.20,8080,HIGH,CVE-2023-2222,Squid proxy bypass
EOF

# Show only CRITICAL and HIGH severity findings
sed -n '1p; /CRITICAL\|HIGH/p' /tmp/vulns.csv

# Convert CSV to a readable report format
sed -E '1d; s/,/ | /g; s/^/| /; s/$/ |/' /tmp/vulns.csv
```

---

## 14. 📝 SED One-Liners Cheat Sheet

### File/Text Operations

```bash
# Print file contents (like cat)
sed '' file.txt

# Print specific line
sed -n '5p' file.txt

# Print line range
sed -n '10,20p' file.txt

# Print first line (like head -1)
sed -n '1p' file.txt

# Print last line (like tail -1)
sed -n '$p' file.txt

# Print first N lines (like head -N)
sed '10q' file.txt

# Count lines (like wc -l)
sed -n '$=' file.txt

# Reverse file (like tac)
sed -n '1!G;h;$p' file.txt
```

### Deletion

```bash
# Delete empty/blank lines
sed '/^$/d' file.txt

# Delete lines with only whitespace
sed '/^[[:space:]]*$/d' file.txt

# Delete comment lines (starting with #)
sed '/^#/d' file.txt

# Delete first line (header)
sed '1d' file.txt

# Delete last line
sed '$d' file.txt

# Delete lines matching pattern
sed '/pattern/d' file.txt

# Delete lines NOT matching pattern
sed '/pattern/!d' file.txt

# Delete trailing whitespace
sed 's/[[:space:]]*$//' file.txt

# Delete leading whitespace
sed 's/^[[:space:]]*//' file.txt

# Delete both leading and trailing whitespace
sed 's/^[[:space:]]*//; s/[[:space:]]*$//' file.txt

# Delete blank lines at top of file
sed '/./,$!d' file.txt

# Delete blank lines at bottom of file
sed -e :a -e '/^\n*$/{$d;N;ba' -e '}' file.txt
```

### Substitution

```bash
# Replace first occurrence per line
sed 's/old/new/' file.txt

# Replace all occurrences
sed 's/old/new/g' file.txt

# Replace Nth occurrence
sed 's/old/new/3' file.txt

# Case-insensitive replace (GNU)
sed 's/old/new/gI' file.txt

# Replace only on lines matching pattern
sed '/pattern/s/old/new/g' file.txt

# Replace entire line matching pattern
sed '/pattern/c\new line content' file.txt

# Add prefix to every line
sed 's/^/PREFIX: /' file.txt

# Add suffix to every line
sed 's/$/ :SUFFIX/' file.txt

# Remove HTML tags
sed 's/<[^>]*>//g' file.html

# Double space a file
sed G file.txt

# Single space a double-spaced file
sed 'n;d' file.txt

# Number each line
sed = file.txt | sed 'N;s/\n/\t/'

# Convert DOS/Windows line endings to Unix
sed 's/\r$//' file.txt

# Convert Unix line endings to DOS
sed 's/$/\r/' file.txt
```

### Insert/Append

```bash
# Insert line before first line
sed '1i\New first line' file.txt

# Append line after last line
sed '$a\New last line' file.txt

# Insert blank line before every line matching pattern
sed '/pattern/i\\' file.txt

# Insert text before match
sed '/pattern/i\Text before match' file.txt

# Append text after match
sed '/pattern/a\Text after match' file.txt
```

---

## 15. ⚖️ SED vs AWK vs grep

| Feature              | `grep`        | `sed`          | `awk`                   |
| -------------------- | ------------- | -------------- | ----------------------- |
| **Primary use**      | Search/filter | Transform/edit | Process structured data |
| **Pattern matching** | ✅ Excellent  | ✅ Good        | ✅ Good                 |
| **Find & Replace**   | ❌ No         | ✅ Excellent   | ✅ Good                 |
| **Field processing** | ❌ No         | ❌ Limited     | ✅ Excellent            |
| **Math operations**  | ❌ No         | ❌ No          | ✅ Yes                  |
| **Variables**        | ❌ No         | ❌ No          | ✅ Yes                  |
| **Conditionals**     | ❌ No         | ⚠️ Basic       | ✅ Full if/else         |
| **Line editing**     | ❌ No         | ✅ Excellent   | ⚠️ Possible             |
| **In-place editing** | ❌ No         | ✅ `-i` flag   | ❌ No (use `> tmp`)     |
| **Multi-line**       | ⚠️ Limited    | ✅ Hold space  | ✅ Multi-line RS        |
| **Speed**            | ⚡ Fastest    | ⚡ Fast        | ⚡ Fast                 |
| **Learning curve**   | 📗 Easy       | 📙 Medium      | 📕 Medium-Hard          |

### When to Use What

```
┌─────────────────────────────────────────────────────────────┐
│                DECISION GUIDE                                │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  "I want to FIND lines matching a pattern"                   │
│     → grep                                                   │
│                                                              │
│  "I want to REPLACE text or DELETE lines"                    │
│     → sed                                                    │
│                                                              │
│  "I want to PROCESS columns/fields or DO MATH"              │
│     → awk                                                    │
│                                                              │
│  "I want to EDIT a file in-place"                            │
│     → sed -i                                                 │
│                                                              │
│  "I want to GENERATE a REPORT from structured data"         │
│     → awk                                                    │
│                                                              │
│  "I want to COUNT occurrences or AGGREGATE data"            │
│     → awk                                                    │
│                                                              │
│  "I want a SIMPLE filter for log monitoring"                │
│     → grep (or sed -n '/pattern/p')                          │
│                                                              │
│  "I want to CHAIN multiple transformations"                  │
│     → grep ... | sed ... | awk ... (pipeline!)               │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

### Example: Same Task, Three Tools

**Task: Find lines with "ERROR" and extract the IP address**

```bash
# grep — just find the lines
grep "ERROR" server.log

# sed — find and transform
sed -n '/ERROR/s/.*from \([0-9.]*\).*/\1/p' server.log

# awk — find, extract field, and count
awk '/ERROR/ { for(i=1;i<=NF;i++) if($i=="from") print $(i+1) }' server.log
```

> **Pro Tip:** In real SOC work, you combine all three:
>
> ```bash
> grep "Failed password" /var/log/auth.log | sed 's/.*from //;s/ port.*//' | sort | uniq -c | sort -rn | awk '$1 >= 5 { print "BLOCK:", $2, "("$1, "attempts)" }'
> ```

---

## 16. 🏷️ Common Flags Reference

| Flag         | Description                              | Example                             |
| ------------ | ---------------------------------------- | ----------------------------------- | -------------------- |
| `-n`         | Suppress automatic output (silent mode)  | `sed -n '5p' file.txt`              |
| `-e`         | Specify a sed command (multiple allowed) | `sed -e 's/a/b/' -e 's/c/d/' file`  |
| `-f`         | Read commands from a script file         | `sed -f script.sed file.txt`        |
| `-i`         | Edit file in-place (Linux: `sed -i`)     | `sed -i 's/old/new/g' file.txt`     |
| `-i ''`      | Edit in-place on macOS (BSD sed)         | `sed -i '' 's/old/new/g' file.txt`  |
| `-i.bak`     | Edit in-place with backup                | `sed -i.bak 's/old/new/g' file.txt` |
| `-E` or `-r` | Use Extended Regular Expressions         | `sed -E 's/(foo)+/bar/' file.txt`   |
| `-l N`       | Set line-wrap length for `l` command     | `sed -l 70 -n 'l' file.txt`         |
| `-z`         | Treat input as NUL-delimited (GNU)       | `sed -z 's/\n/,/g' file.txt`        |
| `-u`         | Unbuffered output (for real-time pipes)  | `tail -f log                        | sed -u 's/ERR/!!!/'` |
| `--posix`    | Disable GNU extensions                   | `sed --posix 's/old/new/' file.txt` |

### Substitution Flags (after `s/old/new/FLAGS`)

| Flag         | Description                     | Example                  |
| ------------ | ------------------------------- | ------------------------ |
| `g`          | Replace ALL occurrences on line | `s/old/new/g`            |
| `N` (number) | Replace Nth occurrence          | `s/old/new/2`            |
| `p`          | Print line if substitution made | `sed -n 's/old/new/p'`   |
| `w file`     | Write modified line to file     | `s/old/new/w output.txt` |
| `I` or `i`   | Case-insensitive match (GNU)    | `s/hello/Hi/gI`          |
| `m` or `M`   | Multi-line mode (GNU)           | `s/^start/END/m`         |

### SED Commands Summary

| Command      | Description                              |
| ------------ | ---------------------------------------- |
| `s/pat/rep/` | Substitute                               |
| `d`          | Delete pattern space                     |
| `p`          | Print pattern space                      |
| `q`          | Quit (exit sed)                          |
| `a\text`     | Append text after current line           |
| `i\text`     | Insert text before current line          |
| `c\text`     | Change/replace current line              |
| `y/src/dst/` | Transliterate characters                 |
| `=`          | Print current line number                |
| `r file`     | Read and append file contents            |
| `w file`     | Write pattern space to file              |
| `n`          | Read next line into pattern space        |
| `N`          | Append next line to pattern space        |
| `h`          | Copy pattern space to hold space         |
| `H`          | Append pattern space to hold space       |
| `g`          | Copy hold space to pattern space         |
| `G`          | Append hold space to pattern space       |
| `x`          | Exchange pattern and hold spaces         |
| `b label`    | Branch (unconditional jump)              |
| `t label`    | Branch if last `s` succeeded             |
| `T label`    | Branch if last `s` failed (GNU)          |
| `:label`     | Define a label                           |
| `D`          | Delete up to first `\n` in pattern space |
| `P`          | Print up to first `\n` in pattern space  |

---

## 17. 🏋️ Practice Exercises

Use the sample files created in Section 13. Try solving these before checking solutions!

### Beginner

**1.** Remove all comment lines (starting with `#`) from `/tmp/sshd_config`.

**2.** Print only lines containing "Failed" from `/tmp/auth.log`.

**3.** Replace all occurrences of "ssh2" with "SSH-2.0" in `/tmp/auth.log`.

**4.** Delete empty lines from any file.

**5.** Add line numbers to `/tmp/auth.log` output.

<details>
<summary>💡 Solutions</summary>

```bash
# 1
sed '/^#/d' /tmp/sshd_config

# 2
sed -n '/Failed/p' /tmp/auth.log

# 3
sed 's/ssh2/SSH-2.0/g' /tmp/auth.log

# 4
sed '/^$/d' file.txt

# 5
sed = /tmp/auth.log | sed 'N;s/\n/\t/'
```

</details>

### Intermediate

**6.** Extract only the timestamp (first 3 fields) from each line in `/tmp/auth.log`.

**7.** Replace "yes" with "no" ONLY in lines containing "Root" in `/tmp/sshd_config`.

**8.** Print lines 5-8 of `/tmp/auth.log` without using `head`/`tail`.

**9.** Remove everything after "port" (inclusive) from `/tmp/auth.log`.

**10.** Wrap all IP addresses in square brackets in `/tmp/auth.log`.

<details>
<summary>💡 Solutions</summary>

```bash
# 6
sed -E 's/^([A-Za-z]+ [0-9]+ [0-9:]+).*/\1/' /tmp/auth.log

# 7
sed '/Root/s/yes/no/' /tmp/sshd_config

# 8
sed -n '5,8p' /tmp/auth.log

# 9
sed 's/ port.*//' /tmp/auth.log

# 10
sed -E 's/([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3})/[\1]/g' /tmp/auth.log
```

</details>

### Advanced

**11.** Extract all unique IPs from failed logins, sort them, and format each as an iptables DROP rule.

**12.** Using sed only (no awk), count how many lines contain "Failed" (hint: use branching).

**13.** Replace the entire `/tmp/sshd_config` hardening pipeline (5 changes) in a single sed command with a script file.

**14.** Extract text between `BEGIN` and `END` markers from `/tmp/report.txt` (created in Section 11).

**15.** Create a sed command that redacts both IPs AND usernames from `/tmp/auth.log` simultaneously.

<details>
<summary>💡 Solutions</summary>

```bash
# 11
sed -n '/Failed/p' /tmp/auth.log \
    | sed -E 's/.*from ([0-9.]+).*/\1/' \
    | sort -u \
    | sed 's/.*/iptables -A INPUT -s & -j DROP/'

# 12 (count lines - sed isn't ideal for this, but possible)
sed -n '/Failed/p' /tmp/auth.log | sed -n '$='

# 13 — Create script file
cat > /tmp/harden.sed << 'EOF'
s/PermitRootLogin yes/PermitRootLogin no/
s/PasswordAuthentication yes/PasswordAuthentication no/
s/MaxAuthTries 6/MaxAuthTries 3/
s/X11Forwarding yes/X11Forwarding no/
s/AllowTcpForwarding yes/AllowTcpForwarding no/
EOF
sed -f /tmp/harden.sed /tmp/sshd_config

# 14
sed -n '/BEGIN/,/END/{/BEGIN/d;/END/d;p}' /tmp/report.txt

# 15
sed -E 's/([0-9]{1,3}\.){3}[0-9]{1,3}/[REDACTED_IP]/g; s/(for )(invalid user )?[a-z]+( from)/\1\2[REDACTED]\3/g' /tmp/auth.log
```

</details>

---

## 🎯 Quick Reference Card

```
┌─────────────────────────────────────────────────────────────┐
│                    SED QUICK REFERENCE                        │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  SYNTAX:   sed [options] 'command' file                      │
│            sed [options] 'address command' file               │
│                                                              │
│  OPTIONS:  -n         Suppress auto-print                    │
│            -e 'cmd'   Add a command                           │
│            -f file    Commands from file                      │
│            -i[.bak]   Edit in-place                           │
│            -E         Extended regex                           │
│                                                              │
│  ADDRESSES: 5         Line 5                                  │
│             5,10      Lines 5-10                              │
│             $         Last line                                │
│             /regex/   Lines matching regex                    │
│             /s/,/e/   Range: from /s/ to /e/                  │
│             5!        NOT line 5                               │
│                                                              │
│  COMMANDS:  s/old/new/flags   Substitute                     │
│             d                 Delete                          │
│             p                 Print                           │
│             q                 Quit                            │
│             a\text            Append after                    │
│             i\text            Insert before                   │
│             c\text            Replace line                    │
│             y/abc/xyz/        Transliterate                   │
│             =                 Print line number               │
│                                                              │
│  S-FLAGS:   g     Global (all matches)                       │
│             p     Print if matched                            │
│             I     Case-insensitive                            │
│             N     Nth occurrence                              │
│             w f   Write to file                               │
│                                                              │
│  REGEX:     .     Any char        ^     Start of line        │
│             *     0 or more       $     End of line           │
│             []    Char class      [^]   Negated class        │
│             \1    Backreference   &     Matched text          │
│                                                              │
│  PIPELINE COMBO:                                             │
│    grep 'Failed' auth.log | sed 's/.*from //;s/ port.*//'   │
│    | sort | uniq -c | sort -rn                                │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

---

> **🏆 You've completed the Ultimate SED Guide!**
>
> As a SOC analyst and pentester, you'll use SED daily for:
>
> - **Log cleaning** — stripping noise, normalizing formats
> - **Config hardening** — bulk-modifying SSH, Apache, firewall configs
> - **IOC extraction** — pulling IPs, domains, hashes from threat intel
> - **Data redaction** — masking PII before sharing logs
> - **Evidence processing** — cleaning forensic artifacts
> - **CTF challenges** — quick text transformations in TryHackMe rooms
>
> **Next steps:** Practice with real log files and combine `sed` with `grep` and `awk` for powerful command-line pipelines. Refer to your **AWK.MD** guide for the complementary tool!
