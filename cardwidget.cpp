/*
#############################################################################
# COMP.CS.115 Ohjelmointi 3: Rajapinnat / Programming 3: Interfaces         #
# Project: Opettelukortit / Flashcards GUI                                  #
# File: cardwidget.cpp                                                      #
# Description: Implements CardWidget, a QWidget subclass for displaying     #
#              and interacting with a single flashcard.                     #
#              Add mode: input fields for a new card.                       #
#              Edit mode: pre-filled fields for editing an existing card.   #
#              Flip mode: shows prompt, reveals back side on button press.  #
#              Answer mode: user types answers, score shown after submit.   #
#                                                                           #
# Author information:                                                       #
# Name: Manh Tran                                                           #
# Student number: 154123367                                                 #
# Username: kkk243                                                          #
# Tuni email: manh.tran@tuni.fi                                             #
#############################################################################
*/

#include "cardwidget.hh"

#include <QMessageBox>

#include <algorithm>
#include <random>

// ----------------------------------------------------------------------------
// Add/Edit constructor
// ----------------------------------------------------------------------------

CardWidget::CardWidget(std::shared_ptr<Card> card,
                       const Fields& fields,
                       CardMode mode,
                       QWidget* parent)
    : QWidget(parent),
    mode_(mode),
    fields_(fields),
    edit_card_(card),
    current_index_(0),
    total_score_(0.0),
    progress_label_(nullptr),
    score_label_(nullptr),
    card_grid_(nullptr),
    confirm_btn_(nullptr),
    cancel_btn_(nullptr),
    flip_btn_(nullptr),
    prev_btn_(nullptr),
    next_btn_(nullptr),
    submit_btn_(nullptr)
{
    build_add_edit_ui();
}

// ----------------------------------------------------------------------------
// Study constructor
// ----------------------------------------------------------------------------

CardWidget::CardWidget(
    const std::vector<std::shared_ptr<Card>>& cards,
    const Fields& front_fields,
    const Fields& back_fields,
    CardMode mode,
    QWidget* parent)
    : QWidget(parent),
    mode_(mode),
    front_fields_(front_fields),
    back_fields_(back_fields),
    cards_(cards),
    current_index_(0),
    total_score_(0.0),
    progress_label_(nullptr),
    score_label_(nullptr),
    card_grid_(nullptr),
    confirm_btn_(nullptr),
    cancel_btn_(nullptr),
    flip_btn_(nullptr),
    prev_btn_(nullptr),
    next_btn_(nullptr),
    submit_btn_(nullptr)
{
    // Randomize card order for variety
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards_.begin(), cards_.end(), g);

    // Initialize all cards as unanswered (-1.0 = not yet answered)
    answered_scores_.assign(cards_.size(), -1.0);

    if (mode_ == CardMode::Flip)
    {
        build_flip_ui();
    }
    else
    {
        build_answer_ui();
    }
}

// ----------------------------------------------------------------------------
// Public methods
// ----------------------------------------------------------------------------

// Returns user-entered definitions from all input fields
Fields CardWidget::get_definitions() const
{
    Fields defs;
    for (QLineEdit* edit : line_edits_)
    {
        defs.push_back(edit->text().trimmed().toStdString());
    }
    return defs;
}

// Returns field names used in this card widget
Fields CardWidget::get_fields() const
{
    return fields_;
}

// Returns the ID of the card being edited (0 in Add mode)
unsigned int CardWidget::get_card_id() const
{
    if (!edit_card_)
    {
        return 0;
    }
    return edit_card_->get_id();
}

// ----------------------------------------------------------------------------
// Build Add/Edit UI
// ----------------------------------------------------------------------------

