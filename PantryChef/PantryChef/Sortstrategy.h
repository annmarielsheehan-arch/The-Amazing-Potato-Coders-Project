#pragma once
#include <string>
#include "PantryItem.h"


class SortStrategy {
public:
    virtual ~SortStrategy() = default;  // virtual so derived objects are destroyed correctly
    virtual bool before(const PantryItem& a, const PantryItem& b) const = 0;
    virtual std::string label() const = 0;
};


class ByExpiration : public SortStrategy {
public:
    bool before(const PantryItem& a, const PantryItem& b) const override {
        return a.expiration() < b.expiration();
    }
    std::string label() const override { return "sorted by expiration date"; }
};


class ByName : public SortStrategy {
public:
    bool before(const PantryItem& a, const PantryItem& b) const override {
        return a.name() < b.name();
    }
    std::string label() const override { return "sorted alphabetically by name"; }
};