#include "Pantry.h"
#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace {
    std::string toLower(std::string s) {
        for (char& c : s) {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
        return s;
    }


bool sameItem(const std::string& a, const std::string& b) {
    return toLower(a) == toLower(b);
}
}

void Pantry::addItem(const PantryItem& item) {
    for (auto& existing : items_) {
        if (sameItem(existing.name(),item.name())){
            existing.add(item.amount());
            return;
        }
    }
    items_.push_back(item);
}

void Pantry::removeAmount(const std::string& name, double amount) {
    for (auto& existing : items_) {
        if (existing.name() == name) {
            if (amount > existing.amount()) {
                std::ostringstream msg;
                msg << "Cannot remove " << amount << " " << existing.unit() << " of " << name
                    << ": only " << existing.amount() << " available.";
                throw std::runtime_error(msg.str());
            }
            existing.remove(amount);
            return;
        }
    }
    throw std::runtime_error("Item not found: " + name);
}

std::vector<PantryItem> Pantry::sorted(const SortStrategy& strategy) const {
    std::vector<PantryItem> copy = items_;
    std::sort(copy.begin(), copy.end(),
              [&strategy](const PantryItem& a, const PantryItem& b) { return strategy.before(a, b); });
    return copy;
}