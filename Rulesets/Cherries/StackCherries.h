//
// Created by s1935534 on 09/03/2022.
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
	//GameId getIdOrInsertIntoDatabase() override;

private:
	StackCherriesPosition position;
};

StackCherries& createStackCherriesPosition(const std::string& inputString);

extern std::shared_ptr<GameDatabase<StackCherriesPosition, StackCherries>> stackCherriesDatabase;

#endif //CGSYNCH_2_STACKCHERRIES_H
