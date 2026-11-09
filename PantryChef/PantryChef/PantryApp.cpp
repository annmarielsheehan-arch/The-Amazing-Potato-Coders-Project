#include "PantryApp.h"
#include <stdexcept>
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

std::vector<PantryItem> PantryApp::pantryItems(SortMode mode) const {
    if (mode == SortMode::ByName) {
        ByName strategy;
        return pantry_.sorted(strategy);
    }
    ByExpiration strategy;
    return pantry_.sorted(strategy);
}

std::vector<RecipeStatus> PantryApp::whatCanIMake() const {
    return recipeBook_.statuses(pantry_);
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
