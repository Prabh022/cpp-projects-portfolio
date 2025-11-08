# 💰 Personal Finance Tracker (C++17)

A command-line C++ application to record, manage, and analyze personal financial transactions. It allows adding income/expenses, viewing reports, setting budgets, and exporting data to CSV files.

---

## 🚀 Features

- Add / Remove Transactions  
  - Income and expense records with date, category, description, and amount.
- View & Sort  
  - Sort by date or amount (ascending/descending).
- Filter  
  - Filter transactions by type, category, or date range.
- Monthly Summary  
  - Displays total income, total expenses, and net balance for a given month.
- Category Totals  
  - Summarizes spending or income grouped by category.
- Budgets  
  - Set monthly overall and per-category budgets with automatic alerts.
- CSV File Storage  
  - Data is persisted to `transactions.csv` using safe CSV formatting.
- Clean OOP Design  
  - Modular classes for `Transaction`, `Account`, `FileManager`, and `utils`.

---

## 🧱 Project Structure

finance-tracker/
├── Account.h / Account.cpp        # Manage all transactions, summaries, and budgets
├── Transaction.h / Transaction.cpp # Represents one income/expense record
├── FileManager.h / FileManager.cpp # Handles CSV read/write
├── utils.h / utils.cpp             # Helper utilities (date, string, validation)
├── main.cpp                        # CLI interface and user interaction
└── transactions.csv                # Saved data file (auto-created at runtime)

---

## 🧠 Concepts Practiced

- Object-Oriented Programming (Encapsulation, modular design, class interaction)
- File I/O with CSV parsing and writing
- STL Containers (`std::vector`, `std::unordered_map`, and algorithms like `sort`, `transform`)
- Enums & Type Safety (`enum class TransactionType { Income, Expense }`)
- Error Handling & Input Validation
- CLI Interaction and formatted console output

---

## ⚙️ Build Instructions (Visual Studio 2022)

1. Open Visual Studio → Create New Project → **Empty Project (C++)**
2. Add all `.cpp` and `.h` files to the project.
3. Go to **Project → Properties → C/C++ → Language → C++ Language Standard**  
   → Select **ISO C++17 (/std:c++17)**
4. Press **Ctrl + Shift + B** to build.
5. Press **Ctrl + F5** to run.

💡 The app will automatically create or load `transactions.csv` in the output folder:  
`<project-folder>\x64\Debug\transactions.csv`

---

## 🖥️ Example CLI Session

==== Personal Finance Tracker ====

1. Add Transaction  
2. Remove Transaction  
3. View All  
4. Sort  
5. Filter  
6. Monthly Summary  
7. Category Totals  
8. Budgets  
9. Save  
10. Load  
11. Exit  

Choose: 1

=== Add Transaction ===  
Date (YYYY-MM-DD): 2025-11-08  
Type (income/expense): income  
Category: Salary  
Amount (positive number): 3200  
Description (optional): Full-time job pay  
Added.

---

## 📦 Sample CSV Output

date,category,description,amount,type  
2025-11-08,Salary,Full-time job pay,3200.00,income  
2025-11-10,Groceries,Walmart weekly shopping,85.50,expense  

---

## 🧩 Future Improvements

- Recurring transactions (auto-generate monthly)  
- JSON export/import  
- Graphical summary (using SFML or ImGui)  
- Account login support  
- SQLite backend instead of CSV  

---

## 👤 Author

**Prabhjeet Singh**  
📍 Ottawa, Canada  
💻 Interested in software development, database administration, and tech innovation.  
🌐 GitHub: *(https://github.com/Prabh022)*

---

## 📝 License

This project is released under the **MIT License**.  
You’re free to use, modify, and distribute it with attribution.
