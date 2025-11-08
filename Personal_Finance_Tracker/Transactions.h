#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <iostream>

enum class TransactionType {
    Income,
    Expense
};

class Transaction {
private:
    std::string date;        // Format: YYYY-MM-DD
    std::string category;
    std::string description;
    double amount;           // Positive number; sign is implied by type
    TransactionType type;

public:
    // Constructors
    Transaction();
    Transaction(const std::string& date,
        const std::string& category,
        const std::string& description,
        double amount,
        TransactionType type);

    // Getters
    std::string getDate() const;
    std::string getCategory() const;
    std::string getDescription() const;
    double getAmount() const;
    TransactionType getType() const;

    // Setters
    void setDate(const std::string& d);
    void setCategory(const std::string& c);
    void setDescription(const std::string& desc);
    void setAmount(double amt);
    void setType(TransactionType t);

    // Display
    void print() const;

    // CSV handling
    std::string toCSV(bool includeHeader = false) const;
    static Transaction fromCSV(const std::string& line, bool hasHeaderLine = false);

    // Helpers
    static std::string typeToString(TransactionType type);
    static TransactionType stringToType(const std::string& str);
};

#endif // TRANSACTION_H
