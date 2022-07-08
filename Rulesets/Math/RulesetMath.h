//
// Created by Xander Lenstra on 15-02-22.
//

#ifndef CGSYNCH_2_RULESETMATH_H
#define CGSYNCH_2_RULESETMATH_H

#include <string>
#include "../AbstractGame.h"

typedef std::string MathPosition;

/** A class implementing the ruleset called 'the mathematical notation of games'.
 * A board position consists of two sets, the left set and the right set.
 * Each of these sets contain multiple positions.
 * A player may choose any of the positions from their own set to move to on their turn.
 * Some positions may be denoted by symbols, such as *, 3, -3/8 or ^.
 * If the set of the player that has to move is empty, they have lost.
 *
 * Examples of positions are {|}, {3|2}, {1/2|*} and {{3|-1}|{*,3|-1}}
 * The left set is written between the first { and the outermost |,
 * the right set between the outermost | and the last }.
 */
class MathRuleset : public AbstractGame<MathPosition> {
public:
	explicit MathRuleset(MathPosition inputString);
	void exploreAlternating() override;
	std::string getAnyTransposition() const override;
	std::unordered_set<std::string> getTranspositions() const override;
	std::string getDisplayString() override;
	bool tryToDetermineAlternatingId() override;

private:
	MathPosition position;
};




#endif //CGSYNCH_2_RULESETMATH_H
