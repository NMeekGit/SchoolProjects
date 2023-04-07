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

int main() {

    Employee* employee;
    list<Employee*> employee_list;
    char command;
    string type;
    bool salaried = false;
    bool manager = false;
    bool hourly = false;

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

            cout << "Department: ";
            cin >> dept;
            cout << "" << endl;

            cout << "\nType of Employee: ";
            cin >> type;
            cout << "" << endl;

            if (type == "salary") {
                salaried = true;

                cout << "Employee's Salary: ";
                cin >> salary;
                cout << "" << endl;

            } else if (type == "hourly") {
                hourly = true;

                cout << "Weekly Wage: ";
                cin >> weeklyWage;
                cout << "" << endl;

                cout << "Hours Worked: ";
                cin >> hrsWorked;
                cout << "" << endl;

                cout << "Overtime: ";
                cin >> overTime;
                cout << "" << endl;

            } else if (type == "manager") {
                manager = true;

                cout << "Manager's Salary: ";
                cin >> salary;
                cout << "" << endl;

                cout << "Number of Employees: ";
                cin >> numEmployees;
                cout << "" << endl;
            }

            if (salaried) {
                employee = new SalariedEmployee(id, last, first, middle, dept, salary);
                employee_list.push_back(employee);
            } else if (hourly) {
                employee = new HourlyEmployee(id, last, first, middle, dept, weeklyWage, hrsWorked, overTime);
                employee_list.push_back(employee);
            } else if (manager) {
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

        if (command == PRINT) {

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