// Builds a shared UI for both Add and Edit modes
void CardWidget::build_add_edit_ui()
{
    QVBoxLayout* main_layout = new QVBoxLayout(this);

    // Title differs by mode
    QLabel* title = new QLabel(this);
    if (mode_ == CardMode::Add)
    {
        title->setText("Add New Card");
    }
    else
    {
        title->setText("Edit Card");
    }
    title->setStyleSheet(
        "font-weight: bold;"
        "font-size: 16px;"
        "padding-bottom: 10px;");
    main_layout->addWidget(title);

    QGridLayout* grid = new QGridLayout();

    // Fetch existing definitions to pre-fill fields in Edit mode
    Fields existing_defs;
    if (mode_ == CardMode::Edit && edit_card_)
    {
        edit_card_->get_definitions(fields_, existing_defs);
    }

    // One row per field: label on left, line edit on right
    for (size_t i = 0; i < fields_.size(); ++i)
    {
        QLabel* label = new QLabel(
            QString::fromStdString(fields_.at(i)) + ":",
            this);

        QLineEdit* edit = new QLineEdit(this);
        edit->setPlaceholderText(
            QString::fromStdString(fields_.at(i)));

        // Pre-fill with existing value in Edit mode
        if (mode_ == CardMode::Edit && i < existing_defs.size())
        {
            edit->setText(
                QString::fromStdString(existing_defs.at(i)));
        }

        grid->addWidget(label, static_cast<int>(i), 0);
        grid->addWidget(edit,  static_cast<int>(i), 1);
        line_edits_.push_back(edit);
    }

    main_layout->addLayout(grid);

    // Confirm button label differs by mode
    QHBoxLayout* btn_layout = new QHBoxLayout();
    confirm_btn_ = new QPushButton(this);
    if (mode_ == CardMode::Add)
    {
        confirm_btn_->setText("Add Card");
    }
    else
    {
        confirm_btn_->setText("Save");
    }

    cancel_btn_ = new QPushButton("Cancel", this);
    btn_layout->addWidget(confirm_btn_);
    btn_layout->addWidget(cancel_btn_);
    main_layout->addLayout(btn_layout);

    connect(confirm_btn_, &QPushButton::clicked,
            this, &CardWidget::on_confirm_clicked);
    connect(cancel_btn_,  &QPushButton::clicked,
            this, &CardWidget::on_cancel_clicked);
}

// ----------------------------------------------------------------------------
// Build Flip UI
// ----------------------------------------------------------------------------

// Builds UI for Flip study mode: shows front, hides back until flipped
void CardWidget::build_flip_ui()
{
    QVBoxLayout* main_layout = new QVBoxLayout(this);

    // Progress label: "Card X / N"
    progress_label_ = new QLabel(this);
    progress_label_->setAlignment(Qt::AlignCenter);
    main_layout->addWidget(progress_label_);

    card_grid_ = new QGridLayout();
    main_layout->addLayout(card_grid_);

    // Front fields: always visible, read-only labels
    for (size_t i = 0; i < front_fields_.size(); ++i)
    {
        QLabel* field_label = new QLabel(
            QString::fromStdString(front_fields_.at(i)) + ":",
            this);

        QLabel* value_label = new QLabel(this);
        value_label->setStyleSheet(
            "font-weight: bold;"
            "font-size: 16px;"
            "padding: 8px;");

        card_grid_->addWidget(field_label, static_cast<int>(i), 0);
        card_grid_->addWidget(value_label, static_cast<int>(i), 1);
        front_value_labels_.push_back(value_label);
    }

    // Back fields: hidden until Flip is clicked
    int row_offset = static_cast<int>(front_fields_.size());
    for (size_t i = 0; i < back_fields_.size(); ++i)
    {
        QLabel* field_label = new QLabel(
            QString::fromStdString(back_fields_.at(i)) + ":",
            this);

        QLabel* value_label = new QLabel(this);
        value_label->setStyleSheet(
            "font-weight: bold;"
            "font-size: 16px;"
            "padding: 8px;"
            "color: #2196F3;");

        field_label->hide();
        value_label->hide();

        card_grid_->addWidget(
            field_label, row_offset + static_cast<int>(i), 0);
        card_grid_->addWidget(
            value_label, row_offset + static_cast<int>(i), 1);
        back_value_labels_.push_back(value_label);
    }

    // Navigation and action buttons
    QHBoxLayout* btn_layout = new QHBoxLayout();
    prev_btn_   = new QPushButton("Previous", this);
    flip_btn_   = new QPushButton("Flip", this);
    next_btn_   = new QPushButton("Next", this);
    cancel_btn_ = new QPushButton("Close", this);

    btn_layout->addWidget(prev_btn_);
    btn_layout->addWidget(flip_btn_);
    btn_layout->addWidget(next_btn_);
    btn_layout->addWidget(cancel_btn_);
    main_layout->addLayout(btn_layout);

    connect(prev_btn_,   &QPushButton::clicked,
            this, &CardWidget::on_prev_clicked);
    connect(flip_btn_,   &QPushButton::clicked,
            this, &CardWidget::on_flip_clicked);
    connect(next_btn_,   &QPushButton::clicked,
            this, &CardWidget::on_next_clicked);
    connect(cancel_btn_, &QPushButton::clicked,
            this, &CardWidget::on_cancel_clicked);

    show_current_card();
}

