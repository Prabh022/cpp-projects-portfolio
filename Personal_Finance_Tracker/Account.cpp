#include "Account.h"
#include "utils.h"
#include <algorithm>
#include <numeric>
#include <sstream>

void Account::add(const Transaction& t) { transactions.push_back(t); }

bool Account::removeIndex(size_t idx) {
    if (idx >= transactions.size()) return false;
    transactions.erase(transactions.begin() + static_cast<std::ptrdiff_t>(idx));
    return true;
}

const std::vector<Transaction>& Account::all() const { return transactions; }

void Account::sortByDate(bool asc) {
    std::sort(transactions.begin(), transactions.end(), [&](const Transaction& a, const Transaction& b) {
        return asc ? utils::dateLess(a.getDate(), b.getDate())
            : utils::dateLess(b.getDate(), a.getDate());
        });
}

void Account::sortByAmount(bool asc) {
    std::sort(transactions.begin(), transactions.end(), [&](const Transaction& a, const Transaction& b) {
        return asc ? (a.getAmount() < b.getAmount())
            : (a.getAmount() > b.getAmount());
        });
}

std::vector<Transaction> Account::filter(
    const std::string& typeStr,
    const std::string& categorySubstr,
    const std::string& startDate,
    const std::string& endDate
) const {
    std::vector<Transaction> out;
    std::string typeLower = utils::toLower(typeStr);
    std::string catLower = utils::toLower(categorySubstr);

    for (const auto& t : transactions) {
        if (!startDate.empty() || !endDate.empty()) {
            if (!utils::dateInRange(t.getDate(), startDate, endDate)) continue;
        }

        if (!typeLower.empty()) {
            if (typeLower == "income" && t.getType() != TransactionType::Income) continue;
            if (typeLower == "expense" && t.getType() != TransactionType::Expense) continue;
        }

        if (!catLower.empty()) {
            std::string tcat = utils::toLower(t.getCategory());
            if (tcat.find(catLower) == std::string::npos) continue;
        }

        out.push_back(t);
    }
    return out;
}

MonthlySummary Account::monthlySummary(int year, int month) const {
    MonthlySummary ms;
    for (const auto& t : transactions) {
        int y, m, d;
        if (!utils::parseYMD(t.getDate(), y, m, d)) continue;
        if (y == year && m == month) {
            if (t.getType() == TransactionType::Income) ms.income += t.getAmount();
            else ms.expense += t.getAmount();
        }
    }
    return ms;
}

std::unordered_map<std::string, double> Account::totalsByCategory(
    const std::string& startDate,
    const std::string& endDate
) const {
    std::unordered_map<std::string, double> map;
    for (const auto& t : transactions) {
        if (!utils::dateInRange(t.getDate(), startDate, endDate)) continue;
        double signedAmt = (t.getType() == TransactionType::Income) ? t.getAmount() : -t.getAmount();
        map[t.getCategory()] += signedAmt;
    }
    return map;
}

double Account::currentBalance() const {
    double bal = 0.0;
    for (const auto& t : transactions) {
        bal += (t.getType() == TransactionType::Income) ? t.getAmount() : -t.getAmount();
    }
    return bal;
}

void Account::setMonthlyOverallBudget(double amount) { monthlyOverallBudget = amount; }
void Account::setMonthlyCategoryBudget(const std::string& cat, double amount) { monthlyCategoryBudgets[cat] = amount; }
double Account::getMonthlyOverallBudget() const { return monthlyOverallBudget; }
double Account::getMonthlyCategoryBudget(const std::string& cat) const {
    auto it = monthlyCategoryBudgets.find(cat);
    return (it == monthlyCategoryBudgets.end()) ? 0.0 : it->second;
}

Account::BudgetAlert Account::checkBudgetAlerts(int year, int month) const {
    BudgetAlert alert{};
    // Overall expense this month
    double monthExpense = 0.0;
    std::unordered_map<std::string, double> catExpense;

    for (const auto& t : transactions) {
        int y, m, d;
        if (!utils::parseYMD(t.getDate(), y, m, d)) continue;
        if (y == year && m == month && t.getType() == TransactionType::Expense) {
            monthExpense += t.getAmount();
            catExpense[t.getCategory()] += t.getAmount();
        }
    }
    if (monthlyOverallBudget > 0.0 && monthExpense > monthlyOverallBudget) {
        alert.overallExceeded = true;
    }

    for (const auto& kv : monthlyCategoryBudgets) {
        const std::string& cat = kv.first;
        double cap = kv.second;
        double spent = catExpense[cat];
        if (cap > 0.0 && spent > cap) alert.exceededCategories.push_back(cat);
    }
    return alert;
}

void Account::clear() { transactions.clear(); }
void Account::bulkLoad(std::vector<Transaction> list) { transactions = std::move(list); }
