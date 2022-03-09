//
// Created by ardour on 18-02-22.
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
class CherriesGame : public AbstractGame<CherriesPosition> {
public:
	explicit CherriesGame(CherriesPosition position);
	CherriesGame(const CherriesGame& ruleset) = default;

	std::string getDisplayString() override;

	void explore() override;
	CherriesPosition getAnyTransposition() const override;
	std::unordered_set<CherriesPosition> getTranspositions() const override;
//	GameId getIdOrInsertIntoDatabase() override;
	bool tryToDetermineAbstractForm() override;

private:
	void addTranspositionsRecursively(std::unordered_set<CherriesPosition>& transposition, std::unordered_set<size_t>& sectionsToReverse, const size_t& depth) const;


	CherriesPosition position;
};

CherriesGame& createCherriesPosition(const std::string& inputString);

extern std::shared_ptr<GameDatabase<CherriesPosition, CherriesGame>> cherriesDatabase;

#endif //CGSYNCH_2_CHERRIES_H
