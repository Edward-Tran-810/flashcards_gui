/*
#############################################################################
# COMP.CS.115 Ohjelmointi 3: Rajapinnat / Programming 3: Interfaces         #
# Project: Opettelukortit / Flashcards GUI                                  #
# File: mainwindow.hh                                                       #
# Description: Declares the MainWindow class.                               #
#              Main GUI window for the Flashcard application.               #
#              Handles deck and card management through the DeckManager.    #
#                                                                           #
# Author information:                                                       #
# Name: Manh Tran                                                           #
# Student number: 154123367                                                 #
# Username: kkk243                                                          #
# Tuni email: manh.tran@tuni.fi                                             #
#############################################################################
*/

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "deckmanager.hh"
#include "cardwidget.hh"

#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QScrollArea>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the MainWindow and initializes the UI.
     * @param parent Parent widget.
     */
    explicit MainWindow(QWidget* parent = nullptr);

    /**
     * @brief Destructor.
     */
    virtual ~MainWindow();

private slots:

    /**
     * @brief Loads flashcard file.
     */
    void on_load_file_clicked();

    /**
     * @brief Adds a new deck.
     */
    void on_add_deck_clicked();

    /**
     * @brief Removes selected deck.
     */
    void on_remove_deck_clicked();

    /**
     * @brief Updates card table when deck selection changes.
     */
    void on_deck_selection_changed();

    /**
     * @brief Opens CardWidget in Add mode.
     */
    void on_add_card_clicked();

    /**
     * @brief Removes selected card.
     */
    void on_remove_card_clicked();

    /**
     * @brief Opens CardWidget in Edit mode.
     */
    void on_edit_card_clicked();

    /**
     * @brief Opens study setup dialog and starts study session.
     */
    void on_study_card_clicked();

    /**
     * @brief Closes the application.
     */
    void exit_clicked();

    /**
     * @brief Triggered when adding a card is confirmed.
     */
    void on_card_add_confirmed();

    /**
     * @brief Triggered when editing a card is confirmed.
     */
    void on_card_edit_confirmed();

    /**
     * @brief Closes current CardWidget.
     */
    void on_card_widget_cancelled();

    /**
     * @brief Displays final study result.
     * @param total_score Total accumulated score.
     * @param total_cards Number of studied cards.
     */
    void on_study_finished(double total_score,
                           int total_cards);

private:

    /**
     * @brief Initializes the entire UI.
     */
    void init_ui();

    /**
     * @brief Refreshes deck list.
     */
    void refresh_deck_list();

    /**
     * @brief Refreshes card table for selected deck.
     * @param deck_name Deck name.
     */
    void refresh_card_table(const std::string& deck_name);

    /**
     * @brief Returns selected deck name.
     * @return Deck name or empty string.
     */
    std::string get_selected_deck_name() const;

    /**
     * @brief Returns selected card ID.
     * @return Card ID or 0 if none selected.
     */
    unsigned int get_selected_card_id() const;

    /**
     * @brief Displays CardWidget in scroll area.
     * @param widget Widget to display.
     */
    void show_card_widget(CardWidget* widget);

    /**
     * @brief Removes current CardWidget.
     */
    void clear_card_widget();

    /**
     * @brief Sets status bar message.
     * @param msg Message text.
     */
    void set_status(const QString& msg);

private:

    // Backend
    DeckManager deck_manager_;

    // File loading
    QPushButton* load_file_btn_;
    QLineEdit* file_input_;

    // Deck list
    QListWidget* deck_list_;
    QPushButton* add_deck_btn_;
    QPushButton* remove_deck_btn_;

    // Card table
    QTableWidget* card_table_;

    QPushButton* add_card_btn_;
    QPushButton* remove_card_btn_;
    QPushButton* edit_card_btn_;
    QPushButton* study_card_btn_;

    // Card widget display area
    QScrollArea* card_scroll_;
    CardWidget* current_card_widget_;

    // Current selected deck
    std::string current_deck_name_;
};

#endif // MAINWINDOW_HH
