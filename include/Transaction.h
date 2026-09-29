#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

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

    Transaction(
        int id,
        string type,
        double amount,
        string category,
        string description,
        string date
    );

    int getId() const;

    string getType() const;

    double getAmount() const;

    string getCategory() const;

    string getDescription() const;

    string getDate() const;

    void display() const;
};

#endif