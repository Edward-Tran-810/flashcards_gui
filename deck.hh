/*
#############################################################################
# COMP.CS.115 Ohjelmointi 3: Rajapinnat / Programming 3: Interfaces         #
# Project: Opettelukorttien paluu / Return of Flashcards                    #
# File: deck.hh                                                             #
# Description: Declares the Deck class interface.                           #
#                                                                           #
# Notes: * Represents a collection of flashcards.                           #
#        * Stores card objects and field definitions.                       #
#        * Provides operations for handling cards.                          #
#                                                                           #
# Author information:                                                       #
# Name: Manh Tran                                                           #
# Student number: 154123367                                                 #
# Username: kkk243                                                          #
# Tuni email: manh.tran@tuni.fi                                             #
#############################################################################
*/

#ifndef DECK_HH
#define DECK_HH

#include "card.hh"

#include <memory>

using namespace std;

class Deck
{
public:
    /**
     * @brief Constructs a Deck object.
     * Initializes a deck with a name and field structure.
     *
     * @param deck_name Name of the deck
     * @param fields Field type identifiers
     */
    Deck(string deck_name, const Fields& fields);

    /**
     * @brief Destructs the Deck object.
     */
    ~Deck();

    /**
     * @brief Adds a new card to the deck.
     *
     * Creates and inserts a new card using provided field names
     * and definitions. Checks field-definition consistency.
     *
     * @param card_fields Card's field type identifiers
     * @param definitions Card's definitions
     * @return False if card_fields and definitions aren't equal in size,
     * true otherwise.
     */
    bool add_card(const Fields& card_fields,
                  const Fields& definitions);

    /**
     * @brief Removes a given card from the deck.
     *
     * @param deck_name ID of the card to be removed
     * @return True if the card was removed,
     * false otherwise (card with given ID wasn't found).
     */
    bool remove_card(unsigned int card_id);

    /**
     * @brief Fetches a new card to be studied.
     *
     * Selects the next card in order of addition. Can be used for
     * funtionalities, where cards are picked from the deck one by one.
     *
     * @param card_index The index of the current card
     * @return shared_ptr To the next card in the deck,
     * nullptr if no more cards in order or over-indexing the deck size.
     */
    shared_ptr<Card> get_next_card(unsigned int card_index) const;

    /**
     * @brief Returns the field types of the deck.
     *
     * @return Shared pointer to deck fields.
     */
    shared_ptr<Fields> get_fields() const;

    /**
     * @brief Returns the number of cards in the deck.
     *
     * @return Number of cards.
     */
    unsigned int get_deck_size();

    /**
     * @brief Returns cards for unit tests only.
     *
     * @return All cards of the deck.
     */
    vector<shared_ptr<Card>> get_cards() const;

    /**
     * @brief Returns cards for unit tests only.
     *
     * @param Card ID.
     * @return Returns the pointer to the card with the given ID.
     */
    shared_ptr<Card> get_card(unsigned int card_id);

    /**
     * @brief Returns deck name.
     *
     * @return Returns deck name.
     */
    string get_name();

private:
    string name_ = "";
    shared_ptr<Fields> deck_fields_;
    vector<shared_ptr<Card>> cards_;
};

#endif // DECK_HH
