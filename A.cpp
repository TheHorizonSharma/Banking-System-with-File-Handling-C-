#include<iostream>
#include<fstream>
#include<map>
#include<string>
#include<sstream>
#include<vector>
#include<cstring>
#include<algorithm>
using namespace std;
// Structure to represent a bank account
struct account 
{
    string name;
    string phonenumber;
    double balance;
    string password;
    int accnum;
    int dep;
    int m;
    double depositAmount;
    double withdrawAmount;
    int updatedaccnum;
    // Hashes a password by summing ASCII values of characters
    string hashPassword(const string& password) 
    {
        int hash = 0;
        for (char c : password) {
            hash += c;
        }
        return to_string(hash);
    }
    // Generates a new unique account number and saves it to a file
    void generateaccnumber() 
    {
        ifstream inFile("LastAccNumber.txt");
        int lastAccNum = 133443;
        if (inFile.is_open()) {
            inFile >> lastAccNum;
            inFile.close();
        }
        accnum = lastAccNum + 1;
        ofstream outFile("LastAccNumber.txt");
        if (outFile.is_open()) {
            outFile << accnum;
            outFile.close();
        }
    }
    // Stores basic user info in binary format in "Userinfo.dat"
    void store_info() 
    {
        ofstream myFile("Userinfo.dat", ios::binary | ios::app);
        if (!myFile) {
            cout << "Error opening file for writing.\n";
            return;
        }
        char nameArr[50] = {}, phoneArr[15] = {}, passArr[50] = {};
        strncpy(nameArr, name.c_str(), sizeof(nameArr) - 1);
        strncpy(phoneArr, phonenumber.c_str(), sizeof(phoneArr) - 1);
        string hashed = hashPassword(password);
        strncpy(passArr, hashed.c_str(), sizeof(passArr) - 1);
        myFile.write((char*)&accnum, sizeof(accnum));
        myFile.write(nameArr, sizeof(nameArr));
        myFile.write(phoneArr, sizeof(phoneArr));
        myFile.write(passArr, sizeof(passArr));
        myFile.close();
    }
    // Deposits money to the account and records the transaction
    void deposit(double amount) 
    {
        balance += amount;
        cout << "Deposited " << amount << " to the account\n";
        cout << "Updated balance is: " << balance << endl;
        store_transactions('D', amount);
    }
    // Withdraws money if balance is sufficient, and records the transaction
    void withdraw(double amount) 
    {
        if (amount > balance) {
            cout << "Insufficient Balance!!" << endl;
        } else {
            balance -= amount;
            cout << "Withdrawal of " << amount << " was successful from your account" << endl;
            cout << "Updated balance is: " << balance << endl;
            store_transactions('W', amount);
        }
    }
    // Stores each transaction in a text file
    void store_transactions(char type, double amount) 
    {
        ofstream transFile("Transactions.txt", ios::app);
        if (transFile.is_open()) {
            transFile << accnum << "|" << amount << "|" << type << "|" << balance << "|\n";
        } else {
            cout << "Error opening the file Transactions.txt!!" << endl;
        }
    }
    // Searches for an account number in Transactions.txt and updates balance
   bool search_user_by_accnum(int searchAccNum) 
    {
        ifstream myFile("Transactions.txt");
        string line;
        if (myFile.is_open()) {
            bool found = false;
            double latestBalance = 0.0;
            while (getline(myFile, line)) {
                stringstream ss(line);
                string accStr, depositAmount, transType, balanceStr;
                getline(ss, accStr, '|');
                getline(ss, depositAmount, '|');
                getline(ss, transType, '|');
                getline(ss, balanceStr, '|');
                if (accStr.empty() || balanceStr.empty())
                    continue;
                try {
                    int fileAccNum = stoi(accStr);
                    if (fileAccNum == searchAccNum) {
                        latestBalance = stod(balanceStr);
                        accnum = fileAccNum;
                        found = true;
                    }
                } catch (const std::invalid_argument& e) {
                    continue; 
                } catch (const std::out_of_range& e) {
                    continue; 
                }
            }
            if (found) {
                balance = latestBalance;
                cout << "\nAccount found!\n";
                cout << "Account Number: " << accnum << "\n";
            } else {
                cout << "Account number " << searchAccNum << " not found.\n";
            }
            return found;
        } else {
            cout << "Error opening Transactions.txt for reading.\n";
            return false;
        }
    }
    // Searches user by account number and verifies password from Userinfo.dat
    bool search_user_by_accnumU(int searchAccNum) 
{
    ifstream myFile("Userinfo.dat", ios::binary);
    if (!myFile) {
        cout << "Error opening Userinfo.dat for reading.\n";
        return false;
    }
    bool found = false;
    while (!myFile.eof()) {
        int fileAccNum;
        char nameArr[50], phoneArr[15], passArr[50];
        myFile.read((char*)&fileAccNum, sizeof(fileAccNum));
        if (myFile.eof()) break;
        myFile.read(nameArr, sizeof(nameArr));
        myFile.read(phoneArr, sizeof(phoneArr));
        myFile.read(passArr, sizeof(passArr));
        if (fileAccNum == searchAccNum) {
            string inputPassword;
            cout << "Enter your password: ";
            cin >> inputPassword;

            if (hashPassword(inputPassword) != string(passArr)) {
                cout << "Incorrect password. Access denied.\n";
                return false;
            }
            cout << "\nAccount found and password verified!\n";
            accnum = fileAccNum;
            name = string(nameArr);
            phonenumber = string(phoneArr);
            found = true;
            break;
        }
    }
    myFile.close();
    if (!found) {
        cout << "Account number " << searchAccNum << " not found.\n";
    }
    return found;
}
    // Reads and displays transaction history for a given account number
    void read_transactions(int searchAccNum) 
    {
        ifstream readFile("Transactions.txt");
        string line;
        bool found = false;
        if (readFile.is_open()) {
            cout << "\nTransaction History for Account Number " << searchAccNum << ":\n";
            while (getline(readFile, line)) {
                stringstream ss(line);
                string accnumberString, amountString, typeString, balanceString;
                getline(ss, accnumberString, '|');
                getline(ss, amountString, '|');
                getline(ss, typeString, '|');
                getline(ss, balanceString, '|');
                int fileAccNum = stoi(accnumberString);
                if (fileAccNum == searchAccNum) {
                    cout << "Account Number: " << accnumberString
                         << ", Type: " << typeString
                         << ", Amount: " << amountString
                         << ", Current Balance: " << balanceString << endl;
                    found = true;
                }
            }
            if (!found) {
                cout << "No transactions found for this account.\n";
            }
            readFile.close();
        } else {
            cout << "Error opening Transactions.txt for reading." << endl;
        }
    }
    // Updates user info (name, phone, password) in Userinfo.txt
    void update_info() 
{
    int updateAccNum;
    cout << "Enter your Account Number to update info: ";
    cin >> updateAccNum;
    string newName = "", newPhone = "", newPassword = "";
    int fieldChoice;
    cout << "\nWhich field do you want to update?\n";
    cout << "1. Name\n2. Phone Number\n3. Password\n4. All Fields\n";
    cout << "Enter your choice: ";
    cin >> fieldChoice;
    ifstream inFile("Userinfo.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    if (!inFile || !outFile) {
        cout << "Error opening files.\n";
        return;
    }
    bool found = false;
    while (!inFile.eof()) {
        int fileAccNum;
        char nameArr[50] = {}, phoneArr[15] = {}, passArr[50] = {};
        inFile.read((char*)&fileAccNum, sizeof(fileAccNum));
        if (inFile.eof()) break;
        inFile.read(nameArr, sizeof(nameArr));
        inFile.read(phoneArr, sizeof(phoneArr));
        inFile.read(passArr, sizeof(passArr));
        if (fileAccNum == updateAccNum) {
            found = true;
            if (fieldChoice == 1 || fieldChoice == 4) {
                cout << "Enter new name: ";
                cin.ignore();
                getline(cin, newName);
                strncpy(nameArr, newName.c_str(), sizeof(nameArr) - 1);
            }
            if (fieldChoice == 2 || fieldChoice == 4) {
                cout << "Enter new phone number: ";
                cin >> newPhone;
                strncpy(phoneArr, newPhone.c_str(), sizeof(phoneArr) - 1);
            }
            if (fieldChoice == 3 || fieldChoice == 4) {
                cout << "Enter new password: ";
                cin >> newPassword;
                string hashed = hashPassword(newPassword);
                strncpy(passArr, hashed.c_str(), sizeof(passArr) - 1);
            }
        }
        outFile.write((char*)&fileAccNum, sizeof(fileAccNum));
        outFile.write(nameArr, sizeof(nameArr));
        outFile.write(phoneArr, sizeof(phoneArr));
        outFile.write(passArr, sizeof(passArr));
    }
    inFile.close();
    outFile.close();
    if (found) {
        remove("Userinfo.dat");
        rename("temp.dat", "Userinfo.dat");
        cout << "Information updated successfully.\n";
    } else {
        remove("temp.dat");
        cout << "Account number not found.\n";
    }
}
    // Reads and displays all user info from the binary file
    void read_info() 
    {
        ifstream myFile("Userinfo.dat", ios::binary);
        if (!myFile) {
            cout << "Error opening binary file for reading.\n";
            return;
        }
        cout << "\n<--- All Registered Accounts--->\n";
        int count = 1;
        while (true) 
        {
            int accnum;
            char nameArr[50], phoneArr[15], passArr[50];
            myFile.read((char*)&accnum, sizeof(accnum));
            if (myFile.eof()) break;
            myFile.read(nameArr, sizeof(nameArr));
            myFile.read(phoneArr, sizeof(phoneArr));
            myFile.read(passArr, sizeof(passArr));
            cout << "\nAccount #" << count++ << ":\n";
            cout << "  Account Number : " << accnum << "\n";
            cout << "  Name           : " << string(nameArr) << "\n";
            cout << "  Phone Number   : " << string(phoneArr) << "\n";
            cout << "  Password (Hashed) : " << string(passArr) << "\n";
        }
        myFile.close();
    }
    // Closes user account by removing it from Userinfo.dat
    bool close_Account() 
    {
        int accToDelete;
        cout << "Enter your Account Number to close: ";
        cin >> accToDelete;
        ifstream inFile("Userinfo.dat", ios::binary);
        ofstream outFile("temp.dat", ios::binary);
        bool found = false;
        if (!inFile || !outFile) {
            cout << "Error opening files.\n";
            return false;
        }
        while (true) {
            int accnum;
            char nameArr[50], phoneArr[15], passArr[50];
            inFile.read((char*)&accnum, sizeof(accnum));
            if (inFile.eof()) break;
            inFile.read(nameArr, sizeof(nameArr));
            inFile.read(phoneArr, sizeof(phoneArr));
            inFile.read(passArr, sizeof(passArr));
            if (accnum == accToDelete) {
                found = true;
                continue;  
            }
            outFile.write((char*)&accnum, sizeof(accnum));
            outFile.write(nameArr, sizeof(nameArr));
            outFile.write(phoneArr, sizeof(phoneArr));
            outFile.write(passArr, sizeof(passArr));
        }
        inFile.close();
        outFile.close();
        if (found) {
            remove("Userinfo.dat");
            rename("temp.dat", "Userinfo.dat");
            cout << "Account closed successfully.\n";
        } else {
            remove("temp.dat");
            cout << "Account not found.\n";
        }
        return found;
    }
 // Deletes all transaction records of a closed account
    void close_AccountT(int accToDelete) {
        ifstream inFile("Transactions.txt");
        ofstream outFile("tempp.txt");
        string line;
        bool found = false;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string accStr;
            getline(ss, accStr, '|');
            if (stoi(accStr) == accToDelete) {
                found = true;
                continue;
            }
            outFile << line << endl;
        }
        inFile.close();
        outFile.close();
        if (found) {
            remove("Transactions.txt");
            rename("tempp.txt", "Transactions.txt");
            cout << "Transactions deleted successfully.\n";
        } else {
            remove("tempp.txt");
            cout << "No transactions found for this account.\n";
        }
    }
};
int main()
{
    account Useraccount;
    string name;
    string phonenumber;
    double balance;
    string password;
    int accnum;
    int dep;
    string email;
    int ustype;
    string adminUsername, adminPassword;
    const string correctUsername = "admin";
    const string correctPassword = "admin123";
    int m;
    int searchAccNum;
    double depositAmount;
    double withdrawAmount;
    int l;
while (true) 
{
    cout << "\nWELCOME TO C++ BANKING SYSTEM\n";
    cout<<"Main Menu: \n";
    cout<<"1. Admin\n2. User\n3. Exit\n";
    cout<<"Enter your choice: ";
    cin>>l;
    if (l==1) 
    {
        cout<<"Enter username: ";
        cin>>adminUsername;
        cout<<"Enter password: ";
        cin>>adminPassword;
        if (adminUsername == correctUsername && adminPassword == correctPassword) 
        {
           ustype = 1;
           break;
        }
        else
        {
            cout<<"Incorrect Credentials!!";
            continue; 
        }
    }
    else if(l==2) 
    {
        ustype=2;
        break; 
    }
    else if(l==3) 
    {
        cout << "Exiting system. Goodbye!\n";
        return 0;
    }
    else
    {
        cout << "Invalid input! Please try again.\n";
    }
}
  if(ustype==1)
  {
  int choice;
  do {
      cout<<"\nMenu: \n1. Deposit \n2. Withdraw \n3. Check Balance \n4. Create New Account\n5. Update my Information\n6. Transaction History\n7. Close Account\n8. Show all Accounts\n9. Exit to main menu\n10. Exit System\n";
      cout<<"Enter your choice: ";
      cin>>choice;
      cout<<"Your choice is:  "<<choice<<endl;
      if(choice==1)
      {
          cout<<"Enter your Account Number to deposit: ";
          cin>>searchAccNum;
          if (Useraccount.search_user_by_accnum(searchAccNum)) 
          {
            cout<<"Enter amount to deposit: ";
            cin>>depositAmount;
            Useraccount.depositAmount=depositAmount;   
            Useraccount.deposit(depositAmount);     
          } 
          else 
          {
            cout<<"Enter a valid account number to deposit!! \n";
          }
     } 
      else if(choice==2) 
      {
          cout<<"Enter your Account Number to Withdraw: ";
          cin>>searchAccNum;
          if (Useraccount.search_user_by_accnum(searchAccNum)) 
          {
            cout<<"Enter amount to withdraw: ";
            cin>>withdrawAmount;
            Useraccount.withdrawAmount=withdrawAmount; 
            Useraccount.withdraw(withdrawAmount);     
          } 
          else 
          {
            cout<<"Enter a valid account number to withdraw!! \n";
          }
      } 
      else if(choice==3)
      {
        cout<<"Enter Account Number to check your balance: ";
        cin>>searchAccNum;
        if (Useraccount.search_user_by_accnum(searchAccNum)) 
        {
            cout << "Your current balance is: " << Useraccount.balance << endl;
        }
      } 
      else if(choice==4)
      {
        cout<<"Enter your name: ";
        cin.ignore();
        getline(cin, name);
        cout<<"Enter your phone number(ensure 10 digits): ";
        cin>>phonenumber;
        cout<<"Enter password for your account: ";
        string password;
        cin>>password;
        Useraccount.hashPassword(password);
        cout<<"Enter your initial deposit (minimum 1000): ";
        cin>>dep;
    while(dep < 1000)
    {
        cout<<"Initial Deposit should be at least 1000. Try again: ";
        cin>>dep;
    }
    Useraccount.name=name;
    Useraccount.phonenumber = phonenumber;
    Useraccount.password=password;
    Useraccount.generateaccnumber();
    Useraccount.store_info();
    Useraccount.balance = 0; 
 //   Useraccount.read_info();
    Useraccount.deposit(dep);
    cout<< "Account created successfully!\n";
    cout<< "Account Number: "<<Useraccount.accnum<<endl;
    cout<< "Initial Balance: "<<Useraccount.balance;
    } 
    else if(choice==5)
    {
        Useraccount.update_info();
    }
    else if(choice==6)
    {
            cout<<"Enter Account Number for transaction history: "<<endl;
            cin>>searchAccNum;
            Useraccount.read_transactions(searchAccNum);
    }
    else if(choice==7)
    {
        Useraccount.close_Account();
        Useraccount.close_AccountT(Useraccount.accnum); 
        cout<<endl;
    }
    else if(choice==8)
    {
        Useraccount.read_info();
    }
    else if(choice==9)
    {
        cout<<"Exiting to Main Menu....";
        main();
        return 0;
    }
    else if(choice==10)
    {
        return 0;
    }
    else if(choice>11)
    {
        cout<<"Invalid choise entered!!";
        main();
        return 0;
    }
  } while (choice!=11);
  return 0;
  }
if (ustype=2)
{
int choice2;
  do {
      cout<<"\nMenu: \n1. Check Balance\n2. Transaction History\n3. Close Account\n4. Exit to Main Menu\n5. Exit system\n";
      cout<<"Enter your choice: ";
      cin>>choice2;
      cout<<"Your choice is:  "<<choice2<<endl;
    if(choice2==1)
    {
       cout<<"Enter Account Number to check your balance: ";
        cin>>searchAccNum;
        if (Useraccount.search_user_by_accnumU(searchAccNum)) 
        {
           Useraccount.search_user_by_accnum(searchAccNum);
            cout << "Your current balance is: " << Useraccount.balance << endl;
        }
    } 
    else if(choice2==2)
    {
         cout<<"Enter Account Number for transaction history: "<<endl;
    cin>>searchAccNum;
    if(Useraccount.search_user_by_accnumU(searchAccNum))
    {
        Useraccount.read_transactions(searchAccNum);
    }
    }
    else if(choice2==3)
    {
        cout << "Enter Account Number: ";
        cin >> searchAccNum;
        if (Useraccount.search_user_by_accnumU(searchAccNum)) 
        {
            if (Useraccount.close_Account()) 
            {
                Useraccount.close_AccountT(Useraccount.accnum);
            }
        }
    }
    else if(choice2==4)
    {
        cout<<"Exiting to Main Menu....";
        main();
        return 0;
    }
    else if(choice2==5)
    {
        cout<<"Exiting!!";
        return 0;
    }
    else if(choice2>6)
    {
        cout<<"Invalid choice!!";
        main();
        return 0;
    }
  } while (choice2!=6);
  return 0;
    }
}