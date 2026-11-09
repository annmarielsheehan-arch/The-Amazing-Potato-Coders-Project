# The-Amazing-Potato-Coders-Project

**PantryChef** is a C++ application with a Qt GUI that helps a college student track the food in their pantry and when it expires, see which recipes can be made right now, and build a shopping list for a recipe.

**Team:** The Amazing Potato Coders: Annmarie Sheehan and Maria Serpa  
**Course:** Programming II (C++), FGCU

## Setup

- Operating System: Windows 11
- Editor: VS Code (terminal builds use the MSYS2 UCRT64 shell)
- Compiler: g++ 16.1.0 (MSYS2 UCRT64)
- C++ standard: C++17
- GUI framework: Qt Widgets, Qt 6.11.1 (MSYS2 package `mingw-w64-ucrt-x86_64-qt6-base` 6.11.1-1)
- Build tools: CMake 4.3.3, Ninja 1.13.2

## Build and run

### One-time install (MSYS2 UCRT64 terminal)

```
pacman -S --needed mingw-w64-ucrt-x86_64-qt6-base mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
```

### Build and run the GUI

From the project folder that contains `CMakeLists.txt`, in the MSYS2 UCRT64 terminal:

```
cmake -S . -B build -G Ninja
cmake --build build
./build/pantrychef_gui.exe
```

### Other programs

- `./build/pantrychef.exe`: console demo from Checkpoint 5 and the individual changes
- `./build/app_check.exe`: console checks of the application rules, with no GUI

The console demo can also be built without CMake:

```
g++ -std=c++17 -Wall -Wextra -o pantrychef main.cpp Pantry.cpp PantryItem.cpp Recipe.cpp RecipeBook.cpp
```

`build/` is listed in `.gitignore` and is not committed.

## GUI (Checkpoint 6)

PantryChef has one Qt Widgets window that covers all three user tasks.

| Task | GUI control | Application function |
|---|---|---|
| Task 1: manage the pantry | Item name, amount, unit, and expiration date fields; Add item and Remove amount buttons; sort drop-down | `PantryApp::addItem`, `removeAmount`, `pantryItems` |
| Task 2: what can I make? | What can I make? button | `PantryApp::whatCanIMake` |
| Task 3: shopping list | Recipe drop-down and Build shopping list button | `PantryApp::shoppingList` |

- The pantry table on the left always shows the current pantry. The results box on the right shows the output of Tasks 2 and 3.
- A status line at the bottom shows a green message after a successful action and a red error message when an input or operation is rejected. The window stays usable after an error.
- Flow of one action: user input -> event handler in `MainWindow` -> `PantryApp` function -> model classes (`PantryItem`, `Pantry`, `Recipe`, `RecipeBook`) -> result or exception -> table, results box, or status line.
- Rules are enforced in the application and model classes, not in the GUI handlers, so they also work outside the GUI (see `tests/app_check.cpp`).
- The program starts with sample pantry items and five sample recipes. Pantry data is not saved yet.

### GUI checks

| Input or action | Expected result |
|---|---|
| Add `rice`, `1`, `cups`, `2027-01-01` | `rice` appears in the table; green message |
| Add an item with date `2027-02-29` | Red error: `Expiration date must be a real date in YYYY-MM-DD format.`; table unchanged |
| Add an item with amount `abc` | Red error: `Amount must be a number, for example 2 or 0.5.`; table unchanged |
| Remove `5` of `milk` (2 in stock) | Red error: `Cannot remove 5 cups of milk: only 2 available.`; milk stays 2 cups |
| Change sort to Name (A-Z) | Table reorders alphabetically |
| What can I make? (fresh start) | Pancakes READY; Cheese Omelet missing 1; Sugar Cookies missing 2; French Toast missing 1; Crepes READY |
| Build shopping list for Sugar Cookies | `0.5 cups sugar` and `1 cups butter` |

## Current status

Checkpoint 5 and the individual changes are complete (see below). Checkpoint 6 adds a Qt GUI that connects all three user tasks: managing the pantry, finding what can be made, and building a shopping list.

Not yet implemented: saving and loading pantry data (`PantryStorage`), adding custom recipes from the GUI, and unit conversion.
## Project structure

