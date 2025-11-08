#include "Transactions.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

static std::string csvEscape(const std::string& s) {
    bool needQuotes = s.find_first_of(",\"\n\r") != std::string::npos;
    std::string out = s;
    size_t pos = 0;
    while ((pos = out.find('"', pos)) != std::string::npos) {
        out.insert(pos, 1, '"'); // escape double quotes
        pos += 2;
    }
    return needQuotes ? "\"" + out + "\"" : out;
}

static std::string csvUnescape(std::string s) {
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
        s = s.substr(1, s.size() - 2);
        std::string out;
        out.reserve(s.size());
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i] == '"' && i + 1 < s.size() && s[i + 1] == '"') { out.push_back('"'); ++i; }
            else out.push_back(s[i]);
        }
        return out;
    }
    return s;
}

Transaction::Transaction()
    : date("1970-01-01"), category("Unknown"), description(""),
    amount(0.0), type(TransactionType::Expense) {
}

Transaction::Transaction(const std::string& date,
    const std::string& category,
    const std::string& description,
    double amount,
    TransactionType type)
    : date(date), category(category), description(description),
    amount(amount), type(type) {
}

std::string Transaction::getDate() const { return date; }
std::string Transaction::getCategory() const { return category; }
std::string Transaction::getDescription() const { return description; }
double Transaction::getAmount() const { return amount; }
TransactionType Transaction::getType() const { return type; }

void Transaction::setDate(const std::string& d) { date = d; }
void Transaction::setCategory(const std::string& c) { category = c; }
void Transaction::setDescription(const std::string& desc) { description = desc; }
void Transaction::setAmount(double amt) { amount = amt; }
void Transaction::setType(TransactionType t) { type = t; }

void Transaction::print() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[ " << date
        << " | " << std::setw(7) << typeToString(type)
        << " | " << std::setw(12) << category
        << " | $" << std::setw(9) << amount
        << " | " << description << " ]\n";
}

std::string Transaction::toCSV(bool includeHeader) const {
    std::ostringstream oss;
    if (includeHeader) {
        oss << "date,category,description,amount,type\n";
    }
    oss << csvEscape(date) << ','
        << csvEscape(category) << ','
        << csvEscape(description) << ','
        << amount << ','
        << typeToString(type);
    return oss.str();
}

static bool csvSplit(const std::string& line, std::string cols[5]) {
    // Simple CSV split supporting quotes
    size_t i = 0, col = 0;
    while (i < line.size() && col < 5) {
        if (line[i] == '"') {
            // quoted
            size_t j = i + 1;
            std::string acc;
            while (j < line.size()) {
                if (line[j] == '"' && j + 1 < line.size() && line[j + 1] == '"') {
                    acc.push_back('"'); j += 2;
                }
                else if (line[j] == '"' && (j + 1 == line.size() || line[j + 1] == ',')) {
                    j++; // consume closing quote
                    break;
                }
                else acc.push_back(line[j++]);
            }
            cols[col++] = acc;
            if (j < line.size() && line[j] == ',') j++;
            i = j;
        }
        else {
            size_t j = line.find(',', i);
            if (j == std::string::npos) j = line.size();
            cols[col++] = line.substr(i, j - i);
            i = (j < line.size() ? j + 1 : j);
        }
    }
    return col == 5;
}

Transaction Transaction::fromCSV(const std::string& line, bool hasHeaderLine) {
    if (hasHeaderLine) {
        // Not used here; kept for API symmetry
    }
    std::string cols[5];
    if (!csvSplit(line, cols)) {
        // Fallback extremely naive split
        std::istringstream iss(line);
        std::string date, category, description, typeStr, amountStr;
        getline(iss, date, ',');
        getline(iss, category, ',');
        getline(iss, description, ',');
        getline(iss, amountStr, ',');
        getline(iss, typeStr, ',');
        double amount = std::stod(amountStr);
        return Transaction(date, category, description, amount, stringToType(typeStr));
    }
    double amount = std::stod(cols[3]);
    return Transaction(cols[0], cols[1], cols[2], amount, stringToType(cols[4]));
}

std::string Transaction::typeToString(TransactionType type) {
    switch (type) {
    case TransactionType::Income:  return "income";
    case TransactionType::Expense: return "expense";
    default: return "unknown";
    }
}

TransactionType Transaction::stringToType(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower == "income")  return TransactionType::Income;
    if (lower == "expense") return TransactionType::Expense;
    return TransactionType::Expense;
}
