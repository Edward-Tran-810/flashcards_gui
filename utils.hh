/*
#############################################################################
# COMP.CS.115 Ohjelmointi 3: Rajapinnat / Programming 3: Interfaces         #
# Project: Opettelukorttien paluu / Return of Flashcards                    #
# File: utils.hh                                                            #
# Description: Declares utility helper functions.                           #
#                                                                           #
# Notes: * Provides string splitting and validation helpers.                #
#        * Contains Field comparison utilities.                             #
#        * Used across multiple modules.                                    #
#                                                                           #
# Author information:                                                       #
# Name: Manh Tran                                                           #
# Student number: 154123367                                                 #
# Username: kkk243                                                          #
# Tuni email: manh.tran@tuni.fi                                             #
#############################################################################
*/

#ifndef UTILS_HH
#define UTILS_HH

#include <string>
#include <vector>

using namespace std;

using Fields = vector<string>;

/**
     * @brief Prints a formatted field list.
     *
     * Can support forms such as "EN FI DE:"
     *
     * @param prompts List of fields/defintions to print
     * @param delimiter Character printed between fields
     * @param end Character printed after last field
     */
void print_field_line(const Fields& prompts,
                       char delimiter = ' ',
                       char end = ':');

/**
 * @brief Checks whether two field lists match exactly.
 *
 * Compares two field sets regardless of ordering. Used when checking if two decks
 * have the exact same field types.
 *
 * @param group1 Reference field list
 * @param group2 Field list to compare
 * @return True if both sets contain the same fields, false otherwise
 */
bool fields_match(const Fields& group1, const Fields& group2);

/**
 * @brief Checks whether all subgroup fields exist in the main group.
 *
 * Checks that every element of the subgroup is contained
 * within the main field set. Used when checking that requested fields belong
 * in the deck in question.
 *
 * @param maingroup Complete field list
 * @param subgroup Field subset to validate
 * @return True if all subgroup fields exist in maingroup, false otherwise
 */
bool fields_overlap(const Fields& maingroup, const Fields& subgroup);

/**
 * @brief Checks for empty field values.
 *
 * @param fields Field list to validate
 * @return True if any field is empty, false otherwise
 */
bool check_empty_fields(const Fields& fields);

/**
 * @brief Splits a string using a delimiter.
 *
 * Supports quoted substrings, preserves delimiters inside quotes.
 *
 * @param input Input string
 * @param delimiter Delimiting character
 * @return Vector of split substrings
 */
vector<string> split(const string& input, char delimiter);

/**
 * @brief Checks whether a string represents a valid number.
 *
 * @param input Input string
 * @return True if numeric, false otherwise
 */
bool is_number(const string& input);

#endif // UTILS_HH
