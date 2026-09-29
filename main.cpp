#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>

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

    const string filename = "transactions.txt";


public:

    ExpenseTracker() {

        nextId = 1;

        loadTransactions();
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


    void showBalance() const {

        double totalIncome = 0;
        double totalExpense = 0;


        for (const Transaction& transaction : transactions) {

            if (transaction.getType() == "Income" ||
                transaction.getType() == "income") {

                totalIncome += transaction.getAmount();
            }

            else if (transaction.getType() == "Expense" ||
                     transaction.getType() == "expense") {

                totalExpense += transaction.getAmount();
            }
        }


        double balance = totalIncome - totalExpense;


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


            if (idString.empty() || amountString.empty()) {

                continue;
            }


            int id = stoi(idString);

            double amount = stod(amountString);


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
            cout << "4. View Balance\n";
            cout << "5. Exit\n";

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
                    showBalance();
                    break;


                case 5:
                    cout << "\nThank you for using Smart Expense Tracker!\n";
                    break;


                default:
                    cout << "\nInvalid choice. Please try again.\n";
            }

        } while (choice != 5);
    }
};


int main() {

    ExpenseTracker tracker;

    tracker.run();

    return 0;
}