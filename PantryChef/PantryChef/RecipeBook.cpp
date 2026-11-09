#include "RecipeBook.h"
#include <stdexcept>
#include "Pantry.h"
#include "TextUtil.h"

void RecipeBook::addRecipe(const Recipe& recipe) {
    for (const auto& existing : recipes_) {
        if (textutil::toLower(existing.name()) == textutil::toLower(recipe.name())) {
            throw std::invalid_argument("A recipe named " + recipe.name() + " already exists.");
        }
    }
    recipes_.push_back(recipe);
}

const std::vector<Recipe>& RecipeBook::recipes() const { return recipes_; }

const Recipe& RecipeBook::find(const std::string& name) const {
    for (const auto& recipe : recipes_) {
        if (textutil::toLower(recipe.name()) == textutil::toLower(textutil::trim(name))) {
            return recipe;
        }
    }
    throw std::runtime_error("Recipe not found: " + name);
}

std::vector<RecipeStatus> RecipeBook::statuses(const Pantry& pantry) const {
    std::vector<RecipeStatus> result;
    for (const auto& recipe : recipes_) {
        int missingCount = static_cast<int>(recipe.missing(pantry).size());
        result.push_back({recipe.name(), missingCount == 0, missingCount});
    }
    return result;
}

std::vector<Ingredient> RecipeBook::shoppingList(const std::string& recipeName, const Pantry& pantry) const {
    return find(recipeName).missing(pantry);
}
