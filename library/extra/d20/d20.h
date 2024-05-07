/** @file d20.h
 *  @brief Implements Dungeons & Dragons style dice in C
 * 
 * d20.h is a reimplementation of https://github.com/opensourcedoc/d20-c,
 * but following the principles of being a single header/file library with
 * a minimal API
 * 
 * @author adamml
 * @date 2022-11-07
*/

#ifndef COM_GITHUB_ADAMML_D20
#define COM_GITHUB_ADAMML_D20


#define ROLL_FROM_CHAR_ARRAY_PARSE_FAILURE_NO_NUMBER_DICE -9999; /* The number of dices to roll was not correctly parsed */
#define ROLL_FROM_CHAR_ARRAY_PARSE_FAILURE_NO_NUMBER_OF_SIDES -99999; /* The number of sides on the dice was not correctl parsed */

int roll_from_char_array(const char *ca);
int roll(int dice, int sides, int modifer);

#endif