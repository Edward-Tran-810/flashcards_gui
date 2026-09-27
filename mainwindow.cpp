/*
#############################################################################
# COMP.CS.115 Ohjelmointi 3: Rajapinnat / Programming 3: Interfaces         #
# Project: Opettelukortit / Flashcards GUI                                  #
# File: mainwindow.cpp                                                      #
# Description: Implements the MainWindow class.                             #
#              Provides a graphical interface for the Flashcard application.#
#              Users can load decks from files, add/remove decks and cards, #
#              edit cards, and start Flip or Answer study sessions.         #
#                                                                           #
# Author information:                                                       #
# Name: Manh Tran                                                           #
# Student number: 154123367                                                 #
# Username: kkk243                                                          #
# Tuni email: manh.tran@tuni.fi                                             #
#############################################################################
*/

#include "mainwindow.hh"
#include "studydialog.hh"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QInputDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QStatusBar>
#include <QFileDialog>

const int COL_ID = 0;
const int COL_BASE = 1;

// ----------------------------------------------------------------------------
// Constructor / Destructor
// ----------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    load_file_btn_(nullptr),
    file_input_(nullptr),
    deck_list_(nullptr),
    add_deck_btn_(nullptr),
    remove_deck_btn_(nullptr),
    card_table_(nullptr),
    add_card_btn_(nullptr),
    remove_card_btn_(nullptr),
    edit_card_btn_(nullptr),
    study_card_btn_(nullptr),
    card_scroll_(nullptr),
    current_card_widget_(nullptr)
{
    setWindowTitle("Flashcards");
    resize(1000, 600);

    init_ui();
}

MainWindow::~MainWindow()
{
}

// ----------------------------------------------------------------------------
// UI Initialization
// ----------------------------------------------------------------------------

void MainWindow::init_ui()
{
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* root_layout = new QVBoxLayout(central);

    // ------------------------------------------------------------------------
    // File loading section
    // ------------------------------------------------------------------------

    QGroupBox* file_box =
        new QGroupBox("Load Deck from File", central);

    QHBoxLayout* file_layout =
        new QHBoxLayout(file_box);

    file_input_ = new QLineEdit(file_box);
    file_input_->setPlaceholderText("Enter filename");

    load_file_btn_ =
        new QPushButton("Load File", file_box);

    file_layout->addWidget(new QLabel("File:", file_box));
    file_layout->addWidget(file_input_);
    file_layout->addWidget(load_file_btn_);

    root_layout->addWidget(file_box);

    // ------------------------------------------------------------------------
    // Main splitter
    // ------------------------------------------------------------------------

    QSplitter* splitter =
        new QSplitter(Qt::Horizontal, central);

    // ------------------------------------------------------------------------
    // Left side - Deck list
    // ------------------------------------------------------------------------

    QGroupBox* deck_box =
        new QGroupBox("Decks", splitter);

    QVBoxLayout* deck_layout =
        new QVBoxLayout(deck_box);

    deck_list_ = new QListWidget(deck_box);

    deck_layout->addWidget(deck_list_);

    QHBoxLayout* deck_btn_layout =
        new QHBoxLayout();

    add_deck_btn_ =
        new QPushButton("Add Deck", deck_box);

    remove_deck_btn_ =
        new QPushButton("Remove Deck", deck_box);

    deck_btn_layout->addWidget(add_deck_btn_);
    deck_btn_layout->addWidget(remove_deck_btn_);

    deck_layout->addLayout(deck_btn_layout);

    splitter->addWidget(deck_box);

    // ------------------------------------------------------------------------
    // Right side - Card table
    // ------------------------------------------------------------------------

    QWidget* card_panel = new QWidget(splitter);

    QVBoxLayout* card_panel_layout =
        new QVBoxLayout(card_panel);

    QGroupBox* card_box =
        new QGroupBox("Cards", card_panel);

    QVBoxLayout* card_layout =
        new QVBoxLayout(card_box);

    card_table_ =
        new QTableWidget(0, 1, card_box);

    card_table_->setHorizontalHeaderLabels({"ID"});

    card_table_->setSelectionBehavior(
        QAbstractItemView::SelectRows);

    card_table_->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    card_table_->horizontalHeader()->
        setStretchLastSection(true);

    card_layout->addWidget(card_table_);

    // Buttons

    QHBoxLayout* card_btn_layout =
        new QHBoxLayout();

    add_card_btn_ =
        new QPushButton("Add Card", card_box);

    remove_card_btn_ =
        new QPushButton("Remove Card", card_box);

    edit_card_btn_ =
        new QPushButton("Edit Card", card_box);

    study_card_btn_ =
        new QPushButton("Study", card_box);

    card_btn_layout->addWidget(add_card_btn_);
    card_btn_layout->addWidget(remove_card_btn_);
    card_btn_layout->addWidget(edit_card_btn_);
    card_btn_layout->addWidget(study_card_btn_);

    card_layout->addLayout(card_btn_layout);

    card_panel_layout->addWidget(card_box);

    // ------------------------------------------------------------------------
    // Card widget area
    // ------------------------------------------------------------------------

    card_scroll_ = new QScrollArea(card_panel);

    card_scroll_->setWidgetResizable(true);
    card_scroll_->setMinimumHeight(250);
    card_scroll_->hide();

    card_panel_layout->addWidget(card_scroll_);

    splitter->addWidget(card_panel);

    splitter->setSizes({300, 700});

    root_layout->addWidget(splitter);

    // ------------------------------------------------------------------------
    // Bottom area
    // ------------------------------------------------------------------------

    QHBoxLayout* bottom_layout =
        new QHBoxLayout();

    bottom_layout->addStretch();

    QPushButton* exit_btn =
        new QPushButton("Exit", central);

    exit_btn->setFixedWidth(100);

    bottom_layout->addWidget(exit_btn);

    root_layout->addLayout(bottom_layout);

    // ------------------------------------------------------------------------
    // Status bar
    // ------------------------------------------------------------------------

    statusBar()->showMessage("Ready");

    // ------------------------------------------------------------------------
    // Signal-slot connections
    // ------------------------------------------------------------------------

    connect(load_file_btn_, &QPushButton::clicked,
            this, &MainWindow::on_load_file_clicked);

    connect(file_input_, &QLineEdit::returnPressed,
            this, &MainWindow::on_load_file_clicked);

    connect(exit_btn, &QPushButton::clicked,
            this, &MainWindow::exit_clicked);

    connect(add_deck_btn_, &QPushButton::clicked,
            this, &MainWindow::on_add_deck_clicked);

    connect(remove_deck_btn_, &QPushButton::clicked,
            this, &MainWindow::on_remove_deck_clicked);

    connect(deck_list_, &QListWidget::currentRowChanged,
            this, &MainWindow::on_deck_selection_changed);

    connect(add_card_btn_, &QPushButton::clicked,
            this, &MainWindow::on_add_card_clicked);

    connect(remove_card_btn_, &QPushButton::clicked,
            this, &MainWindow::on_remove_card_clicked);

    connect(edit_card_btn_, &QPushButton::clicked,
            this, &MainWindow::on_edit_card_clicked);

    connect(study_card_btn_, &QPushButton::clicked,
            this, &MainWindow::on_study_card_clicked);
}

