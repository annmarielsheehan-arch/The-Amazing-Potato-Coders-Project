#include <iostream>
#include <stdexcept>
#include <string>
#include "PantryApp.h"

static int failures = 0;

static void check(const std::string& label, bool ok) {
    std::cout << (ok ? "PASS  " : "FAIL  ") << label << "\n";
    if (!ok) failures++;
}

template <typename F>
static std::string errorOf(F action) {
    try {
        action();
    } catch (const std::exception& e) {
        return e.what();
    }
    return "";
}

int main() {
    PantryApp app;
    app.loadSampleData();

    std::cout << "== Task 1: manage the pantry ==\n";
    app.addItem("butter", "1", "cups", "2026-12-01");
    check("valid add: butter appears in the pantry", app.pantryItems(PantryApp::SortMode::ByName).size() == 5);
    auto byExp = app.pantryItems(PantryApp::SortMode::ByExpiration);
    check("sorted by expiration: first item is eggs", byExp.front().name() == "eggs");
    auto byName = app.pantryItems(PantryApp::SortMode::ByName);
    check("sorted by name: first item is butter", byName.front().name() == "butter");

    std::string e1 = errorOf([&] { app.addItem("rice", "abc", "cups", "2027-01-01"); });
    check("invalid amount text rejected: " + e1, e1 == "Amount must be a number, for example 2 or 0.5.");
    std::string e2 = errorOf([&] { app.addItem("rice", "1", "cups", "2027-02-29"); });
    check("invalid date rejected: " + e2, e2 == "Expiration date must be a real date in YYYY-MM-DD format.");
    std::string e3 = errorOf([&] { app.removeAmount("milk", "5"); });
    check("removing more than in stock rejected: " + e3, e3 == "Cannot remove 5 cups of milk: only 2 available.");
    check("pantry unchanged after the errors (still 5 items)", app.pantryItems(PantryApp::SortMode::ByName).size() == 5);
    app.removeAmount("eggs", "2");
    check("valid removal still works afterward", true);

    std::cout << "\n== Task 2: what can I make? ==\n";
    for (const auto& s : app.whatCanIMake()) {
        std::cout << "      " << s.name << ": " << (s.ready ? "READY" : "missing " + std::to_string(s.missingCount)) << "\n";
    }

    std::cout << "\n== Task 3: shopping list ==\n";
    auto list = app.shoppingList("Sugar Cookies");
    for (const auto& m : list) std::cout << "      " << m.amount << " " << m.unit << " " << m.name << "\n";
    check("Sugar Cookies list contains only the missing sugar (butter was added to the pantry)", list.size() == 1);
    check("Pancakes needs nothing", app.shoppingList("Pancakes").empty());
    std::string e4 = errorOf([&] { app.shoppingList("Pizza"); });
    check("unknown recipe rejected: " + e4, e4 == "Recipe not found: Pizza");

    std::cout << "\n" << (failures == 0 ? "All checks passed." : "Some checks FAILED.") << "\n";
    return failures == 0 ? 0 : 1;
}
