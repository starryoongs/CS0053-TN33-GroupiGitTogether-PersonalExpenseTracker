#include "expense_functions.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <string>

using namespace std;

void showSpendingSummary(const vector<Expense>& expenses)
{
    string selectedMonth;
    long long monthlyTotal = 0;

    map<string, long long> categoryTotals;

    cout << "\n--- Spending Summary ---\n";
    cout << "Enter month (YYYY-MM): ";
    cin >> selectedMonth;

    for (const Expense& expense : expenses) {
        if (expense.date.substr(0, 7) == selectedMonth) {
            monthlyTotal += expense.amountCentavos;
            categoryTotals[expense.category] += expense.amountCentavos;
        }
    }

    if (monthlyTotal == 0) {
        cout << "No expenses found for " << selectedMonth << ".\n";
        return;
    }

    cout << fixed << setprecision(2);

    cout << "\nTotal spending for " << selectedMonth
         << ": PHP " << monthlyTotal / 100.0 << "\n";

    cout << "\nTotals by category:\n";

    for (const auto& category : categoryTotals) {
        cout << category.first << ": PHP "
             << category.second / 100.0 << "\n";
    }
}
