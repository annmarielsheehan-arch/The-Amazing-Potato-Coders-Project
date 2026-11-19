cmake_minimum_required(VERSION 3.16)
project(PantryChef LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Core model and application logic (no GUI)
add_library(pantry_core STATIC
    src/PantryItem.cpp
    src/Pantry.cpp
    src/Recipe.cpp
    src/RecipeBook.cpp
    src/PantryStorage.cpp
    src/PantryApp.cpp)
target_include_directories(pantry_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Console demo
add_executable(pantrychef src/main.cpp)
target_link_libraries(pantrychef PRIVATE pantry_core)

# Qt GUI
find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOMOC ON)
add_executable(pantrychef_gui src/gui/gui_main.cpp src/gui/MainWindow.cpp src/gui/MainWindow.h)
target_link_libraries(pantrychef_gui PRIVATE pantry_core Qt6::Widgets)

# Tests and debugger program
add_executable(app_check tests/app_check.cpp)
target_link_libraries(app_check PRIVATE pantry_core)

add_executable(debug_date tests/debug_date.cpp)
target_link_libraries(debug_date PRIVATE pantry_core)
