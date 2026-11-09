#pragma once
#include <string>
#include <vector>
#include "Pantry.h"
#include "PantryItem.h"
#include "Recipe.h"
#include "RecipeBook.h"

class PantryApp {
public:
    enum class SortMode { ByExpiration, ByName };

private:
    Pantry pantry_;
    RecipeBook recipeBook_;

    static double parseAmount(const std::string& text);

public:
    void addItem(const std::string& name, const std::string& amountText,
                 const std::string& unit, const std::string& expiration);
    void removeAmount(const std::string& name, const std::string& amountText);
    std::vector<PantryItem> pantryItems(SortMode mode) const;

    std::vector<RecipeStatus> whatCanIMake() const;

    std::vector<std::string> recipeNames() const;
    std::vector<Ingredient> shoppingList(const std::string& recipeName) const;

    void loadSampleData();
};
