/*
#############################################################################
# COMP.CS.115 Ohjelmointi 3: Rajapinnat / Programming 3: Interfaces         #
# Project: Opettelukorttien paluu / Return of Flashcards                    #
# File: deckmanager.hh                                                      #
# Description: Declares the DeckManager class interface.                    #
#                                                                           #
# Notes: * Manages multiple decks.                                          #
#        * Provides operations for creating and studying decks.  	        #
#        * Acts as an abstraction layer between GUI and Deck objects.       #
#                                                                           #
# Author information:                                                       #
# Name: Manh Tran                                                           #
# Student number: 154123367                                                 #
# Username: kkk243                                                          #
# Tuni email: manh.tran@tuni.fi                                             #
#############################################################################
*/

#ifndef DECKMANAGER_HH
#define DECKMANAGER_HH

#include "deck.hh"
#include <memory>

class DeckManager
{
public:
    /**
     * @brief Constructs a DeckManager object.
     *
     * Initializes an empty deck container.
     */
    DeckManager();

    /**
     * @brief Deconstructs the DeckManager object.
     */
    ~DeckManager();

    /**
     * @brief Reads a deck definition file.
     *
     * Parses the file header, field names, and all cards.
     * Checks for inconsitencies in the input file.
     *
     * @param file_name Name of the file to read
     * @return True if file was successfully read, false otherwise.
     */
    bool read_file(const string& file_name);

    /**
     * @brief Creates a new empty deck.
     * Initializes a deck with the given name and field structure.
     *
     * Called when saving deck from a file.
     *
     * @param deck_name Deck name
     * @param field_names Field type identifiers
     * @return shared_ptr to the created deck,
     * nullptr if a deck with the same name already existed.
     */
    shared_ptr<Deck> add_deck(const string& deck_name,
                              const Fields& field_types);

    /**
     * @brief Checks if a deck exists.
     *
     * @param deck_name Deck name
     * @return True if the deck exists,
     * false otherwise.
     */
    bool deck_exists(const string& deck_name) const;

    /**
     * @brief Removes the deck with the given name.
     *
     * @param deck_name Deck name
     * @return True if the deck existed and was removed,
     * false otherwise.
     */
    bool remove_deck(const string& deck_name);

    /**
     * @brief Removes the given card in the given deck.
     *
     * @param deck_name Deck name
     * @param card_id Card ID
     * @return True if the card existed and was removed,
     * false otherwise.
     */
    bool remove_card(const string& deck_name,
                     unsigned int card_id);

    /**
     * @brief Returns all deck names.
     *
     * @return A vector containing all the deck names.
     */
    vector<string> get_deck_names() const;

    /**
     * @brief Returns a pointer to the given deck.
     *
     * @param deck_name Deck name
     * @return A pointer to the deck if successful,
     * nullptr if the deck wasn't found.
     */
    shared_ptr<Deck> get_deck(const string& deck_name) const;

    /**
     * @brief Returns a pointer to the given card.
     *
     * @param deck_name Deck name
     * @param card_id Card ID
     * @return A pointer to the card if successful,
     * nullptr if the card wasn't found.
     */
    shared_ptr<Card> get_card(const string& deck_name,
                              unsigned int card_id) const;

private:
    map<string, shared_ptr<Deck>> decks_;

    /**
     * @brief Reads file header line.
     *
     * Extracts deck name and number of fields.
     *
     * @param file Input file stream
     * @param deck_name Output deck name
     * @param field_count Output field count
     * @return True on success, false otherwise.
     */
    bool read_file_header(ifstream& file_object,
                          string& deck_name,
                          unsigned int& field_count);

    /**
     * @brief Reads second header line.
     *
     * Extracts field names.
     *
     * @param file Input file stream
     * @param expected_count Expected number of fields
     * @param field_names Output field list
     * @return True on success, false otherwise.
     */
    bool read_field_names(ifstream& file_object,
                          unsigned int expected_count,
                          Fields& field_names);

    /**
     * @brief Reads all cards from file.
     *
     * @param file Input file stream
     * @param deck_name Pointer to the Deck
     * @param field_names Field definitions
     * @param card_count Output number of successfully read cards
     */
    void read_cards(ifstream& file_object,
                    shared_ptr<Deck> deck,
                    const Fields& field_names,
                    unsigned int& card_count);

};

#endif // DECKMANAGER_HH
