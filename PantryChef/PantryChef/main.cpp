#include <iostream>
#include "Pantry.h"
#include "SortStrategy.h"


void show(const Pantry& pantry, const SortStrategy& strategy) {
    std::cout << "Pantry " << strategy.label() << ":\n";
    for (const auto& item : pantry.sorted(strategy)) {
        std::cout << "  " << item.name() << " - " << item.amount() << " " << item.unit()
                  << " (exp " << item.expiration() << ")\n";
    }
}

int main() {
    Pantry pantry;
    pantry.addItem(PantryItem("milk", 2, "cups", "2026-11-10"));
    pantry.addItem(PantryItem("flour", 4, "cups", "2027-03-01"));
    pantry.addItem(PantryItem("eggs", 6, "count", "2026-11-05"));

    std::cout << "== 1. Normal task + first derived behavior ==\n";
    show(pantry, ByExpiration());

    std::cout << "\n== 2. Second derived behavior ==\n";
    show(pantry, ByName());

    std::cout << "\n== 3. Invalid operation and continued use ==\n";
    try {
        pantry.removeAmount("milk", 3);
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
    try {
        pantry.addItem(PantryItem("sugar", -1, "cups", "2027-01-01"));
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
    pantry.removeAmount("eggs", 2);  // program still works after the errors
    show(pantry, ByExpiration());
    return 0;
}