// ----------------------------------------------------------------------------
// Helpers
// ----------------------------------------------------------------------------

void MainWindow::refresh_deck_list()
{
    // Clear existing UI list before reloading
    deck_list_->clear();

    // Populate deck list from deck manager
    for (const std::string& name :
         deck_manager_.get_deck_names())
    {
        deck_list_->addItem(
            QString::fromStdString(name));
    }
}

void MainWindow::refresh_card_table(
    const std::string& deck_name)
{
    // Reset table before repopulating
    card_table_->clear();
    card_table_->setRowCount(0);

    // Retrieve selected deck from backend
    auto deck = deck_manager_.get_deck(deck_name);

    // If deck does not exist, show minimal table
    if (!deck)
    {
        card_table_->setColumnCount(1);
        card_table_->setHorizontalHeaderLabels({"ID"});
        return;
    }

    // Get field structure of the deck (dynamic columns)
    Fields fields = *(deck->get_fields());

    int col_count =
        COL_BASE + static_cast<int>(fields.size());

    card_table_->setColumnCount(col_count);

    // Build table headers (ID + field names)
    QStringList headers;
    headers << "ID";

    for (const std::string& field : fields)
    {
        headers << QString::fromStdString(field);
    }

    card_table_->setHorizontalHeaderLabels(headers);

    // Load all cards from deck
    auto cards = deck->get_cards();

    card_table_->setRowCount(
        static_cast<int>(cards.size()));

    // Fill table row by row
    for (size_t row = 0; row < cards.size(); row++)
    {
        auto card = cards.at(row);

        // ID column item
        QTableWidgetItem* id_item =
            new QTableWidgetItem(
                QString::number(card->get_id()));

        // Store ID as internal data for reliable retrieval
        id_item->setData(
            Qt::UserRole,
            static_cast<uint>(card->get_id()));

        card_table_->setItem(
            row, COL_ID, id_item);

        // Get card field values mapped to field definitions
        Fields defs;
        card->get_definitions(fields, defs);

        // Fill each field column
        for (size_t col = 0; col < defs.size(); col++)
        {
            card_table_->setItem(
                row,
                COL_BASE + static_cast<int>(col),
                new QTableWidgetItem(
                    QString::fromStdString(defs.at(col))));
        }
    }

    // Auto-fit columns to content
    card_table_->resizeColumnsToContents();
}

