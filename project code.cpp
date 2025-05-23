#include<iostream>
#include<string>
using namespace std;

class CustomerAccount {
protected:
    static const int maxCusAccount = 50;
    static int cusAccountIndex;
    const int phoneLen=12;
    const int idCardLen=15;
    int accountNumber[maxCusAccount];
    string accHolName[maxCusAccount];
    int cusAge[maxCusAccount];
    string cusPhoneNum[maxCusAccount];
    string cusIdCardNo[maxCusAccount];
    double balance[maxCusAccount];

public:
    void caCreateAccount() {
        int newAccount,index=0;
        char choice;
        cout << "===== Creating Account =====\n";
        cout << "Enter the number of customers aaccount you want to create: ";
        cin >> newAccount;

        if (cusAccountIndex + newAccount > maxCusAccount) {
            cout << "Error: Cannot add more than " << maxCusAccount << " customer accounts.\n";
            return;
        }
        cin.ignore();
        for (int i = 0; i < newAccount; i++) {
          cout << "\nEnter details for customer account " << i + 1 << ":\n";
          cout << "Enter the account number you want to work with: ";
            cin >> accountNumber[cusAccountIndex];
            cin.ignore();
            cout << "Enter customer account Holder Name: ";
            getline(cin, accHolName[cusAccountIndex]);
            cout<<"Enter customer age:";
            cin>> cusAge[cusAccountIndex];
            do{
            cout<<"Enter customer Phone Number with dash(-): ";
            cin>> cusPhoneNum[cusAccountIndex];
            if(cusPhoneNum[cusAccountIndex].length()==phoneLen){
            	cout<<"Entered phone number is correct"<<endl;
            	break;
			}else if(cusPhoneNum[cusAccountIndex].length()>phoneLen){
				cout<<"your phone number has more digit than required:"<<endl;
			}else{
				cout<<"your phone number has fewer digits"<<endl;
			}
			cout << "Would you like to re-enter the phone number? (Y/y for yes, any other key for no): ";
				 cin >> choice;
        } while (choice == 'Y' || choice == 'y');
        
    do {
        cout << "Enter customer identity card Number with dashes (-): ";
        cin >> cusIdCardNo[cusAccountIndex];

        bool hasAlphabet = false;
        for (int i=0;i<cusIdCardNo[cusAccountIndex].length();i++) {
            if(isalpha(cusIdCardNo[cusAccountIndex][i])) {
                hasAlphabet = true;
                break;
            }
        }
        if (cusIdCardNo[cusAccountIndex].length() == idCardLen && !hasAlphabet) {
            cout << "Entered ID card number is correct.\n";
            break;
        } else if (cusIdCardNo[cusAccountIndex].length() > idCardLen) {
            cout << "Your ID card has more digits than required.\n";
        } else if (hasAlphabet) {
            cout << "Entered ID card number contains invalid letters.\n";
        } else {
            cout << "Your ID card number has fewer digits.\n";
        }

        cout << "Would you like to re-enter the ID card number? (Y/y for yes, any other key for no): ";
        cin >> choice;
    } while (choice == 'Y' || choice == 'y');
	   cout << "Enter customer account initial balance: ";
            cin >> balance[cusAccountIndex];
            cusAccountIndex++;
        }
    }

    void caDepositAmount(int index) {
        double deposit;
        cout << "Enter money you want to deposit: ";
        cin >> deposit;
        if (deposit > 0) {
            balance[index] += deposit;
            cout << "Deposited amount: " << deposit << endl;
        } else {
            cout << "Deposit amount must be positive!" << endl;
        }
    }

    

    void viewTransactionDetail() {
        cout << "========== Displaying account information =========\n";
        for (int i = 0; i < cusAccountIndex; i++) {
            cout << "Customer account: " << i + 1 << endl;
            cout << "Customer Account Number: " << accountNumber[i] << endl;
            cout << "Customer Account Holder Name: " << accHolName[i] << endl;
            cout << "customer Age: "<<cusAge[i]<<endl;
            cout << "Customer Phone number: "<< cusPhoneNum[i]<<endl;
            cout << "Customer identity card number: "<<cusIdCardNo[i]<<endl;
            cout << "Customer Current balance: " << balance[i] << endl;
        }
    }

    double getBalance(int index) {
        return balance[index];
    }
 
    void setBalance(int index, double bal) {
        balance[index] = bal;
    }
};

