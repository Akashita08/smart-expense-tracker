#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <unordered_map>

using namespace std;


class Transaction {

private:

    int id;
    string type;
    double amount;
    string category;
    string description;
    string date;

public:

    Transaction(int id, string type, double amount,
                string category, string description, string date) {

        this->id = id;
        this->type = type;
        this->amount = amount;
        this->category = category;
        this->description = description;
        this->date = date;
    }


    int getId() const {
        return id;
    }


    string getType() const {
        return type;
    }


    double getAmount() const {
        return amount;
    }


    string getCategory() const {
        return category;
    }


    string getDescription() const {
        return description;
    }


    string getDate() const {
        return date;
    }


    void display() const {

        cout << left
             << setw(5) << id
             << setw(12) << type
             << setw(12) << fixed << setprecision(2) << amount
             << setw(15) << category
             << setw(25) << description
             << date << endl;
    }
};


class ExpenseTracker {

private:

    vector<Transaction> transactions;

    int nextId;

    double monthlyBudget;

    const string filename = "transactions.txt";

    const string budgetFile = "budget.txt";


    string toLower(string text) const {

        for (char& ch : text) {

            ch = static_cast<char>(
                tolower(static_cast<unsigned char>(ch))
            );
        }

        return text;
    }


    void printHeader() const {

        cout << "\n===================== TRANSACTIONS =====================\n";

        cout << left
             << setw(5) << "ID"
             << setw(12) << "Type"
             << setw(12) << "Amount"
             << setw(15) << "Category"
             << setw(25) << "Description"
             << "Date" << endl;

        cout << "---------------------------------------------------------"
             << "----------------\n";
    }


public:

    ExpenseTracker() {

        nextId = 1;

        monthlyBudget = 0;

        loadTransactions();

        loadBudget();
    }


    void addTransaction() {

        string type;
        double amount;
        string category;
        string description;
        string date;


        cout << "\nEnter transaction type (Income/Expense): ";
        cin >> type;


        cout << "Enter amount: ";
        cin >> amount;


        cin.ignore();


        cout << "Enter category: ";
        getline(cin, category);


        cout << "Enter description: ";
        getline(cin, description);


        cout << "Enter date (DD-MM-YYYY): ";
        getline(cin, date);


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


    void displayTransactions() const {

        if (transactions.empty()) {

            cout << "\nNo transactions found.\n";

            return;
        }


        printHeader();


        for (const Transaction& transaction : transactions) {

            transaction.display();
        }
    }


    void deleteTransaction() {

        if (transactions.empty()) {

            cout << "\nNo transactions available to delete.\n";

            return;
        }


        int id;


        cout << "\nEnter transaction ID to delete: ";

        cin >> id;


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


    void searchTransactions() const {

        if (transactions.empty()) {

            cout << "\nNo transactions available.\n";

            return;
        }


        string keyword;


        cin.ignore();


        cout << "\nEnter keyword to search: ";

        getline(cin, keyword);


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


    void filterTransactions() const {

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

        cin >> choice;


        if (choice == 1 || choice == 2) {

            string requiredType;


            if (choice == 1) {

                requiredType = "income";
            }

            else {

                requiredType = "expense";
            }


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


            cin.ignore();


            cout << "Enter category: ";

            getline(cin, category);


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


    void sortTransactions() {

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

        cin >> choice;


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


    void showBalance() const {

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
             << totalIncome << endl;


        cout << "Total Expense : Rs. "
             << fixed << setprecision(2)
             << totalExpense << endl;


        cout << "------------------------------------------\n";


        cout << "Current Balance: Rs. "
             << fixed << setprecision(2)
             << balance << endl;
    }


    void showAnalytics() const {

        if (transactions.empty()) {

            cout << "\nNo transactions available for analysis.\n";

            return;
        }


        double totalIncome = 0;

        double totalExpense = 0;

        double highestExpense = 0;

        double expenseCount = 0;


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


                if (amount > highestExpense) {

                    highestExpense = amount;
                }


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
             << transactions.size() << endl;


        cout << "Total Income       : Rs. "
             << totalIncome << endl;


        cout << "Total Expenses     : Rs. "
             << totalExpense << endl;


        cout << "Current Balance    : Rs. "
             << balance << endl;


        cout << "Highest Expense    : Rs. "
             << highestExpense << endl;


        cout << "Average Expense    : Rs. "
             << averageExpense << endl;


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


    void setBudget() {

        double budget;


        cout << "\nEnter monthly budget: Rs. ";

        cin >> budget;


        if (budget <= 0) {

            cout << "Budget must be greater than zero.\n";

            return;
        }


        monthlyBudget = budget;


        saveBudget();


        cout << "\nMonthly budget updated successfully!\n";
    }


    void showBudgetStatus() const {

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
             << monthlyBudget << endl;


        cout << "Spent          : Rs. "
             << totalExpense << endl;


        cout << "Remaining      : Rs. "
             << remaining << endl;


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


private:

    void saveTransactions() const {

        ofstream file(filename);


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


    void loadTransactions() {

        ifstream file(filename);


        if (!file) {

            return;
        }


        string line;


        while (getline(file, line)) {

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
                amountString.empty()) {

                continue;
            }


            int id =
                stoi(idString);


            double amount =
                stod(amountString);


            Transaction transaction(
                id,
                type,
                amount,
                category,
                description,
                date
            );


            transactions.push_back(transaction);


            if (id >= nextId) {

                nextId = id + 1;
            }
        }


        file.close();
    }


    void saveBudget() const {

        ofstream file(budgetFile);


        if (!file) {

            cout << "\nError: Could not save budget.\n";

            return;
        }


        file << monthlyBudget;


        file.close();
    }


    void loadBudget() {

        ifstream file(budgetFile);


        if (!file) {

            return;
        }


        file >> monthlyBudget;


        file.close();
    }


public:

    void run() {

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

            cin >> choice;


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
};


int main() {

    ExpenseTracker tracker;

    tracker.run();

    return 0;
}