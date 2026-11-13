#ifndef EXPENSE_H
#define EXPENSE_H

#include <string>
using namespace std;

struct Expense {
    int id = 0;
    string date;
    string category;
    string description;
    long long amountCentavos = 0;
};

#endif 
