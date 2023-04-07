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
#include "SalariedEmployee.h"
#include "HourlyEmployee.h"
#include "Manager.h"

using namespace std;

#define QUIT    'q'
#define NEW     'n'
#define REPORT  'r'
#define SALES   's'
#define CHEF    'c'
#define WAITER  'w'

int main() {

    Employee* employee;
    list<Employee*> employee_list;
    char command;
    char type;
    bool chef = false;
    bool manager = false;
    bool waiter = false;

    cout << "What would you like to do? (n: new member, s: today's sales, r: report)";
    cin >> command;
    cout << "" << endl;

    while (command != QUIT) {
        
        if (command == NEW) {

            string first;
            string last;
            char middle;
            long id;
            int dept;
            int numEmployees;
            double salary;
            double percent;
            double tips;
            double weeklyWage;
            double hrsWorked;
            double overTime;
            
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

            cout << "\nType of Employee: (m: Manager, c: Chef, w: Waiter)";
            cin >> type;
            cout << "" << endl;

            if (type == CHEF) {
                chef = true;

                cout << "Chef's Salary: ";
                cin >> salary;
                cout << "" << endl;

                cout << "Chef's Percentage: ";
                cin >> percent;
                cout << "" << endl;

                cout << "Chef's Expertise: ";
                cin >> exp;
                cout << "" << endl;

            } else if (type == WAITER) {
                waiter = true;

                cout << "Weekly Wage: ";
                cin >> weeklyWage;
                cout << "" << endl;

                cout << "Hours Worked: ";
                cin >> hrsWorked;
                cout << "" << endl;

                cout << "Overtime: ";
                cin >> overTime;
                cout << "" << endl;

                cout << "Tips Made: ";
                cin >> tips;
                cout << "" << endl;

            } else if (type == MANAGER) {
                manager = true;

                cout << "Manager's Salary: ";
                cin >> salary;
                cout << "" << endl;

                cout << "Manager's Percentage: ";
                cin >> percent;
                cout << "" << endl;

                cout << "Number of Employees: ";
                cin >> numEmployees;
                cout << "" << endl;
            }

            if (chef) {
                chef = false;
                employee = new Chef(id, last, first, middle, dept, exp, salary, percentage,);
                employee_list.push_back(employee);
            } else if (hourly) {
                hourly = false;
                employee = new Waiter(id, last, first, middle, dept, weeklyWage, hrsWorked, overTime);
                employee_list.push_back(employee);
            } else if (manager) {
                manager = false;
                employee = new Manager(id, last, first, middle, dept, salary, numEmployees);
                employee_list.push_back(employee);
            } else {
                employee = new Employee(id, last, first, middle, dept);
                employee_list.push_back(employee);
            }

        }

        fflush(stdin);

        if (command == SALES) {


        }

        if (command == REPORT) {

            for (list<Employee*>::iterator idx = employee_list.begin(); idx != employee_list.end(); idx++) {
                employee = *idx;
                cout << *employee << endl;
            }
        }

        cout << "What would you like to do? : ";
        cin >> command;
        cout << "" << endl;
    }

    return 0;
}
