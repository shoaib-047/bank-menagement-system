#include<iostream>
#include<string>
using namespace std;
void caCreateAccount() {
    int newAccount;
    char choice;
    cout << "===== Creating Account =====\n";
    cout << "Enter the number of customer accounts you want to create: ";
    cin >> newAccount;

    if (cusAccountIndex + newAccount > maxCusAccount) {
        cout << "Error: Cannot add more than " << maxCusAccount << " customer accounts.\n";
        return;
    }
    cin.ignore();

    for (int i = 0; i < newAccount; i++) {
        cout << "\nEnter details for customer account " << i + 1 << ":\n";
        
        // ? FIX: Use cusAccountIndex instead of maxCusAccount
        cout << "Enter the account number you want to work with: ";
        cin >> accountNumber[cusAccountIndex];
        cin.ignore();

        cout << "Enter customer account Holder Name: ";
        getline(cin, accHolName[cusAccountIndex]);  // ?? Use getline for names with spaces

        cout << "Enter customer age: ";
        cin >> cusAge[cusAccountIndex];

        // Validate phone number
        do {
            cout << "Enter customer Phone Number with dash(-): ";
            cin >> cusPhoneNum[cusAccountIndex];
            if (cusPhoneNum[cusAccountIndex].length() == phoneLen) {
                cout << "Entered phone number is correct.\n";
                break;
            } else if (cusPhoneNum[cusAccountIndex].length() > phoneLen) {
                cout << "Your phone number has more digits than required.\n";
            } else {
                cout << "Your phone number has fewer digits.\n";
            }
            cout << "Would you like to re-enter the phone number? (Y/y for yes, any other key for no): ";
            cin >> choice;
        } while (choice == 'Y' || choice == 'y');

        // Validate ID card
        do {
            cout << "Enter customer identity card Number with dashes (-): ";
            cin >> cusIdCardNo[cusAccountIndex];

            bool hasAlphabet = false;
            for (char c : cusIdCardNo[cusAccountIndex]) {
                if (isalpha(c)) {
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

        // ?? Enter balance after all validations
        cout << "Enter customer account initial balance: ";
        cin >> balance[cusAccountIndex];

        cusAccountIndex++;  // Move to next customer
    }
}

