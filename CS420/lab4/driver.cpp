/*
 * Main driver file for lab 4 assignment
 *
 * The job of the program is to track certain employees at a restaurant
 *
 * The goal of the assignment is to gain a better understanding of Object-Orient Programming with C++
 *
 * @author: Noah Meeker
 * @class: CS420
 * @red id: 821272563
 */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <string>

#include "Employee.h"
#include "Chef.h"
#include "Waiter.h"
#include "Manager.h"
#include "Sales.h"

using namespace std;

#define QUIT    'q'
#define NEW     'n'
#define REPORT  'r'
#define IS_SALES   's'
#define IS_CHEF    'c'
#define IS_WAITER  'w'
#define IS_OWNER 'o'

int main() {

    Employee* employee;
    list<Employee*> employee_list;
    list<Sales*> monthlySales;
    char command;
    char type;
    int numMonths = 1;
    double totalSales = 0;
    bool chef = false;
    bool manager = false;
    bool waiter = false;

    cout << "\nWhat would you like to do? (n: new member, s: today's sales, r: report)";
    cin >> command;
    cout << "" << endl;

    while (command != QUIT) {
        
        if (command == NEW) {

            string first;
            string last;
            string exp;
            char middle;
            long id;
            char dept;
            int yearsWorked;
            double salary;
            double percent;
            
            cout << "First Name: ";
            cin >> first;
            cout << "" << endl;

            cout << "Middle Initial: ";
            cin >> middle;
            cout << "" << endl;
            
            cout << "Last Name: ";
            cin >> last;
            cout << "" << endl;

            cout << "ID Number: ";
            cin >> id;
            cout << "" << endl;

            cout << "Salary: ";
            cin >> salary;
            cout << "" << endl;

            cout << "\nType of Employee: (o: Owner, c: Chef, w: Waiter)";
            cin >> type;
            cout << "" << endl;

            if (type == IS_CHEF) {
                chef = true;

                dept = 'C';

                cout << "Chef's Percentage: ";
                cin >> percent;
                percent = percent / 100;
                cout << "" << endl;

                cout << "Chef's Expertise: ";
                cin >> exp;
                cout << "" << endl;

            } else if (type == IS_WAITER) {
                waiter = true;
                
                dept = 'W';

                cout << "Years Worked: ";
                cin >> yearsWorked;
                cout << "" << endl;

            } else if (type == IS_OWNER) {
                manager = true;

                dept = 'O';

                cout << "Owner's Percentage: ";
                cin >> percent;
                percent = percent / 100;
                cout << "" << endl;

            }

            if (chef) {
                chef = false;
                employee = new Chef(id, last, first, middle, dept, salary, percent, exp);
                employee_list.push_back(employee);
            } else if (waiter) {
                waiter = false;
                employee = new Waiter(id, last, first, middle, dept, salary, yearsWorked);
                employee_list.push_back(employee);
            } else if (manager) {
                manager = false;
                employee = new Manager(id, last, first, middle, dept, salary, percent);
                employee_list.push_back(employee);
            } else {
                employee = new Employee(id, last, first, middle, dept);
                employee_list.push_back(employee);
            }

            cout << "***Employee Entered***" << endl;

        }

        fflush(stdin);

        if (command == IS_SALES) {
            double sale;
            double tips;

            numMonths++;

            cout << "\nSales for this month: ";
            cin >> sale;
            cout << "" << endl;

            totalSales += sale;

            Sales* sales = new Sales(numMonths, sale);
            monthlySales.push_back(sales);

            for (list<Employee*>::iterator idx = employee_list.begin(); idx != employee_list.end(); idx++) {
                employee = *idx;

                if (employee->GetClass() == 'W') {
                    cout << "Tips for " << employee->GetName() << " ";
                    cin >> tips;
                    cout << "" << endl;

                    employee->SetTips(tips);
                }
            }

            char print;
            cout << "Print current report? y or n\t";
            cin >> print;
            cout << "" << endl;

            if (print == 'y') {
                for (list<Sales*>::iterator idx = monthlySales.begin(); idx != monthlySales.end(); idx++) {
                    sales = *idx;
                    cout << *sales << endl;
                }
                cout << "\nTotal Sales: " << totalSales << endl;
            }
        }

        fflush(stdin);

        if (command == REPORT) {

            for (list<Employee*>::iterator idx = employee_list.begin(); idx != employee_list.end(); idx++) {
                employee = *idx;
                cout << *employee << endl;
            }
        }

        fflush(stdin);

        cout << "\nWhat would you like to do? (n: new member, s: today's sales, r: report) ";
        cin >> command;
        cout << "" << endl;
    }

    return 0;
}
