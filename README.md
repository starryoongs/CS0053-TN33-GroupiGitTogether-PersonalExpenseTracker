Personal Expense Tracker

## Project Purpose

The Personal Expense Tracker is a console-based C++ application that helps users record, organize, and monitor their expenses. Users can manage expense records, find specific transactions, and review monthly and category spending totals. Records are saved to a local file and loaded when the program starts so they remain available between sessions.

## Features

- **Add expense:** Enter a date, category, description, and positive amount. Each expense is assigned a unique ID.
- **View expenses:** Display expense records in a readable table, with a helpful message when no records exist.
- **Search/filter expenses:** Find expenses by category, date range, or description.
- **Edit expense:** Select an expense by ID and update its details.
- **Delete expense:** Select an expense by ID and confirm before removing it.
- **Spending summary:** View total spending for a selected month and totals by category.
- **Save and load records:** Store expenses in a local file and load them for future use.

## Required Compiler

Use the G++ C++ compiler with support for C++17 or later. Ensure that `g++` is available in your terminal.

Check the compiler installation:

```powershell
g++ --version
```

## Compilation and Run Instructions

The commands below use Windows PowerShell and the source filenames specified in the project plan.

1. Open a terminal in the project folder containing `main.cpp`, the other `.cpp` files, `expense.h`, and `expense_functions.h`.
2. Compile all source files:

   ```powershell
   g++ -std=c++17 main.cpp expense_view.cpp expense_add.cpp expense_summary.cpp expense_manage.cpp storage.cpp -o expense_tracker.exe
   ```

3. Run the application from the same folder:

   ```powershell
   .\expense_tracker.exe
   ```

4. Follow the on-screen menu to manage expenses and view spending summaries.

## Sample Input


## Sample Output