std::string MainWindow::get_selected_deck_name() const
{
    // Get currently selected deck item from list
    QListWidgetItem* item =
        deck_list_->currentItem();

    // No selection case
    if (!item)
    {
        return "";
    }

    return item->text().toStdString();
}

unsigned int MainWindow::get_selected_card_id() const
{
    // Get selected row in card table
    int row = card_table_->currentRow();

    // No row selected
    if (row < 0)
    {
        return 0;
    }

    // Retrieve ID cell from first column
    QTableWidgetItem* item =
        card_table_->item(row, COL_ID);

    if (!item)
    {
        return 0;
    }

    // Extract stored ID from item data (safe retrieval)
    return item->data(Qt::UserRole).toUInt();
}

void MainWindow::show_card_widget(CardWidget* widget)
{
    clear_card_widget();

    current_card_widget_ = widget;

    card_scroll_->setWidget(widget);
    card_scroll_->show();
}

void MainWindow::clear_card_widget()
{
    if (current_card_widget_)
    {
        card_scroll_->takeWidget();

        delete current_card_widget_;

        current_card_widget_ = nullptr;
    }

    card_scroll_->hide();
}

void MainWindow::set_status(const QString& msg)
{
    statusBar()->showMessage(msg);
}

// ----------------------------------------------------------------------------
// Slots
// ----------------------------------------------------------------------------

void MainWindow::on_load_file_clicked()
{
    QString filename;

    // Open file dialog if input field is empty
    if (file_input_->text().trimmed().isEmpty())
    {
        filename = QFileDialog::getOpenFileName(
            this, "Open Flashcard File",
            "", "Text Files (*.txt);;All Files (*)");

        if (filename.isEmpty())
        {
            return;
        }
    }
    else
    {
        // Use manually entered filename
        filename = file_input_->text().trimmed();
    }

    if (deck_manager_.read_file(filename.toStdString()))
    {
        refresh_deck_list();
        set_status(QString("Loaded file: %1").arg(filename));
        file_input_->clear();
    }
    else
    {
        QMessageBox::warning(this, "Load File",
                             QString("Failed to load '%1'.\n"
                                     "Check that the file exists and has a valid format.")
                                 .arg(filename));
    }
}

void MainWindow::on_add_deck_clicked()
{
    bool ok = false;

    // Ask for deck name
    QString deck_name = QInputDialog::getText(
        this, "Add Deck", "Deck name:",
        QLineEdit::Normal, "", &ok);

    if (!ok || deck_name.trimmed().isEmpty())
    {
        return;
    }

    // Ask for field names (semicolon-separated)
    QString fields_input = QInputDialog::getText(
        this, "Add Deck",
        "Field names (semicolon-separated, e.g. EN;FI;DE):",
        QLineEdit::Normal, "", &ok);

    if (!ok || fields_input.trimmed().isEmpty())
    {
        return;
    }

    // Parse fields
    Fields fields;
    for (const QString& f : fields_input.split(';', Qt::SkipEmptyParts))
    {
        fields.push_back(f.trimmed().toStdString());
    }

    if (fields.empty())
    {
        QMessageBox::warning(this, "Add Deck",
                             "At least one field is required.");
        return;
    }

    if (deck_manager_.add_deck(deck_name.toStdString(), fields))
    {
        refresh_deck_list();
        set_status(QString("Deck '%1' added.").arg(deck_name));
    }
    else
    {
        QMessageBox::warning(this, "Add Deck",
                QString("Deck '%1' already exists.").arg(deck_name));
    }
}

