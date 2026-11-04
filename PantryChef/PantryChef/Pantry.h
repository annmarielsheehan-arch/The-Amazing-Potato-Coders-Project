#pragma once
#pragma once
#include <string>
#include <vector>
#include "PantryItem.h"
#include "SortStrategy.h"

class Pantry {
private:
    std::vector<PantryItem> items_;  

public:
    void addItem(const PantryItem& item);                
    void removeAmount(const std::string& name, double amount);   
    std::vector<PantryItem> sorted(const SortStrategy& strategy) const;
};