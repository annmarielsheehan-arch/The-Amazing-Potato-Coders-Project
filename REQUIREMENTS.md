# Requirement checklist

Each technical requirement is linked to the file or function that meets it and to the evidence a reviewer can check. Paths are relative to the project folder (the one that contains `CMakeLists.txt`).

| Requirement | File / function | Evidence |
|---|---|---|
| Build and launch from the README | `CMakeLists.txt`; README "Build" section | `cmake -S . -B build -G Ninja`, `cmake --build build`, `./build/pantrychef_gui.exe` opens the window |
| Task 1: manage the pantry | `gui/MainWindow.cpp`: `onAddItem`, `onRemoveAmount`, `refreshPantry`; `PantryApp::addItem`, `removeAmount`, `pantryItems`; `Pantry` | README check sequence steps 5 and 6; `app_check` "Task 1" |
| Task 2: what can I make? | `MainWindow::onWhatCanIMake`; `PantryApp::whatCanIMake`; `RecipeBook::statuses`; `Recipe::missing` | README step 3; `app_check` "Task 2" |
| Task 3: shopping list | `MainWindow::onShoppingList`; `PantryApp::shoppingList`; `RecipeBook::shoppingList` | README step 4; `app_check` "Task 3" |
| Persistence | `PantryStorage::save` and `load`; `PantryApp::saveTo` and `loadFrom`; `MainWindow::onSave` and `onLoad`; `sample_data/pantry_sample.txt` | README step 7: save, close, restart, load, and the table matches; `app_check` "Persistence" |
| Exception handling (invalid operation) | `throw` in `PantryItem` constructor (`isValidDate`), `PantryApp::parseAmount`, `Pantry::removeAmount`; `catch` in every `MainWindow` handler | README step 6: red message, pantry unchanged, window still usable |
| File or parsing failure | `PantryStorage::load` (per-line problems), `PantryApp::loadFrom`; `sample_data/pantry_malformed.txt`, `pantry_empty.txt` | README step 8: warning with skipped lines, error for empty or missing file, pantry stays usable |
| Classes, composition, modularity | GUI: `gui/`; application rules: `PantryApp`, `Recipe`, `RecipeBook`, `Pantry`, `PantryItem`; file handling: `PantryStorage` | `Pantry` holds `vector<PantryItem>`; `RecipeBook` holds `vector<Recipe>`; `Recipe` holds `vector<Ingredient>`; `MainWindow` holds `PantryApp`; GUI handlers contain no rules |
| Runtime polymorphism | `SortStrategy` (pure virtual `before()`), `ByExpiration`, `ByName` in `SortStrategy.h`; `Pantry::sorted` calls through `const SortStrategy&`; `PantryApp::pantryItems` | README step 2 and the sort drop-down give two different orderings from the same call |
| Overload | `std::ostream& operator<<(std::ostream&, const PantryItem&)` in `PantryItem.cpp` | `PantryStorage::save` writes each record with it; `app_check` "Overload" |
| Template used with two data types | `filterBy<T, Predicate>` in `Filter.h` | Used with `PantryItem` (search box, `PantryApp::pantryItems`) and `RecipeStatus` (only-READY checkbox, `PantryApp::whatCanIMake`); `app_check` "Template" |
| Lifetime and debugger evidence | `LIFETIME_AND_DEBUGGING.md`; `tests/debug_date.cpp` | Lifetime table and the recorded debugger session |
| Individual changes | `PantryItem.cpp` (date validation, Maria Serpa); `Pantry.cpp` (case-insensitive names, Annmarie Sheehan) | Artifact 1 submissions |
