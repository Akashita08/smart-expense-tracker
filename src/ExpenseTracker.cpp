#include "../include/ExpenseTracker.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <limits>

ExpenseTracker::ExpenseTracker() {

    nextId = 1;

    loadTransactions();
    loadBudgets();
}

// --------------------------------------------------
// Utility Functions
// --------------------------------------------------

std::string ExpenseTracker::toLower(std::string text) const {

    for (char& c : text) {
        c = static_cast<char>(
            std::tolower(static_cast<unsigned char>(c))
        );
    }

    return text;
}

bool ExpenseTracker::isValidDate(const std::string& date) const {

    if (date.length() != 10) {
        return false;
    }

    if (date[2] != '-' || date[5] != '-') {
        return false;
    }

    for (int i = 0; i < 10; i++) {

        if (i == 2 || i == 5) {
            continue;
        }

        if (!std::isdigit(
                static_cast<unsigned char>(date[i])
            )) {
            return false;
        }
    }

    int day = std::stoi(date.substr(0, 2));
    int month = std::stoi(date.substr(3, 2));

    if (month < 1 || month > 12) {
        return false;
    }

    if (day < 1 || day > 31) {
        return false;
    }

    return true;
}

bool ExpenseTracker::isValidMonth(std::string month) const {

    if (month.length() != 7) {
        return false;
    }

    if (month[2] != '-') {
        return false;
    }

    for (int i = 0; i < 7; i++) {

        if (i == 2) {
            continue;
        }

        if (!std::isdigit(
                static_cast<unsigned char>(month[i])
            )) {
            return false;
        }
    }

    int selectedMonth = std::stoi(month.substr(0, 2));
    int selectedYear = std::stoi(month.substr(3, 4));

    if (selectedMonth < 1 || selectedMonth > 12) {
        return false;
    }

    if (selectedYear < 2000 || selectedYear > 2100) {
        return false;
    }

    return true;
}

std::string ExpenseTracker::getMonthYear(
    const std::string& date
) const {

    if (date.length() != 10) {
        return "";
    }

    return date.substr(3, 2) + "-" + date.substr(6, 4);
}

// --------------------------------------------------
// Display Helpers
// --------------------------------------------------

void ExpenseTracker::printHeader() const {

    std::cout << std::left
              << std::setw(5) << "ID"
              << std::setw(12) << "Type"
              << std::setw(12) << "Amount"
              << std::setw(15) << "Category"
              << std::setw(25) << "Description"
              << "Date"
              << std::endl;

    std::cout << std::string(85, '-') << std::endl;
}

// --------------------------------------------------
// Add Transaction
// --------------------------------------------------

