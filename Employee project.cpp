#include<iostream>
#include<fstream>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include<iomanip>
 using namespace std;

 class Rates
{   
	private:	float DAr, HRAr, GPFr; 

	public:		void getRate();
				void putRate();
				float retDAr() { return DAr;}
				float retHRAr() { return HRAr;}
				float retGPFr() { return GPFr;}
}R;

 void Rates :: getRate()
{ cout<<"\nRate of Dearness allowance: ";    cin>>DAr;
  cout<<"Rate of Home Rent allowance: ";     cin>>HRAr;
  cout<<"Rate of G. Provident funds: ";      cin>>GPFr;
} 

 void Rates :: putRate()
{ cout<<"\nRate of Dearness allowance: ";  cout<<DAr<<"%";
  cout<<"\nRate of Home Rent allowance: "; cout<<HRAr<<"%";
  cout<<"\nRate of G. Provident funds: ";  cout<<GPFr<<"%";
} 


 class Employee
{
	private:	int empnum; char fname[50], lname[50]; char dob[11], doj[11];
				char addr1[20], addr2[20]; 
				float BP, TA;			
				float CGIS, CGHS;		
										
	public:		void getdata();
				void putdata();
				int reteno() { return empnum;}
}e1, emp;

 void Employee :: getdata()
{ cout<<"\nFirst Name: "; cin>>fname;
  cout<<"Last Name: "; cin>>lname;
  cout<<"Employee number: "; cin>>empnum;
  cout<<"Date of Birth (dd/mm/yyyy): "; cin>>dob;
  cout<<"Date of Joining (dd/mm/yyyy): "; cin>>doj;
  cout<<"Address (city): "; cin>>addr1;
  cout<<"Address (state): "; cin>>addr2;
  cout<<"Basic Pay: "; cin>>BP;
  cout<<"Transport Allowance: "; cin>>TA;
  cout<<"CGIS: "; cin>>CGIS;
  cout<<"CGHS: "; cin>>CGHS;
} 

 void Employee :: putdata()
{ fstream fi;
  float ITAXr, net;
  fi.open("Rate.dat" , ios::in | ios::binary);
  fi.read((char*)&R, sizeof(R));
  
  net=(BP+CGIS+CGHS+(BP*(R.retDAr()/100))+(BP*(R.retHRAr()/100))+TA)-(BP*(R.retGPFr()/100));
  
  if(net*12>1000000) ITAXr=30;
  else if(net*12>500000) ITAXr=20;
  else if(net*12>250000) ITAXr=5;
  else ITAXr=0;
  cout<<"\nEmployee number: "; cout<<empnum;
  cout<<"\nName: "<<fname<<" "<<lname;
  cout<<"\nDate of Birth : "; cout<<dob;
  cout<<"\nDate of Joining : "; cout<<doj;
  cout<<"\nAddress: "; cout<<addr1<<" "<<addr2;
  cout<<"\n================================================================================";
  cout<<"\nCREDITS\t\t\t\t| DEBITS "; 
  cout<<"\nBasic Pay: "<<BP<<setw(29)<<"| Income Tax: "<<BP*(ITAXr/100);
  cout<<"\nCGIS:"<<CGIS<<setw(33)<<"| GPF at "<<R.retGPFr()<<"%: "<<BP*(R.retGPFr()/100);
  cout<<"\nCGHS:"<<CGHS<<setw(26)<<"|  ";
  cout<<"\nTA:"<<TA<<setw(27)<<"| ";
  cout<<"\nHRA at "<<R.retHRAr()<<"% : "<<BP*(R.retHRAr()/100)<<setw(16)<<"| ";
  cout<<"\nDA at "<<R.retDAr()<<"% : "<<BP*(R.retDAr()/100)<<setw(19)<<"| ";
  cout<<"\n_______________________________________________________________________________";
  cout<<"\nTotal Credits: Rs"<<CGIS+CGHS+(BP*(R.retDAr()/100))+(BP*(R.retHRAr()/100))+TA<<setw(28)<<"| Total Debits: Rs"<<BP*(ITAXr/100)+(BP*(R.retGPFr()/100));
  cout<<"\n_______________________________________________________________________________";
  cout<<"\nNet salary= Rs"<<net-BP*(ITAXr/100)<<endl;

  fi.close();
} 
 
 

///////Creation of Employee, Rates & writing in Binary files///////
 void Create()
{ fstream foE, fR;
  foE.open("Employee.dat" , ios::out | ios::binary);
  fR.open("Rate.dat" , ios::out | ios::binary);
  int n, i;
  cout<<"\nEnter the number of employees you want to write(minimum 10): "; cin>>n;
  cout<<"\nEnter the employee number in ascending order\n";  getch();
  
  for(i=0;i<n;i++)
  	 { cout<<"\nEnter the details of employee "<<i+1;
	   e1.getdata();
  	   foE.write((char*)&e1, sizeof(e1));
  	 }
  
  cout<<"\nEnter the Rates...";
  R.getRate();
  fR.write((char*)&R, sizeof(R));
  
  foE.close();
  fR.close();
}


