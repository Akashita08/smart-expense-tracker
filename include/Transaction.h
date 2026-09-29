#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {

private:

    int id;
    std::string type;
    double amount;
    std::string category;
    std::string description;
    std::string date;

public:

    Transaction(
        int id,
        std::string type,
        double amount,
        std::string category,
        std::string description,
        std::string date
    );

    int getId() const;
    std::string getType() const;
    double getAmount() const;
    std::string getCategory() const;
    std::string getDescription() const;
    std::string getDate() const;

    void display() const;
};

#endif