int CustomerAccount::cusAccountIndex = 0;

class SavingAccount : public CustomerAccount {
protected:
    int withdrawalCount = 0;
    int maxWithdrawals = 3;
 
public:
    void saMaxWithdrawal(int index) {
        double amount;
        if (withdrawalCount >= maxWithdrawals) {
            cout << "Sorry, withdrawal limit reached for this month.\n";
            return;
        }

        cout << "Enter amount you want to withdraw: ";
        cin >> amount;
        if (amount <= getBalance(index)) {
            setBalance(index, getBalance(index) - amount);
            withdrawalCount++;
            cout << "Withdrawn: Rs. " << amount << endl;
            cout << "New Balance: Rs. " << getBalance(index) << endl;
        } else {
            cout << "Insufficient balance!\n";
        }
    }
};

class CurrentAccount : public CustomerAccount {
protected:
    double overDraftLimit = 5000;

public:
    void crWithdrawLimit(int index) {
        double amount;
        cout << "Enter amount to withdraw: ";
        cin >> amount;
        if (amount <= (getBalance(index) + overDraftLimit)) {
            setBalance(index, getBalance(index) - amount);
            cout << "Withdrawn: Rs. " << amount << endl;
            cout << "New Balance: Rs. " << getBalance(index) << endl;
        } else {
            cout << "Overdraft Limit exceeded!" << endl;
        }
    }
    
	bool isValidPassword(const string& password) {
    int digits = 0, letters = 0, special = 0;
    for (int i = 0; i < password.length(); i++) {
    	char ch = password[i];
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
            letters++;
        } else if (ch >= '0' && ch <= '9') {
            digits++;
        } else {
            special++;
        }
    }
	 return (letters >= 3 && digits >= 3 && special >= 1);
}
void AccountDashboard() {
    SavingAccount sa;
    CurrentAccount ca;
    int choice, index = 0, attempts = 0;
    string lName, lpassWord;
    bool running=true;
    cout<<"----Sign in------"<<endl;
    cin.ignore();
    cout << "To sign in first please enter your login name: "<<endl;
   getline(cin,lName);
  do {
    cout << "Enter Your password: "<<endl;
    cin >> lpassWord;
    if (!isValidPassword(lpassWord)) {
        cout << "Your password is not strong enough. Please try again.\n";
        attempts++;
    }
   } while (!isValidPassword(lpassWord) && attempts < 3);
     if (attempts == 3) {
    cout << "Too many invalid attempts. Exiting...\n";
    return;
}else{

    cout << "Login successful: " << lName << endl;
}
    do {
        cout << "\n=== Customer Dashboard ===\n";
        cout<<"Enter choice you want to perform operation between 1-8"<<endl;
        cout << "1. Create new Customer account\n";
        cout << "2. Deposit Customer amount\n";
        cout << "3. Withdraw Customer amount\n";
        cout << "4. Current account overdraft withdraw\n";
        cout << "5. View transaction detail\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: 
			sa.caCreateAccount();
			 break;
            case 2:
			 sa.caDepositAmount(index);
			 break;
            case 3: 
			sa.saMaxWithdrawal(index);
			 break;
            case 4: 
			ca.crWithdrawLimit(index); 
			break;
            case 5:
			 sa.viewTransactionDetail(); 
			break;
            case 6:
            	running =false;
            	break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (running);
}

};
class Cashier : public SavingAccount {
protected:
    const int cashIdLen = 15;
    string cashName;
    string cashAddress;
    string cashIdCardNo;
    int transactionId = 1000;
    float cashSalary;
    char choice;
    string transactionDate;
    string trasactionType;
    double trasactionAmount;

public:
    int accNum;
    CustomerAccount ca[100]; // Assuming size
    SavingAccount sa[100];   // Assuming size

