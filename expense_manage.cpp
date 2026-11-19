#include "expense_functions.h"

#include <cctype>
#include <iostream>
#include <sstream>
#include <limits>
#include <string>

using namespace std;

namespace
{

    string trim(const string &text)
    {
        const string whitespace = " \t\r\n";
        const size_t first = text.find_first_not_of(whitespace);

        if (first == string::npos)
        {
            return "";
        }

        const size_t last = text.find_last_not_of(whitespace);
        return text.substr(first, last - first + 1);
    }

    int findExpenseIndex(const vector<Expense> &expenses, int id)
    {
        for (size_t index = 0; index < expenses.size(); ++index)
        {
            if (expenses[index].id == id)
            {
                return static_cast<int>(index);
            }
        }

        return -1;
    }

    bool parsePositiveId(const string &text, int &id)
    {
        string value = trim(text);

        if (value.empty())
        {
            return false;
        }

        for (char character : value)
        {
            if (!isdigit(static_cast<unsigned char>(character)))
            {
                return false;
            }
        }

        try
        {
            size_t charactersRead = 0;
            id = stoi(value, &charactersRead);

            return charactersRead == value.size() && id > 0;
        }
        catch (...)
        {
            return false;
        }
    }

    bool parsePositiveAmount(const string &text,
                             long long &amountCentavos)
    {
        string value = trim(text);

        if (value.empty() || value[0] == '-')
        {
            return false;
        }

        size_t decimalPosition = value.find('.');

        string pesos;
        string centavos;

        if (decimalPosition == string::npos)
        {
            pesos = value;
            centavos = "00";
        }
        else
        {
            pesos = value.substr(0, decimalPosition);
            centavos = value.substr(decimalPosition + 1);

            if (value.find('.', decimalPosition + 1) != string::npos ||
                centavos.empty() ||
                centavos.size() > 2)
            {
                return false;
            }

            if (centavos.size() == 1)
            {
                centavos += "0";
            }
        }

        if (pesos.empty())
        {
            pesos = "0";
        }

        for (char character : pesos)
        {
            if (!isdigit(static_cast<unsigned char>(character)))
            {
                return false;
            }
        }

        for (char character : centavos)
        {
            if (!isdigit(static_cast<unsigned char>(character)))
            {
                return false;
            }
        }

        try
        {
            long long pesoValue = stoll(pesos);
            long long centavoValue = stoll(centavos);

            const long long maxAmount =
                numeric_limits<long long>::max();

            if (pesoValue > (maxAmount - centavoValue) / 100)
            {
                return false;
            }

            amountCentavos = pesoValue * 100 + centavoValue;
            return amountCentavos > 0;
        }
        catch (...)
        {
            return false;
        }
    }

    bool containsComma(const string &value)
    {
        return value.find(',') != string::npos;
    }

    string readRequiredText(const string &prompt)
    {
        while (true)
        {
            cout << prompt;

            string value;
            getline(cin, value);

            value = trim(value);

            if (value.empty())
            {
                cout << "Input cannot be empty.\n";
                continue;
            }

            if (containsComma(value))
            {
                cout << "Input cannot contain a comma because "
                     << "the file uses CSV format.\n";
                continue;
            }

            return value;
        }
    }

    int readExpenseId()
    {
        while (true)
        {
            cout << "Enter expense ID: ";

            string input;
            getline(cin, input);

            int id = 0;

            if (parsePositiveId(input, id))
            {
                return id;
            }

            cout << "Invalid ID. Enter a positive whole number.\n";
        }
    }

    long long readAmountCentavos()
    {
        while (true)
        {
            cout << "Enter amount: ";

            string input;
            getline(cin, input);

            long long amountCentavos = 0;

            if (parsePositiveAmount(input, amountCentavos))
            {
                return amountCentavos;
            }

            cout << "Invalid amount. Enter a positive amount, "
                 << "such as 150 or 150.50.\n";
        }
    }

    bool confirmDeletion()
    {
        while (true)
        {
            cout << "Confirm deletion? (y/n): ";

            string answer;
            getline(cin, answer);

            answer = trim(answer);

            if (answer == "y" || answer == "Y")
            {
                return true;
            }

            if (answer == "n" || answer == "N")
            {
                return false;
            }

            cout << "Please enter y or n.\n";
        }
    }

    string formatAmount(long long amountCentavos)
    {
        ostringstream output;

        output << amountCentavos / 100 << '.';

        int cents = static_cast<int>(amountCentavos % 100);

        if (cents < 10)
        {
            output << '0';
        }

        output << cents;

        return output.str();
    }

