/*
#############################################################################
# COMP.CS.115 Ohjelmointi 3: Rajapinnat / Programming 3: Interfaces         #
# Project: Opettelukorttien paluu / Return of Flashcards                    #
# File: deck.cpp                                                            #
# Description: Implements the Deck class functionality.                     #
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

#include "deck.hh"

Deck::Deck(string deck_name, const Fields& fields) :
    name_(deck_name), deck_fields_(make_shared<Fields>(fields))
{

}

Deck::~Deck()
{

}

bool Deck::add_card(const Fields &card_fields, const Fields &definitions)
{
    if ( card_fields.size() != definitions.size() )
    {
        return false;
    }

    shared_ptr<Card> new_card = make_shared<Card>();
    new_card->add_new_definitions(card_fields, definitions);

    cards_.push_back(new_card);
    return true;
}

bool Deck::remove_card(unsigned int card_id)
{
    vector<shared_ptr<Card>>::iterator card;
    for ( card = cards_.begin() ; card < cards_.end() ; ++card )
    {
        if ( card->get()->get_id() == card_id )
        {
            cards_.erase(card);
            return true;
        }
    }
    return false;
}

shared_ptr<Card> Deck::get_next_card(unsigned int card_index) const
{
    if ( card_index >= cards_.size() )
    {
        return nullptr;
    }

    return cards_.at(card_index);
}

shared_ptr<Fields> Deck::get_fields() const
{
    return deck_fields_;
}

unsigned int Deck::get_deck_size()
{
    return cards_.size();
}

vector<shared_ptr<Card>> Deck::get_cards() const
{
    return cards_;
}

shared_ptr<Card> Deck::get_card(unsigned int card_id)
{
    vector<shared_ptr<Card>>::iterator card;
    for ( card = cards_.begin() ; card < cards_.end() ; ++card )
    {
        if ( card->get()->get_id() == card_id )
        {
            return *card;
        }
    }
    return nullptr;
}

string Deck::get_name()
{
    return name_;
}