////////////////////////Insertion of record////////////////////////
 void InsertRec()
{ fstream fi, fo;
  char last='y';
  fi.open("Employee.dat" , ios::in | ios::binary);
  fo.open("Temp.dat" , ios::out | ios::binary);

  cout<<"\nEnter the detail of the employee whose record is to be inserted\n";
  e1.getdata();

  while(fi.read((char*)&emp, sizeof(emp)))
	  {
		 if(e1.reteno() <= emp.reteno())
			{ fo.write((char*)&e1, sizeof(e1));
			  last='n';
			  break;
			}
		 else  fo.write((char*)&emp, sizeof(emp));
	  }

  if(last=='y') fo.write((char*)&e1, sizeof(e1));
  else if(!fi.eof())
	  { while(fi.read((char*)&emp, sizeof(emp)))
			  fo.write((char*)&emp, sizeof(emp));
	  }

  cout<<"\nNew Record of Employee added.";
  fi.close();  fo.close();

  remove("Employee.dat");
  rename("Temp.dat", "Employee.dat");
}



///////////////////////Modification of record//////////////////////
 void ModifyRec()
{ fstream file;
  file.open("Employee.dat" , ios::in | ios::out | ios::binary);
  int eno, found=0; long pos;

  cout<<"\nEnter the Employee number of the employee whose record you want to modify: ";
  cin>>eno;

  while(!file.eof())
	 { pos=file.tellg();
		file.read((char*)&emp, sizeof(emp));

		if(eno==emp.reteno())
		  { found++;
			 cout<<"\nThe employee's ORIGINAL details\n\n";
  			 emp.putdata();
			 cout<<"\nEnter the NEW details of Employee...";
			 cout<<"\nNOTE: Keep the EMPLOYEE NUMBER, DOB and DOJ same.";
			 file.seekg(pos);
			 emp.getdata();
			 file.write((char*)&emp, sizeof(emp));
			 break;
		  }
	 }

  if(found==0) cout<<"\nNo matching record found...";
  else         cout<<"\nDetails of the Employee are modified."; 
  
  file.close();
}


////////////////////////Deletion of record/////////////////////////
 void DelRec()
{ fstream fi, fo;
  fi.open("Employee.dat" , ios::in | ios::binary);
  fo.open("Temp.dat" , ios::out | ios::binary);
  int found=0, eno, ans;

  cout<<"\nEnter the Employee Number of the employee whose record is to be deleted:";
  cin>>eno;

  while(fi.read((char*)&emp, sizeof(emp)))
	  {
		 if(emp.reteno() == eno && !found)
			{ cout<<"\nRecord to be deleted is found. \nAre u sure u want to delete it?\t 1=YES \t 0=NO\n";
			  cin>>ans;
			  found++;

			  if(ans==0) fo.write((char*)&emp, sizeof(emp));

			}
		 else fo.write((char*)&emp, sizeof(emp));
	  }

  if(found==0)  cout<<"\nNo matching Record found...";
  else          cout<<"\nRecord of Employee deleted...";

  fi.close();  fo.close();

  remove("Employee.dat");
  rename("Temp.dat", "Employee.dat");
}
 
  
///////////////////////Modification of Rates///////////////////////
 void ModifyRates()
{ fstream file;
  file.open("Rate.dat" , ios::in | ios::out | ios::binary);
  long pos;
  
  pos=file.tellg();
  file.read((char*)&R, sizeof(R));
  cout<<"\n\nThe old rates are...";
  R.putRate();
  file.seekg(pos);
  cout<<"\n\nEnter the new Rates...";
  R.getRate();
  file.write((char*)&R, sizeof(R));
  
  cout<<"\nRates are modified..."; 
  file.close();
}


////////////////Generation of One Pay Slips////////////////
 void GenOneRec()
{ fstream fi, fo;
  fi.open("Employee.dat" , ios::in | ios::binary);
 
  int found=0, eno, ans;
  
  cout<<"\nEnter the Employee Number of the employee whose Payslip is to be generated: ";
  cin>>eno;
  
  while(!fi.eof())
     { fi.read((char*)&emp, sizeof(emp));
       
       if(emp.reteno() == eno)
         { cout<<"\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n";
		   cout<<"\t\t\t\tDoofenshmirtz Evil Inc\n\t\t\t  9297 Polly Parkway, Tri-State Area\n\t\t\t             Delhi-110034";
           emp.putdata();
           cout<<"\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\n";
		   getch();
           found++;
		 }
     }
  
  if(found==0)  cout<<"\nNo matching Record found...";

  fi.close(); 
}
 
 
 int main()
{ int op=1, choice;

  if(!ifstream("Employee.dat"))	  Create();
  
  while(op)
    { cout<<"\n\nMENU";
      cout<<"\n\t1. Insert New record\n\t2. Modify a Record\n\t3. Delete a Record\n\t4. Genarate payslip of one employee\n\t5. Modify the Rates";
      cout<<"\nEnter your choice: ";
      cin>>choice;
      
      switch(choice)
        { case 1: InsertRec();   break;
          case 2: ModifyRec();   break;
          case 3: DelRec();      break;
          case 4: GenOneRec();   break;
		  case 5: ModifyRates(); break;
          default: cout<<"\nWrong input";
        }
      cout<<"\nDo you want to continue?\t 1=YES \t 0=NO \n";
      cin>>op;
    }
  return 0;  
}

