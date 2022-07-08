//
// Created by Xander Lenstra on 09/03/2022.
//

#ifndef CGSYNCH_2_STACKCHERRIES_H
#define CGSYNCH_2_STACKCHERRIES_H

#include <functional>
#include <boost/functional/hash/hash.hpp>

#include "../AbstractGame.h"
#include "CherriesUtil.h"

/** Position in a game of cherries.
 * Identical to a position in normal cherries, so we just take that.
 */
typedef CherriesPosition StackCherriesPosition;


/** A game of stack cherries.
 * This is basically a container for both the position of the game board,
 * and the ID of the abstract game with these left and right positions.
 * It can be visualised as a node in a tree, where the left and right
 */
class StackCherries : public AbstractGame<StackCherriesPosition> {
public:
	explicit StackCherries(StackCherriesPosition position);
	StackCherries(const StackCherries& ruleset) = default;

	std::string getDisplayString() override;

	void exploreAlternating() override;
	StackCherriesPosition getAnyTransposition() const override;
	std::unordered_set<StackCherriesPosition> getTranspositions() const override;
	void exploreSynched() override;
	bool determineDecidedSynchedValue() override;

private:
	StackCherriesPosition position;
};

StackCherries& createStackCherriesPosition(const std::string& inputString);

namespace synchedGamesParser {
	extern std::set<std::string> rulesetsForWhichUndecidableErrorWasShown;
	extern bool ignoreNonSeparable;
}

#endif //CGSYNCH_2_STACKCHERRIES_H
