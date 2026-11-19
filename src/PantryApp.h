#pragma once
#include <string>
#include <vector>
#include "Pantry.h"
#include "PantryItem.h"
#include "Recipe.h"
#include "RecipeBook.h"

struct LoadSummary {
    int loaded = 0;
    int skipped = 0;
    std::string message;
};

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
    std::vector<PantryItem> pantryItems(SortMode mode, const std::string& searchText = "") const;

    std::vector<RecipeStatus> whatCanIMake(bool onlyReady = false) const;

    std::vector<std::string> recipeNames() const;
    std::vector<Ingredient> shoppingList(const std::string& recipeName) const;

    std::size_t saveTo(const std::string& path) const;
    LoadSummary loadFrom(const std::string& path);

    void loadSampleData();
};