// ----------------------------------------------------------------------------
// Build Answer UI
// ----------------------------------------------------------------------------

// Builds UI for Answer study mode: shows prompt, user types answers
void CardWidget::build_answer_ui()
{
    QVBoxLayout* main_layout = new QVBoxLayout(this);

    // Progress label: "Card X / N"
    progress_label_ = new QLabel(this);
    progress_label_->setAlignment(Qt::AlignCenter);
    main_layout->addWidget(progress_label_);

    card_grid_ = new QGridLayout();
    main_layout->addLayout(card_grid_);

    // Prompt fields: always visible, read-only labels
    for (size_t i = 0; i < front_fields_.size(); ++i)
    {
        QLabel* field_label = new QLabel(
            QString::fromStdString(front_fields_.at(i)) + ":",
            this);

        QLabel* value_label = new QLabel(this);
        value_label->setStyleSheet(
            "font-weight: bold;"
            "font-size: 16px;"
            "padding: 8px;");

        card_grid_->addWidget(field_label, static_cast<int>(i), 0);
        card_grid_->addWidget(value_label, static_cast<int>(i), 1);
        front_value_labels_.push_back(value_label);
    }

    // Answer fields: editable line edits
    int row_offset = static_cast<int>(front_fields_.size());
    for (size_t i = 0; i < back_fields_.size(); ++i)
    {
        QLabel* field_label = new QLabel(
            QString::fromStdString(back_fields_.at(i)) + ":",
            this);

        QLineEdit* edit = new QLineEdit(this);
        edit->setPlaceholderText("Your answer...");

        card_grid_->addWidget(
            field_label, row_offset + static_cast<int>(i), 0);
        card_grid_->addWidget(
            edit, row_offset + static_cast<int>(i), 1);
        answer_edits_.push_back(edit);
    }

    // Per-card score label (hidden until card is submitted)
    score_label_ = new QLabel(this);
    score_label_->setAlignment(Qt::AlignCenter);
    score_label_->setStyleSheet("font-size: 14px; padding: 4px;");
    score_label_->hide();
    main_layout->addWidget(score_label_);

    // Navigation and action buttons
    QHBoxLayout* btn_layout = new QHBoxLayout();
    prev_btn_   = new QPushButton("Previous", this);
    submit_btn_ = new QPushButton("Submit", this);
    next_btn_   = new QPushButton("Next", this);
    cancel_btn_ = new QPushButton("Close", this);

    btn_layout->addWidget(prev_btn_);
    btn_layout->addWidget(submit_btn_);
    btn_layout->addWidget(next_btn_);
    btn_layout->addWidget(cancel_btn_);
    main_layout->addLayout(btn_layout);

    connect(prev_btn_,   &QPushButton::clicked,
            this, &CardWidget::on_prev_clicked);
    connect(submit_btn_, &QPushButton::clicked,
            this, &CardWidget::on_submit_clicked);
    connect(next_btn_,   &QPushButton::clicked,
            this, &CardWidget::on_next_clicked);
    connect(cancel_btn_, &QPushButton::clicked,
            this, &CardWidget::on_cancel_clicked);

    show_current_card();
}

