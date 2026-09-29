#include "../include/ExpenseTracker.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <unordered_map>
#include <limits>

using namespace std;


ExpenseTracker::ExpenseTracker() {

    nextId = 1;

    monthlyBudget = 0;

    loadTransactions();

    loadBudget();
}


string ExpenseTracker::toLower(string text) const {

    for (char& ch : text) {

        ch = static_cast<char>(
            tolower(static_cast<unsigned char>(ch))
        );
    }

    return text;
}


void ExpenseTracker::printHeader() const {

    cout << "\n===================== TRANSACTIONS =====================\n";

    cout << left
         << setw(5) << "ID"
         << setw(12) << "Type"
         << setw(12) << "Amount"
         << setw(15) << "Category"
         << setw(25) << "Description"
         << "Date"
         << endl;

    cout << "---------------------------------------------------------"
         << "----------------\n";
}


void ExpenseTracker::addTransaction() {

    string type;
    double amount;
    string category;
    string description;
    string date;


    while (true) {

        cout << "\nEnter transaction type (Income/Expense): ";

        cin >> type;

        type = toLower(type);


        if (type == "income" || type == "expense") {

            break;
        }


        cout << "Invalid type. Please enter Income or Expense.\n";
    }


    while (true) {

        cout << "Enter amount: ";


        if (cin >> amount && amount > 0) {

            break;
        }


        cout << "Invalid amount. Enter a positive number.\n";

        cin.clear();

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );
    }


    cin.ignore(
        numeric_limits<streamsize>::max(),
        '\n'
    );


    while (true) {

        cout << "Enter category: ";

        getline(cin, category);


        if (category.empty()) {

            cout << "Category cannot be empty.\n";

            continue;
        }


        if (category.find('|') != string::npos) {

            cout << "Category cannot contain '|'.\n";

            continue;
        }


        break;
    }


    while (true) {

        cout << "Enter description: ";

        getline(cin, description);


        if (description.empty()) {

            cout << "Description cannot be empty.\n";

            continue;
        }


        if (description.find('|') != string::npos) {

            cout << "Description cannot contain '|'.\n";

            continue;
        }


        break;
    }


    while (true) {

        cout << "Enter date (DD-MM-YYYY): ";

        getline(cin, date);


        if (date.length() != 10 ||
            date[2] != '-' ||
            date[5] != '-') {

            cout << "Invalid date format.\n";

            continue;
        }


        bool valid = true;


        for (int i = 0; i < 10; i++) {

            if (i == 2 || i == 5) {

                continue;
            }


            if (!isdigit(
                    static_cast<unsigned char>(date[i])
                )) {

                valid = false;

                break;
            }
        }


        if (!valid) {

            cout << "Date must contain only numbers and '-'.\n";

            continue;
        }


        break;
    }


    Transaction newTransaction(
        nextId,
        type,
        amount,
        category,
        description,
        date
    );


    transactions.push_back(newTransaction);

    nextId++;


    saveTransactions();


    cout << "\nTransaction added successfully!\n";
}


void ExpenseTracker::displayTransactions() const {

    if (transactions.empty()) {

        cout << "\nNo transactions found.\n";

        return;
    }


    printHeader();


    for (const Transaction& transaction : transactions) {

        transaction.display();
    }
}


void ExpenseTracker::deleteTransaction() {

    if (transactions.empty()) {

        cout << "\nNo transactions available to delete.\n";

        return;
    }


    int id;


    while (true) {

        cout << "\nEnter transaction ID to delete: ";


        if (cin >> id && id > 0) {

            break;
        }


        cout << "Please enter a valid positive ID.\n";

        cin.clear();

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );
    }


    for (auto it = transactions.begin();
         it != transactions.end();
         ++it) {


        if (it->getId() == id) {

            transactions.erase(it);

            saveTransactions();


            cout << "Transaction deleted successfully!\n";

            return;
        }
    }


    cout << "Transaction with ID "
         << id
         << " not found.\n";
}


void ExpenseTracker::searchTransactions() const {

    if (transactions.empty()) {

        cout << "\nNo transactions available.\n";

        return;
    }


    string keyword;


    cin.ignore(
        numeric_limits<streamsize>::max(),
        '\n'
    );


    cout << "\nEnter keyword to search: ";

    getline(cin, keyword);


    if (keyword.empty()) {

        cout << "Search keyword cannot be empty.\n";

        return;
    }


    keyword = toLower(keyword);


    bool found = false;


    printHeader();


    for (const Transaction& transaction : transactions) {

        string category =
            toLower(transaction.getCategory());

        string description =
            toLower(transaction.getDescription());


        if (category.find(keyword) != string::npos ||
            description.find(keyword) != string::npos) {

            transaction.display();

            found = true;
        }
    }


    if (!found) {

        cout << "\nNo matching transactions found.\n";
    }
}