    bool isValidDate(const string &date)
    {
        if (date.size() != 10 || date[4] != '-' || date[7] != '-')
            return false;

        for (size_t i = 0; i < date.size(); ++i)
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

        bool leapYear =
            (year % 400 == 0) ||
            (year % 4 == 0 && year % 100 != 0);

        if (leapYear)
            daysInMonth[1] = 29;

        return day >= 1 && day <= daysInMonth[month - 1];
    }

} // namespace

bool editExpense(vector<Expense> &expenses)
{
    if (expenses.empty())
    {
        cout << "No expenses recorded.\n";
        return false;
    }

    int id = readExpenseId();
    int index = findExpenseIndex(expenses, id);

    if (index == -1)
    {
        cout << "No expense found with ID " << id << ".\n";
        return false;
    }

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

    // Save changes only after all input has been completed.
    Expense updatedExpense = expenses[index];

    cout << "\nEditing expense ID " << updatedExpense.id << '\n';
    cout << "Current date: " << updatedExpense.date << '\n';
    cout << "Current category: " << updatedExpense.category << '\n';
    cout << "Current description: "
         << updatedExpense.description << '\n';
    cout << "Current amount: PHP "
         << formatAmount(updatedExpense.amountCentavos) << "\n\n";

    cout << "Press Enter to keep a field's current value.\n\n";

    string input;

    // Date
    while (true)
    {
        cout << "Enter new date (YYYY-MM-DD): ";

        if (!getline(cin, input))
            return false;

        if (input.empty())
            break;

        if (!isValidDate(input))
        {
            cout << "Invalid date. Use YYYY-MM-DD and enter "
                 << "a valid calendar date.\n";
            continue;
        }

        updatedExpense.date = input;
        break;
    }

    // Category
    cout << "\nCategories:\n";

    for (size_t i = 0; i < categories.size(); ++i)
    {
        cout << i + 1 << ". " << categories[i] << '\n';
    }

    while (true)
    {
        cout << "Select new category (1-" << categories.size()
             << ", or Enter to keep current): ";

        if (!getline(cin, input))
            return false;

        if (input.empty())
            break;

        input = trim(input);

        bool validCategory = false;

        for (size_t i = 0; i < categories.size(); ++i)
        {
            if (input == to_string(i + 1))
            {
                updatedExpense.category = categories[i];
                validCategory = true;
                break;
            }
        }

        if (validCategory)
            break;

        cout << "Invalid category. Select a number from the menu.\n";
    }

    // Description
    while (true)
    {
        cout << "Enter new description: ";

        if (!getline(cin, input))
            return false;

        if (input.empty())
            break;

        input = trim(input);

        if (input.empty())
        {
            cout << "Description cannot contain only spaces.\n";
            continue;
        }

        if (containsComma(input))
        {
            cout << "Description cannot contain a comma because "
                 << "the file uses CSV format.\n";
            continue;
        }

        updatedExpense.description = input;
        break;
    }

    // Amount
    while (true)
    {
        cout << "Enter new amount: PHP ";

        if (!getline(cin, input))
            return false;

        if (input.empty())
            break;

        long long amountCentavos = 0;

        if (!parsePositiveAmount(input, amountCentavos))
        {
            cout << "Invalid amount. Enter a positive amount "
                 << "with at most two decimal places, "
                 << "such as 150 or 150.50.\n";
            continue;
        }

        updatedExpense.amountCentavos = amountCentavos;
        break;
    }

    const Expense &original = expenses[index];

    if (updatedExpense.date == original.date &&
        updatedExpense.category == original.category &&
        updatedExpense.description == original.description &&
        updatedExpense.amountCentavos == original.amountCentavos)
    {
        cout << "No changes made.\n";
        return false;
    }

    expenses[index] = updatedExpense;

    cout << "Expense ID " << updatedExpense.id
         << " was updated successfully.\n";

    return true;
}

bool deleteExpense(vector<Expense> &expenses)
{
    if (expenses.empty())
    {
        cout << "No expenses recorded.\n";
        return false;
    }

    int id = readExpenseId();
    int index = findExpenseIndex(expenses, id);

    if (index == -1)
    {
        cout << "No expense found with ID "
             << id << ".\n";
        return false;
    }

    const Expense &expense = expenses[index];

    cout << "\nSelected expense:\n";
    cout << "ID: " << expense.id << '\n';
    cout << "Date: " << expense.date << '\n';
    cout << "Category: " << expense.category << '\n';
    cout << "Description: " << expense.description << '\n';
    cout << "Amount: PHP "
         << formatAmount(expense.amountCentavos)
         << '\n';

    if (!confirmDeletion())
    {
        cout << "Deletion cancelled.\n";
        return false;
    }

    expenses.erase(expenses.begin() + index);

    cout << "Expense ID "
         << id
         << " was deleted successfully.\n";

    return true;
}