void MainWindow::on_remove_deck_clicked()
{
    // Get currently selected deck name from UI
    std::string name = get_selected_deck_name();

    // Ensure a deck is selected before attempting removal
    if (name.empty())
    {
        QMessageBox::warning(this, "Remove Deck",
                             "Please select a deck to remove.");
        return;
    }

    // Ask user for confirmation before performing destructive action
    int confirm = QMessageBox::question(
        this, "Remove Deck",
        QString("Remove deck '%1' and all its cards?")
            .arg(QString::fromStdString(name)),
        QMessageBox::Yes | QMessageBox::No);

    // Proceed only if user confirms
    if (confirm == QMessageBox::Yes)
    {
        // Remove deck from backend manager
        deck_manager_.remove_deck(name);

        // Reset card table UI since selected deck is gone
        card_table_->clearContents();
        card_table_->setRowCount(0);
        card_table_->setColumnCount(1);
        card_table_->setHorizontalHeaderLabels({"ID"});

        // Clear current selection state
        current_deck_name_ = "";

        // Clear any open card widget (avoid dangling context)
        clear_card_widget();

        // Refresh deck list in UI
        refresh_deck_list();

        // Update status bar
        set_status(QString("Deck '%1' removed.")
                       .arg(QString::fromStdString(name)));
    }
}

void MainWindow::on_deck_selection_changed()
{
    // Clear any open card widget when switching decks to avoid edit wrong deck
    clear_card_widget();

    // Update currently selected deck name from UI
    current_deck_name_ = get_selected_deck_name();

    // If no deck is selected, clear the card table and stop here
    if (current_deck_name_.empty())
    {
        card_table_->clearContents();
        card_table_->setRowCount(0);
        return;
    }

    // Load and display cards of the newly selected deck
    refresh_card_table(current_deck_name_);
}

void MainWindow::on_add_card_clicked()
{
    // Ensure a deck is selected before adding a card
    if (current_deck_name_.empty())
    {
        QMessageBox::warning(this, "Add Card",
                             "Please select a deck first.");
        return;
    }

    // Retrieve the selected deck from deck manager
    auto deck = deck_manager_.get_deck(current_deck_name_);
    if (!deck)
    {
        // Safety check: deck should exist if name is valid
        return;
    }

    // Create a copy of field structure for the new card
    Fields fields = *(deck->get_fields());

    // Create CardWidget in "Add mode" (no existing card data)
    CardWidget* w = new CardWidget(nullptr, fields, CardMode::Add);

    // Connect widget signals to MainWindow slots
    connect(w, &CardWidget::card_confirmed,
            this, &MainWindow::on_card_add_confirmed);
    connect(w, &CardWidget::card_cancelled,
            this, &MainWindow::on_card_widget_cancelled);

    // Display the card input widget
    show_card_widget(w);

    // Update status bar to guide user
    set_status("Fill in the fields and click 'Add Card'.");
}

void MainWindow::on_remove_card_clicked()
{
    // Ensure a deck is currently selected before proceeding
    if (current_deck_name_.empty())
    {
        QMessageBox::warning(this, "Remove Card",
                             "Please select a deck first.");
        return;
    }

    // Get ID of the currently selected card in the UI
    unsigned int card_id = get_selected_card_id();

    // No card selected -> cannot proceed with removal
    if (card_id == 0)
    {
        QMessageBox::warning(this, "Remove Card",
                             "Please select a card to remove.");
        return;
    }

    // Attempt to remove the card from the deck manager (core logic)
    if (deck_manager_.remove_card(current_deck_name_, card_id))
    {
        // Success: update UI to reflect removal
        clear_card_widget();
        refresh_card_table(current_deck_name_);
        set_status(QString("Card %1 removed.").arg(card_id));
    }
    else
    {
        // Failure case: inform user that removal did not succeed
        QMessageBox::warning(this, "Remove Card",
                             "Failed to remove card.");
    }
}

void MainWindow::on_edit_card_clicked()
{
    if (current_deck_name_.empty())
    {
        QMessageBox::warning(this, "Edit Card",
                             "Please select a deck first.");
        return;
    }

    unsigned int card_id = get_selected_card_id();

    if (card_id == 0)
    {
        QMessageBox::warning(this, "Edit Card",
                             "Please select a card to edit.");
        return;
    }

    // Retrieve deck and card from backend
    auto deck = deck_manager_.get_deck(current_deck_name_);
    auto card = deck ? deck->get_card(card_id) : nullptr;

    if (!card)
    {
        return;
    }

    // Create CardWidget in Edit mode with current card data
    Fields fields = *(deck->get_fields());
    CardWidget* w = new CardWidget(card, fields, CardMode::Edit);

    // Connect signals to handle save and cancel actions
    connect(w, &CardWidget::card_confirmed,
            this, &MainWindow::on_card_edit_confirmed);
    connect(w, &CardWidget::card_cancelled,
            this, &MainWindow::on_card_widget_cancelled);

    // Display widget in scroll area
    show_card_widget(w);
    set_status("Edit the card fields and click 'Save'.");
}

