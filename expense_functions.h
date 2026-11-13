#ifndef EXPENSE_FUNCTIONS_H
#define EXPENSE_FUNCTIONS_H

#include "expense.h"

#include <string>
#include <vector>
using namespace std;

void viewExpenses(const vector<Expense>& expenses);

// Member 1: expense_view.cpp
void viewExpenses(const vector<Expense>& expenses);

void searchExpenses(const vector<Expense>& expenses);

// Member 2: expense_add.cpp
bool addExpense(vector<Expense>& expenses);

// Member 2: expense_summary.cpp
void showSpendingSummary(const vector<Expense>& expenses);

// Member 3: expense_manage.cpp
bool editExpense(vector<Expense>& expenses);

bool deleteExpense(vector<Expense>& expenses);

// Member 3: storage.cpp
bool loadExpenses(vector<Expense>& expenses,
                  const string& filename);


bool saveExpenses(const vector<Expense>& expenses,
                  const string& filename);

#endif 