// ----------------------------------------------------------------------------
// Helpers
// ----------------------------------------------------------------------------

// Loads current card content and restores answered/unanswered state
void CardWidget::show_current_card()
{
    if (cards_.empty())
    {
        return;
    }

    auto card = cards_.at(static_cast<size_t>(current_index_));

    // Update progress label
    progress_label_->setText(
        QString("Card %1 / %2")
            .arg(current_index_ + 1)
            .arg(cards_.size()));

    // Prev/Next button availability
    prev_btn_->setEnabled(current_index_ > 0);
    next_btn_->setEnabled(
        current_index_ < static_cast<int>(cards_.size()) - 1);

    // Fill prompt (front) field values
    Fields front_defs;
    card->get_definitions(front_fields_, front_defs);
    for (size_t i = 0; i < front_value_labels_.size(); ++i)
    {
        front_value_labels_.at(i)->setText(
            i < front_defs.size()
                ? QString::fromStdString(front_defs.at(i))
                : "");
    }

    if (mode_ == CardMode::Flip)
    {
        // Reset flip state so back side is hidden again
        reset_flip();
    }

    if (mode_ == CardMode::Answer)
    {
        double saved_score =
            answered_scores_.at(
                static_cast<size_t>(current_index_));

        bool already_answered = (saved_score >= 0.0);

        if (already_answered)
        {
            // Restore the locked answered state for this card
            Fields correct_defs;
            card->get_definitions(back_fields_, correct_defs);

            for (size_t i = 0; i < answer_edits_.size(); ++i)
            {
                std::string correct =
                    (i < correct_defs.size())
                        ? correct_defs.at(i) : "";

                // Re-apply color coding without re-checking answers
                bool ok = (answer_edits_.at(i)
                                   ->text()
                                   .trimmed()
                                   .toStdString() == correct
                           || correct.empty());

                answer_edits_.at(i)->setReadOnly(true);
                answer_edits_.at(i)->setStyleSheet(
                    ok ? "background-color: #c8e6c9;"
                       : "background-color: #ffcdd2;");
            }

            // Show saved score for this card
            score_label_->setText(
                QString("Score: %1 / 1.0")
                    .arg(saved_score, 0, 'f', 2));
            score_label_->show();
            submit_btn_->setEnabled(false);
        }
        else
        {
            // Fresh card: clear fields and enable Submit
            for (QLineEdit* edit : answer_edits_)
            {
                edit->clear();
                edit->setReadOnly(false);
                edit->setStyleSheet("");
            }

            score_label_->hide();
            submit_btn_->setEnabled(true);
        }
    }
}

// Hides back-side labels and re-enables Flip button
void CardWidget::reset_flip()
{
    for (QLabel* label : back_value_labels_)
    {
        label->hide();
    }

    int row_offset = static_cast<int>(front_fields_.size());
    for (size_t i = 0; i < back_fields_.size(); ++i)
    {
        QLayoutItem* item =
            card_grid_->itemAtPosition(
                row_offset + static_cast<int>(i), 0);
        if (item && item->widget())
        {
            item->widget()->hide();
        }
    }

    flip_btn_->setEnabled(true);
}

// Returns true if every card in the session has been answered
bool CardWidget::all_answered() const
{
    for (double score : answered_scores_)
    {
        if (score < 0.0)
        {
            return false;
        }
    }
    return true;
}

// ----------------------------------------------------------------------------
// Slots
// ----------------------------------------------------------------------------

// Validates Add/Edit fields and emits confirmation signal
void CardWidget::on_confirm_clicked()
{
    // Reject if any field is empty
    for (size_t i = 0; i < line_edits_.size(); ++i)
    {
        if (line_edits_.at(i)->text().trimmed().isEmpty())
        {
            QMessageBox::warning(
                this, "Validation Error",
                QString("Field '%1' cannot be empty!")
                    .arg(QString::fromStdString(fields_.at(i))));
            return;
        }
    }

    emit card_confirmed();
}

