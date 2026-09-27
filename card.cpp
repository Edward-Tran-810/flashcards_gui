/*
#############################################################################
# COMP.CS.115 Ohjelmointi 3: Rajapinnat / Programming 3: Interfaces         #
# Project: Opettelukorttien paluu / Return of Flashcards                    #
# File: card.cpp                                                            #
# Description: Implements Card class functionality.                         #
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

#include "card.hh"
#include <iostream>

unsigned int Card::NEXT_ID = 1;

Card::Card() :
    ID_(NEXT_ID++)
{

}

Card::~Card()
{

}

bool Card::add_new_definitions(const Fields &field_types,
                               const Fields &definitions)
{
    if ( field_types.size() != definitions.size() )
    {
        return false;
    }

    for ( Fields::size_type field = 0 ; field < field_types.size() ; ++field )
    {
        definitions_[field_types.at(field)] = definitions.at(field);
    }
    return true;
}

bool Card::has_fields(const Fields &field_types) const
{
    for ( const string& field : field_types )
    {
        if ( definitions_.find(field) == definitions_.end() )
        {
            return false;
        }
    }
    return true;
}

Fields Card::get_fields() const
{
    Fields field_types;
    for ( const pair<string, string> definition : definitions_ )
    {
        field_types.push_back(definition.first);
    }
    return field_types;
}

bool Card::get_definitions(const Fields &requested_fields,
                           Fields &return_definitions)
{
    return_definitions.clear();
    for ( Fields::size_type i = 0 ; i < requested_fields.size() ; ++i )
    {
        if ( definitions_.find(requested_fields.at(i)) == definitions_.end() )
        {
            return false;
        }

        return_definitions.push_back(definitions_[requested_fields.at(i)]);
    }
    return true;
}

double Card::check_answers(const Fields& answer_fields,
                           const Fields& answers) const
{
    // Partial result = Points collected for each correct field on this card
    double partial_result = 0;
    for ( Fields::size_type i = 0 ; i < answers.size(); ++i )
    {
        // Check if field can be found
        if ( definitions_.find(answer_fields.at(i)) == definitions_.end() )
        {
            continue;
        }

        // If "correct answer" is empty, any answer is accepted and accumulates points
        if ( definitions_.at(answer_fields.at(i)).empty() )
        {
            ++partial_result;
            continue;
        }

        // Check answers by comparing "correct" and "user provided" answers
        if ( definitions_.at(answer_fields.at(i)) == answers.at(i) )
        {
            ++partial_result;
        }
    }

    // Add 0-1 points to the result depending on how many fields were correct
    return partial_result/answer_fields.size();
}

unsigned int Card::get_id() const
{
    return ID_;
}

bool Card::operator==(const Card &other) const
{
    return ID_ == other.ID_;
}

