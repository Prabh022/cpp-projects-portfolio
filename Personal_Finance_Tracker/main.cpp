#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>

#include "Transactions.h"
#include "Account.h"
#include "FileManager.h"
#include "utils.h"

static const std::string kDBFile = "transactions.csv";

void pauseEnter() {
    std::cout << "\nPress ENTER to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

double promptDouble(const std::string& label) {
    while (true) {
        std::cout << label;
        double v;
        if (std::cin >> v) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return v;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid number. Try again.\n";
    }
}

std::string promptString(const std::string& label, bool allowEmpty = false) {
    while (true) {
        std::cout << label;
        std::string s; std::getline(std::cin, s);
        s = utils::trim(s);
        if (allowEmpty || !s.empty()) return s;
        std::cout << "Cannot be empty. Try again.\n";
    }
}

std::string promptDate(const std::string& label) {
    while (true) {
        std::string d = promptString(label);
        if (utils::isValidDate(d)) return d;
        std::cout << "Enter date as YYYY-MM-DD.\n";
    }
}

TransactionType promptType() {
    while (true) {
        std::string t = utils::toLower(promptString("Type (income/expense): "));
        if (t == "income") return TransactionType::Income;
        if (t == "expense") return TransactionType::Expense;
        std::cout << "Enter 'income' or 'expense'.\n";
    }
}

void printList(const std::vector<Transaction>& list) {
    if (list.empty()) { std::cout << "(no transactions)\n"; return; }
    std::cout << "Idx  Date        Type     Category      Amount      Description\n";
    std::cout << "---- ----------- -------- ------------- ----------- --------------------\n";
    for (size_t i = 0; i < list.size(); ++i) {
        const auto& t = list[i];
        std::cout << std::setw(3) << i << "  "
            << t.getDate() << "  "
            << std::setw(7) << Transaction::typeToString(t.getType()) << "  "
            << std::setw(13) << t.getCategory() << "  "
            << std::fixed << std::setprecision(2) << std::setw(11) << t.getAmount() << "  "
            << t.getDescription() << "\n";
    }
}

void addFlow(Account& acc) {
    std::cout << "\n=== Add Transaction ===\n";
    std::string date = promptDate("Date (YYYY-MM-DD): ");
    TransactionType type = promptType();
    std::string category = promptString("Category: ");
    double amt = promptDouble("Amount (positive number): ");
    std::string desc = promptString("Description (optional): ", true);

    if (amt < 0) { std::cout << "Amount should be positive. Using absolute value.\n"; amt = std::abs(amt); }

    acc.add(Transaction(date, category, desc, amt, type));
    std::cout << "Added.\n";
}

void removeFlow(Account& acc) {
    std::cout << "\n=== Remove Transaction ===\n";
    printList(acc.all());
    if (acc.all().empty()) return;
    size_t idx;
    std::cout << "Index to remove: ";
    if (std::cin >> idx) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (acc.removeIndex(idx)) std::cout << "Removed.\n";
        else std::cout << "Invalid index.\n";
    }
    else {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input.\n";
    }
}

void sortFlow(Account& acc) {
    std::cout << "\n=== Sort ===\n"
        << "1) Date asc   2) Date desc   3) Amount asc   4) Amount desc\nChoose: ";
    int ch; if (!(std::cin >> ch)) { std::cin.clear(); std::cin.ignore(1e9, '\n'); return; }
    std::cin.ignore(1e9, '\n');
    switch (ch) {
    case 1: acc.sortByDate(true); break;
    case 2: acc.sortByDate(false); break;
    case 3: acc.sortByAmount(true); break;
    case 4: acc.sortByAmount(false); break;
    default: std::cout << "No change.\n"; break;
    }
    std::cout << "Sorted.\n";
}

void filterFlow(const Account& acc) {
    std::cout << "\n=== Filter ===\n";
    std::string typeStr = utils::toLower(promptString("Type (income/expense or empty): ", true));
    std::string cat = promptString("Category contains (or empty): ", true);
    std::string start = promptString("Start date YYYY-MM-DD (or empty): ", true);
    std::string end = promptString("End date YYYY-MM-DD (or empty): ", true);

    auto res = acc.filter(typeStr, cat, start, end);
    printList(res);
}

