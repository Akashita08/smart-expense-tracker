# Smart Expense Tracker

A command-line expense management application built in C++.

The project started as a basic transaction manager and was progressively developed into a modular expense tracking system with persistent storage, searching, filtering, sorting, budgeting, and monthly financial analytics.

## Features

- Add income and expense transactions
- Persistent transaction storage
- Delete transactions
- Search by category or description
- Filter by transaction type or category
- Sort transactions by amount or ID
- Overall balance calculation
- Monthly income and expense analytics
- Category-wise spending analysis
- Monthly budgets
- Budget usage tracking
- Monthly transaction filtering
- Input validation
- Corrupted file record handling
- Modular C++ architecture

## Project Structure

```text
smart-expense-tracker/
│
├── include/
│   ├── Transaction.h
│   └── ExpenseTracker.h
│
├── src/
│   ├── Transaction.cpp
│   ├── ExpenseTracker.cpp
│   └── main.cpp
│
├── data/
│   ├── transactions.txt
│   └── budget.txt
│
├── .gitignore
└── README.md