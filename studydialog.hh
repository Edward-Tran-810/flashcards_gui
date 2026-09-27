/*
#############################################################################
# COMP.CS.115 Ohjelmointi 3: Rajapinnat / Programming 3: Interfaces         #
# Project: Opettelukortit / Flashcards GUI                                  #
# File: studydialog.hh                                                      #
# Description: Declares StudyDialog, a setup dialog for study sessions.     #
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

#ifndef STUDYDIALOG_HH
#define STUDYDIALOG_HH

#include "utils.hh"
#include <QDialog>
#include <QCheckBox>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <vector>

enum class StudyMode { Flip, Answer };

class StudyDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the study setup dialog.
     * @param fields All fields of the selected deck
     * @param mode Flip or Answer mode
     * @param parent Parent widget
    */
    StudyDialog(const Fields& fields, StudyMode mode,
                QWidget* parent = nullptr);

    /**
     * @brief Returns user-selected front/prompt fields.
    */
    Fields get_front_fields() const;

    /**
     * @brief Returns user-selected back/answer fields.
    */
    Fields get_back_fields() const;

    /**
     * @brief Returns number of cards to study (Answer mode only).
    */
    int get_card_count() const;

private slots:
    void on_start_clicked();

private:
    Fields fields_;
    StudyMode mode_;

    // Front/Prompt checkboxes
    std::vector<QCheckBox*> front_checks_;

    // Back/Answer checkboxes
    std::vector<QCheckBox*> back_checks_;

    // Spinbox for card count (Answer mode only)
    QSpinBox* card_count_spin_;

    QPushButton* start_btn_;
    QPushButton* cancel_btn_;

    /**
     * @brief Builds UI for Flip mode.
     */
    void build_flip_ui();

    /**
     * @brief Builds UI for Answer mode.
     */
    void build_answer_ui();

    /**
     * @brief Validates that front and back fields don't overlap.
     * @return True if valid, false if overlap detected.
     */
    bool validate_fields() const;
};

#endif // STUDYDIALOG_HH
