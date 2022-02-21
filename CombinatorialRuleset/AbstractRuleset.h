//
// Created by ardour on 15-02-22.
//

#ifndef CGSYNCH_2_ABSTRACTRULESET_H
#define CGSYNCH_2_ABSTRACTRULESET_H

#include <concepts>
#include "RulesetUtil.h"
#include "../AbstractGame/CombinatorialGameDatabase.h"
#include "RulesetDatabase.h"

template<ComparableType comparable>
class AbstractRuleset {
public:

	virtual std::unordered_set<RulesetId> getLeftOptions();
	virtual std::unordered_set<RulesetId> getRightOptions();

	virtual std::string getDisplayString() = 0;

	virtual comparable getAnyTransposition() const = 0;
	virtual std::unordered_set<comparable> getTranspositions() const = 0;
//	virtual RulesetId getIdOrInsertIntoDatabase() = 0;
	bool hasBeenExplored() { return explored; }
	virtual void explore() = 0;

	virtual CombinatorialGame& getAbstractForm() {};

protected:
	AbstractRuleset() = default;

	bool explored = false;
	std::unordered_set<RulesetId> leftOptions {};
	std::unordered_set<RulesetId> rightOptions {};
	RulesetId rulesetId = -1ul;
	GameId abstractForm = -1;
	std::string displayString;
};

// Templated class, so member functions need to go here

template<ComparableType comparable>
std::unordered_set<RulesetId> AbstractRuleset<comparable>::getLeftOptions() {
	if (!explored) explore();
	return leftOptions;
}

template<ComparableType comparable>
std::unordered_set<RulesetId> AbstractRuleset<comparable>::getRightOptions() {
	if (!explored) explore();
	return rightOptions;
}

template<ComparableType comparable, isRuleset<comparable> ruleset>
GameId getAbstractFormId(ruleset position) {
	if (!position.hasBeenExplored()) position.explore();
	std::unordered_set<GameId> leftOptions;
	std::unordered_set<GameId> rightOptions;
	RulesetDatabase<comparable, ruleset>& database = *RulesetDatabase<comparable, ruleset>::getInstance();
	for (const auto& leftPosition : position.getLeftOptions()) {
		leftOptions.insert(getAbstractFormId<comparable, ruleset>(database.getGame(leftPosition)));
	}
	for (const auto& rightPosition : position.getRightOptions()) {
		rightOptions.insert(getAbstractFormId<comparable, ruleset>(database.getGame(rightPosition)));
	}
	return cgDatabase.getGameId(leftOptions, rightOptions);
}


#endif //CGSYNCH_2_ABSTRACTRULESET_H
