#include "expense_functions.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

void showSpendingSummary(const vector<Expense> &expenses)
{
    const string monthNames[] = {
        "January", "February", "March", "April",
        "May", "June", "July", "August",
        "September", "October", "November", "December"};

    string selectedMonth;

    cout << "\n--- Spending Summary ---\n";
    cout << "Enter month (YYYY-MM): ";
    if (!getline(cin, selectedMonth))
    {
        return;
    }

    // Check the exact YYYY-MM format.
    bool validMonth =
        selectedMonth.size() == 7 &&
        selectedMonth[4] == '-';

    if (validMonth)
    {
        for (size_t i = 0; i < selectedMonth.size(); ++i)
        {
            if (i == 4)
                continue;

            if (selectedMonth[i] < '0' || selectedMonth[i] > '9')
            {
                validMonth = false;
                break;
            }
        }
    }

    if (!validMonth)
    {
        cout << "Invalid month. Use YYYY-MM, such as 2026-05.\n";
        return;
    }

    int year = stoi(selectedMonth.substr(0, 4));
    int month = stoi(selectedMonth.substr(5, 2));

    if (year < 1 || month < 1 || month > 12)
    {
        cout << "Invalid month. Enter a valid year and month (01-12).\n";
        return;
    }

    string monthLabel =
        monthNames[month - 1] + " " + selectedMonth.substr(0, 4);

    long long monthlyTotal = 0;
    map<string, long long> categoryTotals;

    for (const Expense &expense : expenses)
    {
        if (expense.date.substr(0, 7) == selectedMonth)
        {
            monthlyTotal += expense.amountCentavos;
            categoryTotals[expense.category] += expense.amountCentavos;
        }
    }

    if (categoryTotals.empty())
    {
        cout << "\nNo expenses found for " << monthLabel << ".\n";
        return;
    }

    // Format money without converting centavos to floating point.
    auto formatMoney = [](long long centavos)
    {
        ostringstream output;
        output << "PHP " << centavos / 100 << '.'
               << setfill('0') << setw(2) << centavos % 100;
        return output.str();
    };

    // Expand columns when category names or amounts are longer.
    size_t categoryWidth = 20;
    size_t amountWidth = 16;

    string totalLabel = "Total spending for " + monthLabel;
    string totalAmount = formatMoney(monthlyTotal);

    if (totalLabel.size() > categoryWidth)
        categoryWidth = totalLabel.size();

    if (totalAmount.size() > amountWidth)
        amountWidth = totalAmount.size();

    for (const auto &category : categoryTotals)
    {
        if (category.first.size() > categoryWidth)
            categoryWidth = category.first.size();

        string amount = formatMoney(category.second);

        if (amount.size() > amountWidth)
            amountWidth = amount.size();
    }

    int labelColumn = static_cast<int>(categoryWidth);
    int amountColumn = static_cast<int>(amountWidth);
    string divider(categoryWidth + amountWidth + 3, '-');

    // Restore the original output alignment afterward.
    ios::fmtflags originalFlags = cout.flags();

    cout << '\n'
         << divider << '\n';
    cout << "SPENDING SUMMARY | " << monthLabel << '\n';
    cout << divider << '\n';

    cout << left << setw(labelColumn) << "Category"
         << "   "
         << right << setw(amountColumn) << "Amount" << '\n';

    cout << divider << '\n';

    for (const auto &category : categoryTotals)
    {
        cout << left << setw(labelColumn) << category.first
             << "   "
             << right << setw(amountColumn)
             << formatMoney(category.second) << '\n';
    }

    cout << divider << '\n';

    cout << left << setw(labelColumn) << totalLabel
         << "   "
         << right << setw(amountColumn) << totalAmount << '\n';

    cout << divider << '\n';

    cout.flags(originalFlags);
}