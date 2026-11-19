#pragma once
#include <string>
#include <vector>

class Pantry;

struct Ingredient {
    std::string name;
    double amount;
    std::string unit;
};

class Recipe {
private:
    std::string name_;
    std::vector<Ingredient> ingredients_;

public:
    explicit Recipe(std::string name);

    void addIngredient(const std::string& name, double amount, const std::string& unit);

    const std::string& name() const;
    const std::vector<Ingredient>& ingredients() const;

    std::vector<Ingredient> missing(const Pantry& pantry) const;
    bool canMake(const Pantry& pantry) const;
};