void MainWindow::on_study_card_clicked()
{
    if (current_deck_name_.empty())
    {
        QMessageBox::warning(this, "Study",
                             "Please select a deck first.");
        return;
    }

    auto deck = deck_manager_.get_deck(current_deck_name_);
    if (!deck || deck->get_deck_size() == 0)
    {
        QMessageBox::warning(this, "Study",
                             "This deck has no cards to study.");
        return;
    }

    Fields fields = *(deck->get_fields());

    // Ask user which study mode to use
    QStringList modes;
    modes << "Flip Cards" << "Answer Questions";

    bool ok = false;
    QString chosen = QInputDialog::getItem(
        this, "Study Mode",
        "Choose study mode:", modes, 0, false, &ok);

    if (!ok)
    {
        return;
    }

    StudyMode mode = (chosen == "Flip Cards")
                         ? StudyMode::Flip
                         : StudyMode::Answer;

    // Show setup dialog for field selection
    StudyDialog dialog(fields, mode, this);

    if (dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    Fields front = dialog.get_front_fields();
    Fields back  = dialog.get_back_fields();

    // Collect cards for study session
    auto all_cards = deck->get_cards();
    std::vector<std::shared_ptr<Card>> study_cards;

    if (mode == StudyMode::Answer)
    {
        // Limit to requested card count
        int count = dialog.get_card_count();
        for (int i = 0;
             i < count && i < static_cast<int>(all_cards.size());
             ++i)
        {
            study_cards.push_back(all_cards.at(i));
        }
    }
    else
    {
        // Use all cards for Flip mode
        study_cards = all_cards;
    }

    CardMode card_mode = (mode == StudyMode::Flip)
                             ? CardMode::Flip
                             : CardMode::Answer;

    CardWidget* w = new CardWidget(
        study_cards, front, back, card_mode);

    connect(w, &CardWidget::card_cancelled,
            this, &MainWindow::on_card_widget_cancelled);
    connect(w, &CardWidget::study_finished,
            this, &MainWindow::on_study_finished);

    show_card_widget(w);
    set_status("Studying deck: " +
               QString::fromStdString(current_deck_name_));
}

void MainWindow::exit_clicked()
{
    // Close the application window
    close();
}

void MainWindow::on_card_add_confirmed()
{
    if (!current_card_widget_)
    {
        return;
    }

    auto deck = deck_manager_.get_deck(current_deck_name_);
    if (!deck)
    {
        return;
    }

    // Get field names and user-entered definitions from widget
    Fields fields = current_card_widget_->get_fields();
    Fields defs   = current_card_widget_->get_definitions();

    if (deck->add_card(fields, defs))
    {
        clear_card_widget();
        refresh_card_table(current_deck_name_);
        set_status("Card added successfully.");
    }
    else
    {
        QMessageBox::warning(this, "Add Card",
                             "Failed to add card.");
    }
}

void MainWindow::on_card_edit_confirmed()
{
    if (!current_card_widget_)
    {
        return;
    }

    // Get card ID from the active widget
    unsigned int card_id = current_card_widget_->get_card_id();

    // Retrieve the deck and card from backend
    auto deck = deck_manager_.get_deck(current_deck_name_);
    auto card = deck ? deck->get_card(card_id) : nullptr;

    // Safety check: card must exist
    if (!card)
    {
        return;
    }

    // Get updated fields and definitions from widget
    Fields fields = current_card_widget_->get_fields();
    Fields defs   = current_card_widget_->get_definitions();

    // Overwrite card definitions with new values
    card->add_new_definitions(fields, defs);

    // Close widget and refresh card table
    clear_card_widget();
    refresh_card_table(current_deck_name_);
    set_status(QString("Card %1 updated.").arg(card_id));
}

void MainWindow::on_card_widget_cancelled()
{
    // Close card widget without saving changes
    clear_card_widget();
    set_status("Ready.");
}

void MainWindow::on_study_finished(double total_score,
                                   int total_cards)
{
    // Calculate average score as percentage
    double avg = (total_cards > 0)
                     ? (total_score / total_cards) * 100.0
                     : 0.0;

    QMessageBox::information(
        this, "Study Complete",
        QString("Session complete!\n"
                "Cards studied: %1\n"
                "Average score: %2%")
            .arg(total_cards)
            .arg(avg, 0, 'f', 1));

    set_status(QString("Study finished. Average score: %1%")
                   .arg(avg, 0, 'f', 1));
}