| File | Responsibility |
|---|---|
| `PantryItem.h/.cpp` | One pantry item: name, amount, unit, expiration date (`YYYY-MM-DD`). Rejects negative amounts and invalid expiration dates (wrong format or not a real calendar date, including leap years) by throwing `std::invalid_argument`. |
| `Pantry.h/.cpp` | Contains a `std::vector<PantryItem>` (composition). Adds items (same name, ignoring capitalization, adds to the existing item), removes amounts, and returns items sorted by a strategy. Throws if there is not enough stock or the item is not found. |
| `SortStrategy.h` | Abstract base class `SortStrategy` with pure virtual `before()`. Derived classes: `ByExpiration` (soonest first) and `ByName` (alphabetical). |
| `main.cpp` | Runs the demo. `show()` takes a `const SortStrategy&`, so the same call produces different behavior at runtime. Contains the `try`/`catch` blocks and the test blocks for each individual change. |
| `PantryApp.h/.cpp` | Application layer between the GUI and the model. Parses and validates the text the user typed and calls the model classes. Throws exceptions with user-readable messages. |
| `Recipe.h/.cpp` | One recipe: name and ingredients. Works out which ingredients the pantry is missing. |
| `RecipeBook.h/.cpp` | Contains `Recipe` objects. Reports which recipes are READY or missing ingredients, and builds the shopping list (missing items only). |
| `TextUtil.h` | Small shared helpers (trim, lowercase). |
| `gui/MainWindow.h/.cpp`, `gui/gui_main.cpp` | The Qt window and program entry point. Handlers only collect input, call `PantryApp`, and display results or errors. |
| `tests/app_check.cpp` | Console checks of the application rules, with no GUI. |
| `CMakeLists.txt` | Builds the GUI, the console demo, and the checks. |
| `docs/` | Worksheet photos and the team GUI sketch. |

## Checkpoint 5 requirements and where they are met

- **One complete user task:** adding items and displaying the pantry sorted (`Pantry::addItem`, `Pantry::sorted`, `show()`).
- **Composition:** `Pantry` holds `PantryItem` objects in a vector.
- **Runtime polymorphism:** `SortStrategy` (pure virtual `before()`), overridden by `ByExpiration` and `ByName`; called through a base reference in `show()` and in `Pantry::sorted`.
- **Handled invalid operation:** `throw` in `Pantry::removeAmount` and the `PantryItem` constructor, `catch` in `main`. Checks run before any change, so valid data is preserved and the program keeps running.

## Individual Artifact 1 changes

| Member | Change | File | What it does |
|---|---|---|---|
| Maria Serpa | Expiration-date validation | `PantryItem.cpp` | `PantryItem` rejects any date that is not a real calendar date in `YYYY-MM-DD` format (checks shape, month 1-12, day within the month, leap years) and throws `std::invalid_argument`. |
| Annmarie Sheehan | Case-insensitive item names | `Pantry.cpp` | `Pantry::addItem` and `Pantry::removeAmount` compare names ignoring capitalization, so `Milk`, `milk`, and `MILK` are one item. The first capitalization entered is the one displayed. |

## Check sequence (Checkpoint 5)

| Input or action | Expected result | Observed result |
|---|---|---|
| Normal task case: add milk, flour, eggs and display | All three items listed with correct amounts | Matched expected output |
| First derived behavior: `ByExpiration` | eggs (2026-11-05), milk (2026-11-10), flour (2027-03-01) | Matched expected output |
| Second derived behavior: `ByName` | eggs, flour, milk | Matched expected output |
| Invalid operation and continued use: remove 3 cups of milk (only 2), add item with negative amount, then remove 2 eggs and redisplay | `Error: Cannot remove 3 cups of milk: only 2 available.` then `Error: Amount cannot be negative.`; milk stays at 2; eggs drop to 4; list still displays | Matched expected output |

## Check sequence (individual changes)

