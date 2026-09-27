/*
#############################################################################
# COMP.CS.115 Ohjelmointi 3: Rajapinnat / Programming 3: Interfaces         #
# Project: Opettelukortit / Flashcards GUI                                  #
# File: studydialog.cpp                                                     #
# Description: Implements StudyDialog, a setup dialog for study sessions.   #
#              Lets the user select front/back fields and card count        #
#              before starting a Flip or Answer study session.              #
#                                                                           #
# Author information:                                                       #
# Name: Manh Tran                                                           #
# Student number: 154123367                                                 #
# Username: kkk243                                                          #
# Tuni email: manh.tran@tuni.fi                                             #
#############################################################################
*/

#include "studydialog.hh"
#include <QMessageBox>
#include <set>

// ---- Constructor -----------------------------------------------------------

StudyDialog::StudyDialog(const Fields& fields, StudyMode mode,
                         QWidget* parent)
    : QDialog(parent),
    fields_(fields),
    mode_(mode),
    card_count_spin_(nullptr)
{
    // Window title reflects the chosen study mode
    setWindowTitle(mode == StudyMode::Flip ?
                       "Flip Cards - Setup" : "Answer Questions - Setup");
    setMinimumWidth(300);

    if (mode_ == StudyMode::Flip)
    {
        build_flip_ui();
    }
    else
    {
        build_answer_ui();
    }
}

// ---- Public methods --------------------------------------------------------

// Returns fields the user checked as front (prompt) fields
Fields StudyDialog::get_front_fields() const
{
    Fields result;
    for (size_t i = 0; i < front_checks_.size(); i++)
    {
        if (front_checks_.at(i)->isChecked())
        {
            result.push_back(fields_.at(i));
        }
    }
    return result;
}

// Returns fields the user checked as back (answer) fields
Fields StudyDialog::get_back_fields() const
{
    Fields result;
    for (size_t i = 0; i < back_checks_.size(); i++)
    {
        if (back_checks_.at(i)->isChecked())
        {
            result.push_back(fields_.at(i));
        }
    }
    return result;
}

// Returns the number of cards to study (only relevant in Answer mode)
int StudyDialog::get_card_count() const
{
    if (card_count_spin_)
    {
        return card_count_spin_->value();
    }
    return 0;
}

// ---- Private methods -------------------------------------------------------

// Builds UI for Flip mode:
// Two checkbox groups - front fields and back fields
void StudyDialog::build_flip_ui()
{
    QVBoxLayout* main_layout = new QVBoxLayout(this);

    // Front field checkboxes
    QGroupBox* front_box = new QGroupBox("Front (shown first)", this);
    QVBoxLayout* front_layout = new QVBoxLayout(front_box);

    for (const std::string& field : fields_)
    {
        QCheckBox* cb = new QCheckBox(
            QString::fromStdString(field), front_box);
        front_layout->addWidget(cb);
        front_checks_.push_back(cb);
    }
    main_layout->addWidget(front_box);

    // Back field checkboxes (revealed after flip)
    QGroupBox* back_box = new QGroupBox("Back (shown after flip)", this);
    QVBoxLayout* back_layout = new QVBoxLayout(back_box);

    for (const std::string& field : fields_)
    {
        QCheckBox* cb = new QCheckBox(
            QString::fromStdString(field), back_box);
        back_layout->addWidget(cb);
        back_checks_.push_back(cb);
    }
    main_layout->addWidget(back_box);

    // Start and Cancel buttons
    QHBoxLayout* btn_layout = new QHBoxLayout();
    start_btn_ = new QPushButton("Start", this);
    cancel_btn_ = new QPushButton("Cancel", this);
    btn_layout->addWidget(start_btn_);
    btn_layout->addWidget(cancel_btn_);
    main_layout->addLayout(btn_layout);

    connect(start_btn_, &QPushButton::clicked, this,
            &StudyDialog::on_start_clicked);
    connect(cancel_btn_, &QPushButton::clicked, this,
            &QDialog::reject);
}

// Builds UI for Answer mode:
// Two checkbox groups - prompt fields and answer fields - plus a card count spinbox
void StudyDialog::build_answer_ui()
{
    QVBoxLayout* main_layout = new QVBoxLayout(this);

    // Prompt field checkboxes (shown as question)
    QGroupBox* front_box = new QGroupBox("Prompt fields", this);
    QVBoxLayout* front_layout = new QVBoxLayout(front_box);

    for (const std::string& field : fields_)
    {
        QCheckBox* cb = new QCheckBox(
            QString::fromStdString(field), front_box);
        front_layout->addWidget(cb);
        front_checks_.push_back(cb);
    }
    main_layout->addWidget(front_box);

    // Answer field checkboxes (user must type these)
    QGroupBox* back_box = new QGroupBox("Answer fields", this);
    QVBoxLayout* back_layout = new QVBoxLayout(back_box);

    for (const std::string& field : fields_)
    {
        QCheckBox* cb = new QCheckBox(
            QString::fromStdString(field), back_box);
        back_layout->addWidget(cb);
        back_checks_.push_back(cb);
    }
    main_layout->addWidget(back_box);

    // Spinbox for how many cards to include in this session
    QGroupBox* count_box = new QGroupBox("Number of cards", this);
    QHBoxLayout* count_layout = new QHBoxLayout(count_box);
    card_count_spin_ = new QSpinBox(count_box);
    card_count_spin_->setMinimum(1);
    card_count_spin_->setMaximum(100);
    card_count_spin_->setValue(5);
    count_layout->addWidget(new QLabel("Cards:", count_box));
    count_layout->addWidget(card_count_spin_);
    main_layout->addWidget(count_box);

    // Start and Cancel buttons
    QHBoxLayout* btn_layout = new QHBoxLayout();
    start_btn_ = new QPushButton("Start", this);
    cancel_btn_ = new QPushButton("Cancel", this);
    btn_layout->addWidget(start_btn_);
    btn_layout->addWidget(cancel_btn_);
    main_layout->addLayout(btn_layout);

    connect(start_btn_, &QPushButton::clicked, this,
            &StudyDialog::on_start_clicked);
    connect(cancel_btn_, &QPushButton::clicked, this,
            &QDialog::reject);
}

// Validates field selections:
// - Both groups must have at least one field selected
// - The same field cannot appear in both front and back
bool StudyDialog::validate_fields() const
{
    Fields front = get_front_fields();
    Fields back  = get_back_fields();

    if (front.empty())
    {
        QMessageBox::warning(const_cast<StudyDialog*>(this),
                             "Validation Error",
                             "Please select at least one front field.");
        return false;
    }

    if (back.empty())
    {
        QMessageBox::warning(const_cast<StudyDialog*>(this),
                             "Validation Error",
                             "Please select at least one back field.");
        return false;
    }

    // Reject if any back field also appears in front
    std::set<std::string> front_set(front.begin(), front.end());
    for (const std::string& f : back)
    {
        if (front_set.count(f))
        {
            QMessageBox::warning(
                const_cast<StudyDialog*>(this),
                "Validation Error",
                QString("Field '%1' cannot be in both front and back.")
                    .arg(QString::fromStdString(f)));
            return false;
        }
    }

    return true;
}

// ---- Slots -----------------------------------------------------------------

// Validates field selections and closes the dialog if valid
void StudyDialog::on_start_clicked()
{
    if (validate_fields())
    {
        accept();
    }
}