// Closes the widget; emits study_finished if in Answer mode
void CardWidget::on_cancel_clicked()
{
    // Emit final score summary if the session was started
    if (mode_ == CardMode::Answer && !cards_.empty())
    {
        emit study_finished(total_score_,
                            static_cast<int>(cards_.size()));
    }

    emit card_cancelled();
}

// Reveals back-side field values in Flip mode
void CardWidget::on_flip_clicked()
{
    if (cards_.empty())
    {
        return;
    }

    auto card = cards_.at(static_cast<size_t>(current_index_));

    Fields back_defs;
    card->get_definitions(back_fields_, back_defs);

    int row_offset = static_cast<int>(front_fields_.size());

    for (size_t i = 0; i < back_value_labels_.size(); ++i)
    {
        back_value_labels_.at(i)->setText(
            i < back_defs.size()
                ? QString::fromStdString(back_defs.at(i))
                : "");
        back_value_labels_.at(i)->show();

        // Show the corresponding field label too
        QLayoutItem* item =
            card_grid_->itemAtPosition(
                row_offset + static_cast<int>(i), 0);
        if (item && item->widget())
        {
            item->widget()->show();
        }
    }

    // Prevent flipping again on the same card
    flip_btn_->setEnabled(false);
}

// Advances to the next card
void CardWidget::on_next_clicked()
{
    if (current_index_ < static_cast<int>(cards_.size()) - 1)
    {
        ++current_index_;
        show_current_card();
    }
}

// Goes back to the previous card (answer state is preserved, not re-editable)
void CardWidget::on_prev_clicked()
{
    if (current_index_ > 0)
    {
        --current_index_;
        show_current_card();
    }
}

// Checks answers for the current card, locks fields, shows per-card score,
// and emits study_finished if all cards have been answered
void CardWidget::on_submit_clicked()
{
    if (cards_.empty() || !submit_btn_->isEnabled())
    {
        return;
    }

    auto card = cards_.at(static_cast<size_t>(current_index_));

    // Collect user answers
    Fields user_answers;
    for (QLineEdit* edit : answer_edits_)
    {
        user_answers.push_back(
            edit->text().trimmed().toStdString());
    }

    // Check answers and compute score for this card
    double score = card->check_answers(back_fields_, user_answers);

    // Save score and add to session total
    answered_scores_.at(
        static_cast<size_t>(current_index_)) = score;
    total_score_ += score;

    // Retrieve correct answers to color-code the fields
    Fields correct_defs;
    card->get_definitions(back_fields_, correct_defs);

    for (size_t i = 0; i < answer_edits_.size(); ++i)
    {
        std::string correct =
            (i < correct_defs.size()) ? correct_defs.at(i) : "";
        std::string user =
            answer_edits_.at(i)->text().trimmed().toStdString();

        bool ok = (user == correct || correct.empty());

        // Green = correct, red = wrong
        answer_edits_.at(i)->setStyleSheet(
            ok ? "background-color: #c8e6c9;"
               : "background-color: #ffcdd2;");

        // Lock field so it cannot be re-answered
        answer_edits_.at(i)->setReadOnly(true);
    }

    // Show per-card score immediately below the fields
    score_label_->setText(
        QString("Score: %1 / 1.0").arg(score, 0, 'f', 2));
    score_label_->show();

    // Disable Submit to prevent double-submission
    submit_btn_->setEnabled(false);

    // If all cards are answered, show final summary
    if (all_answered())
    {
        int total = static_cast<int>(cards_.size());
        double avg = (total > 0)
                         ? (total_score_ / total) * 100.0
                         : 0.0;

        QMessageBox::information(
            this, "Session Complete",
            QString("All cards answered!\n\n"
                    "Total score: %1 / %2\n"
                    "Correct rate: %3%")
                .arg(total_score_, 0, 'f', 2)
                .arg(total)
                .arg(avg, 0, 'f', 1));

        emit study_finished(total_score_, total);
    }
}
