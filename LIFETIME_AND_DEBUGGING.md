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

Setup (MSYS2 UCRT64 terminal, from the repository root):

```
pacman -S --needed mingw-w64-ucrt-x86_64-gdb
g++ -g -O0 -std=c++17 -Isrc -o debug_date tests/debug_date.cpp src/PantryItem.cpp
gdb --batch -x tests/gdb_commands.txt ./debug_date.exe 2>&1 | tee tests/gdb_session.txt
```

`--batch` runs the commands in [tests/gdb_commands.txt](tests/gdb_commands.txt) and exits; `tee` saves the output and prints it at the same time.

Commands in `tests/gdb_commands.txt`:

```
set pagination off
set confirm off
break isValidDate
run
bt
info args
until 30
info locals
finish
delete
break PantryItem::~PantryItem
continue
bt
continue
```

`until 30` runs to line 30 of `src/PantryItem.cpp` (`return day <= maxDay;`), where every local has been computed. `delete` removes the first breakpoint so the run stops at the destructor and not at the second `isValidDate` call for `2028-02-29`.

**Recorded session:** [tests/gdb_session.txt](tests/gdb_session.txt) (GNU gdb 17.2, g++ 16.1.0, MSYS2 UCRT64).

What the session showed for `2027-02-29`:

| Expected | Observed in gdb |
|---|---|
| `year` 2027, `month` 2, `day` 29 | `year = 2027`, `month = 2`, `day = 29` |
| `leap` false, `maxDay` 28 | `leap = false`, `maxDay = 28` |
| `isValidDate` returns false | `Value returned is $1 = false`, back in `PantryItem::PantryItem` at line 40, which then throws |
| The valid item is destroyed when its scope ends | `PantryItem::~PantryItem (this=0x5ffce0)` called from `main () at tests/debug_date.cpp:15`, the closing brace of the block |

Notes on reading the output: in the constructor frame, `name`, `unit` and `expiration` show as `""` because the constructor has already moved them into the members. The program's own output (`Rejected: ...`, `created ...`, `good has been destroyed`) appears at the end because its standard output is buffered when it goes into a pipe. The destructor stopped at the same address (`0x5ffce0`) as the rejected object because the two objects live in separate blocks and the compiler reused the same stack space for them.
