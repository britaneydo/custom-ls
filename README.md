# custom-ls 📁

A custom implementation of the UNIX `ls` command written in C++. Explores how Linux manages files, directories, inodes, and permissions under the hood.

---

## What It Does

Reads real metadata from Linux kernel using system calls, just like the actual ls command. Displays:

- **Permissions** — decoded from `st_mode` into a human-readable `rwxr-xr-x` string
- **Owner** — looked up by user ID via `getpwuid()`
- **Group** — looked up by group ID via `getgrgid()`
- **Size** — in bytes from `st_size`
- **Last Modified** — formatted timestamp from `st_mtime`
- **Filename**

Hidden files (dotfiles) are filtered out by default.

---

## Files

| File | Description |
|---|---|
| `lscommand.cpp` | Basic version --> lists filenames only |
| `ls -lcommand.cpp` | Full version --> lists all metadata fields |

---

## How to Compile & Run

```bash
# Compile
g++ -o myls "ls -lcommand.cpp"

# List a single directory
./myls /etc

# List multiple directories
./myls /home /etc /var

# List current directory (no arguments)
./myls
```

---

## Example Output

```
-rw-r--r--    root    root    9       Mar 21 14:32    hostname
-rw-r--r--    root    root    2847    Mar 15 09:11    passwd
drwxr-xr-x    root    root    4096    Mar 20 18:44    apt
```

---

## Key Concepts Demonstrated

**System Calls Used**
- `opendir()` / `readdir()` / `closedir()` --> open and iterate through a directory
- `stat()` --> fetch inode metadata for each file
- `getpwuid()` / `getgrgid()` --> convert numeric IDs to human-readable names
- `localtime()` / `strftime()` --> format Unix timestamps

**OS Concepts Covered**
- **Inodes** --> stores all metadata about the file
- **File permissions** --> the `rwx` system and how it controls access for owner, group, and others
- **Directory structure** --> how Linux represents directories as special files containing entry lists
- **Hidden files** --> Linux convention of dot-prefixed filenames

---

## Linux Requirements

- Linux OS (Ubuntu recommended)
- `g++` compiler

---
 
## Group Members
 
### Britaney Do
### Bassma Ennarah
### Elizabeth Philip
### Sehaj Dhaliwal
