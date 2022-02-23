//
// Created by ardour on 18-02-22.
//

#ifndef CGSYNCH_2_CHERRIESGAME_H
#define CGSYNCH_2_CHERRIESGAME_H

#include <functional>

#include "AbstractGame.h"
#include "boost/functional/hash/hash.hpp"

/** Colours of the stones in a position of the game Cherries */
enum class StoneColour {
	WHITE,
	BLACK,
	NONE,
};

/** Position in a game of cherries */
typedef std::unordered_multiset<std::deque<StoneColour>> CherriesPosition;
std::ostream& operator<<(std::ostream& os, CherriesPosition position);

// Required to make a hashMap from transposed CherriesPositions to actual positions
namespace std {
	template<>
	struct hash<std::deque<StoneColour>> {
		size_t operator()(const std::deque<StoneColour>& component) const {
			return boost::hash_range(component.begin(), component.end());
		}
	};
	template<>
	struct hash<CherriesPosition> {
		size_t operator()(const CherriesPosition& position) const {
			return boost::hash_range(position.begin(), position.end());
		}
	};
}

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

private:
	void addTranspositionsRecursively(std::unordered_set<CherriesPosition>& transposition, std::unordered_set<size_t>& sectionsToReverse, const size_t& depth) const;


	CherriesPosition position;
};

CherriesGame& createCherriesPosition(const std::string& inputString);

extern std::shared_ptr<RulesetDatabase<CherriesPosition, CherriesGame>> cherriesDatabase;

#endif //CGSYNCH_2_CHERRIESGAME_H
