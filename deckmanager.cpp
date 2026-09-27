/*
#############################################################################
# COMP.CS.115 Ohjelmointi 3: Rajapinnat / Programming 3: Interfaces         #
# Project: Opettelukorttien paluu / Return of Flashcards                    #
# File: deckmanager.cpp                                                     #
# Description: Implements DeckManager functionality.                        #
#                                                                           #
# Notes:                                                                    #
#                                                                           #
# Author information:                                                       #
# Name: Manh Tran                                                           #
# Student number: 154123367                                                 #
# Username: kkk243                                                          #
# Tuni email: manh.tran@tuni.fi                                             #
#############################################################################
*/

#include "deckmanager.hh"
#include "utils.hh"

#include <iostream>
#include <fstream>

const unsigned int DECK_NAME = 0;
const unsigned int FIELDS_COUNT = 1;
const unsigned int HEADER_SIZE = 2;

const string ERROR_FILE = "Error: Unable to open file.";
const string ERROR_FILE_HEADER = "Error: Erroneous file header.";
const string ERROR_FILE_CARD = "Error: Erroneous card in file.";

DeckManager::DeckManager() {}

DeckManager::~DeckManager() {}

bool DeckManager::read_file(const string &file_name)
{
    ifstream file_object(file_name);
    if ( not file_object )
    {
        cout << ERROR_FILE << endl;
        return false;
    }

    string deck_name = "";
    unsigned int field_count = 0;
    Fields field_names;
    unsigned int card_count = 0;

    if ( not read_file_header(file_object, deck_name, field_count) )
    {
        file_object.close();
        return false;
    }

    if ( not read_field_names(file_object, field_count, field_names) )
    {
        file_object.close();
        return false;
    }

    shared_ptr<Deck> new_deck = add_deck(deck_name, field_names);
    if ( not new_deck )
    {
        file_object.close();
        return false;
    }

    read_cards(file_object, new_deck, field_names, card_count);
    return true;
}

shared_ptr<Deck> DeckManager::add_deck(const string& deck_name,
                                       const Fields &field_types)
{
    if ( deck_exists(deck_name) )
    {
        return nullptr;
    }

    shared_ptr<Deck> new_deck = make_shared<Deck>(deck_name, field_types);
    decks_[deck_name] = new_deck;
    return new_deck;
}

bool DeckManager::deck_exists(const string &deck_name) const
{
    return decks_.find(deck_name) != decks_.end();
}

bool DeckManager::remove_deck(const string& deck_name)
{
    if ( not deck_exists(deck_name) )
    {
        return false;
    }

    decks_.erase(deck_name);
    return true;
}

bool DeckManager::remove_card(const string& deck_name,
                              unsigned int card_id)
{
    if ( not deck_exists(deck_name) )
    {
        return false;
    }

    return decks_.at(deck_name)->remove_card(card_id);
}

vector<string> DeckManager::get_deck_names() const
{
    vector<string> names;

    for ( pair<string, shared_ptr<Deck>> deck : decks_ )
    {
        names.push_back(deck.first);
    }

    return names;
}

shared_ptr<Deck> DeckManager::get_deck(const string& deck_name) const
{
    if ( decks_.find(deck_name) == decks_.end())
    {
        return nullptr;
    }
    return decks_.at(deck_name);
}

bool DeckManager::read_file_header(ifstream &file_object,
                                   string &deck_name,
                                   unsigned int &field_count)
{
    string header_line = "";
    getline(file_object, header_line);
    vector<string> split_header = split(header_line, ';');

    if ( split_header.size() != HEADER_SIZE or
         split_header.at(DECK_NAME).empty() or
         split_header.at(FIELDS_COUNT).empty() or
         not is_number(split_header.at(FIELDS_COUNT)) )
    {
        cout << ERROR_FILE_HEADER << endl;
        file_object.close();
        return false;
    }

    deck_name = split_header.at(DECK_NAME);
    field_count = stoi(split_header.at(FIELDS_COUNT));
    return true;
}

bool DeckManager::read_field_names(ifstream &file_object,
                                   unsigned int expected_count,
                                   Fields &field_names)
{
    string field_line = "";
    getline(file_object, field_line);
    field_names = split(field_line, ';');

    if ( field_names.size() != expected_count or
         check_empty_fields(field_names) )
    {
        cout << ERROR_FILE_HEADER << endl;
        file_object.close();
        return false;
    }
    return true;
}

void DeckManager::read_cards(ifstream &file,
                             shared_ptr<Deck> deck,
                             const Fields &field_names,
                             unsigned int &card_count)
{
    string card_line = "";
    card_count = 0;

    while ( getline(file, card_line) )
    {
        vector<string> split_line = split(card_line, ';');

        if ( split_line.size() != field_names.size() )
        {
            cout << ERROR_FILE_CARD << endl;
            continue;
        }

        if ( not deck->add_card(field_names, split_line) )
        {
            cout << ERROR_FILE_CARD << endl;
            continue;
        }
        ++card_count;
    }
}




