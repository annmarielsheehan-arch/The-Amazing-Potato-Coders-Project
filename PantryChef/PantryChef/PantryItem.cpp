#include "PantryItem.h"
#include <cctype>
#include <stdexcept>
#include <utility>

namespace {
    bool allDigits(const std::string&s, std::size_t start, std::size_t count) {
        for (std::size_t i=start; i < start + count; i++) {
            if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
        }
        return true;
    }

    bool isValidDate(const std::string& d) {
        if (d.size() != 10 || d[4] != '-' || d[7] != '-') return false;
        if (!allDigits(d, 0, 4) || !allDigits (d, 5, 2) || !allDigits(d, 8, 2)) return false;

        int year = std::stoi(d.substr(0 ,4));
        int month = std::stoi(d.substr(5, 2));
        int day = std::stoi(d.substr(8, 2));
        if (month < 1 || month >12 || day < 1) return false;

        static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        bool leap = (year % 4 ==0 && year % 100 != 0) || (year % 400 == 0);
        int maxDay = (month ==2 && leap) ? 29 : daysInMonth[month-1];
        return day <= maxDay;
    }
}
PantryItem::PantryItem(std::string name, double amount, std::string unit, std::string expiration)
    : name_(std::move(name)), amount_(amount), unit_(std::move(unit)), expiration_(std::move(expiration)) {
    if (amount_ < 0) {
        throw std::invalid_argument("Amount cannot be negative.");
    }
    if (!isValidDate(expiration_)) {
        throw std::invalid_argument("Expiration date must be a real date in YYYY-MM-DD format.");
    }
}

const std::string& PantryItem::name() const { return name_; }
double PantryItem::amount() const { return amount_; }
const std::string& PantryItem::unit() const { return unit_; }
const std::string& PantryItem::expiration() const { return expiration_; }

void PantryItem::add(double amount) { amount_ += amount; }
void PantryItem::remove(double amount) { amount_ -= amount; }