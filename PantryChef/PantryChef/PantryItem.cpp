#include "PantryItem.h"
#include <stdexcept>
#include <utility>

PantryItem::PantryItem(std::string name, double amount, std::string unit, std::string expiration)
    : name_(std::move(name)), amount_(amount), unit_(std::move(unit)), expiration_(std::move(expiration)) {
    if (amount_ < 0) {
        throw std::invalid_argument("Amount cannot be negative.");
    }
}

const std::string& PantryItem::name() const { return name_; }
double PantryItem::amount() const { return amount_; }
const std::string& PantryItem::unit() const { return unit_; }
const std::string& PantryItem::expiration() const { return expiration_; }

void PantryItem::add(double amount) { amount_ += amount; }
void PantryItem::remove(double amount) { amount_ -= amount; }