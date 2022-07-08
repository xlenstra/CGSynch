//
// Created by Xander Lenstra on 18-02-22.
//

#ifndef CGSYNCH_2_CHERRIES_H
#define CGSYNCH_2_CHERRIES_H

#include <functional>

#include "AbstractGame.h"
#include "CherriesUtil.h"


/** A game of cherries.
 * This is basically a container for both the position of the game board,
 * and the ID of the abstract game with these left and right positions.
 * It can be visualised as a node in a tree, where the left and right
 */
class Cherries : public AbstractGame<CherriesPosition> {
public:
	explicit Cherries(CherriesPosition position);
	Cherries(const Cherries& ruleset) = default;

	std::string getDisplayString() override;

	void exploreAlternating() override;
	CherriesPosition getAnyTransposition() const override;
	std::unordered_set<CherriesPosition> getTranspositions() const override;
	bool tryToDetermineAlternatingId() override;
	void exploreSynched() override;
	bool determineDecidedSynchedValue() override;

private:
	void addTranspositionsRecursively(std::unordered_set<CherriesPosition>& transposition, std::unordered_set<size_t>& sectionsToReverse, const size_t& depth) const;


	CherriesPosition position;
};

Cherries& createCherriesPosition(const std::string& inputString);

extern const std::shared_ptr<GameDatabase<CherriesPosition, Cherries>> cherriesDatabase;

namespace synchedGamesParser {
	extern std::set<std::string> rulesetsForWhichUndecidableErrorWasShown;
	extern bool ignoreNonSeparable;
}

#endif //CGSYNCH_2_CHERRIES_H
