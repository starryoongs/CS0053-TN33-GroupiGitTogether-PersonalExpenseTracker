#include "expense_functions.h"

#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
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

            amountCentavos = pesoValue * 100 + centavoValue;
            return amountCentavos > 0;
        }
        catch (...)
        {
            return false;
        }
    }

    bool validCsvField(const string &value)
    {
        return !value.empty() &&
               value.find(',') == string::npos &&
               value.find('\n') == string::npos &&
               value.find('\r') == string::npos;
    }

    string formatAmount(long long amountCentavos)
    {
        ostringstream output;

        output << amountCentavos / 100
               << '.'
               << setfill('0')
               << setw(2)
               << amountCentavos % 100;

        return output.str();
    }

    bool parseExpenseLine(const string &line,
                          Expense &expense)
    {
        stringstream input(line);

        string idText;
        string amountText;

        if (!getline(input, idText, ','))
        {
            return false;
        }

        if (!getline(input, expense.date, ','))
        {
            return false;
        }

        if (!getline(input, expense.category, ','))
        {
            return false;
        }

        if (!getline(input, expense.description, ','))
        {
            return false;
        }

        if (!getline(input, amountText))
        {
            return false;
        }

        if (!parsePositiveId(idText, expense.id))
        {
            return false;
        }

        if (!parsePositiveAmount(amountText,
                                 expense.amountCentavos))
        {
            return false;
        }

        expense.date = trim(expense.date);
        expense.category = trim(expense.category);
        expense.description = trim(expense.description);

        return validCsvField(expense.date) &&
               validCsvField(expense.category) &&
               validCsvField(expense.description);
    }

} // namespace

bool saveExpenses(const vector<Expense> &expenses,
                  const string &filename)
{
    ofstream output(filename);

    if (!output)
    {
        cerr << "Error: Could not open "
             << filename
             << " for writing.\n";
        return false;
    }

    for (const Expense &expense : expenses)
    {
        if (expense.id <= 0 ||
            expense.amountCentavos <= 0 ||
            !validCsvField(expense.date) ||
            !validCsvField(expense.category) ||
            !validCsvField(expense.description))
        {
            cerr << "Error: Invalid expense record with ID "
                 << expense.id
                 << ".\n";
            return false;
        }

        output << expense.id << ','
               << expense.date << ','
               << expense.category << ','
               << expense.description << ','
               << formatAmount(expense.amountCentavos)
               << '\n';
    }

    if (!output)
    {
        cerr << "Error: Failed while writing to "
             << filename
             << ".\n";
        return false;
    }

    return true;
}

bool loadExpenses(vector<Expense> &expenses,
                  const string &filename)
{
    ifstream input(filename);

    if (!input)
    {
        expenses.clear();
        return true;
    }

    vector<Expense> loadedExpenses;
    set<int> usedIds;

    string line;
    int lineNumber = 0;

    while (getline(input, line))
    {
        ++lineNumber;

        if (trim(line).empty())
        {
            continue;
        }

        Expense expense{};

        if (!parseExpenseLine(line, expense))
        {
            cerr << "Warning: Skipping malformed record on line "
                 << lineNumber
                 << ".\n";
            continue;
        }

        if (usedIds.count(expense.id) > 0)
        {
            cerr << "Warning: Skipping duplicate ID "
                 << expense.id
                 << " on line "
                 << lineNumber
                 << ".\n";
            continue;
        }

        usedIds.insert(expense.id);
        loadedExpenses.push_back(expense);
    }

    if (input.bad())
    {
        cerr << "Error: Problem occurred while reading "
             << filename
             << ".\n";
        return false;
    }

    expenses = loadedExpenses;
    return true;
}