#ifndef EXPENSE_TRACKER_H
#define EXPENSE_TRACKER_H

#include "Transaction.h"

#include <vector>
#include <string>

using namespace std;

class ExpenseTracker {

private:

    vector<Transaction> transactions;

    int nextId;

    double monthlyBudget;

    const string transactionFile =
        "data/transactions.txt";

    const string budgetFile =
        "data/budget.txt";


    string toLower(string text) const;

    void printHeader() const;

    void saveTransactions() const;

    void loadTransactions();

    void saveBudget() const;

    void loadBudget();


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

    void run();
};

#endif