# The-Amazing-Potato-Coders-Project

**PantryChef** is a C++ application with a Qt GUI that helps a college student track the food in their pantry and when it expires, see which recipes can be made right now, build a shopping list for a recipe, and save and load the pantry.

**Team:** The Amazing Potato Coders: Annmarie Sheehan and Maria Serpa
**Course:** Programming II

## Setup

- Operating System: Windows 11
- Compiler: g++ 16.1.0 (MSYS2 UCRT64)
- C++ standard: C++17
- GUI framework: Qt Widgets, Qt 6.11.1 (MSYS2 package `mingw-w64-ucrt-x86_64-qt6-base` 6.11.1-1)
- Build tools: CMake 4.3.3, Ninja 1.13.2
- Optional, for the debugger step: gdb (`mingw-w64-ucrt-x86_64-gdb`)

### Install the dependencies (MSYS2 UCRT64 terminal)

```
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-qt6-base mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
```

## Get the code

```
git clone https://github.com/annmarielsheehan-arch/The-Amazing-Potato-Coders-Project.git
cd The-Amazing-Potato-Coders-Project
git checkout <exact checkpoint commit>
```

All commands below are run from the repository root (the folder that contains `CMakeLists.txt`) in the MSYS2 UCRT64 terminal.

## Build

```
cmake -S . -B build -G Ninja
cmake --build build
```

This builds four programs in `build/`: `pantrychef_gui.exe` (the GUI), `app_check.exe` (console checks of the application rules), `pantrychef.exe` (the console demo), and `debug_date.exe` (a small program for the debugger session).

## Run

```
./build/pantrychef_gui.exe
```

Run it from the repository root so the relative paths to `sample_data/` work.

## Sample data (fictional)

| File | Contents | Used for |
|---|---|---|
| `sample_data/pantry_sample.txt` | 5 valid pantry lines | Loading a clean file |
| `sample_data/pantry_malformed.txt` | 3 valid and 3 bad lines (non-numeric amount, missing field, impossible date) | File and parsing failure |
| `sample_data/pantry_empty.txt` | Empty file | Failure with no valid data |

File format: one item per line, `name,amount,unit,YYYY-MM-DD`, for example `bread,8,slices,2026-11-18`.

## Check sequence (GUI)

| Step | Input or action | Expected result |
|---|---|---|
| 1 | Launch the GUI | Window opens; green message "Ready. Sample pantry and recipes are loaded."; table shows eggs, milk, flour, sugar sorted by expiration |
| 2 | Type `sample_data/pantry_sample.txt` in the File box and click Load | "Loaded 5 items from sample_data/pantry_sample.txt."; table order: bread, milk, eggs, flour, sugar. Change the sort menu to Name (A-Z) for a different order (bread, eggs, flour, milk, sugar) |
| 3 | Task 2: click What can I make? | Pancakes READY; Cheese Omelet missing 1; Sugar Cookies missing 1; French Toast READY; Crepes READY. With "Only show recipes I can make now" checked: only Pancakes, French Toast, Crepes |
| 4 | Task 3: choose Sugar Cookies, click Build shopping list | "Shopping list for Sugar Cookies:" with `1 cups butter` only |
| 5 | Task 1 valid: add `rice`, `3`, `cups`, `2027-01-15`. Then type `mi` in the search box | Green "Added rice to the pantry."; rice appears; search shows only milk |
| 6 | Invalid operations: add an item with date `2027-02-29`; remove `5` of `milk` | Red "Expiration date must be a real date in YYYY-MM-DD format." and "Cannot remove 5 cups of milk: only 2 available."; table unchanged; window still usable |
| 7 | Persistence: type `my_pantry.txt`, click Save; close the program; run it again; type `my_pantry.txt` and click Load | "Saved 6 items to my_pantry.txt."; after restart and Load: "Loaded 6 items" and the table matches (includes rice) |
| 8 | File failure: Load `sample_data/pantry_malformed.txt`; then `sample_data/pantry_empty.txt`; then `missing.txt` | Orange warning: "Loaded 3 items ... Skipped 3 bad line(s). First problem: Line 2: amount "four" is not a number."; then red "No valid items found ... The current pantry was not changed."; then red "Could not open missing.txt." Pantry stays usable after each |

Console check of the application rules (no GUI):

```
./build/app_check.exe
```

It prints PASS or FAIL for each rule and ends with "All checks passed."

## Project structure

```
├── CMakeLists.txt
├── README.md
├── REQUIREMENTS.md
├── LIFETIME_AND_DEBUGGING.md
├── src/            model and application classes, console demo (main.cpp)
│   └── gui/        Qt window
├── tests/          console checks and debugger program
├── sample_data/    fictional pantry files
└── docs/           GUI sketch
```

All paths below are inside `src/` unless they start with `tests/`.

| File | Responsibility |
|---|---|
| `gui/MainWindow.h/.cpp`, `gui/gui_main.cpp` | The Qt window and entry point. Handlers only collect input, call `PantryApp`, and show results or errors. |
| `PantryApp.h/.cpp` | Application layer: parses and validates typed input, filters and sorts, saves and loads, and calls the model classes. |
| `PantryItem.h/.cpp` | One item. Rejects negative amounts and invalid dates. Overloads `operator<<` to write a save-file record. |
| `Pantry.h/.cpp` | Contains the `PantryItem` objects; adds, removes, and sorts through a `SortStrategy`. |
| `SortStrategy.h` | Abstract base class with pure virtual `before()`; derived `ByExpiration` and `ByName`. |
| `Recipe.h/.cpp`, `RecipeBook.h/.cpp` | Recipes, which ingredients are missing, READY status, and the shopping list. |
| `PantryStorage.h/.cpp` | File handling: saves and loads the pantry, collecting a problem for each bad line. |
| `Filter.h` | Function template `filterBy`, used with `PantryItem` and `RecipeStatus`. |
| `TextUtil.h` | Trim and lowercase helpers. |
| `tests/app_check.cpp`, `tests/debug_date.cpp` | Console checks and the small program used for the debugger session. |

## Requirements and evidence

- Requirement checklist: [REQUIREMENTS.md](REQUIREMENTS.md)
- Object lifetime and debugger session: [LIFETIME_AND_DEBUGGING.md](LIFETIME_AND_DEBUGGING.md)

## Team contributions

| Member | Own commit & change |
|---|---|
| Annmarie Sheehan | Created the project and README. Individual change: case-insensitive item names in `Pantry.cpp`. |
| Maria Serpa | Added the GUI, application layer, and recipe classes. Individual change: expiration-date validation in `PantryItem.cpp`. |

## Known limitations

- Recipes are built in; the user cannot add recipes yet.
- Each ingredient must use the same unit in the pantry and the recipe (no unit conversion).
- Loading a file replaces the current pantry with the valid lines of that file.
- `ByName` sorting compares raw text, so a capitalized name can sort ahead of lowercase names.
