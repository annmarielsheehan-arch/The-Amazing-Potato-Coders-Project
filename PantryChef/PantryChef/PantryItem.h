#pragma once
#include <string>

class PantryItem {
private:
    std::string name_;
    double amount_;
    std::string unit_;
    std::string expiration_;  // YYYY-MM-DD, sorts correctly as text

public:
    PantryItem(std::string name, double amount, std::string unit, std::string expiration);

    const std::string& name() const;
    double amount() const;
    const std::string& unit() const;
    const std::string& expiration() const;

    void add(double amount);
    void remove(double amount);
};