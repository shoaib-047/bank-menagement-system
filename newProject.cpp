#include<iostream>
#include<string>
using namespace std;
// customer account dashboard
class CreateAccount{
	int choice;
	static const int maxCusAccount = 50;
    int cusAccountIndex=0;
    static const int phoneLen=12;
    static const int idCardLen=15;
    int accountNumber[maxCusAccount];
    string accHolName[maxCusAccount];
    int cusAge[maxCusAccount];
    string cusPhoneNum[maxCusAccount];
    string cusIdCardNo[maxCusAccount];
    double balance[maxCusAccount];


	void caCreateAccount() {
        int newAccount;
        cout << "===== Creating Account =====\n";
        cout << "How many of customers account you want to create: ";
        cin >> newAccount;

        if (cusAccountIndex + newAccount > maxCusAccount) {
            cout << "Error: Cannot add more than " << maxCusAccount << " customer accounts.\n";
            return;
        }
        
        cin.ignore();
        for (int i = 0; i < newAccount; i++) {
            cout << "\nEnter details for customer account " << cusAccountIndex + 1 << ":\n";
            cout << "Enter customer account Number: ";
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
				 choice=toupper(choice);
        	} while (choice == 'Y' || choice == 'y');
        }
        
	    do {
	        cout << "Enter customer identity card Number with dashes (-): ";
	        cin >> cusIdCardNo[cusAccountIndex];
	
	        bool hasAlphabet = false;
	        for (int i=0;i<cusIdCardNo[cusAccountIndex].length();i++) {
	            if (isalpha(isalpha(cusIdCardNo[cusAccountIndex][i]))){
	                hasAlphabet = true;
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

    void caWithdrawAmount(int index) {
        double amount;
        cout << "Enter amount you want to withdraw: ";
        cin >> amount;
        if (amount <= 0) {
            cout << "Invalid amount. Please enter a positive value." << endl;
        } else if (amount > balance[index]) {
            cout << "Insufficient balance. Transaction failed." << endl;
        } else {
            balance[index] -= amount;
            cout << "Withdrawal of Rs. " << amount << " successful." << endl;
            cout << "Remaining balance: Rs. " << balance[index] << endl;
        }
    }

    void caDispAccountdetail() {
        if (cusAccountIndex == 0) {
            cout << "NO Account available" << endl;
            return;
        }
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
	
	public:
	void xyz(){
		
		int index=0;
		
	 do {
        cout << "\n=== Customer Dashboard ===\n";
        cout<<"Enter choice you want to perform operation between 1-8"<<endl;
        cout << "1. Create new Customer account\n";
        cout << "2. Deposit Customer amount\n";
        cout << "3. Withdraw Customer amount\n";
        cout << "4. Savings max withdrawals\n";
        cout << "5. Current account overdraft withdraw\n";
        cout << "6. Display all account details\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            	case 1: 
            		caCreateAccount();
            		break;
            		case 2:
            		caDepositAmount(index);
            		break;
                    case 3:
                	caWithdrawAmount(index);
                	break;
                	case 4:
                		
                	break;
                	case 5:
                			
                	break;
                	case 6:
                	 caDispAccountdetail();			
                	break;
                	case 7:
                	exit(0);				
                	break;
                				
            	default: cout << "Invalid choice. Try again.\n";
        	}
        	
    	} while (false);
    
	}
	
	
};

int main(){
	int choice;
	CreateAccount obj;
        	cout<<"======WELCOME TO BANK MENAGEMENT SYSTEM====="<<endl;
            cout << "\n====== MAIN MENU ======\n";
            cout<<"Enter choice you want to perform operation between 1-4"<<endl;
            cout << "1. Customer"<<endl;
            cout << "2. Bank"<<endl;
         	cout << "3. Cashier"<<endl;
            cout << "4. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
				 obj.xyz();
				  break;
				   case 2:
				   	
		    	break;
		      	case 3:

		        	break;
                case 4:
				 exit(0);
                default: 
				cout << "Invalid choice. Try again.\n";
            }
    return 0;
}

