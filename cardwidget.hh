/*
#############################################################################
# COMP.CS.115 Ohjelmointi 3: Rajapinnat / Programming 3: Interfaces         #
# Project: Opettelukortit / Flashcards GUI                                  #
# File: cardwidget.hh                                                       #
# Description: Declares CardWidget, a QWidget subclass representing a       #
#              single flashcard in the GUI.                                 #
#              Supports Add, Edit, Flip, and Answer modes.                  #
#                                                                           #
# Author information:                                                       #
# Name: Manh Tran                                                           #
# Student number: 154123367                                                 #
# Username: kkk243                                                          #
# Tuni email: manh.tran@tuni.fi                                             #
#############################################################################
*/

#ifndef CARDWIDGET_HH
#define CARDWIDGET_HH

#include "card.hh"
#include "utils.hh"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>

#include <memory>
#include <vector>

enum class CardMode
{
    Add,
    Edit,
    Flip,
    Answer
};

class CardWidget : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief Constructor for Add/Edit mode.
     * @param card Existing card (nullptr if Add mode)
     * @param fields Deck fields
     * @param mode Add or Edit
     * @param parent Parent widget
     */
    CardWidget(std::shared_ptr<Card> card,
               const Fields& fields,
               CardMode mode,
               QWidget* parent = nullptr);

    /**
     * @brief Constructor for Flip/Answer study mode.
     * @param cards Cards to study
     * @param front_fields Fields shown first
     * @param back_fields Fields shown after flip / as answers
     * @param mode Flip or Answer
     * @param parent Parent widget
     */
    CardWidget(const std::vector<std::shared_ptr<Card>>& cards,
               const Fields& front_fields,
               const Fields& back_fields,
               CardMode mode,
               QWidget* parent = nullptr);

    /**
     * @brief Returns definitions from input fields.
     */
    Fields get_definitions() const;

    /**
     * @brief Returns deck field names.
     */
    Fields get_fields() const;

    /**
     * @brief Returns edited card ID.
     */
    unsigned int get_card_id() const;

signals:

    /**
     * @brief Emitted when Add/Edit confirmed.
     */
    void card_confirmed();

    /**
     * @brief Emitted when widget closed/cancelled.
     */
    void card_cancelled();

    /**
     * @brief Emitted when all cards in answer session are submitted.
     * @param total_score Sum of per-card scores (0.0-1.0 each).
     * @param total_cards Number of cards in the session.
     */
    void study_finished(double total_score, int total_cards);

private slots:

    void on_confirm_clicked();

    void on_cancel_clicked();

    void on_flip_clicked();

    void on_next_clicked();

    void on_prev_clicked();

    void on_submit_clicked();

private:

    // ---- General --------------------------------------------------------

    CardMode mode_;

    Fields fields_;

    std::shared_ptr<Card> edit_card_;

    // ---- Study mode -----------------------------------------------------

    Fields front_fields_;

    Fields back_fields_;

    std::vector<std::shared_ptr<Card>> cards_;

    int current_index_;

    double total_score_;

    // Stores per-card score; -1.0 means not yet answered
    std::vector<double> answered_scores_;

    // ---- Add/Edit widgets -----------------------------------------------

    std::vector<QLineEdit*> line_edits_;

    // ---- Study widgets --------------------------------------------------

    // Shows "Card X / N" progress
    QLabel* progress_label_;

    // Shows per-card score immediately after submission
    QLabel* score_label_;

    QGridLayout* card_grid_;

    std::vector<QLabel*> front_value_labels_;

    std::vector<QLabel*> back_value_labels_;

    std::vector<QLineEdit*> answer_edits_;

    // ---- Buttons --------------------------------------------------------

    QPushButton* confirm_btn_;

    QPushButton* cancel_btn_;

    QPushButton* flip_btn_;

    QPushButton* prev_btn_;

    QPushButton* next_btn_;

    QPushButton* submit_btn_;

    // ---- UI builders ----------------------------------------------------

    /**
     * @brief Builds Add/Edit UI.
     */
    void build_add_edit_ui();

    /**
     * @brief Builds Flip study UI.
     */
    void build_flip_ui();

    /**
     * @brief Builds Answer study UI.
     */
    void build_answer_ui();

    // ---- Helpers --------------------------------------------------------

    /**
     * @brief Loads prompt values and restores answer state for current card.
     *        If the card was already answered, locks the fields and shows score.
     *        If not yet answered, clears fields and enables Submit.
     */
    void show_current_card();

    /**
     * @brief Hides back side labels in Flip mode.
     */
    void reset_flip();

    /**
     * @brief Checks whether all cards have been answered.
     * @return True if every card has a score >= 0.
     */
    bool all_answered() const;
};

#endif // CARDWIDGET_HH