void ExpenseTracker::filterTransactions() const {

    if (transactions.empty()) {

        cout << "\nNo transactions available.\n";

        return;
    }


    int choice;


    cout << "\n========== FILTER ==========\n";

    cout << "1. Income\n";

    cout << "2. Expense\n";

    cout << "3. Category\n";

    cout << "Enter choice: ";


    if (!(cin >> choice)) {

        cout << "Invalid input.\n";

        cin.clear();

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );

        return;
    }


    if (choice == 1 || choice == 2) {

        string requiredType =
            (choice == 1) ? "income" : "expense";


        bool found = false;


        printHeader();


        for (const Transaction& transaction : transactions) {

            if (toLower(transaction.getType())
                == requiredType) {

                transaction.display();

                found = true;
            }
        }


        if (!found) {

            cout << "\nNo matching transactions found.\n";
        }
    }


    else if (choice == 3) {

        string category;


        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );


        cout << "Enter category: ";

        getline(cin, category);


        if (category.empty()) {

            cout << "Category cannot be empty.\n";

            return;
        }


        category = toLower(category);


        bool found = false;


        printHeader();


        for (const Transaction& transaction : transactions) {

            if (toLower(transaction.getCategory())
                == category) {

                transaction.display();

                found = true;
            }
        }


        if (!found) {

            cout << "\nNo matching transactions found.\n";
        }
    }


    else {

        cout << "\nInvalid filter choice.\n";
    }
}


void ExpenseTracker::sortTransactions() {

    if (transactions.empty()) {

        cout << "\nNo transactions available.\n";

        return;
    }


    int choice;


    cout << "\n========== SORT ==========\n";

    cout << "1. Amount: Low to High\n";

    cout << "2. Amount: High to Low\n";

    cout << "3. ID: Low to High\n";

    cout << "Enter choice: ";


    if (!(cin >> choice)) {

        cout << "Invalid input.\n";

        cin.clear();

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );

        return;
    }


    if (choice == 1) {

        sort(
            transactions.begin(),
            transactions.end(),

            [](const Transaction& a,
               const Transaction& b) {

                return a.getAmount()
                     < b.getAmount();
            }
        );
    }


    else if (choice == 2) {

        sort(
            transactions.begin(),
            transactions.end(),

            [](const Transaction& a,
               const Transaction& b) {

                return a.getAmount()
                     > b.getAmount();
            }
        );
    }


    else if (choice == 3) {

        sort(
            transactions.begin(),
            transactions.end(),

            [](const Transaction& a,
               const Transaction& b) {

                return a.getId()
                     < b.getId();
            }
        );
    }


    else {

        cout << "\nInvalid sorting choice.\n";

        return;
    }


    cout << "\nTransactions sorted successfully.\n";

    displayTransactions();
}


void ExpenseTracker::showBalance() const {

    double totalIncome = 0;

    double totalExpense = 0;


    for (const Transaction& transaction : transactions) {

        if (toLower(transaction.getType())
            == "income") {

            totalIncome +=
                transaction.getAmount();
        }


        else if (toLower(transaction.getType())
                 == "expense") {

            totalExpense +=
                transaction.getAmount();
        }
    }


    double balance =
        totalIncome - totalExpense;


    cout << "\n================ BALANCE =================\n";


    cout << "Total Income  : Rs. "
         << fixed << setprecision(2)
         << totalIncome
         << endl;


    cout << "Total Expense : Rs. "
         << fixed << setprecision(2)
         << totalExpense
         << endl;


    cout << "------------------------------------------\n";


    cout << "Current Balance: Rs. "
         << fixed << setprecision(2)
         << balance
         << endl;
}


void ExpenseTracker::showAnalytics() const {

    if (transactions.empty()) {

        cout << "\nNo transactions available for analysis.\n";

        return;
    }


    double totalIncome = 0;

    double totalExpense = 0;

    double highestExpense = 0;

    int expenseCount = 0;


    unordered_map<string, double> categorySpending;


    for (const Transaction& transaction : transactions) {

        string type =
            toLower(transaction.getType());


        if (type == "income") {

            totalIncome +=
                transaction.getAmount();
        }


        else if (type == "expense") {

            double amount =
                transaction.getAmount();


            totalExpense += amount;

            expenseCount++;


            highestExpense =
                max(highestExpense, amount);


            categorySpending[
                transaction.getCategory()
            ] += amount;
        }
    }


    double balance =
        totalIncome - totalExpense;


    double averageExpense = 0;


    if (expenseCount > 0) {

        averageExpense =
            totalExpense / expenseCount;
    }


    cout << "\n========== ANALYTICS DASHBOARD ==========\n";


    cout << fixed << setprecision(2);


    cout << "\nTotal Transactions : "
         << transactions.size()
         << endl;


    cout << "Total Income       : Rs. "
         << totalIncome
         << endl;


    cout << "Total Expenses     : Rs. "
         << totalExpense
         << endl;


    cout << "Current Balance    : Rs. "
         << balance
         << endl;


    cout << "Highest Expense    : Rs. "
         << highestExpense
         << endl;


    cout << "Average Expense    : Rs. "
         << averageExpense
         << endl;


    cout << "\n---------- CATEGORY SPENDING ----------\n";


    if (categorySpending.empty()) {

        cout << "No expenses recorded.\n";
    }


    else {

        for (const auto& entry : categorySpending) {

            double percentage =
                (entry.second / totalExpense) * 100;


            cout << left
                 << setw(18)
                 << entry.first

                 << "Rs. "
                 << setw(10)
                 << entry.second

                 << percentage
                 << "%\n";
        }
    }
}