    void InputCashDetail() {
        cin.ignore();
        cout << "Enter cashier Name: ";
        getline(cin, cashName);

        do {
            cout << "Enter cashier identity card Number with dashes (-): ";
            cin >> cashIdCardNo;
            bool hasAlphabet = false;
            for (int i = 0; i < cashIdCardNo.length(); i++) {
                if (isalpha(cashIdCardNo[i])) {
                    hasAlphabet = true;
                    break;
                }
            }
            if (cashIdCardNo.length() == cashIdLen && !hasAlphabet) {
                cout << "Entered ID card number is correct.\n";
                break;
            } else if (cashIdCardNo.length() > cashIdLen) {
                cout << "Your ID card has more digits than required.\n";
            } else if (hasAlphabet) {
                cout << "Entered ID card number contains invalid letters.\n";
            } else {
                cout << "Your ID card number has fewer digits.\n";
            }
            cout << "Would you like to re-enter the ID card number? (Y/y for yes, any other key for no): ";
            cin >> choice;
        } while (choice == 'Y' || choice == 'y');

        cin.ignore();
        cout << "Enter the cashier address: ";
        getline(cin, cashAddress);
        cout << "Enter cashier Salary: ";
        cin >> cashSalary;
    }

    void disPlayCashDet() {
        cout << "===== Displaying Cashier Detail =====" << endl;
        cout << "Name: " << cashName << endl;
        cout << "Identity Card Number: " << cashIdCardNo << endl;
        cout << "Address: " << cashAddress << endl;
        cout << "Salary: " << cashSalary << endl;
    }

    void cashDepositAmount() {
        cout << "===== Deposit to Customer Account =====" << endl;
        cout << "Enter account number: ";
        cin >> accNum;
        sa[accNum].caDepositAmount(accNum);
        cout << "Amount deposited to account number " << accNum << " successfully." << endl;
    }

    void cashWithdrawAmount() {
        cout << "===== Withdraw from Customer Account =====" << endl;
        cout << "Enter account number: ";
        cin >> accNum;
        sa[accNum].saMaxWithdrawal(accNum);
        cout << "Amount withdrawn from account number " << accNum << " successfully." << endl;
    }

    void transactionMoney() {
        int fromAcc, toAcc;
        double amount;
        cout << "===== Money Transaction =====" << endl;
        cout << "Enter sender account number: ";
        cin >> fromAcc;
        cout << "Enter receiver account number: ";
        cin >> toAcc;
        cout<<"Enter transaction Date: ";
        cin>> transactionDate;
        cout<<"Enter transaction Type: ";
        cin>> trasactionType;
        cout << "Enter amount to transfer: ";
        cin >> amount;

        if (sa[fromAcc].getBalance(fromAcc) >= amount) {
            sa[fromAcc].setBalance(fromAcc, sa[fromAcc].getBalance(fromAcc) - amount);
            sa[toAcc].setBalance(toAcc, sa[toAcc].getBalance(toAcc) + amount);

            transactionId++;
            transactionDate = __DATE__;
            trasactionType = "Transfer";
            trasactionAmount = amount;

            cout << "Transaction successful: Rs. " << amount << " transferred from Account " << fromAcc << " to Account " << toAcc << ".\n";
        } else {
            cout << "Insufficient balance in sender's account.\n";
        }
    }

    void genTransactionReport() {
        cout << "\n===== Transaction Report =====\n";
        cout << "Transaction ID: " << transactionId << endl;
        cout << "Transaction Type: " << trasactionType << endl;
        cout << "Transaction Amount: Rs. " << trasactionAmount << endl;
        cout << "Transaction Date: " << transactionDate << endl;
    }

    void CashierDetail() {
        int choice;
        bool running = true;
        while (running) {
            cout << "\n========== Cashier Dashboard ==========" << endl;
            cout << "1. Input Cashier Detail" << endl;
            cout << "2. Display Cashier Detail" << endl;
            cout << "3. Customer Deposit Amount" << endl;
            cout << "4. Customer Withdraw Amount" << endl;
            cout << "5. Transaction Money" << endl;
            cout << "6. Generate Transaction Report" << endl;
            cout << "7. Exit" << endl;
            cout << "Enter choice: ";
            cin >> choice;
 
            switch (choice) {
                case 1: 
			    InputCashDetail(); 
				break;
                case 2: 
				disPlayCashDet();
				 break;
                case 3: 
				cashDepositAmount(); 
				break;
                case 4: 
				cashWithdrawAmount(); 
				break;
                case 5: 
				transactionMoney(); 
				break;
                case 6: 
				genTransactionReport(); 
				break;
                case 7: 
				running = false;
				 break;
                default:
				 cout << "Invalid choice. Try again.\n";
				 break;
            }
        }
    }
};
class BankManager : public Cashier, public CustomerAccount {
protected:
    const int managIdLen = 15;
    string managName;
    string managAddress;
    string managIdCardNo;
    float managSalary;
    char choice;
    int accNum;

public:
    SavingAccount sa[100]; 

