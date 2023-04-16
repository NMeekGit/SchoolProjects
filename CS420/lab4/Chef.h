/* Chef.h ----------------------------------------------------
   Header file for a class Chef derived from Employee that
   adds the attributes unique to salaried employees. 
   Operations are:  A constructor and an output operation.
   ----------------------------------------------------------------------*/
#include "Employee.h"
#include "Manager.h"

#ifndef CHEF
#define CHEF

class Chef : public Manager
{
 public:
   Chef (long id = 0, string last = "", 
                    string first = "", char initial = ' ',
                    char dept = ' ', double salary = 0, double percentage = 0, string exp = "");
  /*-----------------------------------------------------------------------
    Salaried Employee constructor.
    Preconditions:  None.
    Postconditions: Data members myIdNum, myLastName, myFirstName,
       myMiddleInitial, and myDeptCode are initialized by the Employee
       constructor; mySalary is initialized to salary (default 0).
   -----------------------------------------------------------------------*/ 

  virtual void display(ostream & out) const;
  /*-----------------------------------------------------------------------
    Output function member.
    Precondition:  ostream out is open.
    Postcondition: A text representation of this SalariedEmployee 
        object has been output to out.
   -----------------------------------------------------------------------*/ 

  // ... Other salaried employee operations ...

 private:
   string myExpertise;
};

//--- Definition of SalariedEmployee's Constructor
inline Chef::Chef
                   (long id, string last, string first, char initial,
                    char dept, double sal, double per, string exp)
: Manager(id, last, first, initial, dept, sal, per), 
    myExpertise(exp)
{ }

//--- Definition of SalariedEmployee's display()
inline void Chef::display(ostream & out) const
{
  Manager::display(out);            //inherited members
  out << left << setw(WIDTH) << "Expertise: " << myExpertise << endl;
}
#endif
