#include "expense_functions.h"

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace {

string trim(const string& text) {
    const string whitespace = " \t\r\n";
    const size_t first = text.find_first_not_of(whitespace);

    if (first == string::npos) {
        return "";
    }

    const size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

int findExpenseIndex(const vector<Expense>& expenses, int id) {
    for (size_t index = 0; index < expenses.size(); ++index) {
        if (expenses[index].id == id) {
            return static_cast<int>(index);
        }
    }

    return -1;
}

bool parsePositiveId(const string& text, int& id) {
    string value = trim(text);

    if (value.empty()) {
        return false;
    }

    for (char character : value) {
        if (!isdigit(static_cast<unsigned char>(character))) {
            return false;
        }
    }

    try {
        size_t charactersRead = 0;
        id = stoi(value, &charactersRead);

        return charactersRead == value.size() && id > 0;
    } catch (...) {
        return false;
    }
}

bool parsePositiveAmount(const string& text,
                         long long& amountCentavos) {
    string value = trim(text);

    if (value.empty() || value[0] == '-') {
        return false;
    }

    size_t decimalPosition = value.find('.');

    string pesos;
    string centavos;

    if (decimalPosition == string::npos) {
        pesos = value;
        centavos = "00";
    } else {
        pesos = value.substr(0, decimalPosition);
        centavos = value.substr(decimalPosition + 1);

        if (value.find('.', decimalPosition + 1) != string::npos ||
            centavos.empty() ||
            centavos.size() > 2) {
            return false;
        }

        if (centavos.size() == 1) {
            centavos += "0";
        }
    }

    if (pesos.empty()) {
        pesos = "0";
    }

    for (char character : pesos) {
        if (!isdigit(static_cast<unsigned char>(character))) {
            return false;
        }
    }

    for (char character : centavos) {
        if (!isdigit(static_cast<unsigned char>(character))) {
            return false;
        }
    }

    try {
        long long pesoValue = stoll(pesos);
        long long centavoValue = stoll(centavos);

        amountCentavos = pesoValue * 100 + centavoValue;
        return amountCentavos > 0;
    } catch (...) {
        return false;
    }
}

bool containsComma(const string& value) {
    return value.find(',') != string::npos;
}

string readRequiredText(const string& prompt) {
    while (true) {
        cout << prompt;

        string value;
        getline(cin, value);

        value = trim(value);

        if (value.empty()) {
            cout << "Input cannot be empty.\n";
            continue;
        }

        if (containsComma(value)) {
            cout << "Input cannot contain a comma because "
                 << "the file uses CSV format.\n";
            continue;
        }

        return value;
    }
}

int readExpenseId() {
    while (true) {
        cout << "Enter expense ID: ";

        string input;
        getline(cin, input);

        int id = 0;

        if (parsePositiveId(input, id)) {
            return id;
        }

        cout << "Invalid ID. Enter a positive whole number.\n";
    }
}

long long readAmountCentavos() {
    while (true) {
        cout << "Enter amount: ";

        string input;
        getline(cin, input);

        long long amountCentavos = 0;

        if (parsePositiveAmount(input, amountCentavos)) {
            return amountCentavos;
        }

        cout << "Invalid amount. Enter a positive amount, "
             << "such as 150 or 150.50.\n";
    }
}

bool confirmDeletion() {
    while (true) {
        cout << "Confirm deletion? (y/n): ";

        string answer;
        getline(cin, answer);

        answer = trim(answer);

        if (answer == "y" || answer == "Y") {
            return true;
        }

        if (answer == "n" || answer == "N") {
            return false;
        }

        cout << "Please enter y or n.\n";
    }
}

string formatAmount(long long amountCentavos) {
    ostringstream output;

    output << amountCentavos / 100 << '.';

    int cents = static_cast<int>(amountCentavos % 100);

    if (cents < 10) {
        output << '0';
    }

    output << cents;

    return output.str();
}

} // namespace

bool editExpense(vector<Expense>& expenses) {
    if (expenses.empty()) {
        cout << "No expenses recorded.\n";
        return false;
    }

    int id = readExpenseId();
    int index = findExpenseIndex(expenses, id);

    if (index == -1) {
        cout << "No expense found with ID "
             << id << ".\n";
        return false;
    }

    Expense& expense = expenses[index];

    cout << "\nEditing expense ID "
         << expense.id << '\n';

    cout << "Current date: "
         << expense.date << '\n';

    cout << "Current category: "
         << expense.category << '\n';

    cout << "Current description: "
         << expense.description << '\n';

    cout << "Current amount: PHP "
         << formatAmount(expense.amountCentavos)
         << "\n\n";

    expense.date = readRequiredText("Enter new date: ");
    expense.category = readRequiredText("Enter new category: ");
    expense.description = readRequiredText("Enter new description: ");
    expense.amountCentavos = readAmountCentavos();

    cout << "Expense ID "
         << expense.id
         << " was updated successfully.\n";

    return true;
}

bool deleteExpense(vector<Expense>& expenses) {
    if (expenses.empty()) {
        cout << "No expenses recorded.\n";
        return false;
    }

    int id = readExpenseId();
    int index = findExpenseIndex(expenses, id);

    if (index == -1) {
        cout << "No expense found with ID "
             << id << ".\n";
        return false;
    }

    const Expense& expense = expenses[index];

    cout << "\nSelected expense:\n";
    cout << "ID: " << expense.id << '\n';
    cout << "Date: " << expense.date << '\n';
    cout << "Category: " << expense.category << '\n';
    cout << "Description: " << expense.description << '\n';
    cout << "Amount: PHP "
         << formatAmount(expense.amountCentavos)
         << '\n';

    if (!confirmDeletion()) {
        cout << "Deletion cancelled.\n";
        return false;
    }

    expenses.erase(expenses.begin() + index);

    cout << "Expense ID "
         << id
         << " was deleted successfully.\n";

    return true;
}