#include <iostream>
#include "PantryItem.h"

int main() {
    try {
        PantryItem bad("rice", 1, "cups", "2027-02-29");
        std::cout << "created " << bad << "\n";
    } catch (const std::exception& e) {
        std::cout << "Rejected: " << e.what() << "\n";
    }

    {
        PantryItem good("rice", 1, "cups", "2028-02-29");
        std::cout << "created " << good << "\n";
    }
    std::cout << "good has been destroyed\n";
    return 0;
}
