#pragma once
#include <string>
#include <vector>
#include "Pantry.h"
#include "PantryItem.h"

struct LoadResult {
    std::vector<PantryItem> items;
    std::vector<std::string> problems;
    int linesRead = 0;
};

class PantryStorage {
public:
    static std::size_t save(const Pantry& pantry, const std::string& path);
    static LoadResult load(const std::string& path);
};
