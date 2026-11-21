#include <cstdio>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "Filter.h"
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
    check("sorted by expiration: first item is eggs", app.pantryItems(PantryApp::SortMode::ByExpiration).front().name() == "eggs");
    check("sorted by name: first item is butter", app.pantryItems(PantryApp::SortMode::ByName).front().name() == "butter");

    std::string e1 = errorOf([&] { app.addItem("rice", "abc", "cups", "2027-01-01"); });
    check("invalid amount text rejected: " + e1, e1 == "Amount must be a number, for example 2 or 0.5.");
    std::string e2 = errorOf([&] { app.addItem("rice", "1", "cups", "2027-02-29"); });
    check("invalid date rejected: " + e2, e2 == "Expiration date must be a real date in YYYY-MM-DD format.");
    std::string e3 = errorOf([&] { app.removeAmount("milk", "5"); });
    check("removing more than in stock rejected: " + e3, e3 == "Cannot remove 5 cups of milk: only 2 available.");
    std::string e4 = errorOf([&] { app.addItem("a,b", "1", "cups", "2027-01-01"); });
    check("comma in name rejected: " + e4, e4 == "Item name and unit cannot contain commas.");
    check("pantry unchanged after the errors (still 5 items)", app.pantryItems(PantryApp::SortMode::ByName).size() == 5);

    std::cout << "\n== Case-insensitive add and remove use the same matching ==\n";
    PantryApp caseApp;
    caseApp.addItem("Milk", "2", "cups", "2026-12-01");
    caseApp.addItem("milk", "1", "cups", "2026-12-01");
    auto caseRows = caseApp.pantryItems(PantryApp::SortMode::ByName);
    check("milk merges into Milk (one row, 3 cups)", caseRows.size() == 1 && caseRows[0].amount() == 3);
    std::string c1 = errorOf([&] { caseApp.removeAmount("milk", "1"); });
    caseRows = caseApp.pantryItems(PantryApp::SortMode::ByName);
    check("removing 1 of milk from the Milk row works (2 cups left)",
          c1.empty() && caseRows.size() == 1 && caseRows[0].amount() == 2);
    std::string c2 = errorOf([&] { caseApp.removeAmount("milk", "99"); });
    check("removing more than in stock still rejected: " + c2, c2 == "Cannot remove 99 cups of milk: only 2 available.");
    std::string c3 = errorOf([&] { caseApp.removeAmount("zzz", "1"); });
    check("removing an item that is not there still rejected: " + c3, c3 == "Item not found: zzz");
    caseRows = caseApp.pantryItems(PantryApp::SortMode::ByName);
    check("Milk row unchanged after the two rejected removals (2 cups)", caseRows.size() == 1 && caseRows[0].amount() == 2);

    std::cout << "\n== Template filterBy with two data types ==\n";
    auto matches = app.pantryItems(PantryApp::SortMode::ByName, "MI");
    check("filterBy<PantryItem>: search \"MI\" finds only milk", matches.size() == 1 && matches[0].name() == "milk");
    auto readyOnly = app.whatCanIMake(true);
    check("filterBy<RecipeStatus>: only READY recipes are returned", !readyOnly.empty() && readyOnly.size() < app.whatCanIMake(false).size());

    std::cout << "\n== Overload: operator<< for PantryItem ==\n";
    std::ostringstream text;
    text << PantryItem("milk", 0.5, "cups", "2027-01-01");
    check("operator<< writes name,amount,unit,date: " + text.str(), text.str() == "milk,0.5,cups,2027-01-01");

    std::cout << "\n== Persistence ==\n";
    const std::string file = "app_check_pantry.txt";
    std::size_t saved = app.saveTo(file);
    check("save writes every item", saved == 5);
    PantryApp fresh;
    LoadSummary loaded = fresh.loadFrom(file);
    check("load restores all 5 items with no skipped lines", loaded.loaded == 5 && loaded.skipped == 0);
    auto before = app.pantryItems(PantryApp::SortMode::ByName);
    auto after = fresh.pantryItems(PantryApp::SortMode::ByName);
    bool same = before.size() == after.size();
    for (std::size_t i = 0; same && i < before.size(); i++) {
        same = before[i].name() == after[i].name() && before[i].amount() == after[i].amount() &&
               before[i].unit() == after[i].unit() && before[i].expiration() == after[i].expiration();
    }
    check("restored pantry matches the saved pantry", same);
    std::remove(file.c_str());

    std::cout << "\n== File and parsing failures ==\n";
    PantryApp target;
    target.loadSampleData();
    target.addItem("rice", "3", "cups", "2027-01-15");
    std::size_t sizeBeforeFailures = target.pantryItems(PantryApp::SortMode::ByName).size();
    std::string f1 = errorOf([&] { target.loadFrom("no_such_file.txt"); });
    check("missing file reported: " + f1, f1.rfind("Could not open no_such_file.txt", 0) == 0);
    std::string f2 = errorOf([&] { target.loadFrom("sample_data/pantry_empty.txt"); });
    check("empty file reported: " + f2, f2.rfind("No valid items found", 0) == 0);
    check("pantry unchanged after failed loads", target.pantryItems(PantryApp::SortMode::ByName).size() == sizeBeforeFailures);
    LoadSummary partial = target.loadFrom("sample_data/pantry_malformed.txt");
    check("malformed file: 3 valid lines loaded, 3 bad lines skipped: " + partial.message,
          partial.loaded == 3 && partial.skipped == 3);
    check("pantry still usable after a partial load", target.pantryItems(PantryApp::SortMode::ByName).size() == 3);
    LoadSummary good = target.loadFrom("sample_data/pantry_sample.txt");
    check("sample file loads cleanly", good.loaded == 5 && good.skipped == 0);

    std::cout << "\n== Task 2: what can I make? (sample file loaded) ==\n";
    for (const auto& s : target.whatCanIMake()) {
        std::cout << "      " << s.name << ": " << (s.ready ? "READY" : "missing " + std::to_string(s.missingCount)) << "\n";
    }

    std::cout << "\n== Task 3: shopping list ==\n";
    auto list = target.shoppingList("Sugar Cookies");
    for (const auto& m : list) std::cout << "      " << m.amount << " " << m.unit << " " << m.name << "\n";
    check("Sugar Cookies list contains only butter", list.size() == 1 && list[0].name == "butter");
    check("Pancakes needs nothing", target.shoppingList("Pancakes").empty());
    std::string e5 = errorOf([&] { target.shoppingList("Pizza"); });
    check("unknown recipe rejected: " + e5, e5 == "Recipe not found: Pizza");

    std::cout << "\n" << (failures == 0 ? "All checks passed." : "Some checks FAILED.") << "\n";
    return failures == 0 ? 0 : 1;
}