| Input or action | Expected result |
|---|---|
| Add `2028-02-29` (leap day) | `Accepted: 2028-02-29` |
| Add `2027-02-29` (not a leap year) | `Rejected 2027-02-29: Expiration date must be a real date in YYYY-MM-DD format.` Pantry unchanged |
| Add `2026-13-01` (month out of range) | `Rejected 2026-13-01: Expiration date must be a real date in YYYY-MM-DD format.` |
| Add `not-a-date` | Rejected with the same date error |
| Add `2026-1-5` (no zero padding) | Rejected with the same date error |
| Add `Milk` 1, `milk` 2, `MILK` 1 to a new pantry | One line: `Milk - 4 cups (exp 2026-11-10)` |
| Remove 1 using `mIlK` | `Milk - 3 cups`; the item is found despite different capitalization |
| Remove 5 using `MILK` (only 3 in stock) | `Error: Cannot remove 5 cups of MILK: only 3 available.` Quantity stays 3 |
| Remove `butter` (not in pantry) | `Error: Item not found: butter` |

## Expected console demo output

```
== 1. Normal task + first derived behavior ==
Pantry sorted by expiration date:
  eggs - 6 count (exp 2026-11-05)
  milk - 2 cups (exp 2026-11-10)
  flour - 4 cups (exp 2027-03-01)

== 2. Second derived behavior ==
Pantry sorted alphabetically by name:
  eggs - 6 count (exp 2026-11-05)
  flour - 4 cups (exp 2027-03-01)
  milk - 2 cups (exp 2026-11-10)

== Case-insensitive names (individual change) ==
Pantry sorted alphabetically by name:
  Milk - 4 cups (exp 2026-11-10)
Pantry sorted alphabetically by name:
  Milk - 3 cups (exp 2026-11-10)
Error: Cannot remove 5 cups of MILK: only 3 available.
Error: Item not found: butter
Pantry sorted alphabetically by name:
  Milk - 3 cups (exp 2026-11-10)

== 3. Invalid operation and continued use ==
Error: Cannot remove 3 cups of milk: only 2 available.
Error: Amount cannot be negative.
Pantry sorted by expiration date:
  eggs - 4 count (exp 2026-11-05)
  milk - 2 cups (exp 2026-11-10)
  flour - 4 cups (exp 2027-03-01)

== 4. Date validation ==
Accepted: 2028-02-29
Rejected 2027-02-29: Expiration date must be a real date in YYYY-MM-DD format.
Rejected 2026-13-01: Expiration date must be a real date in YYYY-MM-DD format.
Rejected not-a-date: Expiration date must be a real date in YYYY-MM-DD format.
Rejected 2026-1-5: Expiration date must be a real date in YYYY-MM-DD format.
Pantry sorted by expiration date:
  eggs - 4 count (exp 2026-11-05)
  milk - 2 cups (exp 2026-11-10)
  flour - 4 cups (exp 2027-03-01)
  rice - 1 cups (exp 2028-02-29)
```

## Team contributions and build checks

| Member | Own commit & change | Teammate update checked | Build result & setup correction |
|---|---|---|---|
| Annmarie Sheehan | Creating Project and answering question 3 in README.md. Individual Artifact 1: made item names case-insensitive in `Pantry.cpp` (`Milk`, `milk`, and `MILK` are one item) and added a test block in `main.cpp`. | Maria Serpa: expiration-date validation in `PantryItem.cpp` | Saw changes & everything looks good. |
| Maria Serpa | Added to the README.md in the folder for PantryChef, and added an overall README.md file. Individual Artifact 1: added expiration-date validation to `PantryItem.cpp` (rejects dates that are not real `YYYY-MM-DD` dates) and a test block in `main.cpp`. | Annmarie Sheehan: case-insensitive item names in `Pantry.cpp` | Saw changes & everything looks good. |

## Known limitations

- Dates are checked for format and calendar validity, but not for whether they are in the past.
- `ByName` sorting compares raw text, so a capitalized name can sort ahead of lowercase names.
- Item names with leading or trailing spaces are not trimmed, so ` milk` and `milk` are different items.
- Each ingredient must use a single unit; there is no unit conversion.
- Pantry data is not saved between runs, and recipes are built-in samples.

## Next improvements

1. `PantryStorage` to save and load pantry and recipe data from text files, skipping malformed lines.
2. Let the user add their own recipes from the GUI.
3. Unit conversion (for example cups to ounces).
4. Make `ByName` sorting case-insensitive and trim extra spaces from names.

