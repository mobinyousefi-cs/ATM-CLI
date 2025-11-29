# Command-Line ATM Interface  
A Fully Modular C Project by **Mobin Yousefi**

---

## Status & Metadata

[![C Language](https://img.shields.io/badge/language-C-blue.svg)]()  
[![Build](https://img.shields.io/badge/build-Makefile-success)]()  
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)]()  
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey)]()  
[![Author](https://img.shields.io/badge/Author-Mobin%20Yousefi-black)](https://github.com/mobinyousefi-cs)

---

## Overview

The **Command-Line ATM Interface** is a professionally structured C application that simulates an ATM in the terminal.

Features:

- Secure user authentication with PIN hashing (FNV-1a demo hash)  
- Auto-locking accounts after multiple failed attempts  
- Balance inquiry, deposit, and withdrawal operations  
- Persistent account storage in **CSV** or **JSON** format  
- Auto-detection of DB format by file extension (`.db` / `.csv` / `.json`)  
- ANSI-colored terminal output (errors, info messages, banners)  
- Cross-platform **secure masked PIN input** (characters replaced by `*`)

This project is ideal as a teaching/portfolio example for:

- Modular C program design  
- File I/O and simple persistence layers  
- Lightweight parsing (CSV + minimal JSON)  
- Basic security concepts in a CLI environment  

---

## Project Structure

```text
atm-cli/
├── Makefile
├── README.md
├── accounts.db        # sample CSV DB (optional)
├── accounts.json      # sample JSON DB (optional)
├── include/
│   ├── common.h
│   ├── colors.h
│   ├── account.h
│   ├── auth.h
│   ├── ui.h
│   ├── db_json.h
│   └── atm.h
└── src/
    ├── main.c
    ├── atm.c
    ├── account.c
    ├── auth.c
    ├── ui.c
    └── db_json.c
```

---

## Build Instructions

Make sure you have **GCC** or a compatible C compiler installed.

### Default build

```bash
make
```

### Debug build

```bash
make debug
```

### Optimized release build

```bash
make release
```

### Clean

```bash
make clean
```

The resulting executable is:

```bash
./atm_cli
```

(on Windows: `atm_cli.exe`)

---

## Usage

### Run with default database (CSV)

```bash
./atm_cli
```

If no argument is given, the program uses:

```text
accounts.db
```

in the current directory. This file is treated as **CSV**.

---

### Run with explicit CSV database

```bash
./atm_cli data/my_accounts.csv
```

---

### Run with JSON database

```bash
./atm_cli data/my_accounts.json
```

The program automatically detects `.json` and switches to the JSON persistence layer.

---

## Database Formats

### CSV Format (Default)

```text
# account_id,holder_name,balance,pin_hash,is_locked,failed_attempts
1001,John Doe,1500.00,3356862322,0,0
1002,Jane Smith,2500.00,4123456789,0,1
```

Fields:

| Field             | Description                                  |
|-------------------|----------------------------------------------|
| `account_id`      | Unique account identifier                    |
| `holder_name`     | Account holder full name                     |
| `balance`         | Current balance (double)                     |
| `pin_hash`        | 32-bit FNV-1a hash of the PIN                |
| `is_locked`       | 0 = active, 1 = locked                       |
| `failed_attempts` | Number of consecutive failed login attempts  |

---

### JSON Format

```json
{
  "accounts": [
    {
      "id": "1001",
      "holder": "John Doe",
      "balance": 1500.00,
      "pin_hash": 3356862322,
      "locked": 0,
      "failed": 0
    },
    {
      "id": "1002",
      "holder": "Jane Smith",
      "balance": 2500.00,
      "pin_hash": 4123456789,
      "locked": 0,
      "failed": 1
    }
  ]
}
```

The JSON parser is intentionally lightweight and expects a structure similar to the above.

---

## Secure PIN Input

The login PIN is read with **masked input**:

- Characters are not shown directly.
- Each character is echoed as `*`.
- Backspace editing is supported.
- Implementation:
  - On **Windows**, uses `_getch()` from `<conio.h>`.
  - On **Linux/macOS**, uses `termios` to disable echo and manual character reading.

This is a **demonstration-level** implementation, not a production-grade secure input system, but it is significantly better than plain `scanf`.

---

## ANSI Color Output

The project uses ANSI escape sequences for colored messages:

- **Errors** are printed in red.  
- **Informational messages** are printed in green.  
- The **banner** is printed in cyan.

If your terminal does not support ANSI colors, the escape codes will simply be printed as plain text.

---

## Example Session

```text
--------------------------------------------------
  Command-Line ATM Interface
--------------------------------------------------
Database file: accounts.json (JSON)

Type 'q' to exit the ATM.
Enter account ID: 1001
Enter PIN: ****
[INFO] Authentication successful. Welcome!
--------------------------------------------------
Account ID: 1001
Account Holder: John Doe
--------------------------------------------------
1) Balance inquiry
2) Deposit
3) Withdraw
4) Logout
Select an option: 1
Current balance: 1500.00
```

---

## Security Notes

- PINs are never stored in plain text; only a 32-bit hash is saved.  
- The hashing algorithm is FNV-1a, which is **not cryptographically secure** and is used here purely for demonstration.  
- Real-world ATM or banking systems must use strong cryptographic primitives and secure storage mechanisms (e.g., PBKDF2, bcrypt, Argon2, HSMs, etc.).

---

## Roadmap / Possible Extensions

Some natural extensions if you want to evolve this project further:

- Transaction history log (CSV/JSON per account)  
- Admin CLI for creating/locking/unlocking accounts  
- Multi-currency support  
- Unit tests (e.g., using a simple C test harness)  
- GitHub Actions CI workflow (build + basic tests on push/PR)  
- Configuration file for paths, limits, and security settings  

---

## License

This project is released under the **MIT License**.

---

## Author

Developed by **Mobin Yousefi**  
GitHub: [mobinyousefi-cs](https://github.com/mobinyousefi-cs)
