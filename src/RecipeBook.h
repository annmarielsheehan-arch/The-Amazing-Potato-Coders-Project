#pragma once
#include <string>
#include <vector>
#include "Recipe.h"

class Pantry;

struct RecipeStatus {
    std::string name;
    bool ready;
    int missingCount;
};

class RecipeBook {
private:
    std::vector<Recipe> recipes_;

public:
    void addRecipe(const Recipe& recipe);

    const std::vector<Recipe>& recipes() const;
    const Recipe& find(const std::string& name) const;

    std::vector<RecipeStatus> statuses(const Pantry& pantry) const;
    std::vector<Ingredient> shoppingList(const std::string& recipeName, const Pantry& pantry) const;
};
