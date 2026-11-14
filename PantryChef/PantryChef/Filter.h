#pragma once
#include <vector>

template <typename T, typename Predicate>
std::vector<T> filterBy(const std::vector<T>& items, Predicate keep) {
    std::vector<T> result;
    for (const T& item : items) {
        if (keep(item)) {
            result.push_back(item);
        }
    }
    return result;
}
