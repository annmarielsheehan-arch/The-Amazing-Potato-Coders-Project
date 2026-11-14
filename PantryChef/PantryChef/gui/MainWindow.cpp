#include "MainWindow.h"

#include <QAbstractItemView>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

#include <exception>
#include <string>

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
    app_.loadSampleData();
    buildUi();
    refreshPantry();
    showMessage(QString("Ready. Sample pantry and recipes are loaded."), Level::Info);
}

void MainWindow::buildUi() {
    setWindowTitle(QString("PantryChef"));
    resize(1000, 700);

    QGroupBox* pantryBox = new QGroupBox(QString("Task 1: Manage the pantry"));

    nameEdit_ = new QLineEdit;
    nameEdit_->setPlaceholderText(QString("e.g. milk"));
    amountEdit_ = new QLineEdit;
    amountEdit_->setPlaceholderText(QString("e.g. 2"));
    unitEdit_ = new QLineEdit;
    unitEdit_->setPlaceholderText(QString("e.g. cups"));
    dateEdit_ = new QLineEdit;
    dateEdit_->setPlaceholderText(QString("YYYY-MM-DD"));

    QFormLayout* form = new QFormLayout;
    form->addRow(QString("Item name:"), nameEdit_);
    form->addRow(QString("Amount:"), amountEdit_);
    form->addRow(QString("Unit:"), unitEdit_);
    form->addRow(QString("Expiration date:"), dateEdit_);

    QPushButton* addButton = new QPushButton(QString("Add item"));
    QPushButton* removeButton = new QPushButton(QString("Remove amount"));
    QHBoxLayout* buttonRow = new QHBoxLayout;
    buttonRow->addWidget(addButton);
    buttonRow->addWidget(removeButton);

    sortBox_ = new QComboBox;
    sortBox_->addItem(QString("Expiration date (soonest first)"));
    sortBox_->addItem(QString("Name (A-Z)"));

    searchEdit_ = new QLineEdit;
    searchEdit_->setPlaceholderText(QString("Type part of an item name to filter the list"));

    pantryTable_ = new QTableWidget(0, 4);
    QStringList headers;
    headers << QString("Item") << QString("Amount") << QString("Unit") << QString("Expires");
    pantryTable_->setHorizontalHeaderLabels(headers);
    pantryTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pantryTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    pantryTable_->horizontalHeader()->setStretchLastSection(true);

    fileEdit_ = new QLineEdit;
    fileEdit_->setText(QString("pantry_data.txt"));
    QPushButton* saveButton = new QPushButton(QString("Save"));
    QPushButton* loadButton = new QPushButton(QString("Load"));
    QHBoxLayout* fileRow = new QHBoxLayout;
    fileRow->addWidget(new QLabel(QString("File:")));
    fileRow->addWidget(fileEdit_, 1);
    fileRow->addWidget(saveButton);
    fileRow->addWidget(loadButton);

    QVBoxLayout* pantryLayout = new QVBoxLayout(pantryBox);
    pantryLayout->addLayout(form);
    pantryLayout->addLayout(buttonRow);
    pantryLayout->addWidget(new QLabel(QString("Sort the pantry list by:")));
    pantryLayout->addWidget(sortBox_);
    pantryLayout->addWidget(new QLabel(QString("Search by name:")));
    pantryLayout->addWidget(searchEdit_);
    pantryLayout->addWidget(pantryTable_);
    pantryLayout->addWidget(new QLabel(QString("Save or load the pantry (a text file):")));
    pantryLayout->addLayout(fileRow);

    QGroupBox* recipeGroup = new QGroupBox(QString("Tasks 2 and 3: Recipes"));

    QPushButton* whatButton = new QPushButton(QString("Task 2: What can I make?"));
    onlyReadyBox_ = new QCheckBox(QString("Only show recipes I can make now"));

    recipeBox_ = new QComboBox;
    for (const std::string& name : app_.recipeNames()) {
        recipeBox_->addItem(QString::fromStdString(name));
    }
    QPushButton* shopButton = new QPushButton(QString("Task 3: Build shopping list"));

    resultsView_ = new QPlainTextEdit;
    resultsView_->setReadOnly(true);

    QVBoxLayout* recipeLayout = new QVBoxLayout(recipeGroup);
    recipeLayout->addWidget(whatButton);
    recipeLayout->addWidget(onlyReadyBox_);
    recipeLayout->addWidget(new QLabel(QString("Choose a recipe for the shopping list:")));
    recipeLayout->addWidget(recipeBox_);
    recipeLayout->addWidget(shopButton);
    recipeLayout->addWidget(new QLabel(QString("Results:")));
    recipeLayout->addWidget(resultsView_);

    statusLabel_ = new QLabel;
    statusLabel_->setWordWrap(true);
    statusLabel_->setMinimumHeight(40);

    QHBoxLayout* columns = new QHBoxLayout;
    columns->addWidget(pantryBox, 3);
    columns->addWidget(recipeGroup, 2);

    QVBoxLayout* root = new QVBoxLayout(this);
    root->addLayout(columns);
    root->addWidget(statusLabel_);

    connect(addButton, &QPushButton::clicked, this, [this]() { onAddItem(); });
    connect(removeButton, &QPushButton::clicked, this, [this]() { onRemoveAmount(); });
    connect(whatButton, &QPushButton::clicked, this, [this]() { onWhatCanIMake(); });
    connect(shopButton, &QPushButton::clicked, this, [this]() { onShoppingList(); });
    connect(saveButton, &QPushButton::clicked, this, [this]() { onSave(); });
    connect(loadButton, &QPushButton::clicked, this, [this]() { onLoad(); });
    connect(sortBox_, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            [this](int) { refreshPantry(); });
    connect(searchEdit_, &QLineEdit::textChanged, this, [this](const QString&) { refreshPantry(); });
}