void ExpenseTracker::setBudget() {

    double budget;


    while (true) {

        cout << "\nEnter monthly budget: Rs. ";


        if (cin >> budget && budget > 0) {

            break;
        }


        cout << "Budget must be greater than zero.\n";


        cin.clear();


        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );
    }


    monthlyBudget = budget;


    saveBudget();


    cout << "\nMonthly budget updated successfully!\n";
}


void ExpenseTracker::showBudgetStatus() const {

    if (monthlyBudget <= 0) {

        cout << "\nNo monthly budget has been set.\n";

        return;
    }


    double totalExpense = 0;


    for (const Transaction& transaction : transactions) {

        if (toLower(transaction.getType())
            == "expense") {

            totalExpense +=
                transaction.getAmount();
        }
    }


    double remaining =
        monthlyBudget - totalExpense;


    double percentageUsed =
        (totalExpense / monthlyBudget) * 100;


    cout << "\n========== BUDGET STATUS ==========\n";


    cout << fixed << setprecision(2);


    cout << "Monthly Budget : Rs. "
         << monthlyBudget
         << endl;


    cout << "Spent          : Rs. "
         << totalExpense
         << endl;


    cout << "Remaining      : Rs. "
         << remaining
         << endl;


    cout << "Budget Used    : "
         << percentageUsed
         << "%\n";


    cout << "\nStatus: ";


    if (remaining > 0) {

        cout << "Within budget";
    }


    else if (remaining == 0) {

        cout << "Budget fully used";
    }


    else {

        cout << "Budget exceeded";
    }


    cout << endl;
}


void ExpenseTracker::saveTransactions() const {

    ofstream file(transactionFile);


    if (!file) {

        cout << "\nError: Could not save transactions.\n";

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


    file.close();
}


void ExpenseTracker::loadTransactions() {

    ifstream file(transactionFile);


    if (!file) {

        return;
    }


    string line;


    while (getline(file, line)) {

        try {

            stringstream ss(line);


            string idString;
            string type;
            string amountString;
            string category;
            string description;
            string date;


            getline(ss, idString, '|');

            getline(ss, type, '|');

            getline(ss, amountString, '|');

            getline(ss, category, '|');

            getline(ss, description, '|');

            getline(ss, date, '|');


            if (idString.empty() ||
                amountString.empty() ||
                type.empty() ||
                category.empty() ||
                description.empty() ||
                date.empty()) {

                continue;
            }


            int id =
                stoi(idString);


            double amount =
                stod(amountString);


            if (amount <= 0) {

                continue;
            }


            Transaction transaction(
                id,
                toLower(type),
                amount,
                category,
                description,
                date
            );


            transactions.push_back(transaction);


            nextId =
                max(nextId, id + 1);
        }


        catch (...) {

            cout << "Warning: Skipping corrupted transaction data.\n";
        }
    }


    file.close();
}


void ExpenseTracker::saveBudget() const {

    ofstream file(budgetFile);


    if (!file) {

        cout << "\nError: Could not save budget.\n";

        return;
    }


    file << monthlyBudget;


    file.close();
}


void ExpenseTracker::loadBudget() {

    ifstream file(budgetFile);


    if (!file) {

        return;
    }


    if (!(file >> monthlyBudget)) {

        monthlyBudget = 0;
    }


    file.close();
}


void ExpenseTracker::run() {

    int choice;


    do {

        cout << "\n\n==========================================\n";

        cout << "          SMART EXPENSE TRACKER\n";

        cout << "==========================================\n";


        cout << "1. Add Transaction\n";

        cout << "2. View Transactions\n";

        cout << "3. Delete Transaction\n";

        cout << "4. Search Transactions\n";

        cout << "5. Filter Transactions\n";

        cout << "6. Sort Transactions\n";

        cout << "7. View Balance\n";

        cout << "8. Analytics Dashboard\n";

        cout << "9. Set Monthly Budget\n";

        cout << "10. View Budget Status\n";

        cout << "11. Exit\n";


        cout << "==========================================\n";


        cout << "Enter your choice: ";


        if (!(cin >> choice)) {

            cout << "\nInvalid input. Enter a number from 1 to 11.\n";


            cin.clear();


            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );


            continue;
        }


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
                cout << "\nThank you for using Smart Expense Tracker!\n";
                break;


            default:
                cout << "\nInvalid choice. Please try again.\n";
        }


    } while (choice != 11);
}