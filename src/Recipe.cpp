#include "Recipe.h"
#include <stdexcept>
#include <utility>
#include "Pantry.h"
#include "TextUtil.h"

Recipe::Recipe(std::string name) : name_(std::move(name)) {
    if (textutil::trim(name_).empty()) {
        throw std::invalid_argument("Recipe name cannot be empty.");
    }
}

void Recipe::addIngredient(const std::string& name, double amount, const std::string& unit) {
    if (textutil::trim(name).empty()) {
        throw std::invalid_argument("Ingredient name cannot be empty.");
    }
    if (amount <= 0) {
        throw std::invalid_argument("Ingredient amount must be greater than 0.");
    }
    ingredients_.push_back({name, amount, unit});
}

const std::string& Recipe::name() const { return name_; }
const std::vector<Ingredient>& Recipe::ingredients() const { return ingredients_; }

std::vector<Ingredient> Recipe::missing(const Pantry& pantry) const {
    std::vector<Ingredient> result;
    for (const auto& needed : ingredients_) {
        double have = 0;
        for (const auto& item : pantry.items()) {
            if (textutil::toLower(item.name()) == textutil::toLower(needed.name) &&
                textutil::toLower(item.unit()) == textutil::toLower(needed.unit)) {
                have += item.amount();
            }
        }
        if (have + 1e-9 < needed.amount) {
            result.push_back({needed.name, needed.amount - have, needed.unit});
        }
    }
    return result;
}

bool Recipe::canMake(const Pantry& pantry) const {
    return missing(pantry).empty();
}
