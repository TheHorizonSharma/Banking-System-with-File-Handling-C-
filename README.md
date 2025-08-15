This project is a console-based banking management system developed in C++ that provides essential banking functionalities with secure storage and efficient transaction handling. It is designed using structured programming principles and demonstrates practical use of binary and text file handling, password security, and menu-driven interaction.

The system offers two main operation modes — Admin Mode and User Mode — each with distinct permissions and features.

Admin Mode allows the administrator to:

Create new accounts with unique account numbers.

Store account details securely in a binary file (Userinfo.dat).

Deposit and withdraw funds.

Check account balances.

Update account information (name, phone number, password, or all).

View complete transaction history.

Display all registered accounts.

Close accounts and delete related transactions.

User Mode enables customers to:

Log in securely using their account number and password.

Check their current balance.

View personal transaction history.

Close their account if desired.

When creating an account, the system generates a unique account number and stores details in binary format for security. Passwords are hashed before storage, ensuring they are never saved in plain text.

Every deposit and withdrawal is logged in a text file (Transactions.txt), recording the account number, transaction type, amount, and updated balance. This allows for transparent tracking of all financial activities.

Updating account information is flexible, allowing modification of specific fields or all details at once. Closing an account securely removes the record from the binary file and deletes any associated transaction history.

The system combines binary file storage for sensitive data with text logs for transaction history, balancing security and accessibility.

Key Features & Concepts:

Struct-based account representation.

Binary file handling for secure data storage.

Text file logging for transaction records.

Password hashing for enhanced security.

Menu-driven interface for ease of use.

Persistent storage for data across sessions.
