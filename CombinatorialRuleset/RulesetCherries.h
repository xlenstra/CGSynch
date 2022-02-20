//
// Created by ardour on 18-02-22.
//

#ifndef CGSYNCH_2_RULESETCHERRIES_H
#define CGSYNCH_2_RULESETCHERRIES_H

#include "AbstractRuleset.h"
#include "boost/functional/hash/hash.hpp"

enum class StoneColour {
	WHITE,
	BLACK,
	NONE,
};

typedef std::unordered_multiset<std::deque<StoneColour>> CherriesPosition;

// Required to make a hashMap from transposed CherriesPositions to actual positions
namespace std {
	template<>
	struct hash<std::deque<StoneColour, std::allocator<StoneColour>>> {
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

std::ostream& operator<<(std::ostream& os, CherriesPosition position);



class RulesetCherries : public AbstractRuleset<CherriesPosition> {
public:
	explicit RulesetCherries(CherriesPosition position);
	RulesetCherries(const RulesetCherries& ruleset) = default;

	std::string getDisplayString() override;

	void explore() override;
	CherriesPosition getAnyTransposition() const override;
	std::unordered_set<CherriesPosition> getTranspositions() const override;
//	RulesetId getIdOrInsertIntoDatabase() override;

private:
	void addTranspositionsRecursively(std::unordered_set<CherriesPosition>& transposition, std::unordered_set<size_t>& sectionsToReverse, const size_t& depth) const;


	CherriesPosition position;
};

RulesetCherries& createCherriesPosition(const std::string& inputString);

extern std::shared_ptr<RulesetDatabase<CherriesPosition, RulesetCherries>> cherriesDatabase;

#endif //CGSYNCH_2_RULESETCHERRIES_H
