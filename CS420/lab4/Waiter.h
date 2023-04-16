/* HourlyEmployee.h -------------------------------------------------------
   Header file for a class HourlyEmployee derived from Employee that
   adds the attributes unique to hourly employees. 
   Operations are:  A constructor and an output operation.
   -----------------------------------------------------------------------*/
#include "Employee.h"

#ifndef WAITER
#define WAITER

class Waiter : public Employee
{
 public:
    Waiter (long id = 0, string last = "", string first = "", 
		   char initial = ' ', char dept = ' ', double sal = 0,
		    int years = 0);
   /*----------------------------------------------------------------------
    Hourly Employee constructor.
    Preconditions:  None.
    Postconditions: Data members myIdNum, myLastName, myFirstName,
       myMiddleInitial, and myDeptCode are initialized by the Employee
       constructor; myWeeklyWage is initialized to weeklyWage (default 
       0), myHoursWorked to hoursWorked (default 0), and 
       myOverTimeFactor to overTimeFactor (default 1.5).
   -----------------------------------------------------------------------*/ 

  double GetTips();
  void SetTips(double tips = 0);

  virtual void display(ostream & out) const;
  /*-----------------------------------------------------------------------
    Output function member.
    Precondition:  ostream out is open.
    Postcondition: A text representation of this HourlyEmployee 
        object has been output to out.
   -----------------------------------------------------------------------*/ 

  // ... Other hourly employee operations ...

 private:
  double myTips = 0;
  int myYears;
};

//--- Definition of HourlyEmployee's Constructor
inline Waiter::Waiter 
                 (long id, string last, string first, char initial,
                  char dept, double sal, int year)
: Employee(id, last, first, initial, dept, sal),
    myYears(year)
{ }

inline double Waiter::GetTips() {

    return this->myTips;

};

inline void Waiter::SetTips(double tips) 
{
    this->myTips += tips;
}

//--- Definition of HourlyEmployee's display()
inline void Waiter::display(ostream & out) const
{
  Employee::display(out);              //inherited members
  out << left << setw(WIDTH) << "Tips Made: " <<"$" << myTips << endl;
  out << left << setw(WIDTH) << "Years With Company: " << myYears << endl;
}
#endif
