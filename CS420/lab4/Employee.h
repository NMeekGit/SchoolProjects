/* Employee.h --------------------------------------------------------------
   Header file for a class Employee that encapsulates the attributes common
   to all employees.
   Operations are:  A constructor and an output operation.
   -----------------------------------------------------------------------*/
#include <iostream>
#include <string>
#include <iomanip>

#ifndef EMPLOYEE
#define EMPLOYEE

using namespace std;

class Employee
{
 public:
   Employee(long id = 0, string last = "", string first = "", 
	        char initial = ' ', char dept = ' ', double salary = 0);
  /*-----------------------------------------------------------------------
    Employee constructor.
    Preconditions:  None.
    Postconditions: myIdNum is initialized to id (default 0), myLastName 
        to last (default empty string), myFirstName to first (default 
        empty string), myMiddleInitial to initial (default blank char),
        and myDeptCode to dept (default 0). 
    ----------------------------------------------------------------------*/ 

  string GetName();

  char GetClass();

  virtual double GetTips();
  
  virtual double GetPercentage();

  virtual void SetTips(double);

  virtual void display(ostream & out) const;
  /*-----------------------------------------------------------------------
    Output function member.
    Precondition:  ostream out is open.
    Postcondition: A text representation of this Employee object has 
        been output to out.
  ------------------------------------------------------------------------*/ 

  // ... Other employee operations ...

 private:
  long myIdNum;             // Employee's id number
  string myLastName,        //     "      last name
         myFirstName;       //     "      first name
  char myMiddleInitial;     //     "      middle initial
  char myEmpClass;           //     "      employee class
  double mySalary;
                            // ... other attributes common to all employees
};


//--- Definition of Employee's Constructor
inline Employee::Employee(long id, string last, string first, 
                          char initial, char dept, double sal)
  : myIdNum(id), myLastName(last), myFirstName(first),
     myMiddleInitial(initial), myEmpClass(dept), mySalary(sal)
{ }

inline string Employee::GetName() {

    return this->myFirstName;

}

inline char Employee::GetClass() {

    return this->myEmpClass;

}

inline double Employee::GetTips()
{ }

inline void Employee::SetTips(double tips)
{ }

inline double Employee::GetPercentage()
{ }

//--- Definition of Employee's display()
inline void Employee::display(ostream & out) const
{
  out << left << setw(20) << "ID: " << myIdNum << endl;
  out << left << setw(20) << "Name: " << myLastName << ", " << myFirstName << ' ' << myMiddleInitial << endl; 
  out << left << setw(20) << "Class: " << myEmpClass << endl;
  out << left << setw(20) << "Salary: " << "$" << mySalary << endl;
}

//--- Definition of output operator <<
inline ostream & operator<<(ostream & out, const Employee & emp)
{
  emp.display(out);
  return out;
};

#endif