    void InputManagDetail() {
        cin.ignore();
        cout << "Enter Bank Manager Name: ";
        getline(cin, managName);

        do {
            cout << "Enter bank manager identity card Number with dashes (-): ";
            cin >> managIdCardNo;
            bool hasAlphabet = false;
            for (int i=0;i < managIdCardNo.length(); i++) {
                if (isalpha(managIdCardNo[i]))  {
                    hasAlphabet = true;
                    break;
                }
            }
            if (managIdCardNo.length() == managIdLen && !hasAlphabet) {
                cout << "Entered ID card number is correct.\n";
                break;
            } else if (managIdCardNo.length() > managIdLen) {
                cout << "Your ID card has more digits than required.\n";
            } else if (hasAlphabet) {
                cout << "Entered ID card number contains invalid letters.\n";
            } else {
                cout << "Your ID card number has fewer digits.\n";
            }
            cout << "Would you like to re-enter the ID card number? (Y/y for yes, any other key for no): ";
            cin >> choice;
        } while (choice == 'Y' || choice == 'y');

        cin.ignore();
        cout << "Enter the bank manager address: ";
        getline(cin, managAddress);
        cout << "Enter bank manager Salary: ";
        cin >> managSalary;
    }
    void managDepositAmount() {
        cout << "Enter account number to deposit: ";
        cin >> accNum;
        sa[accNum].caDepositAmount(accNum);
    }

    void managWithdrawAmount() {
        cout << "Enter account number to withdraw: ";
        cin >> accNum;
        sa[accNum].saMaxWithdrawal(accNum);
    }
    void disPlayManagDet() {
        cout << "\n===== Displaying Bank Manager Details =====" << endl;
        cout << "Name: " << managName << endl;
        cout << "Identity Card Number: " << managIdCardNo << endl;
        cout << "Address: " << managAddress << endl;
        cout << "Salary: " << managSalary << endl;
    }

    void bankManagDashboard() {
    	 CustomerAccount ca1;
    	 Cashier cash;
        int choice;
        bool running = true;

        while (running) {
            cout << "\n========== Bank Manager Dashboard ==========" << endl;
            cout << "1. Create Manager Information\n";
            cout << "2. Display Manager Information\n";
            cout << "3. Deposit Amount\n";
            cout << "4. Withdraw Amount\n";
            cout << "5. Create Customer Account\n";
            cout << "6. View All Customer Transaction Details\n";
            cout << "7. Input Cashier Information\n";
            cout << "8. Perform Cashier Transaction\n";
            cout << "9. Generate Cashier Report\n";
            cout << "10. Display Cashier Details\n";
            cout << "11. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    InputManagDetail();
                    break;
                case 2:
                    disPlayManagDet();
                    break;
                case 3:
                    managDepositAmount();
                    break;
                case 4:
                    managWithdrawAmount();
                    break;
                case 5:
                 ca1.caCreateAccount();
                    break;
                case 6:
                    cash.viewTransactionDetail();
                    break;
                case 7:
                    InputCashDetail();
                    break;
                case 8:
                    transactionMoney();
                    break;
                case 9:
                    genTransactionReport();
                    break;
                case 10:
                    disPlayCashDet();
                    break;
                case 11:
                    running = false;
                    break;
                default:
                    cout << "Invalid choice. Try again.\n";
                    break;
            }
        }
    }
};

int main() {
	CurrentAccount ca1;
	Cashier cash1;
	BankManager BM;
        int choice;
        do {
        	cout<<"----------------------------------------------"<<endl;
           cout<<"|    WELCOME TO BANK MENAGEMENT SYSTEM        | "<<endl;
            cout<<"-----------------------------------------------"<<endl;
       
            cout << "\n====== MAIN MENU ======\n";
            cout<<"Enter choice you want to perform operation between 1-4"<<endl;
            cout << "1. Customer Account Dashboard\n";
            cout << "2. Cashier Dashboard"<<endl;
         	cout << "3. Bank Dashboard"<<endl;
            cout << "4. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
				 ca1.AccountDashboard();
				  break;
				   case 2:
				   	cash1.CashierDetail();
		    	break;
		      	case 3:
                    BM.bankManagDashboard();
		        	break;
                case 4:
				 return 0;
                default: 
				cout << "Invalid choice. Try again.\n";
            }
        } while (true);
    return 0;
}
