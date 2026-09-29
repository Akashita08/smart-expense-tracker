#include "../include/Transaction.h"

#include <iostream>
#include <iomanip>

Transaction::Transaction(
    int id,
    std::string type,
    double amount,
    std::string category,
    std::string description,
    std::string date
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

std::string Transaction::getType() const {
    return type;
}

double Transaction::getAmount() const {
    return amount;
}

std::string Transaction::getCategory() const {
    return category;
}

std::string Transaction::getDescription() const {
    return description;
}

std::string Transaction::getDate() const {
    return date;
}

void Transaction::display() const {

    std::cout << std::left
              << std::setw(5) << id
              << std::setw(12) << type
              << std::setw(12)
              << std::fixed
              << std::setprecision(2)
              << amount
              << std::setw(15) << category
              << std::setw(25) << description
              << date
              << std::endl;
}