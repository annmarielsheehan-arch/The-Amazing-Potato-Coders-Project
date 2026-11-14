#include "PantryStorage.h"
#include <fstream>
#include <stdexcept>
#include "TextUtil.h"

namespace {

std::vector<std::string> splitFields(const std::string& line) {
    std::vector<std::string> fields;
    std::size_t start = 0;
    while (true) {
        std::size_t comma = line.find(',', start);
        if (comma == std::string::npos) {
            fields.push_back(line.substr(start));
            break;
        }
        fields.push_back(line.substr(start, comma - start));
        start = comma + 1;
    }
    return fields;
}

}

std::size_t PantryStorage::save(const Pantry& pantry, const std::string& path) {
    std::ofstream out(path);
    if (!out) {
        throw std::runtime_error("Could not open " + path + " for writing.");
    }
    for (const PantryItem& item : pantry.items()) {
        out << item << "\n";
    }
    out.flush();
    if (!out) {
        throw std::runtime_error("Could not write all data to " + path + ".");
    }
    return pantry.items().size();
}

LoadResult PantryStorage::load(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Could not open " + path + ". Check the file name and location.");
    }

    LoadResult result;
    std::string line;
    int lineNumber = 0;
    while (std::getline(in, line)) {
        lineNumber++;
        if (textutil::trim(line).empty()) {
            continue;
        }
        result.linesRead++;
        std::string where = "Line " + std::to_string(lineNumber) + ": ";

        std::vector<std::string> fields = splitFields(line);
        if (fields.size() != 4) {
            result.problems.push_back(where + "expected 4 fields (name,amount,unit,date).");
            continue;
        }
        std::string name = textutil::trim(fields[0]);
        std::string amountText = textutil::trim(fields[1]);
        std::string unit = textutil::trim(fields[2]);
        std::string date = textutil::trim(fields[3]);
        if (name.empty() || unit.empty()) {
            result.problems.push_back(where + "name and unit cannot be empty.");
            continue;
        }

        double amount = 0;
        std::size_t used = 0;
        try {
            amount = std::stod(amountText, &used);
        } catch (const std::exception&) {
            used = 0;
        }
        if (amountText.empty() || used != amountText.size()) {
            result.problems.push_back(where + "amount \"" + amountText + "\" is not a number.");
            continue;
        }

        try {
            result.items.push_back(PantryItem(name, amount, unit, date));
        } catch (const std::exception& e) {
            result.problems.push_back(where + e.what());
        }
    }
    return result;
}
