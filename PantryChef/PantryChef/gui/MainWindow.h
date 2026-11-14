#pragma once
#include <QWidget>
#include "PantryApp.h"

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QTableWidget;

class MainWindow : public QWidget {
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    enum class Level { Info, Warning, Error };

    PantryApp app_;

    QLineEdit* nameEdit_ = nullptr;
    QLineEdit* amountEdit_ = nullptr;
    QLineEdit* unitEdit_ = nullptr;
    QLineEdit* dateEdit_ = nullptr;
    QLineEdit* searchEdit_ = nullptr;
    QLineEdit* fileEdit_ = nullptr;
    QComboBox* sortBox_ = nullptr;
    QTableWidget* pantryTable_ = nullptr;
    QComboBox* recipeBox_ = nullptr;
    QCheckBox* onlyReadyBox_ = nullptr;
    QPlainTextEdit* resultsView_ = nullptr;
    QLabel* statusLabel_ = nullptr;

    void buildUi();
    void refreshPantry();
    void showMessage(const QString& text, Level level);

    void onAddItem();
    void onRemoveAmount();
    void onWhatCanIMake();
    void onShoppingList();
    void onSave();
    void onLoad();
};
