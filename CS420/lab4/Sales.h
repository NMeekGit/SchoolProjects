

#include <iostream>
#include <string>
#include <iomanip>

#include "Employee.h"

#ifndef SALES
#define SALES

using namespace std;

class Sales
{
    public:
        Sales(int month = 0, double sale = 0);

        virtual void display(ostream & out) const;

    private:
        int myMonth;
        double mySale;

};

inline Sales::Sales(int month, double sale)
: myMonth(month), mySale(sale)
{ }

inline void Sales::display(ostream & out) const
{
    out << "Month " << myMonth << ":" << setw(8) << mySale << endl;
};

inline ostream & operator<<(ostream & out, const Sales & emp)
{
    emp.display(out);
    return out;
};

#endif
