#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <vector>
#include <string>
#include <unordered_map>
#include "Transactions.h"

struct MonthlySummary {
    double income = 0.0;
    double expense = 0.0;
    double net() const { return income - expense; }
};

class Account {
private:
    std::vector<Transaction> transactions;
    // Budgets: either per-category monthly or an overall monthly cap
    double monthlyOverallBudget = 0.0; // 0 => disabled
    std::unordered_map<std::string, double> monthlyCategoryBudgets; // category -> cap

public:
    // CRUD
    void add(const Transaction& t);
    bool removeIndex(size_t idx);
    const std::vector<Transaction>& all() const;

    // Sort
    void sortByDate(bool ascending = true);
    void sortByAmount(bool ascending = true);

    // Filter
    std::vector<Transaction> filter(
        const std::string& typeStr,              // "", "income", "expense"
        const std::string& categorySubstr,       // case-insensitive substring
        const std::string& startDate,            // inclusive, "" to ignore
        const std::string& endDate               // inclusive, "" to ignore
    ) const;

    // Aggregates
    MonthlySummary monthlySummary(int year, int month) const;
    std::unordered_map<std::string, double> totalsByCategory(
        const std::string& startDate = "",
        const std::string& endDate = ""
    ) const;

    double currentBalance() const; // all time income - expense

    // Budgets
    void setMonthlyOverallBudget(double amount);
    void setMonthlyCategoryBudget(const std::string& category, double amount);
    double getMonthlyOverallBudget() const;
    double getMonthlyCategoryBudget(const std::string& category) const;

    struct BudgetAlert {
        bool overallExceeded = false;
        std::vector<std::string> exceededCategories;
    };

    BudgetAlert checkBudgetAlerts(int year, int month) const;

    // Persistence passthrough helpers
    void clear();
    void bulkLoad(std::vector<Transaction> list);
};

#endif
