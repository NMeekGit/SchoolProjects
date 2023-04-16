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
#include <fstream>
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
#define TOTAL   't'
#define IS_SALES   's'
#define IS_CHEF    'c'
#define IS_WAITER  'w'
#define IS_OWNER 'o'
#define PRINT_WIDTH 32

void PrintReport(list<Sales*>, list<Employee*>);
int main() {

    Employee* employee;
    list<Employee*> employee_list;
    list<Sales*> monthlySales;
    char command;
    char type;
    int numMonths = 0;
    double totalSales = 0;
    bool chef = false;
    bool manager = false;
    bool waiter = false;

    cout << "\nWhat would you like to do? (n: new employee, r: employee report, s: today's sales, t: total sales) ";
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
            
            /* Insert standard Employee Info */
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

            cout << "\nType of Employee: (o: Owner, c: Chef, w: Waiter) ";
            cin >> type;
            cout << "" << endl;

            /* Data needed for Chef Employees */
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

            /* Data needed for Waiter Employees */
            } else if (type == IS_WAITER) {
                waiter = true;
                
                dept = 'W';

                cout << "Years Worked: ";
                cin >> yearsWorked;
                cout << "" << endl;

            /* Data needed for Owners */
            } else if (type == IS_OWNER) {
                manager = true;

                dept = 'O';

                cout << "Owner's Percentage: ";
                cin >> percent;
                percent = percent / 100;
                cout << "" << endl;

            }

            /* Create Employee Object */
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

                cout << "[ERROR]: Incorrect Type Entered" << endl;
                command = 'q';
                continue;

            }

            cout << "***Employee Entered***" << endl;

        }

        fflush(stdout);

        /* Calculate Month's Sales */
        if (command == IS_SALES) {
            double sale;
            double tips;

            numMonths++;

            cout << "\nSales for this month: ";
            cin >> sale;
            cout << "" << endl;

            totalSales += sale;

            /* Create Sales Object */
            Sales* sales = new Sales(numMonths, sale);
            /* Add Sales Object to List */
            monthlySales.push_back(sales);

            for (list<Employee*>::iterator idx = employee_list.begin(); idx != employee_list.end(); idx++) {
                employee = *idx;

                /* Add Waiter's Tips for the month */
                if (employee->GetClass() == 'W') {
                    cout << "Tips for " << employee->GetName() << " : ";
                    cin >> tips;
                    cout << "" << endl;

                    employee->SetTips(tips);
                }
            }

            char print;
            cout << "Print current sales report? y or n\t";
            cin >> print;
            cout << "" << endl;

            /* Print current Sales Report */
            if (print == 'y') {
                for (list<Sales*>::iterator idx = monthlySales.begin(); idx != monthlySales.end(); idx++) {
                    sales = *idx;
                    cout << *sales << endl;
                }

                /* Report on negative profit */
                if (sale < 0) {

                    cout << "Why is the owner and chefs getting paid six figure incomes at a family restaurant. \n \
                        Along with 60% / 20% partake of profit. This restaurant is not going to make it if $6,000 \"profit\" is average." << endl;

                }

                cout << left << setw(PRINT_WIDTH) << "Total Sales: " << "$" << totalSales << endl;

                /* Employee's take-home from month's profit */
                for (list<Employee*>::iterator idx = employee_list.begin(); idx != employee_list.end(); idx++) {
                    employee = *idx;

                    if (employee->GetClass() != 'W') {

                        cout << left << setw(10) << employee->GetName() << setw(PRINT_WIDTH) << " total revenue split: " << "$" << totalSales * employee->GetPercentage() << endl;

                    } else {

                        cout << left << setw(10) << employee->GetName() << setw(PRINT_WIDTH) << " total tips: " << "$" << employee->GetTips() << endl;
                    }
                }
            }

            /* Print report to the .txt. file */
            PrintReport(monthlySales, employee_list);
            cout << "Report Printed" << endl;
        }

        fflush(stdout);

        /* Print Employee Report */
        if (command == REPORT) {

            for (list<Employee*>::iterator idx = employee_list.begin(); idx != employee_list.end(); idx++) {
                employee = *idx;
                cout << *employee << endl;
            }

            char in = 'n';
            cout << "Print Employee Report? (y or n) ";
            cin >> in;
            cout << "" << endl;

            /* Print Employee Report to .txt file */
            if (in == 'y') {
                ofstream report;
                
                report.open("employees.txt");
                for (list<Employee*>::iterator idx = employee_list.begin(); idx != employee_list.end(); idx++) {
                    employee = *idx;
                    report << *employee << endl;
                }
            } 
        }

        fflush(stdout);

        /* Print total sales report */
        if (command == TOTAL) {

            double sum = 0;
            for (list<Sales*>::iterator idx = monthlySales.begin(); idx != monthlySales.end(); idx++) {
                Sales* sales = *idx;
                cout << *sales << endl;
                sum += sales->GetSales();
            }

            cout << left << setw(PRINT_WIDTH) << "Total Sales : " << "$" << sum << endl;

            for (list<Employee*>::iterator idx = employee_list.begin(); idx != employee_list.end(); idx++) {
                employee = *idx;

                if (employee->GetClass() != 'W') {

                    cout << left << setw(10) << employee->GetName() << setw(PRINT_WIDTH) << " total revenue split: " << "$" << sum * employee->GetPercentage() << endl;

                } else {

                    cout << left << setw(10) << employee->GetName() << setw(PRINT_WIDTH) << " total tips: " << "$" << employee->GetTips() << endl;
                }
            }
        }


        fflush(stdout);

        cout << "\nWhat would you like to do? (n: new employee, r: employee report, s: today's sales, t: total sales) ";
        cin >> command;
        cout << "" << endl;
    }

    return 0;
}

/* @summary : prints report to .txt file
 *
 * @para: (sales) List of monthly sales
 * @para: (employees) List of employees
 */
void PrintReport(list<Sales*> sales, list<Employee*> employees) {
    ofstream report;

    report.open("report.txt", ios_base::app);

    report << setfill('-') << setw(PRINT_WIDTH) << ">" << endl;
    report << setfill(' ');
    fflush(stdout);
        double sum = 0;
        for (list<Sales*>::iterator idx = sales.begin(); idx != sales.end(); idx++) {
            Sales* sales = *idx;
            report << *sales << endl;
            sum += sales->GetSales();
        }

        report << left << setw(PRINT_WIDTH) << "Total Sales : " << "$" << sum << endl;

        for (list<Employee*>::iterator idx = employees.begin(); idx != employees.end(); idx++) {
            Employee* employee = *idx;

            if (employee->GetClass() != 'W') {

                report << left << setw(10) << employee->GetName() << setw(PRINT_WIDTH) << " total revenue split: " << "$" << sum * employee->GetPercentage() << endl;

            } else {

                report << left << setw(10) << employee->GetName() << setw(PRINT_WIDTH) << " total tips: " << "$" << employee->GetTips() << endl;
            }
        }

        fflush(stdout);

    report.close();

}
