#include "expense_functions.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>

using namespace std;

bool addExpense(vector<Expense>& expenses)
{
    Expense newExpense;

    cout << "\n--- Add Expense ---\n";

    cout << "Enter date (YYYY-MM-DD): ";
    cin >> newExpense.date;

    cout << "Enter category: ";
    cin >> newExpense.category;

    cin.ignore();

    cout << "Enter description: ";
    getline(cin, newExpense.description);

    double amount;

    cout << "Enter amount: PHP ";

    if (!(cin >> amount) || amount <= 0) {
        cout << "Invalid amount. Please enter a positive number.\n";
        return false;
    }

    newExpense.amountCentavos =
        static_cast<long long>(round(amount * 100));

    int highestId = 0;

    for (const Expense& expense : expenses) {
        highestId = max(highestId, expense.id);
    }

    newExpense.id = highestId + 1;
    expenses.push_back(newExpense);

    cout << fixed << setprecision(2);
    cout << "Expense added successfully!\n";
    cout << "Expense ID: " << newExpense.id << "\n";

    return true;
}
