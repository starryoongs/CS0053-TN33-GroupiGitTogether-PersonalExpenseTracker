#include "expense_functions.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace {

string toLowercase(string text) {
    transform(text.begin(), text.end(), text.begin(),
              [](unsigned char character) {
                  return static_cast<char>(tolower(character));
              });

    return text;
}

string formatAmount(long long centavos) {
    ostringstream output;

    output << "PHP " << centavos / 100 << "."
           << setfill('0') << setw(2) << centavos % 100;

    return output.str();
}

void printTableHeader() {
    cout << left
         << setw(8) << "ID"
         << setw(14) << "Date"
         << setw(20) << "Category"
         << right << setw(18) << "Amount"
         << "  Description\n"
         << string(85, '-') << '\n';
}

void printExpense(const Expense& expense) {
    cout << left
         << setw(8) << expense.id
         << setw(14) << expense.date
         << setw(20) << expense.category
         << right << setw(18) << formatAmount(expense.amountCentavos)
         << "  " << expense.description << '\n';
}

} 

void viewExpenses(const vector<Expense>& expenses) {
    cout << "\n=== ALL EXPENSES ===\n";

    if (expenses.empty()) {
        cout << "No expenses recorded.\n";
        return;
    }

    printTableHeader();

    for (const Expense& expense : expenses) {
        printExpense(expense);
    }

    cout << "\nRecords displayed: " << expenses.size() << '\n';
}

void searchExpenses(const vector<Expense>& expenses) {
    cout << "\n=== SEARCH / FILTER EXPENSES ===\n";

    if (expenses.empty()) {
        cout << "No expenses available to search.\n";
        return;
    }

    cout << "Enter a category, description, or date.\n"
         << "For dates, use YYYY-MM-DD or YYYY-MM for a month.\n"
         << "Search: ";

    string keyword;

    if (!getline(cin, keyword)) {
        cout << "\nSearch cancelled because input was closed.\n";
        return;
    }

    const string whitespace = " \t\r\n";
    size_t first = keyword.find_first_not_of(whitespace);

    if (first == string::npos) {
        cout << "Search cannot be empty.\n";
        return;
    }

    size_t last = keyword.find_last_not_of(whitespace);
    keyword = toLowercase(keyword.substr(first, last - first + 1));

    size_t matchCount = 0;

    for (const Expense& expense : expenses) {
        bool matches =
            toLowercase(expense.category).find(keyword) != string::npos ||
            toLowercase(expense.description).find(keyword) != string::npos ||
            expense.date.find(keyword) != string::npos;

        if (matches) {
            if (matchCount == 0) {
                printTableHeader();
            }

            printExpense(expense);
            ++matchCount;
        }
    }

    if (matchCount == 0) {
        cout << "No matching expenses found.\n";
    } else {
        cout << "\nMatching records: " << matchCount << '\n';
    }
}