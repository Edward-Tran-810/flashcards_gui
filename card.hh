/*
#############################################################################
# COMP.CS.115 Ohjelmointi 3: Rajapinnat / Programming 3: Interfaces         #
# Project: Opettelukorttien paluu / Return of Flashcards                    #
# File: card.hh                                                             #
# Description: Declares the Card class interface.                           #
#                                                                           #
# Notes: * Represents a single flashcard.                                   #
#        * Stores definitions mapped to named fields.                       #
#        * Provides answer checking and prompt retrieval.                   #
#                                                                           #
# Author information:                                                       #
# Name: Manh Tran                                                           #
# Student number: 154123367                                                 #
# Username: kkk243                                                          #
# Tuni email: manh.tran@tuni.fi                                             #
#############################################################################
*/

#ifndef CARD_HH
#define CARD_HH

#include "utils.hh"

#include <string>
#include <map>

using namespace std;

class Card
{
public:
    /**
     * @brief Constructs a Card object.
     *
     * Initializes an empty card with a unique ID.
     */
    Card();

    /**
     * @brief Destructs the Card object.
     */
    ~Card();

    /**
     * @brief Assigns definitions to card field types.
     *
     * Stores provided definitions for the given field types.
     * Overwrites previous definitions of the same field type.
     *
     * @param field_types Field type identifiers
     * @param definitions Field definitions
     * @return False if field_types and definitions are't equal size,
     * true otherwise.
     */
    bool add_new_definitions(const Fields& field_types,
                             const Fields& definitions);

    /**
     * @brief Checks if the card contains the specified field_types.
     *
     * @param field_types Field identifiers
     * @return True if all field_types exist in the card, false otherwise
     */
    bool has_fields(const Fields& field_types) const;

    /**
     * @brief Returns card fields.
     *
     * @return Card fields
     */
    Fields get_fields() const;

    /**
     * @brief Fetches definitions of chosen field types
     *
     * @param requested_fields Requested definition field types
     * @param return_definitions Output of requested definitions for prompts
     * @return True if all requested fields existed,
     * false otherwise.
     */
    bool get_definitions(const Fields& requested_fields,
                         Fields& return_definitions);

    /**
     * @brief Checks study answers against stored definitions.
     *
     * Compares user answers with correct values and updates
     * the accumulated study result.
     *
     * User gets 0-1 points per card.
     * If there are multiple fields to be checked, the 1 point is divided
     * between them.
     *
     * @param answer_fields Fields to be checked
     * @param answers User-provided answers
     * @return result Study result [0-1] points
     */
    double check_answers(const Fields& answer_fields,
                         const Fields& answers) const;

    /**
     * @brief Returns the card ID.
     *
     * @return The card ID number.
     */
    unsigned int get_id() const;

    /**
     * @brief Compares two cards by ID.
     *
     * @param other Card to compare against
     * @return True if both cards have the same ID, false otherwise.
     */
    bool operator==(const Card& other) const;

private:
    static unsigned int NEXT_ID;
    unsigned int ID_;

    // Contains card info in the format <field type, definition>
    map<string, string> definitions_;
};

#endif // CARD_HH
