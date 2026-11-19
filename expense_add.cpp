#include "expense_functions.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <limits>

using namespace std;

namespace
{
    bool isValidDate(const string &date)
    {
        if (date.length() != 10 || date[4] != '-' || date[7] != '-')
            return false;

        for (size_t i = 0; i < date.length(); ++i)
        {
            if (i == 4 || i == 7)
                continue;

            if (date[i] < '0' || date[i] > '9')
                return false;
        }

        int year = stoi(date.substr(0, 4));
        int month = stoi(date.substr(5, 2));
        int day = stoi(date.substr(8, 2));

        if (year < 1 || month < 1 || month > 12)
            return false;

        int daysInMonth[] = {
            31, 28, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31};

        bool isLeapYear =
            (year % 400 == 0) ||
            (year % 4 == 0 && year % 100 != 0);

        if (isLeapYear)
            daysInMonth[1] = 29;

        return day >= 1 && day <= daysInMonth[month - 1];
    }
}

bool addExpense(vector<Expense> &expenses)
{
    Expense newExpense;

    const vector<string> categories = {
        "Food",
        "Transportation",
        "Utilities",
        "Rent",
        "Shopping",
        "Healthcare",
        "Entertainment",
        "Education",
        "Other"};

    cout << "\n--- Add Expense ---\n";

    cout << "Enter date (YYYY-MM-DD): ";
    getline(cin >> ws, newExpense.date);

    if (!isValidDate(newExpense.date))
    {
        cout << "Invalid date. Enter a valid date in YYYY-MM-DD format.\n";
        return false;
    }

    cout << "\nCategories:\n";

    for (size_t i = 0; i < categories.size(); ++i)
    {
        cout << i + 1 << ". " << categories[i] << '\n';
    }

    cout << "Select category (1-" << categories.size() << "): ";

    string categoryChoice;
    getline(cin, categoryChoice);

    bool validCategory = false;

    for (size_t i = 0; i < categories.size(); ++i)
    {
        if (categoryChoice == to_string(i + 1))
        {
            newExpense.category = categories[i];
            validCategory = true;
            break;
        }
    }

    if (!validCategory)
    {
        cout << "Invalid category. Select a number from the menu.\n";
        return false;
    }

    cout << "Enter description: ";
    getline(cin, newExpense.description);

    if (newExpense.description.find_first_not_of(" \t\r\n") == string::npos ||
    newExpense.description.find(',') != string::npos)
    {
    cout << "Invalid description. It cannot be empty or contain commas.\n";
    return false;
    }

    double amount;

    cout << "Enter amount: PHP ";

    if (!(cin >> amount))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Invalid amount. Please enter a positive number.\n";
        return false;
    }

    // Discard the remaining input, including the newline.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (!isfinite(amount) || amount <= 0)
    {
        cout << "Invalid amount. Please enter a positive number.\n";
        return false;
    }

    newExpense.amountCentavos =
        static_cast<long long>(round(amount * 100));

    int highestId = accumulate(
        expenses.begin(), expenses.end(), 0,
        [](int highest, const Expense &expense)
        {
            return max(highest, expense.id);
        });

    newExpense.id = highestId + 1;
    expenses.push_back(newExpense);

    cout << fixed << setprecision(2);
    cout << "Expense added successfully!\n";
    cout << "Expense ID: " << newExpense.id << "\n";

    return true;
}
