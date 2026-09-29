#include "../include/Transaction.h"

#include <iostream>
#include <iomanip>

using namespace std;


Transaction::Transaction(
    int id,
    string type,
    double amount,
    string category,
    string description,
    string date
) {

    this->id = id;
    this->type = type;
    this->amount = amount;
    this->category = category;
    this->description = description;
    this->date = date;
}


int Transaction::getId() const {

    return id;
}


string Transaction::getType() const {

    return type;
}


double Transaction::getAmount() const {

    return amount;
}


string Transaction::getCategory() const {

    return category;
}


string Transaction::getDescription() const {

    return description;
}


string Transaction::getDate() const {

    return date;
}


void Transaction::display() const {

    cout << left
         << setw(5) << id
         << setw(12) << type
         << setw(12)
         << fixed
         << setprecision(2)
         << amount
         << setw(15) << category
         << setw(25) << description
         << date
         << endl;
}