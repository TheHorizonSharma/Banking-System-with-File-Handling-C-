# Console-Based Banking Management System (C++)

This project is a console-based banking management system built entirely in C++, designed to simulate basic banking operations while demonstrating core programming concepts like file handling, data security, and structured code organization.

## Overview
The system supports two operation modes — **Admin** and **User** — each with separate menus and permissions to ensure secure and role-specific access.

### Admin Mode
Allows full management of bank accounts, including:
- Creating new accounts (auto-generated unique account numbers)
- Depositing and withdrawing funds
- Checking balances
- Updating account information
- Viewing transaction history
- Displaying all registered accounts
- Closing accounts

### User Mode
Provides account holders with secure access to:
- View balances
- Check transaction history
- Close accounts (after password verification)

## Data Storage & Security
- **Binary File**: All user details are stored in `Userinfo.dat` for security and efficiency.
- **Password Hashing**: Only hashed passwords are stored; no plain text.
- **Text Logs**: Transactions are recorded in `Transactions.txt` for human-readable auditing.

## Transactions
- Supports deposits and withdrawals
- Records:
  - Transaction type
  - Amount
  - Resulting balance

## Account Updates
Admins can:
- Update name, phone number, or password individually
- Update all fields at once

## Account Closure
- Removes user data from the binary file
- Deletes related transaction records from the text file

## Technical Highlights
- Struct-based design for account representation
- Password hashing for basic security
- Binary I/O for secure and compact storage
- Text file handling for clear transaction records
- Menu-driven programming for smooth interaction
- Persistent storage across sessions

---

## How to Run
1. Compile:
   ```bash
   g++ main.cpp -o banking
