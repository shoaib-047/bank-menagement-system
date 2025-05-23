#include<iostream>
#include<string>
using namespace std;
class Cashier{
	 protected:
	 	double  balance,deposit,amount;
	 	public:
	 		void cashdepositAmount(){
	 			cout<<"Enter initial balance: ";
	 			cin>> balance;
	 			cout<<"Enter amount you want to deposit: ";
	 			cin>> deposit;
	 			if(deposit>0){
	 				deposit+=balance;
	 				cout<<" desposited Amount:Rs"<<deposit<<"successfully"<<endl;
	 			}else{
	 				cout<<" desposited Amount must be postive"<<endl;
	 		}
				 }
				 void cashWithdrawAmount(){
				 	cout<<"Enter amount you want to withdraw: "<<endl;	
				 if(amount<0)	{
				 	cout<<"amount must be postive"<<endl; 	
				 }else if(amount>balance){
				 	cout<<"Insufficient amount,Trasaction failed"<<endl;
				 	}else{
				 		amount-=balance;
				 		cout<<"Withdraw amount:Rs"<<amount<<"Succesfully"<<endl;
						cout<<"Remaining balance: Rs"<<balance<<endl; 
					 }
				 }
				 };
				 class TrasactionRecipient:public Cashier{
				 	protected:
				 	int transactionId;
				 	string transactionDate;
				 	string trasactionType;
				 	double trasactionAmount;	 	
				public:
				 void trasactionMoney(){	
				 }
			     void trasactionReport(){
				 }
};
                 int main(){
                TrasactionRecipient tr1;
            	int choice;
            	cout<<"==========Cashier Dashboard========"<<endl;
            	cout<<"Enter number you want to perform operation between(1-5): "<<endl;
            	cout<<"1.create account"<<endl;
            	cout<<"2.deposit amount"<<endl;	
				cout<<"3.withdraw amount"<<endl;
            	cout<<"4.trasaction money from one account to another"<<endl;
            	cout<<"5.Generate trasaction money repoert"<<endl;
                cout<<"6.trasaction money"<<endl;
                cout<<"7.Exit"<<endl;
                cout<<"Enter choice:";
                cin>> choice;
                do{
                switch(choice){
                	    case 1:
                	    break;
                	    case 2:
                	    tr1.cashdepositAmount();
                	    break;
                	    case 3:
                	    tr1.cashWithdrawAmount();
                		break;
                		case 4:
                		tr1.trasactionMoney();
                		break;
                		case 5:
                			tr1.trasactionReport();
                		break;
                		case 6:
                		break;
                		case 7:
                			exit(0);
                		break;
                		default:
                			cout<<"Invalid choice.try again"<<endl;
				}
				}	while(true);
            	
            	
			}    
			
