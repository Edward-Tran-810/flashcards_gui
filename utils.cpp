/*
#############################################################################
# COMP.CS.115 Ohjelmointi 3: Rajapinnat / Programming 3: Interfaces         #
# Project: Opettelukorttien paluu / Return of Flashcards                    #
# File: utils.cpp                                                           #
# Description: Implements general-purpose helper functions.                 #
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

#include "utils.hh"

#include <set>
#include <iostream>

void print_field_line(const Fields& prompts,
                      char delimiter, char end)
{
    for (Fields::const_iterator field = prompts.begin();
         field != prompts.end();
         ++field)
    {
        if (field != prompts.begin())
        {
            cout << delimiter;
        }
        cout << *field;
    }
    cout << end << " ";
}

bool fields_match(const Fields& group1, const Fields& group2)
{
    set<string> main_fields;
    set<string> other_fields;

    for ( const string& field : group1 )
    {
        main_fields.insert(field);
    }
    for ( const string& field : group2 )
    {
        other_fields.insert(field);
    }

    if ( main_fields != other_fields )
    {
        return false;
    }

    return true;
}

bool fields_overlap(const Fields& maingroup, const Fields& subgroup)
{
    set<string> maingroup_fields;

    for ( const string& field : maingroup )
    {
        maingroup_fields.insert(field);
    }
    for ( const string& field : subgroup )
    {
        if ( maingroup_fields.find(field) == maingroup_fields.end() )
        {
            return false;
        }
    }

    return true;
}

bool check_empty_fields(const Fields &fields)
{
    for ( const string& field : fields )
    {
        if ( field.empty() )
        {
            return true;
        }
    }
    return false;
}

vector<string> split(const string& input, char delimiter)
{
    vector<string> result = { "" };
    bool in_quotes = false;

    for ( char c : input )
    {
        if ( c == '"' )
        {
            in_quotes = not in_quotes;
            continue;
        }

        if ( c == delimiter and not in_quotes )
        {
            result.push_back("");
        }
        else
        {
            result.back().push_back(c);
        }
    }

    return result;
}

bool is_number(const string& input)
{
    for ( char c : input )
    {
        if ( not isdigit(c) )
        {
            return false;
        }
    }
    return true;
}


