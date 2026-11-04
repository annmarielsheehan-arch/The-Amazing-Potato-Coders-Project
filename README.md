# The-Amazing-Potato-Coders-Project

**PantryChef** is a C++ console application that helps a college student track the food in their pantry and when it expires. Planned later features include matching pantry contents to recipes and building a shopping list.

**Team:** The Amazing Potato Coders: Annmarie Sheehan and Maria Serpa
**Course:** COP 3003; Programming II

## Setup

- Operating System: Windows 11
- Editor: VS Code
- Compiler: g++ 14.x (MSYS2 MinGW-w64) (confirm with `g++ --version` and update if different)
- C++ standard: C++17
- Dependencies: None (standard library only)

## Build and run

From the project folder in a terminal:

```
g++ -std=c++17 -Wall -Wextra -o pantrychef *.cpp
.\pantrychef.exe
```

If the `*.cpp` wildcard does not work in your shell, list the files:

```
g++ -std=c++17 -Wall -Wextra -o pantrychef main.cpp Pantry.cpp PantryItem.cpp Recipe.cpp RecipeBook.cpp
```

## Current status (Checkpoint 5)

Implemented task: **Track the pantry.** Add items, then view them sorted. The program runs a scripted demo in `main.cpp`; no keyboard input is needed yet.

Not yet implemented: recipe matching ("what can I make?"), shopping list, file saving/loading (`PantryStorage`), a user menu, and a GUI. `Recipe` and `RecipeBook` are placeholders.

## Project structure

| File | Responsibility |
|---|---|
| `PantryItem.h/.cpp` | One pantry item: name, amount, unit, expiration date (`YYYY-MM-DD`). Rejects negative amounts by throwing an exception. |
| `Pantry.h/.cpp` | Contains a `std::vector<PantryItem>` (composition). Adds items (same name adds to the existing item), removes amounts, and returns items sorted by a strategy. Throws if there is not enough stock or the item is not found. |
| `SortStrategy.h` | Abstract base class `SortStrategy` with pure virtual `before()`. Derived classes: `ByExpiration` (soonest first) and `ByName` (alphabetical). |
| `main.cpp` | Runs the demo. `show()` takes a `const SortStrategy&`, so the same call produces different behavior at runtime. Contains the `try`/`catch` blocks. |
| `Recipe.h/.cpp`, `RecipeBook.h/.cpp` | Planned for the recipe-matching task. |

## Checkpoint 5 requirements and where they are met

- **One complete user task:** adding items and displaying the pantry sorted (`Pantry::addItem`, `Pantry::sorted`, `show()`).
- **Composition:** `Pantry` holds `PantryItem` objects in a vector.
- **Runtime polymorphism:** `SortStrategy` (pure virtual `before()`), overridden by `ByExpiration` and `ByName`; called through a base reference in `show()` and in `Pantry::sorted`.
- **Handled invalid operation:** `throw` in `Pantry::removeAmount` and the `PantryItem` constructor, `catch` in `main`. Checks run before any change, so valid data is preserved and the program keeps running.

## Check sequence

| Input or action | Expected result | Observed result |
|---|---|---|
| Normal task case: add milk, flour, eggs and display | All three items listed with correct amounts | |
| First derived behavior: `ByExpiration` | eggs (2026-11-05), milk (2026-11-10), flour (2027-03-01) | |
| Second derived behavior: `ByName` | eggs, flour, milk | |
| Invalid operation and continued use: remove 3 cups of milk (only 2 in stock), add item with negative amount, then remove 2 eggs and redisplay | `Error: Cannot remove 3 cups of milk: only 2 available.` then `Error: Amount cannot be negative.`; milk stays at 2; eggs drop to 4; list still displays | |

Expected program output:

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
```

## Team contributions and build checks

| Member | Own commit & change | Teammate update checked | Build result & setup correction |
|---|---|---|---|
| Annmarie Sheehan | Creating Project and answering question 3 in README.md | Maria Serpa | |
| Maria Serpa | | Annmarie Sheehan | |

## Known limitations

- Item names are case-sensitive, so "Milk" and "milk" would be separate items.
- Dates are stored as `YYYY-MM-DD` text and are not validated yet.
- Each ingredient must use a single unit; there is no unit conversion.

## Next improvements

1. Task 2: recipe matching ("what can I make?") using `Recipe` and `RecipeBook`.
2. `PantryStorage` to save and load pantry and recipe data from text files, skipping malformed lines.
3. A menu so the user can enter items instead of using the scripted demo.
4. Input normalization (lowercase names) and date validation.

