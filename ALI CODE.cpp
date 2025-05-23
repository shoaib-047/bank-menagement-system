#include<iostream>
#include<ctime>
using namespace std;
const int customerMax=150;

void printTime();
void printDate();
bool checkAlphabet(string str);
bool checkDigit(string no);
bool checkCharacter(string specialCharacter);
bool checkName(string name);

class Passwords {
	string str;
	string no;
	string specialCharacter;
public:
	Passwords(): str(""),no(""),specialCharacter("") {};
	Passwords(string str,string no,string specialCharacter){
		this->str=str;
		this->no=no;
		this->specialCharacter=specialCharacter;
	}
	void setPassword() {
		cout <<"Instructions : \n1. It must contain at least  4 string letters, atleast 2 numbers and atleast one special character .\n2. It should be atleast 7 digits\n";
			bool chkNameAlphabet ,chkNoDigit,chkSpecialCharacter;
			cout <<"Enter Password : \n";
		do {
			cout <<"Enter String letters (at least 4) : ";
			cin>>str;
			chkNameAlphabet=checkAlphabet(str);
			if(str.length()<4){
				cout <<"Please Enter At least 4 letters !\n";
			}
		}while(!chkNameAlphabet || str.length()<4);
		do{
			cout <<"Enter Number Elements (at least 2) : ";
			cin>>no;
			chkNoDigit=checkDigit(no);
			if(no.length()<2){
				cout <<"Please Enter Atleast Two Numbers !\n";
			}
		} while (! chkNoDigit || no.length()<2);
		do{
			cout <<"Enter Special Character (atleast 1): ";
			cin>>specialCharacter;
			chkSpecialCharacter=checkCharacter(specialCharacter);
			if(specialCharacter.length()<1){
				cout <<"Please Enter Atleast 1 special Character!\n";
			}
		}while(!chkSpecialCharacter || specialCharacter.length()<1);
			
	}
	string returnPassword(){
		string retPassword = str+no+specialCharacter;
		return retPassword;
	}
	bool verifyPassword(string enteredPassword){
		if(returnPassword()!=enteredPassword){
			return false;
		}
		return true;
	}
};
class userName{
	string usrName;
	public:
		userName():usrName("N/A"){};
		userName(string usrName) {
			this->usrName=usrName;
		}
		void inputUserName(){
			cout <<"Enter Username : ";
			cin>>usrName;
		}
		string returnUsername(){
		return usrName;
		}
		bool verifyUserName(string enteredUserName){
			if(enteredUserName!=usrName){
				return false;
			}
			return true;
		}
};
void displayMainMenu_01() {
	cout <<"\n========== Islamic Bank Management System ========== \n";
	cout <<"Main Menu : \n";
	cout <<"1. Customer Sign In\n";
	cout <<"2. Administration Sign In\n";
	cout <<"3. Cashier Sign In\n";
	cout <<"4. Bank Manager\n";
	cout <<"Enter Option : ";
}
void administrationSignIn(userName AdminsU[2],Passwords AdminsP[2],int max){
	cout <<"\n=== Sign In === \n";
	bool adminVerification;
	do{
		cout <<"Please Enter Username : ";
		string username;
		for(int i=0;i<max;i++){
			if(!AdminsU[i].verifyUserName(username)){
				adminVerification=false;
				if(i==max-1){
					cout<<"Incorrect Username!\n";
				}
			}
		}
	}while(!adminVerification);
	do{
		cout<<"Please Enter Password : ";
		string password;
		cin>>password;
		for(int i=0;i<max;i++){
			if(!AdminsP[i].verifyPassword(password)){
				adminVerification=false;
				if(i==max-1) cout<<"Incorrect Password !\n";
			}
		}
	}while(!adminVerification);
}
void cashierSignIn(){
	const int cashierMax=4;
	int recordCountCashier = 0;
	bool cashierVerification=false;
	userName cashierUserName[cashierMax];
	Passwords cashierPasswords[cashierMax];
	cout<<"\n=== Sign In === \n";
	do{
		string username;
		cout <<"Enter Username : ";
		cin>>username;
		for(int i=0;i<recordCountCashier;i++){
			if(!cashierUserName[i].verifyUserName(username)){
				cashierVerification=true;
				if(i==recordCountCashier-1){
					cout<<"Incorrect Username!\n";
				}
			}
		}
	}while(!cashierVerification);
	do{
		string password;
		cin>>password;
		for(int i=0;i<recordCountCashier;i++){
			if(cashierUserName[i].verifyUserName(password)){
				cashierVerification=false;
				if(i==recordCountCashier-1){
					cout<<"Incorrect Password!\n";
					cashierVerification=false;
				}
			}
		}
	}while(!cashierVerification);
}
bool checkAlphabet(string str){
    for(char ch:str){
        if(!isalpha(ch)){
        	cout <<"Incorrect String Entered!\n";
            return false;
        }
    }
    return true;
}
bool checkDigit(string str){
	for(char ch:str){
		if(!isdigit(ch)){
			cout <<"The Number Entered Contains letters or specal character !\n";
			return false;
		}
	}
	return true;
}
bool checkCharacter(string str){
	for(char ch:str){
		if(isdigit(ch)||isalpha(ch)){
			cout <<"The Entered string contains alphabets or digits!\n";
			return false;
		}
	}
	return true;
}
bool checkName(string name){
	for(char ch:name){
		if(!isalpha(ch) && !isspace(ch)){
			return false;
		}
	}
	return true;
}
void printTime(){
	time_t currentTime=time(0);
	tm* localTime=localtime(&currentTime);
	cout<<"Time : "<<localTime->tm_hour<<" : "<<localTime->tm_min<<" : "<<localTime->tm_sec<<endl;
}
void printDate(){
	time_t currentTime=time(0);
	tm* localTime=localtime(&currentTime);
	cout<<"Date : "<<localTime->tm_mday<<" - "<<1+localTime->tm_mon<<" - "<<1900+localTime->tm_year<<endl;
}
class Customer{
	int id ;
	string name ;
	int accountBalance;
	string CNIC;
	string address;
	string phoneNumber;
	string email;
	public:
		Customer():id(0),name(""),accountBalance(0),CNIC(""),address(""){};
		void inputCustomerDetails(){
			bool customerDetails;
			customerDetails=true;
			do{
			cout<<"Please Enter Your Full Name : ";
			cin.ignore();
			getline(cin,name);
			if(!checkName(name)){
				customerDetails=false;
			}else{
				customerDetails=true;
			}
		}while(!customerDetails);
			do{
			cout <<"Please Enter Your CNIC (Without Dashes & Space): ";
			cin>>CNIC;
			if(checkDigit(CNIC)){
				customerDetails=true;
			}else{
				cout <<"Incorrect CNIC entered !\n";
				customerDetails=false;
			}
			if(CNIC.length()<13||CNIC.length()>13){
				cout<<"Entered CNIC is not 13 digits !\n";
				customerDetails=false;
			}
			
		}while(!customerDetails);
		cout <<"PLease Enter Your Address : ";
		cin.ignore();
		getline (cin,address);
		do{
		cout <<"Enter Your Phone Number (without Dashes): ";
		cin>>phoneNumber;
		if(!checkDigit(phoneNumber)){
			cout <<"Incorrect Phone Number Entered !\n";
			customerDetails=false;
		}else{
			customerDetails=true;
		}
		if(phoneNumber.length()!=11){
			cout <<"The Entered Number is not 11 digit!\n";
			customerDetails=false;
		}
    	}while(!customerDetails);
    	cin.ignore();
    	cout<<"Please Enter Your Email : ";
    	getline(cin,email);
			
			
		}
		void showCustomerDetails(){
			cout <<"Showing Customer Details : \n";
			cout <<"Name : "<<name<<endl;
			cout <<"CNIC : " ;
			for(int i=0;i<5;i++){
				cout <<CNIC[i];
			} 
			cout <<"-";
			for(int i=5;i<12;i++){
				cout <<CNIC[i];
			}
			cout <<"-"<<CNIC[12];
			cout <<"\nAddress : "<<address<<endl;
			cout<<"Phone Number : ";
			for(int i=0;i<4;i++){
				cout <<phoneNumber[i];
			}
			cout <<"-";
			for(int i=4;i<12;i++){
				cout <<phoneNumber[i];
			}
			cout <<"\n";
			cout<<"Email : "<<email<<endl;
			cout <<"Available Balance : "<<accountBalance<<" Rs"<<endl;
		}
		void DepositBalance(){
			float balance;
			bool balanceCheck;
			do{
				cout <<"Please Enter the Balance : ";
				cin>>balance;
				if(balance<0){
					cout<<"The Balance Cannot be negative !\n";
					balanceCheck=false;
				}else if(balance>500000){
					cout<<"The Maximum To deposit is 500000\n";
					balanceCheck=false;
				}else{
					balanceCheck=true;
				}
			}while(!balanceCheck);
			accountBalance+=balance;
			cout <<balance<<" Rs Balance Deposited Successfully\n";
		}
		void WithdrawBalance (){
			float balance;
			bool balanceCheck;
			do{
				cout<<"Please Enter the Balance to Withdrawl : ";
				cin>>balance;
				if(balance<0){
					cout<<"Balance Cannot be negative!\n";
					balanceCheck=false;
				}else if(balance>accountBalance){
					cout <<"Not enough Balance to Withdraw !\n";
					balanceCheck=false;
				}else{
					balanceCheck=true;
				}
			}while(!balanceCheck);
			accountBalance-=balance;
			cout<<balance<<" Rs Withdrawed Successfully\n";
		}
		void updateDetails(){
			cout <<"Current Details : \n";
			showCustomerDetails();
			inputCustomerDetails();
		}
		void sendBalance(){
			cout<<"Enter Id to Send Balance : ";    
			
		}
};
class Administration {
	string name;
	int empId;
	float salary;
	public:
		Administration () {};
		Administration(string name,float salary,int empId) {
			this->name=name;
			this->salary=salary;
			this->empId=empId;
		} 
		void setAdminDetails (int id) {
			empId=id;
			cout <<"Id : "<<empId<<endl;
			cin.ignore();
			cout<<"Name : ";
			getline(cin,name);
			cout<<"Salary : ";
			cin>>salary;
		}
		void displayCustomers (Customer c[],int recordCount){
			cout<<"\n===== Displaying Customers =====\n";
			for(int i=0;i<recordCount;i++){
				c[i].showCustomerDetails();
				cout<<"\n=========================\n";
			}
		}
		void displayEmployees(){
			cout<<"\n=== Displaying Employees === \n";
		}
		void displayDetails(){
			cout<<"Name : "<<name<<endl;
			cout<<"Employ Id : "<<empId<<endl;
			cout<<"Salary : Rs "<<salary<<endl;
		}
};
void displayAdminFunctions(){
	cout<<"\n1.Display All Customers\n";
	cout<<"2.Display Details\n";
	cout<<"3.Set Details\n";
	cout<<"4.Update Details\n";
}
class Cashier {
	int empId;
	string phoneNumber;
	string address;
	string name;
	float salary;
	public:
		Cashier():empId(0),name("N/A"),salary(0.0){};
		Cashier(int empId,string name,float salary){
			this->name=name;
			this->salary=salary;
			this->empId=empId;
		}
		void setCashierDetails(){
			bool cashier;
			cin.ignore();
			cout<<"Enter Name : ";
			getline(cin,name);
			bool sal;
			do{
			cout<<"Salary : Rs ";
			cin>>salary;
			if(salary<0 || salary >500000){
				sal=false;
				cout<<"Invalid Salary Input ! Cannot be less than 0 or greater than 5 lac ! \n";
			}
		}while(!sal);
			do{
			cout<<"Enter Phone Number : ";
			cin>>phoneNumber;
			if(!checkDigit(phoneNumber)) {
				cout<<"Icorrect Phone Number Entered !\n";
				cashier=false;
			}else if (phoneNumber.length()!=11){
				cout<<"Enter Number Is not 11 digit ! \n";
				cashier=false;
			}else {
				cashier=true;
			}
		}while (!cashier);
		
		    cin.ignore();
			cout<<"Enter Address : ";
			getline(cin,address);
		}
		void displayCashierDetails () {
			cout<<"\n=== Display ===\n";
			cout <<"Name : "<<name<<endl;
			cout<<"Phone Number : ";
			for(int i=0;i<11;i++ ){
				cout<<phoneNumber[i];
				if(i==3) cout<<"-";
			}
			cout<<"\nAddress : "<<address<<endl;
			cout<<"Salary : Rs "<<salary<<endl;
			cout<<"-----------------------\n";
		}
		void depositCustomerBalance (Customer c[],int recordCount){
			int id;
			cout<<"\n=== Deposit Customer ===\n";
			cout<<"Enter Id to Deposit : ";
			cin>>id;
			c[id].DepositBalance();
			cout<<"Balance Deposited Successfully!\n";
		}
		void withdrawlCustomerBalance (Customer c[],int recordCount){
			int id;
			cout<<"\n=== Withdrawl Customer === \n";
			cout<<"Enter Id to Withdrawl : ";
			c[id].WithdrawBalance();
		}
	
	
};
class Manager {
	string name;
	int empIdM=1;
	float salary;
	string CNIC ;
	string phoneNumber;
	string address;
	public:
		Manager() : name ("N/A"), empIdM(1),salary(0.0),CNIC("N/A"),phoneNumber ("N/A"), address ("N/A") {};
		void setManagerDetails(){
			cout<<"Employ Id : "<<empIdM<<endl;
			cin.ignore();
			cout<<"Name : ";
			getline(cin,name);
			bool sal,cnic,pno;
			do{
				sal=true;
			cout<<"Salary : ";
			cin>>salary;
			if(salary<0||salary > 500000){
				cout<<"Salary cannot Be negative or above 500000 !\n";
				sal=false;
			}
		}while(!sal);
		do{
			cnic=true;
		cout<<"CNIC : ";
		cin>>CNIC;
		if(checkAlphabet(CNIC) || CNIC.length()<13){
			if(checkAlphabet(CNIC)) cout<<"CNIC cant contain Alphabets !\n";
			if(CNIC.length()<13) cout<<"CNIC is not 13 Digits!\n";
			cnic=false;
		}
	}while(!cnic);
	do{
		pno=true;
		cout<<"Enter Phone Number : ";
		cin>>phoneNumber;
		if(checkDigit(phoneNumber)||checkCharacter(phoneNumber)){
			cout<<"Entered Number Contain Letters or Special Characters !\n";
			pno=false;
		}
		if(phoneNumber.length()<11){
			cout<<"Entered Number is not 11 digits!\n";
			pno=false;
		}
	}while(!pno);
	cin.ignore();
	cout<<"Address : ";
	getline(cin,address);
}

void displayManagerDetails () {
	cout<<"Employ Id : "<<empIdM<<endl;
	cout<<"Name : "<<name<<endl;
	cout<<"CNIC : "<<CNIC<<endl;
	cout<<"Phone Number : ";
	for(int i=0;i<11;i++){
		cout<<phoneNumber[i];
		if(i==3) cout<<"-";
	}
	cout<<"Address : "<<address <<endl;
	cout<<"Salary : "<<salary <<endl;
}
void displayCustomerDetails (Customer c[],int recordCount){
	cout<<"=== Displaying All Customers === \n";
	for(int i=0;i<recordCount;i++){
		c[i].showCustomerDetails();
		cout<<"\n=========================\n";
	}
}
void displayEmployees(Administration a[],Cashier c[]){
	cout<<"=== Select One ===\n";
	cout<<"1. Check Numbers ";
	cout<<"\n2. Check All Details\n";
	int s;	
			int length=sizeof(a[2])/sizeof(a[0]);
			int clength=sizeof(c[2])/sizeof(c[0]);


	cout<<"Your Choice : ";
	cin>>s;
	switch(s){
		case 1:{
			cout<<"=== Numbers ===\n";
			int admins=0;
			for(int i=0;i<2;i++){
				admins+=1;
			}
			int cashiers=0;
			for(int i=0;i<3;i++){
				cashiers+=1;
			}
			cout<<"Admins : "<<admins<<endl;
			cout<<"Cashiers : "<<cashiers<<endl;
			break;
		}
		case 2 :{
			cout<<"\n=== Displaying All Employess Details === \n ";
			cout<<"--- Admins ---\n";
			for(int i=0;i<length;i++){
				a[i].displayDetails();
				cout<<"-------------------\n";
			}
			cout<<"===============\n";
			cout<<"--- Cashiers ---\n";
			for(int i=0;i<clength;i++){
				c[i].displayCashierDetails();
				cout<<"-------------------\n";
			}
			cout<<"==================================\n";
			break;
		}
	}
	
}
};
int main() {
	int id;
	int recordCountCustomer=0;
	Customer customers[customerMax];
	userName customersU[customerMax];
	Passwords customersP[customerMax];
	Administration admins[2];
	userName administrationU[2];
	Passwords administrationP[2];
	Cashier cashiers[3];
	userName cashierU [3];
	Passwords cashierP [3];
	Manager manager;
	userName managerU("Ali");
	Passwords managerP("aaaa","11","!");
	char mainMenudo;
	do {
		displayMainMenu_01();
		int mainMenu;
		cin>>mainMenu;
		switch(mainMenu) {
		case 1:{ 
	int customerSwitch;
	cout <<"===== Customer Sign In ===== \n";
	cout <<"1. Sign In \n";
	cout <<"2. Sign Up \n";
	cout <<"Select one : ";
	cin>>customerSwitch;
	string userNme;
	switch(customerSwitch){
		case 1 :{
			cout <<"\n===== Customer SignIn Section ===== \n";
			if(recordCountCustomer==0){
		    cout <<"No Customers to Sign In, Please Sign up 1st\n";
			break;
		}
			bool customerUserVerification;
			do{
			cout <<"Enter Username : ";
			cin>>userNme;
			for(int i=0;i<recordCountCustomer;i++){
				if (!customersU[i].verifyUserName(userNme)){
					cout<<"\n=== "<<i <<" ===\n";
					customerUserVerification=false;
					if(i==recordCountCustomer-1) {
					cout<<"Not Found !\n";
					}
				}else{
					//cout<<"Else  : ";
					cout<<"Found!\n";
					customerUserVerification=true;
					break;
				}
			}
		}while (!customerUserVerification); 
		   string customerPassword;
			bool customerPasswordVerification;
		do{
			cout <<"Enter Password : ";
			cin>>customerPassword;
			for(int i=0;i<recordCountCustomer;i++){
				if(!customersP[i].verifyPassword(customerPassword)){
					cout <<"Incorrect Password Enter entered!\n";
					customerPasswordVerification=false;
				}else{
					customerPasswordVerification = true;
					id=i;
					break;
				}
			}
		}while(!customerPasswordVerification);
		
			break;
		}
		
		case 2:{
			cout <<"\n=== Sign Up === \n";
			customers[recordCountCustomer].inputCustomerDetails();
		    customersU[recordCountCustomer].inputUserName();
	        customersP[recordCountCustomer].setPassword();
	        id=recordCountCustomer;
			recordCountCustomer++;
			
			
			
			break;
		}
		default :{
			cout<<"Incorrect Number Entered!\n";
			break;
	}
}
char customerChoice ;
do{
	cout<<"\n========== Customer Section =========\n";
	int a;
	cout<<"\n1. View Details \n";
	cout<<"2. Update Details.\n";
	cout<<"3. Withdraw Balance\n";
	cout<<"4. Deposit Balance\n";
	cout<<"5. View Transaction History\n";
	cout<<"Select Operation You Want to Perform : ";
	cin>>a;
	switch(a){
		case 1:{
			cout<<"\n=== View Details === \n";
			customers[id].showCustomerDetails();
			break;
		}
		case 2:{
			cout<<"==== Update Details ===\n";
			customers[id].inputCustomerDetails();
			break;
		}
		case 3:{
			cout<<"=== Withdraw Balance ===\n";
			customers[id].WithdrawBalance();
			break;
		}
		case 4:{
			cout<<"=== Deposit Balance ===\n";
			customers[id].DepositBalance();
			break;
		}
		case 5:{
			cout<<"=== View Transaction History === \n";
			cout<<"Note : This will be  available soon!\n";
			break;
		}
		default:{
			break;
		}
	}
	cout<<"Another Operation from Customer Section ( Y / N ) : ";
	cin>>customerChoice;
}while(customerChoice=='Y',customerChoice=='y');
			break;
		
	}
		case 2:{
			int idAdmin=0;
			administrationU[0]= userName("Ali");
			administrationP[0]=Passwords("aaaa","11","!");
			administrationU[1]=userName("Ahmed");
			administrationP[1]=Passwords("bbbb","22","@");
				cout <<"\n=== Sign In === \n";
	bool adminVerification=true;
	do{
		cout <<"Please Enter Username : ";
		string username;
		cin>>username;
		for(int i=0;i<2;i++){
			if(administrationU[i].verifyUserName(username)){
				adminVerification=true;
				idAdmin=i;
				break;
			}else{
				adminVerification=false;
				if(i==1) cout<<"Not Found !\n";
			}
		}
	}while(!adminVerification);
	do{
		cout<<"Please Enter Password : ";
		string password;
		cin>>password;
		for(int i=0;i<2;i++){
			if(administrationP[i].verifyPassword(password)){
				adminVerification=true;
				break;
			}else{
				adminVerification=false;
				if(i==1) cout<<"Incorrect Password \n";
			}
		}
	}while(!adminVerification);
	char adminChoice;   
	   do{
		    displayAdminFunctions();
	        int adminFunctionsSwitch;
	        cout <<"Your Choice : ";
	        cin>>adminFunctionsSwitch;
	
	
	        switch(adminFunctionsSwitch){
	        	case 1 :{
	        		cout<<"=== Displaying All Customers ===\n";
	        		admins[idAdmin].displayCustomers(customers,recordCountCustomer);
					break;
				}
				case 2 :{
					cout<<"=== Displaying Your Details === \n";
					admins[idAdmin].displayDetails();
					break;
				}
				case 3 : {
					cout<<"=== Set Details === \n";
					admins[idAdmin].setAdminDetails(idAdmin);
					break;
				}
				case 4 :{
					cout<<"=== Update Your Details === \n ";
					admins[idAdmin].setAdminDetails(idAdmin);
					break;
				}
				default : {
					cout<<"Incorrect Number Entered!\n";
					break;
				}
			}
			cout<<"Another Operation from Admin Section  ( Y / N ) : ";
			cin>>adminChoice;
		}while(adminChoice=='Y',adminChoice=='y');
	       
			break;
		}
		case 3:{
			int idCashier;
			cashierU[0]=userName("Asad");
			cashierP[0]=Passwords("asad","11","!");
			char cashierOption;
			
	const int cashierMax=3;
	bool cashierVerification=false;
	cout<<"\n=== Sign In === \n";
	do{
		string username;
		cout <<"Enter Username : ";
		cin>>username;
		for(int i=0;i<3;i++){
			if(cashierU[i].verifyUserName(username)){
				cashierVerification=true;
				idCashier=i;
				break;
			}else{
				cashierVerification=false;
				if (i==2)cout<<"Incorrect Username!\n";
				}
		}
	}while(!cashierVerification);
	do{
		string password;
		cout<<"Password : ";
		cin>>password;
		for(int i=0;i<3;i++){
			if(cashierP[i].verifyPassword(password)){
				cashierVerification=true;
				break;
			}else {
					cashierVerification=false;
					if(i==2) cout<<"Incorrect Password!\n";
				}
		}
	}while(!cashierVerification);
	do{
	cout<<"=== Cashier Portal === \n";
	cout<<"1. Set Details \n";
	cout<<"2. View Details\n";
	cout<<"3. Update Details \n";
	cout<<"4. Deposit Customer Balance \n";
	cout<<"5. Customer Withdrawl \n";
	int cashierChoice;
	cin>>cashierChoice;
	switch(cashierChoice){
		case 1:{
			cashiers[idCashier].setCashierDetails();
			break;
		}
		case 2:{
			cashiers[idCashier].displayCashierDetails();
			break;
		}
		case 3 :{
		    cashiers[idCashier].setCashierDetails();
			break;
		}
		case 4 : {
			cashiers[idCashier].depositCustomerBalance(customers,recordCountCustomer);
			break;
		}
		case 5 :{
			cashiers[idCashier].withdrawlCustomerBalance(customers,recordCountCustomer);
			break;
		}
		default :{
			cout<<"Incorrect Number Entered !\n";
			break;
		}
	}

cout<<"Another Operation from Cashier Section (Y / N ) : ";
cin>>cashierOption;
		}while(cashierOption == 'Y' || cashierOption =='y');
			break;
	}
		case 4:{
			
	cout<<"\n=== Sign In ===\n";
	bool managerVerification;
	do{
		string username;
		cout<<"Enter Username : ";
		cin>>username;
		if(managerU.verifyUserName(username)){
			managerVerification=true;
		}else{
			cout<<"Incorrect Username!\n";
			managerVerification=false;
		}
	}while(!managerVerification);
	do{
		string password;
		cout<<"Enter Password : ";
		cin>>password;
		if(managerP.verifyPassword(password)){
			managerVerification=true;
		}else{
			cout<<"Incorrect Password !\n";
			managerVerification=false;
		}
	}while(!managerVerification);
	char managerOption;
	do{	
	cout<<"=== Manager Dashboard === \n";
	cout<<"1. Display Customer Details \n";
	cout<<"2. Display Employee Details\n";
	cout<<"3. Set Manager Details \n";
	cout<<"4. View Manager Details\n";
	cout<<"5. Update Manager Details\n";
	int s;
	cout<<"Your No : ";
	cin>>s;
	switch(s){
		case 1:{
			manager.displayCustomerDetails(customers,recordCountCustomer);
			break;
		}
		case 2:{
			manager.displayEmployees(admins,cashiers);
			break;
		}
		case 3:{
			manager.setManagerDetails();
			break;
		}
		case 4:{
			manager.displayManagerDetails();
			break;
		}
		case 5 :{
			manager.setManagerDetails();
			break;
		}
		default:{
			break;
		}
	}
	cout<<"Another Operation from Manager Section ( Y / N ) ? : ";
	cin>>managerOption;
		}while(managerOption=='Y'|| managerOption=='y');
			break;
		}
		default:{
			cout <<"Incorrect Number Entered! Please Try Again.\n";
			break;
	}
		}
		cout <<"\n===============\nMain Menu ? (Y/N) \n===============\n";
		cin>>mainMenudo;
	} while(mainMenudo=='Y'|| mainMenudo=='y');
	return 0;
}
