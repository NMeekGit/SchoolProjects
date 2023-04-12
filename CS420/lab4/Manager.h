/* Manager.h --------------------------------------------------------------
   Header file for a class Manager derived from Employee that adds
   the attributes unique to managers. 
   Operations are:  A constructor and a virtual output operation.
   -----------------------------------------------------------------------*/
#include "Employee.h"

#ifndef MANAGER
#define MANAGER

class Manager : public Employee
{
public:
  Manager(long id = 0, string last = "", string first = "",
    char initial = ' ', char dept = ' ', double sal = 0, double per = 0);
   /*----------------------------------------------------------------------
    Manager constructor.
    Preconditions:  None.
    Postconditions: Data members myIdNum, myLastName, myFirstName,
       myMiddleInitial, myDeptCode, mySalary are initialized by the
       SalariedEmployee constructor; myNumEmps is initialized to 
       numEmps (default 0).
   -----------------------------------------------------------------------*/ 

  virtual void display(ostream & out) const;
  /*-----------------------------------------------------------------------
    Output function member.
    Precondition:  ostream out is open.
    Postcondition: A text representation of this Manager object has been
        output to out.
   -----------------------------------------------------------------------*/ 
 
 // ... Other Manager operations ...

private:
  double myPercent;
};

//--- Definition of Manager's Constructor
inline Manager::Manager(long id, string last, string first, char initial, 
                        char dept, double sal, double per)
: Employee(id, last, first, initial, dept, sal),
    myPercent(per)
{}

//--- Definition of Manager's display()
inline void Manager::display(ostream & out) const 
{ 
  Employee::display(out);
  out << left << setw(20) << "Percentage: " << myPercent * 100 << "%" << endl;
}
#endif
