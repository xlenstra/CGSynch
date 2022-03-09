//
// Created by ardour on 18-02-22.
//

#ifndef CGSYNCH_2_RULESETUTIL_H
#define CGSYNCH_2_RULESETUTIL_H

#include <concepts>
#include <cstddef>
#include <functional>
#include <iostream>

/** Id of a ruleset, which can be turned into a game in the database corresponding to that game */
typedef size_t GameId;

/** Each position must have a way of expression that position in a
 * way that it can be compared to other instances of that position
 * This Concept captures that property
 */
template<typename T>
concept isPosition =
	std::equality_comparable<T> // Has a == operator
    && requires(T a) {
		{ std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
	} // std::hash() is defined for this type
;

// Forward declaration
template<isPosition U>
class AbstractGame;

/** Databases for games may only be created for games
 * This concept captures that property
 */
template<typename T, typename U>
concept isGame = std::is_base_of<AbstractGame<U>,T>::value;



/** Colours of the stones in a position of a game*/
enum class StoneColour {
	BLACK = 1,
	BLUE = 2,
	WHITE = -1,
	RED = -2,
	NONE = 0,
};

std::ostream& operator<<(std::ostream& os, const StoneColour& stone) {
	switch (stone) {
		case StoneColour::BLACK: os << "B"; break;
		case StoneColour::BLUE : os << "B"; break;
		case StoneColour::WHITE: os << "W"; break;
		case StoneColour::RED  : os << "R"; break;
		case StoneColour::NONE : os << " "; break;
	}
	return os;
}
#endif //CGSYNCH_2_RULESETUTIL_H