void ExpenseTracker::addTransaction() {

    std::string type;
    std::string category;
    std::string description;
    std::string date;

    double amount;

    // Type
    while (true) {

        std::cout << "Enter type (income/expense): ";
        std::cin >> type;

        type = toLower(type);

        if (type == "income" || type == "expense") {
            break;
        }

        std::cout << "Invalid type. Enter income or expense.\n";
    }

    // Amount
    while (true) {

        std::cout << "Enter amount: ";

        if (std::cin >> amount &&
            amount > 0) {

            break;
        }

        std::cout << "Amount must be a positive number.\n";

        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    // Category
    while (true) {

        std::cout << "Enter category: ";
        std::getline(std::cin, category);

        if (category.empty()) {
            std::cout << "Category cannot be empty.\n";
            continue;
        }

        if (category.find('|') != std::string::npos) {
            std::cout << "Category cannot contain '|'.\n";
            continue;
        }

        break;
    }

    // Description
    while (true) {

        std::cout << "Enter description: ";
        std::getline(std::cin, description);

        if (description.empty()) {
            std::cout << "Description cannot be empty.\n";
            continue;
        }

        if (description.find('|') != std::string::npos) {
            std::cout << "Description cannot contain '|'.\n";
            continue;
        }

        break;
    }

    // Date
    while (true) {

        std::cout << "Enter date (DD-MM-YYYY): ";
        std::getline(std::cin, date);

        if (isValidDate(date)) {
            break;
        }

        std::cout << "Invalid date format.\n";
    }

    // Normalize category
    category = toLower(category);

    Transaction transaction(
        nextId,
        type,
        amount,
        category,
        description,
        date
    );

    transactions.push_back(transaction);

    nextId++;

    saveTransactions();

    std::cout << "\nTransaction added successfully.\n";
}

// --------------------------------------------------
// Display Transactions
// --------------------------------------------------

void ExpenseTracker::displayTransactions() const {

    if (transactions.empty()) {

        std::cout << "\nNo transactions available.\n";
        return;
    }

    std::cout << "\nAll Transactions\n\n";

    printHeader();

    for (const Transaction& transaction : transactions) {
        transaction.display();
    }
}

// --------------------------------------------------
// Delete Transaction
// --------------------------------------------------

void ExpenseTracker::deleteTransaction() {

    if (transactions.empty()) {

        std::cout << "\nNo transactions available.\n";
        return;
    }

    int id;

    std::cout << "Enter transaction ID to delete: ";

    if (!(std::cin >> id)) {

        std::cout << "Invalid ID.\n";

        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        return;
    }

    auto it = std::find_if(
        transactions.begin(),
        transactions.end(),
        [id](const Transaction& transaction) {
            return transaction.getId() == id;
        }
    );

    if (it == transactions.end()) {

        std::cout << "Transaction not found.\n";
        return;
    }

    transactions.erase(it);

    saveTransactions();

    std::cout << "Transaction deleted successfully.\n";
}

// --------------------------------------------------
// Search
// --------------------------------------------------

void ExpenseTracker::searchTransactions() const {

    if (transactions.empty()) {

        std::cout << "\nNo transactions available.\n";
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    std::string keyword;

    std::cout << "Enter search keyword: ";
    std::getline(std::cin, keyword);

    keyword = toLower(keyword);

    bool found = false;

    printHeader();

    for (const Transaction& transaction : transactions) {

        std::string category =
            toLower(transaction.getCategory());

        std::string description =
            toLower(transaction.getDescription());

        if (category.find(keyword) != std::string::npos ||
            description.find(keyword) != std::string::npos) {

            transaction.display();

            found = true;
        }
    }

    if (!found) {
        std::cout << "No matching transactions found.\n";
    }
}

// --------------------------------------------------
// Filter
// --------------------------------------------------

void ExpenseTracker::filterTransactions() const {

    if (transactions.empty()) {

        std::cout << "\nNo transactions available.\n";
        return;
    }

    int choice;

    std::cout << "\nFilter By\n";
    std::cout << "1. Income\n";
    std::cout << "2. Expense\n";
    std::cout << "3. Category\n";
    std::cout << "Enter choice: ";

    if (!(std::cin >> choice)) {

        std::cout << "Invalid choice.\n";

        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        return;
    }

    printHeader();

    bool found = false;

    if (choice == 1 || choice == 2) {

        std::string requiredType =
            choice == 1 ? "income" : "expense";

        for (const Transaction& transaction : transactions) {

            if (transaction.getType() == requiredType) {

                transaction.display();
                found = true;
            }
        }
    }

    else if (choice == 3) {

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::string category;

        std::cout << "Enter category: ";
        std::getline(std::cin, category);

        category = toLower(category);

        for (const Transaction& transaction : transactions) {

            if (toLower(transaction.getCategory()) == category) {

                transaction.display();
                found = true;
            }
        }
    }

    else {

        std::cout << "Invalid choice.\n";
        return;
    }

    if (!found) {
        std::cout << "No matching transactions found.\n";
    }
}

// --------------------------------------------------
// Sorting
// --------------------------------------------------

void ExpenseTracker::sortTransactions() {

    if (transactions.empty()) {

        std::cout << "\nNo transactions available.\n";
        return;
    }

    int choice;

    std::cout << "\nSort By\n";
    std::cout << "1. Amount: Low to High\n";
    std::cout << "2. Amount: High to Low\n";
    std::cout << "3. ID\n";
    std::cout << "Enter choice: ";

    if (!(std::cin >> choice)) {

        std::cout << "Invalid choice.\n";

        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        return;
    }

    if (choice == 1) {

        std::sort(
            transactions.begin(),
            transactions.end(),
            [](const Transaction& a, const Transaction& b) {
                return a.getAmount() < b.getAmount();
            }
        );
    }

    else if (choice == 2) {

        std::sort(
            transactions.begin(),
            transactions.end(),
            [](const Transaction& a, const Transaction& b) {
                return a.getAmount() > b.getAmount();
            }
        );
    }

    else if (choice == 3) {

        std::sort(
            transactions.begin(),
            transactions.end(),
            [](const Transaction& a, const Transaction& b) {
                return a.getId() < b.getId();
            }
        );
    }

    else {

        std::cout << "Invalid choice.\n";
        return;
    }

    std::cout << "\nTransactions sorted successfully.\n";

    printHeader();

    for (const Transaction& transaction : transactions) {
        transaction.display();
    }
}

// --------------------------------------------------
// Overall Balance
// --------------------------------------------------

void ExpenseTracker::showBalance() const {

    double income = 0;
    double expenses = 0;

    for (const Transaction& transaction : transactions) {

        if (transaction.getType() == "income") {
            income += transaction.getAmount();
        }

        else {
            expenses += transaction.getAmount();
        }
    }

    std::cout << "\n========== BALANCE ==========\n";

    std::cout << "Total Income   : ₹"
              << std::fixed
              << std::setprecision(2)
              << income
              << "\n";

    std::cout << "Total Expenses : ₹"
              << expenses
              << "\n";

    std::cout << "Balance        : ₹"
              << income - expenses
              << "\n";

    std::cout << "=============================\n";
}

// --------------------------------------------------
// Monthly Analytics
// --------------------------------------------------

void ExpenseTracker::showMonthlyAnalytics(
    const std::string& month
) const {

    double income = 0;
    double expenses = 0;

    double highestExpense = 0;
    double totalExpenseCount = 0;

    std::string highestExpenseDescription;

    std::unordered_map<std::string, double> categorySpending;

    for (const Transaction& transaction : transactions) {

        if (getMonthYear(transaction.getDate()) != month) {
            continue;
        }

        if (transaction.getType() == "income") {

            income += transaction.getAmount();
        }

        else {

            double amount = transaction.getAmount();

            expenses += amount;

            totalExpenseCount++;

            if (amount > highestExpense) {

                highestExpense = amount;

                highestExpenseDescription =
                    transaction.getDescription();
            }

            std::string category =
                toLower(transaction.getCategory());

            categorySpending[category] += amount;
        }
    }

    std::cout << "\n====================================\n";
    std::cout << "      ANALYTICS FOR " << month << "\n";
    std::cout << "====================================\n";

    std::cout << std::fixed
              << std::setprecision(2);

    std::cout << "Total Income       : ₹"
              << income << "\n";

    std::cout << "Total Expenses     : ₹"
              << expenses << "\n";

    std::cout << "Net Balance        : ₹"
              << income - expenses << "\n";

    if (totalExpenseCount > 0) {

        std::cout << "Highest Expense    : ₹"
                  << highestExpense
                  << " (" << highestExpenseDescription << ")\n";

        std::cout << "Average Expense    : ₹"
                  << expenses / totalExpenseCount
                  << "\n";
    }

    else {

        std::cout << "Highest Expense    : None\n";
        std::cout << "Average Expense    : None\n";
    }

    std::cout << "\nCategory Spending\n";
    std::cout << "-----------------------------\n";

    if (categorySpending.empty()) {

        std::cout << "No expenses recorded.\n";
    }

    else {

        for (const auto& entry : categorySpending) {

            double percentage = 0;

            if (expenses > 0) {
                percentage =
                    (entry.second / expenses) * 100;
            }

            std::cout << std::left
                      << std::setw(15)
                      << entry.first
                      << " ₹"
                      << std::setw(10)
                      << entry.second
                      << " ("
                      << percentage
                      << "%)\n";
        }
    }

    std::cout << "====================================\n";
}

// --------------------------------------------------
// Analytics Menu
// --------------------------------------------------

void ExpenseTracker::showAnalytics() const {

    if (transactions.empty()) {

        std::cout << "\nNo transactions available.\n";
        return;
    }

    std::string month;

    std::cout << "Enter month (MM-YYYY): ";
    std::cin >> month;

    if (!isValidMonth(month)) {

        std::cout << "Invalid month. Use MM-YYYY.\n";
        return;
    }

    showMonthlyAnalytics(month);
}

// --------------------------------------------------
// Set Monthly Budget
// --------------------------------------------------

void ExpenseTracker::setBudget() {

    std::string month;
    double amount;

    std::cout << "Enter month (MM-YYYY): ";
    std::cin >> month;

    if (!isValidMonth(month)) {

        std::cout << "Invalid month. Use MM-YYYY.\n";
        return;
    }

    while (true) {

        std::cout << "Enter budget amount: ";

        if (std::cin >> amount &&
            amount > 0) {

            break;
        }

        std::cout << "Budget must be a positive number.\n";

        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );
    }

    monthlyBudgets[month] = amount;

    saveBudgets();

    std::cout << "\nBudget for "
              << month
              << " set to ₹"
              << std::fixed
              << std::setprecision(2)
              << amount
              << ".\n";
}

// --------------------------------------------------
// Budget Status
// --------------------------------------------------

void ExpenseTracker::showBudgetStatus() const {

    std::string month;

    std::cout << "Enter month (MM-YYYY): ";
    std::cin >> month;

    if (!isValidMonth(month)) {

        std::cout << "Invalid month. Use MM-YYYY.\n";
        return;
    }

    auto budgetIt = monthlyBudgets.find(month);

    if (budgetIt == monthlyBudgets.end()) {

        std::cout << "No budget has been set for "
                  << month
                  << ".\n";

        return;
    }

    double expenses = 0;

    for (const Transaction& transaction : transactions) {

        if (transaction.getType() == "expense" &&
            getMonthYear(transaction.getDate()) == month) {

            expenses += transaction.getAmount();
        }
    }

    double budget = budgetIt->second;
    double remaining = budget - expenses;

    std::cout << "\n========== BUDGET STATUS ==========\n";

    std::cout << "Month           : "
              << month << "\n";

    std::cout << "Budget          : ₹"
              << std::fixed
              << std::setprecision(2)
              << budget << "\n";

    std::cout << "Spent           : ₹"
              << expenses << "\n";

    std::cout << "Remaining       : ₹"
              << remaining << "\n";

    if (expenses > budget) {

        std::cout << "Status          : OVER BUDGET\n";

        std::cout << "Exceeded By     : ₹"
                  << expenses - budget
                  << "\n";
    }

    else {

        std::cout << "Status          : WITHIN BUDGET\n";
    }

    if (budget > 0) {

        std::cout << "Budget Used     : "
                  << (expenses / budget) * 100
                  << "%\n";
    }

    std::cout << "===================================\n";
}

// --------------------------------------------------
// Monthly Transactions
// --------------------------------------------------

void ExpenseTracker::showMonthlyTransactions() const {

    if (transactions.empty()) {

        std::cout << "\nNo transactions available.\n";
        return;
    }

    std::string month;

    std::cout << "Enter month (MM-YYYY): ";
    std::cin >> month;

    if (!isValidMonth(month)) {

        std::cout << "Invalid month. Use MM-YYYY.\n";
        return;
    }

    bool found = false;

    std::cout << "\nTransactions for "
              << month
              << "\n\n";

    printHeader();

    for (const Transaction& transaction : transactions) {

        if (getMonthYear(transaction.getDate()) == month) {

            transaction.display();

            found = true;
        }
    }

    if (!found) {
        std::cout << "No transactions found for "
                  << month << ".\n";
    }
}

// --------------------------------------------------
// Transaction Storage
// --------------------------------------------------

void ExpenseTracker::saveTransactions() const {

    std::ofstream file(transactionFile);

    if (!file) {

        std::cout << "Error: Could not save transactions.\n";
        return;
    }

    for (const Transaction& transaction : transactions) {

        file << transaction.getId() << "|"
             << transaction.getType() << "|"
             << transaction.getAmount() << "|"
             << transaction.getCategory() << "|"
             << transaction.getDescription() << "|"
             << transaction.getDate()
             << "\n";
    }
}

void ExpenseTracker::loadTransactions() {

    std::ifstream file(transactionFile);

    if (!file) {
        return;
    }

    std::string line;

    while (std::getline(file, line)) {

        if (line.empty()) {
            continue;
        }

        try {

            std::stringstream ss(line);

            std::string idStr;
            std::string type;
            std::string amountStr;
            std::string category;
            std::string description;
            std::string date;

            std::getline(ss, idStr, '|');
            std::getline(ss, type, '|');
            std::getline(ss, amountStr, '|');
            std::getline(ss, category, '|');
            std::getline(ss, description, '|');
            std::getline(ss, date, '|');

            int id = std::stoi(idStr);
            double amount = std::stod(amountStr);

            if (id <= 0 || amount <= 0) {
                continue;
            }

            type = toLower(type);
            category = toLower(category);

            if (type != "income" &&
                type != "expense") {
                continue;
            }

            if (!isValidDate(date)) {
                continue;
            }

            transactions.emplace_back(
                id,
                type,
                amount,
                category,
                description,
                date
            );

            if (id >= nextId) {
                nextId = id + 1;
            }
        }

        catch (...) {

            std::cout
                << "Warning: Skipped corrupted transaction record.\n";
        }
    }
}

// --------------------------------------------------
// Budget Storage
// --------------------------------------------------

void ExpenseTracker::saveBudgets() const {

    std::ofstream file(budgetFile);

    if (!file) {

        std::cout << "Error: Could not save budgets.\n";
        return;
    }

    for (const auto& entry : monthlyBudgets) {

        file << entry.first
             << "|"
             << entry.second
             << "\n";
    }
}

void ExpenseTracker::loadBudgets() {

    std::ifstream file(budgetFile);

    if (!file) {
        return;
    }

    std::string line;

    while (std::getline(file, line)) {

        if (line.empty()) {
            continue;
        }

        try {

            std::stringstream ss(line);

            std::string month;
            std::string amountStr;

            std::getline(ss, month, '|');
            std::getline(ss, amountStr, '|');

            double amount = std::stod(amountStr);

            if (isValidMonth(month) &&
                amount > 0) {

                monthlyBudgets[month] = amount;
            }
        }

        catch (...) {

            std::cout
                << "Warning: Skipped corrupted budget record.\n";
        }
    }
}

// --------------------------------------------------
// Main Menu
// --------------------------------------------------

void ExpenseTracker::run() {

    int choice;

    while (true) {

        std::cout << "\n";
        std::cout << "====================================\n";
        std::cout << "       SMART EXPENSE TRACKER\n";
        std::cout << "====================================\n";

        std::cout << "1.  Add Transaction\n";
        std::cout << "2.  View Transactions\n";
        std::cout << "3.  Delete Transaction\n";
        std::cout << "4.  Search Transactions\n";
        std::cout << "5.  Filter Transactions\n";
        std::cout << "6.  Sort Transactions\n";
        std::cout << "7.  View Overall Balance\n";
        std::cout << "8.  Monthly Analytics\n";
        std::cout << "9.  Set Monthly Budget\n";
        std::cout << "10. View Budget Status\n";
        std::cout << "11. View Monthly Transactions\n";
        std::cout << "12. Exit\n";

        std::cout << "------------------------------------\n";
        std::cout << "Enter choice: ";

        if (!(std::cin >> choice)) {

            std::cout << "Invalid input. Enter a number.\n";

            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            continue;
        }

        std::cout << "\n";

        switch (choice) {

            case 1:
                addTransaction();
                break;

            case 2:
                displayTransactions();
                break;

            case 3:
                deleteTransaction();
                break;

            case 4:
                searchTransactions();
                break;

            case 5:
                filterTransactions();
                break;

            case 6:
                sortTransactions();
                break;

            case 7:
                showBalance();
                break;

            case 8:
                showAnalytics();
                break;

            case 9:
                setBudget();
                break;

            case 10:
                showBudgetStatus();
                break;

            case 11:
                showMonthlyTransactions();
                break;

            case 12:

                std::cout
                    << "Thank you for using Smart Expense Tracker!\n";

                return;

            default:

                std::cout
                    << "Invalid choice. Try again.\n";
        }
    }
}