void MainWindow::refreshPantry() {
    PantryApp::SortMode mode = (sortBox_->currentIndex() == 1) ? PantryApp::SortMode::ByName
                                                              : PantryApp::SortMode::ByExpiration;
    std::vector<PantryItem> items = app_.pantryItems(mode, searchEdit_->text().toStdString());

    pantryTable_->setRowCount(static_cast<int>(items.size()));
    for (int row = 0; row < static_cast<int>(items.size()); ++row) {
        const PantryItem& item = items[static_cast<std::size_t>(row)];
        pantryTable_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(item.name())));
        pantryTable_->setItem(row, 1, new QTableWidgetItem(QString::number(item.amount())));
        pantryTable_->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(item.unit())));
        pantryTable_->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(item.expiration())));
    }
}

void MainWindow::showMessage(const QString& text, Level level) {
    if (level == Level::Error) {
        statusLabel_->setStyleSheet(QString("color: #b00020; font-weight: bold;"));
        statusLabel_->setText(QString("Error: ") + text);
    } else if (level == Level::Warning) {
        statusLabel_->setStyleSheet(QString("color: #b26a00; font-weight: bold;"));
        statusLabel_->setText(QString("Warning: ") + text);
    } else {
        statusLabel_->setStyleSheet(QString("color: #1b5e20;"));
        statusLabel_->setText(text);
    }
}

void MainWindow::onAddItem() {
    try {
        app_.addItem(nameEdit_->text().toStdString(), amountEdit_->text().toStdString(),
                     unitEdit_->text().toStdString(), dateEdit_->text().toStdString());
        QString message = QString("Added ") + nameEdit_->text().trimmed() + QString(" to the pantry.");
        nameEdit_->clear();
        amountEdit_->clear();
        unitEdit_->clear();
        dateEdit_->clear();
        refreshPantry();
        showMessage(message, Level::Info);
    } catch (const std::exception& e) {
        showMessage(QString::fromStdString(e.what()), Level::Error);
    }
}

void MainWindow::onRemoveAmount() {
    try {
        app_.removeAmount(nameEdit_->text().toStdString(), amountEdit_->text().toStdString());
        QString message = QString("Removed ") + amountEdit_->text().trimmed() + QString(" of ") +
                          nameEdit_->text().trimmed() + QString(".");
        refreshPantry();
        showMessage(message, Level::Info);
    } catch (const std::exception& e) {
        showMessage(QString::fromStdString(e.what()), Level::Error);
    }
}

void MainWindow::onWhatCanIMake() {
    try {
        std::vector<RecipeStatus> statuses = app_.whatCanIMake(onlyReadyBox_->isChecked());
        QString out;
        for (const RecipeStatus& s : statuses) {
            out += QString::fromStdString(s.name) + QString(": ");
            if (s.ready) {
                out += QString("READY");
            } else {
                out += QString("missing %1 ingredient(s)").arg(s.missingCount);
            }
            out += QString("\n");
        }
        if (statuses.empty()) {
            out = QString("No recipes can be made with the current pantry.\n");
        }
        resultsView_->setPlainText(out);
        showMessage(QString("Showing %1 recipe(s).").arg(static_cast<int>(statuses.size())), Level::Info);
    } catch (const std::exception& e) {
        showMessage(QString::fromStdString(e.what()), Level::Error);
    }
}

void MainWindow::onShoppingList() {
    try {
        QString recipe = recipeBox_->currentText();
        std::vector<Ingredient> missing = app_.shoppingList(recipe.toStdString());
        QString out = QString("Shopping list for ") + recipe + QString(":\n");
        if (missing.empty()) {
            out += QString("Nothing needed. You have everything.\n");
        }
        for (const Ingredient& m : missing) {
            out += QString("  - %1 %2 %3\n")
                       .arg(m.amount)
                       .arg(QString::fromStdString(m.unit))
                       .arg(QString::fromStdString(m.name));
        }
        resultsView_->setPlainText(out);
        showMessage(QString("Built the shopping list for ") + recipe + QString("."), Level::Info);
    } catch (const std::exception& e) {
        showMessage(QString::fromStdString(e.what()), Level::Error);
    }
}

void MainWindow::onSave() {
    try {
        std::size_t count = app_.saveTo(fileEdit_->text().toStdString());
        showMessage(QString("Saved %1 items to %2.").arg(static_cast<int>(count)).arg(fileEdit_->text().trimmed()),
                    Level::Info);
    } catch (const std::exception& e) {
        showMessage(QString::fromStdString(e.what()), Level::Error);
    }
}

void MainWindow::onLoad() {
    try {
        LoadSummary summary = app_.loadFrom(fileEdit_->text().toStdString());
        refreshPantry();
        showMessage(QString::fromStdString(summary.message),
                    summary.skipped > 0 ? Level::Warning : Level::Info);
    } catch (const std::exception& e) {
        showMessage(QString::fromStdString(e.what()), Level::Error);
    }
}
