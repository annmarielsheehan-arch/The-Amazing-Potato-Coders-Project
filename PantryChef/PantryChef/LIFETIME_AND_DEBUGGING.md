# Object lifetime and debugger evidence

## Who owns what, and when it is destroyed

PantryChef has no raw `new`/`delete` in its own classes. Every class stores its data in members that clean themselves up (`std::string`, `std::vector`), so no hand-written destructors are needed (the "rule of zero").

| Object | Created | Owner | Destroyed |
|---|---|---|---|
| `MainWindow window` | In `main()` of `gui/gui_main.cpp` | The `main` function | When `main` returns after the window closes |
| Qt widgets (fields, buttons, table) | `new` in `MainWindow::buildUi` | Their parent widget | Qt deletes children when the parent window is destroyed |
| `PantryApp app_` | With the `MainWindow` | `MainWindow` (member) | With the `MainWindow`; its members go in reverse order: `recipeBook_`, then `pantry_` |
| `PantryItem` objects | Inside `Pantry`'s vector | `Pantry` | When the vector is destroyed or replaced |
| `RecipeBook` and `Recipe` objects | `PantryApp::loadSampleData` | `RecipeBook`'s vector | With the `RecipeBook` |
| `ByExpiration` / `ByName` | On the stack in `PantryApp::pantryItems` | That function | At the end of the block, after `Pantry::sorted` has used them |
| Temporary `PantryItem` in `addItem` | At the call in `PantryApp::addItem` | The call expression | At the end of the statement, after `Pantry::addItem` copied it |
| `LoadResult` and local `Pantry loaded` | In `PantryApp::loadFrom` | That function | At the end of the function; the old items in `pantry_` are destroyed when `pantry_ = loaded;` runs |

## What happens when an exception is thrown

If the `PantryItem` constructor throws (for example, the date `2027-02-29`), the object never finishes being constructed, so its destructor does not run. The members that were already built (the strings) are destroyed automatically while the exception unwinds. No bad `PantryItem` exists, and `Pantry` never sees it, so the pantry is unchanged.

## Why the base class destructor is virtual

`SortStrategy` has a virtual destructor so that a derived object (`ByExpiration`, `ByName`) used through a base pointer or reference is destroyed correctly. The strategies here live on the stack, so it would not break today, but it is the safe rule for any class used polymorphically.

## Debugger investigation

Question investigated: why is the date `2027-02-29` rejected, and when is a valid item destroyed?

Program: `tests/debug_date.cpp`, which builds one invalid item inside `try`, then one valid item inside its own scope.

Setup (MSYS2 UCRT64 terminal, from the project folder):

```
pacman -S --needed mingw-w64-ucrt-x86_64-gdb
g++ -g -O0 -std=c++17 -I. -o debug_date tests/debug_date.cpp PantryItem.cpp
gdb ./debug_date.exe
```

Commands to record in gdb:

```
break isValidDate
run
bt
info args
next
next
info locals
finish
continue
```

Expected values for `2027-02-29`: `year` 2027, `month` 2, `day` 29, `leap` false, `maxDay` 28, and `isValidDate` returns false because 29 is greater than 28. To watch a valid item being destroyed, run `break PantryItem::~PantryItem` and `continue` again.

Paste your own session output (the backtrace, the local variable values, and the return value) in the Checkpoint 7 submission document.
