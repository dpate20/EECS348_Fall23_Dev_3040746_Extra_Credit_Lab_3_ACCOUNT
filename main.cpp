#include <iostream>
#include <stdexcept>

class Account {
protected:
    std::string accountNumber;
    std::string accountHolder;
    double balance;

public:
    Account(std::string number, std::string holder, double balance)
        : accountNumber(number), accountHolder(holder), balance(balance) {}

    void displayDetails() const {
        std::cout << "Account Details for " << accountNumber << ": " << std::endl;
        std::cout << "   Holder: " << accountHolder << std::endl;
        std::cout << "   Balance: $" << balance << std::endl;
    }

    void deposit(double amount) {
        balance += amount;
    }

    virtual void withdraw(double amount) {
        if (amount > balance) {
            throw std::runtime_error("Insufficient balance.");
        }
        balance -= amount;
    }

    void transfer(double amount, Account &destination) {
        withdraw(amount);
        destination.deposit(amount);
    }

    friend std::ostream &operator<<(std::ostream &os, const Account &account) {
        account.displayDetails();
        return os;
    }
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(std::string number, std::string holder, double balance, double interestRate)
        : Account(number, holder, balance), interestRate(interestRate) {}

    void displayDetails() const {
        Account::displayDetails();
        std::cout << "   Interest Rate: " << interestRate * 100 << "%" << std::endl;
    }

    void withdraw(double amount) override {
        if (amount > balance) {
            throw std::runtime_error("Insufficient balance.");
        }
        if (balance - amount < 100) {
            throw std::runtime_error("Minimum balance should be $100.");
        }
        balance -= amount;
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(std::string number, std::string holder, double balance, double overdraftLimit)
        : Account(number, holder, balance), overdraftLimit(overdraftLimit) {}

    void displayDetails() const {
        Account::displayDetails();
        std::cout << "   Overdraft Limit: $" << overdraftLimit << std::endl;
    }

    void withdraw(double amount) override {
        if (balance - amount < -overdraftLimit) {
            throw std::runtime_error("Overdraft limit exceeded.");
        }
        balance -= amount;
    }
};

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    std::cout << savings << std::endl;
    std::cout << current << std::endl;

    savings.deposit(500);
    current.withdraw(1000);

    std::cout << savings << std::endl;
    std::cout << current << std::endl;

    // Transfer 300 from savings to current
    savings.transfer(300, current);

    std::cout << savings << std::endl;
    std::cout << current << std::endl;

    return 0;
}