cmake_minimum_required(VERSION 3.16)
project(PantryChef LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(pantry_core STATIC
    PantryItem.cpp
    Pantry.cpp
    Recipe.cpp
    RecipeBook.cpp
    PantryApp.cpp)
target_include_directories(pantry_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(pantrychef main.cpp)
target_link_libraries(pantrychef PRIVATE pantry_core)

add_executable(app_check tests/app_check.cpp)
target_link_libraries(app_check PRIVATE pantry_core)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOMOC ON)
add_executable(pantrychef_gui gui/gui_main.cpp gui/MainWindow.cpp gui/MainWindow.h)
target_link_libraries(pantrychef_gui PRIVATE pantry_core Qt6::Widgets)
