# The-Amazing-Potato-Coders-Project

**PantryChef** is a C++ console application that helps a college student track the food in their pantry and when it expires. Planned later features include matching pantry contents to recipes and building a shopping list.

**Team:** The Amazing Potato Coders: Annmarie Sheehan and Maria Serpa  
**Course:** Programming II (C++), FGCU

## Setup

- Operating System: Windows 11
- Editor: VS Code
- Compiler: g++ 16.1.0 (MSYS2 UCRT64) (confirm with `g++ --version` and update if different)
- C++ standard: C++17
- Dependencies: None (standard library only)

## Build and run

From the project folder in a terminal:

```
g++ -std=c++17 -Wall -Wextra -o pantrychef main.cpp Pantry.cpp PantryItem.cpp Recipe.cpp RecipeBook.cpp
.\pantrychef.exe
```

## Current status

Checkpoint 5 task implemented: **Track the pantry.** Add items, then view them sorted. The program runs a scripted demo in `main.cpp`; no keyboard input is needed yet.

Individual Artifact 1 changes are included: **expiration-date validation** (Maria Serpa, `PantryItem.cpp`). Annmarie Sheehan's change (case-insensitive item names in `Pantry.cpp`) is still to come.

Not yet implemented: recipe matching ("what can I make?"), shopping list, file saving/loading (`PantryStorage`), a user menu, and a GUI. `Recipe` and `RecipeBook` are placeholders.

## Project structure

| File | Responsibility |
|---|---|
| `PantryItem.h/.cpp` | One pantry item: name, amount, unit, expiration date (`YYYY-MM-DD`). Rejects negative amounts and invalid expiration dates (wrong format or not a real calendar date, including leap years) by throwing `std::invalid_argument`. |
| `Pantry.h/.cpp` | Contains a `std::vector<PantryItem>` (composition). Adds items (same name adds to the existing item), removes amounts, and returns items sorted by a strategy. Throws if there is not enough stock or the item is not found. |
| `SortStrategy.h` | Abstract base class `SortStrategy` with pure virtual `before()`. Derived classes: `ByExpiration` (soonest first) and `ByName` (alphabetical). |
| `main.cpp` | Runs the demo. `show()` takes a `const SortStrategy&`, so the same call produces different behavior at runtime. Contains the `try`/`catch` blocks and the test block for the date validation change. |
| `Recipe.h/.cpp`, `RecipeBook.h/.cpp` | Planned for the recipe-matching task. |

## Checkpoint 5 requirements and where they are met

- **One complete user task:** adding items and displaying the pantry sorted (`Pantry::addItem`, `Pantry::sorted`, `show()`).
- **Composition:** `Pantry` holds `PantryItem` objects in a vector.
- **Runtime polymorphism:** `SortStrategy` (pure virtual `before()`), overridden by `ByExpiration` and `ByName`; called through a base reference in `show()` and in `Pantry::sorted`.
- **Handled invalid operation:** `throw` in `Pantry::removeAmount` and the `PantryItem` constructor, `catch` in `main`. Checks run before any change, so valid data is preserved and the program keeps running.

## Individual Artifact 1 changes

| Member | Change | File | What it does |
|---|---|---|---|
| Maria Serpa | Expiration-date validation | `PantryItem.cpp` | `PantryItem` rejects any date that is not a real calendar date in `YYYY-MM-DD` format (checks shape, month 1-12, day within the month, leap years) and throws `std::invalid_argument`. |
| Annmarie Sheehan | Case-insensitive item names (planned) | `Pantry.cpp` | Not yet included in this version. |

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

## Expected program output

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
| Annmarie Sheehan | Creating Project and answering question 3 in README.md | Maria Serpa: expiration-date validation in `PantryItem.cpp` | Saw changes & everything looks good. |
| Maria Serpa | Added to the README.md in the folder for PantryChef, and added an overall README.md file. Individual Artifact 1: added expiration-date validation to `PantryItem.cpp` (rejects dates that are not real `YYYY-MM-DD` dates) and a test block in `main.cpp`. | Annmarie Sheehan | Saw changes & everything looks good. |

## Known limitations

- Dates are checked for format and calendar validity, but not for whether they are in the past.
- Item names are case-sensitive, so `Milk` and `milk` are separate items (Annmarie's change addresses this).
- Each ingredient must use a single unit; there is no unit conversion.

## Next improvements

1. Task 2: recipe matching ("what can I make?") using `Recipe` and `RecipeBook`.
2. `PantryStorage` to save and load pantry and recipe data from text files, skipping malformed lines (and rejecting bad dates through the same `PantryItem` rule).
3. A menu so the user can enter items instead of using the scripted demo.
4. Normalize item names (case-insensitive matching).

