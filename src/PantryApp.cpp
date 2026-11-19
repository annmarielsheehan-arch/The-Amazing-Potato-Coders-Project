#include "PantryApp.h"
#include <stdexcept>
#include "Filter.h"
#include "PantryStorage.h"
#include "SortStrategy.h"
#include "TextUtil.h"

double PantryApp::parseAmount(const std::string& text) {
    std::string t = textutil::trim(text);
    if (t.empty()) {
        throw std::invalid_argument("Amount is required.");
    }
    double value = 0;
    std::size_t used = 0;
    try {
        value = std::stod(t, &used);
    } catch (const std::exception&) {
        throw std::invalid_argument("Amount must be a number, for example 2 or 0.5.");
    }
    if (used != t.size()) {
        throw std::invalid_argument("Amount must be a number, for example 2 or 0.5.");
    }
    if (value <= 0) {
        throw std::invalid_argument("Amount must be greater than 0.");
    }
    return value;
}

void PantryApp::addItem(const std::string& name, const std::string& amountText,
                        const std::string& unit, const std::string& expiration) {
    std::string cleanName = textutil::trim(name);
    std::string cleanUnit = textutil::trim(unit);
    if (cleanName.empty()) {
        throw std::invalid_argument("Item name is required.");
    }
    if (cleanUnit.empty()) {
        throw std::invalid_argument("Unit is required, for example cups or count.");
    }
    if (cleanName.find(',') != std::string::npos || cleanUnit.find(',') != std::string::npos) {
        throw std::invalid_argument("Item name and unit cannot contain commas.");
    }
    double amount = parseAmount(amountText);
    pantry_.addItem(PantryItem(cleanName, amount, cleanUnit, textutil::trim(expiration)));
}

void PantryApp::removeAmount(const std::string& name, const std::string& amountText) {
    std::string cleanName = textutil::trim(name);
    if (cleanName.empty()) {
        throw std::invalid_argument("Item name is required.");
    }
    pantry_.removeAmount(cleanName, parseAmount(amountText));
}

std::vector<PantryItem> PantryApp::pantryItems(SortMode mode, const std::string& searchText) const {
    std::vector<PantryItem> sortedItems;
    if (mode == SortMode::ByName) {
        ByName strategy;
        sortedItems = pantry_.sorted(strategy);
    } else {
        ByExpiration strategy;
        sortedItems = pantry_.sorted(strategy);
    }

    std::string needle = textutil::toLower(textutil::trim(searchText));
    if (needle.empty()) {
        return sortedItems;
    }
    return filterBy(sortedItems, [&needle](const PantryItem& item) {
        return textutil::toLower(item.name()).find(needle) != std::string::npos;
    });
}

std::vector<RecipeStatus> PantryApp::whatCanIMake(bool onlyReady) const {
    std::vector<RecipeStatus> statuses = recipeBook_.statuses(pantry_);
    if (!onlyReady) {
        return statuses;
    }
    return filterBy(statuses, [](const RecipeStatus& status) { return status.ready; });
}

std::vector<std::string> PantryApp::recipeNames() const {
    std::vector<std::string> names;
    for (const auto& recipe : recipeBook_.recipes()) {
        names.push_back(recipe.name());
    }
    return names;
}

std::vector<Ingredient> PantryApp::shoppingList(const std::string& recipeName) const {
    return recipeBook_.shoppingList(recipeName, pantry_);
}

std::size_t PantryApp::saveTo(const std::string& path) const {
    std::string cleanPath = textutil::trim(path);
    if (cleanPath.empty()) {
        throw std::invalid_argument("Enter a file name first.");
    }
    return PantryStorage::save(pantry_, cleanPath);
}

LoadSummary PantryApp::loadFrom(const std::string& path) {
    std::string cleanPath = textutil::trim(path);
    if (cleanPath.empty()) {
        throw std::invalid_argument("Enter a file name first.");
    }
    LoadResult result = PantryStorage::load(cleanPath);

    if (result.items.empty()) {
        std::string message = "No valid items found in " + cleanPath + ". The current pantry was not changed.";
        if (!result.problems.empty()) {
            message += " " + result.problems.front();
        }
        throw std::runtime_error(message);
    }

    Pantry loaded;
    for (const PantryItem& item : result.items) {
        loaded.addItem(item);
    }
    pantry_ = loaded;

    LoadSummary summary;
    summary.loaded = static_cast<int>(result.items.size());
    summary.skipped = static_cast<int>(result.problems.size());
    summary.message = "Loaded " + std::to_string(summary.loaded) + " items from " + cleanPath + ".";
    if (summary.skipped > 0) {
        summary.message += " Skipped " + std::to_string(summary.skipped) + " bad line(s). First problem: " +
                           result.problems.front();
    }
    return summary;
}

void PantryApp::loadSampleData() {
    pantry_.addItem(PantryItem("milk", 2, "cups", "2026-11-10"));
    pantry_.addItem(PantryItem("flour", 4, "cups", "2027-03-01"));
    pantry_.addItem(PantryItem("eggs", 6, "count", "2026-11-05"));
    pantry_.addItem(PantryItem("sugar", 0.5, "cups", "2027-06-01"));

    Recipe pancakes("Pancakes");
    pancakes.addIngredient("flour", 2, "cups");
    pancakes.addIngredient("milk", 1, "cups");
    pancakes.addIngredient("eggs", 2, "count");
    recipeBook_.addRecipe(pancakes);

    Recipe omelet("Cheese Omelet");
    omelet.addIngredient("eggs", 3, "count");
    omelet.addIngredient("milk", 0.25, "cups");
    omelet.addIngredient("cheese", 1, "cups");
    recipeBook_.addRecipe(omelet);

    Recipe cookies("Sugar Cookies");
    cookies.addIngredient("flour", 2.5, "cups");
    cookies.addIngredient("sugar", 1, "cups");
    cookies.addIngredient("butter", 1, "cups");
    cookies.addIngredient("eggs", 1, "count");
    recipeBook_.addRecipe(cookies);

    Recipe frenchToast("French Toast");
    frenchToast.addIngredient("eggs", 2, "count");
    frenchToast.addIngredient("milk", 0.5, "cups");
    frenchToast.addIngredient("bread", 4, "slices");
    recipeBook_.addRecipe(frenchToast);

    Recipe crepes("Crepes");
    crepes.addIngredient("flour", 1, "cups");
    crepes.addIngredient("milk", 1.5, "cups");
    crepes.addIngredient("eggs", 2, "count");
    recipeBook_.addRecipe(crepes);
}