void monthlySummaryFlow(const Account& acc) {
    std::cout << "\n=== Monthly Summary ===\n";
    int y, m;
    std::cout << "Year (e.g., 2025): ";
    if (!(std::cin >> y)) { std::cin.clear(); std::cin.ignore(1e9, '\n'); return; }
    std::cout << "Month (1-12): ";
    if (!(std::cin >> m)) { std::cin.clear(); std::cin.ignore(1e9, '\n'); return; }
    std::cin.ignore(1e9, '\n');

    auto ms = acc.monthlySummary(y, m);
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Income:  $" << ms.income << "\n";
    std::cout << "Expense: $" << ms.expense << "\n";
    std::cout << "Net:     $" << ms.net() << "\n";
}

void categoryTotalsFlow(const Account& acc) {
    std::cout << "\n=== Category Totals ===\n";
    std::string start = promptString("Start date YYYY-MM-DD (or empty): ", true);
    std::string end = promptString("End date YYYY-MM-DD (or empty): ", true);

    auto map = acc.totalsByCategory(start, end);
    if (map.empty()) { std::cout << "(none)\n"; return; }
    std::cout << std::fixed << std::setprecision(2);
    for (auto& kv : map) {
        std::cout << std::setw(14) << kv.first << " : $" << kv.second << "\n";
    }
}

void budgetsFlow(Account& acc) {
    std::cout << "\n=== Budgets ===\n";
    std::cout << "1) Set monthly overall budget\n"
        "2) Set monthly category budget\n"
        "3) Check budget alerts (for month)\n"
        "Choose: ";
    int ch; if (!(std::cin >> ch)) { std::cin.clear(); std::cin.ignore(1e9, '\n'); return; }
    std::cin.ignore(1e9, '\n');

    if (ch == 1) {
        double amt = promptDouble("Overall monthly budget (0 to disable): ");
        acc.setMonthlyOverallBudget(amt);
        std::cout << "Set.\n";
    }
    else if (ch == 2) {
        std::string cat = promptString("Category: ");
        double amt = promptDouble("Monthly budget for this category (0 to disable): ");
        acc.setMonthlyCategoryBudget(cat, amt);
        std::cout << "Set.\n";
    }
    else if (ch == 3) {
        int y, m;
        std::cout << "Year: "; if (!(std::cin >> y)) { std::cin.clear(); std::cin.ignore(1e9, '\n'); return; }
        std::cout << "Month: "; if (!(std::cin >> m)) { std::cin.clear(); std::cin.ignore(1e9, '\n'); return; }
        std::cin.ignore(1e9, '\n');
        auto alert = acc.checkBudgetAlerts(y, m);
        if (!alert.overallExceeded && alert.exceededCategories.empty()) {
            std::cout << "No budget alerts.\n";
        }
        else {
            if (alert.overallExceeded) std::cout << "⚠ Overall monthly budget exceeded.\n";
            for (auto& c : alert.exceededCategories) {
                std::cout << "⚠ Category budget exceeded: " << c << "\n";
            }
        }
    }
}

void saveFlow(const Account& acc) {
    if (FileManager::saveCSV(kDBFile, acc.all()))
        std::cout << "Saved to " << kDBFile << "\n";
    else
        std::cout << "Failed to save.\n";
}

void loadFlow(Account& acc) {
    auto list = FileManager::loadCSV(kDBFile);
    acc.bulkLoad(std::move(list));
    std::cout << "Loaded " << acc.all().size() << " transactions from " << kDBFile << "\n";
}

int main() {
    Account acc;
    // Auto-load if file exists
    acc.bulkLoad(FileManager::loadCSV(kDBFile));

    while (true) {
        std::cout << "\n==== Personal Finance Tracker ====\n"
            << "1. Add Transaction\n"
            << "2. Remove Transaction\n"
            << "3. View All\n"
            << "4. Sort\n"
            << "5. Filter\n"
            << "6. Monthly Summary\n"
            << "7. Category Totals\n"
            << "8. Budgets\n"
            << "9. Save\n"
            << "10. Load\n"
            << "0. Exit\n"
            << "Choose: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1e9, '\n');
            continue;
        }
        std::cin.ignore(1e9, '\n');

        switch (choice) {
        case 1: addFlow(acc); break;
        case 2: removeFlow(acc); break;
        case 3: printList(acc.all()); break;
        case 4: sortFlow(acc); break;
        case 5: filterFlow(acc); break;
        case 6: monthlySummaryFlow(acc); break;
        case 7: categoryTotalsFlow(acc); break;
        case 8: budgetsFlow(acc); break;
        case 9: saveFlow(acc); break;
        case 10: loadFlow(acc); break;
        case 0: std::cout << "Bye.\n"; return 0;
        default: std::cout << "Invalid.\n"; break;
        }
    }
}
