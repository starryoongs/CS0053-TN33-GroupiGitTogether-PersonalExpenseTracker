#include "expense_functions.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <system_error>
#include <vector>

using namespace std;

int main() {
    const string filename = "expenses.csv";
    vector<Expense> expenses;
    bool unsavedChanges = false;

    error_code fileError;
    bool fileExists = filesystem::exists(filename, fileError);

    if (fileError) {
        cout << "Cannot check the expense file.\n";
        return 1;
    }

    if (fileExists) {
        if (!loadExpenses(expenses, filename)) {
            cout << "Cannot load the expense file. "
                 << "Check it before continuing.\n";
            return 1;
        }

        cout << "Saved expenses loaded.\n";
    } else {
        cout << "No saved file found. Starting with no expenses.\n";
    }

    while (true) {
        cout << "\n========================================\n"
            << "        PERSONAL EXPENSE TRACKER\n"
            << "========================================\n"
            << "[1] Add expense\n"
            << "[2] View expenses\n"
            << "[3] Search/filter expenses\n"
            << "[4] Edit expense\n"
            << "[5] Delete expense\n"
            << "[6] Spending summary\n"
            << "---------------------------------"
            << "[0] Exit\n"
            << "========================================\n"
            << "Choose an option: ";

        string choice;

        if (!getline(cin, choice)) {
            if (unsavedChanges && !saveExpenses(expenses, filename)) {
                cout << "\nCould not save unsaved changes.\n";
                return 1;
            }

            return 0;
        }

        if (choice == "0") {
            if (unsavedChanges && !saveExpenses(expenses, filename)) {
                cout << "Saving failed. Your changes are still "
                     << "in memory. Please try again before exiting.\n";
                continue;
            }

            cout << "Goodbye!\n";
            break;
        }

        bool changed = false;

        if (choice == "1") {
            changed = addExpense(expenses);
        } else if (choice == "2") {
            viewExpenses(expenses);
        } else if (choice == "3") {
            searchExpenses(expenses);
        } else if (choice == "4") {
            changed = editExpense(expenses);
        } else if (choice == "5") {
            changed = deleteExpense(expenses);
        } else if (choice == "6") {
            showSpendingSummary(expenses);
        } else {
            cout << "Invalid option. Enter a number from 0 to 6.\n";
        }

        if (changed) {
            unsavedChanges = true;

            if (saveExpenses(expenses, filename)) {
                unsavedChanges = false;
                cout << "Changes saved.\n";
            } else {
                cout << "Saving failed. Changes remain in memory.\n";
            }
        }
    }

    return 0;
}