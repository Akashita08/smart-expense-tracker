#ifndef EXPENSE_TRACKER_H
#define EXPENSE_TRACKER_H

#include "Transaction.h"

#include <string>
#include <vector>
#include <unordered_map>

class ExpenseTracker {

private:

    std::vector<Transaction> transactions;

    int nextId;

    // month-year -> budget
    std::unordered_map<std::string, double> monthlyBudgets;

    const std::string transactionFile = "data/transactions.txt";
    const std::string budgetFile = "data/budget.txt";

    std::string toLower(std::string text) const;

    bool isValidDate(const std::string& date) const;
    bool isValidMonth(std::string month) const;

    std::string getMonthYear(const std::string& date) const;

    void printHeader() const;

    void saveTransactions() const;
    void loadTransactions();

    void saveBudgets() const;
    void loadBudgets();

    void showMonthlyAnalytics(const std::string& month) const;

public:

    ExpenseTracker();

    void addTransaction();
    void displayTransactions() const;
    void deleteTransaction();

    void searchTransactions() const;
    void filterTransactions() const;
    void sortTransactions();

    void showBalance() const;

    void showAnalytics() const;

    void setBudget();
    void showBudgetStatus() const;

    void showMonthlyTransactions() const;

    void run();
};

#endif