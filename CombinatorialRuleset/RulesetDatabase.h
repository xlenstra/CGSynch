//
// Created by ardour on 15-02-22.
//

#ifndef CGSYNCH_2_RULESETDATABASE_H
#define CGSYNCH_2_RULESETDATABASE_H

#include "AbstractRuleset.h"

template<typename T, typename U>
concept isRuleset = std::is_base_of<AbstractRuleset<U>,T>::value;


typedef size_t RulesetId;

template<ComparableType comparableType, isRuleset<comparableType> Ruleset>
class RulesetDatabase {
public:
	RulesetDatabase() = default;
    Ruleset& getGame(RulesetId id);
    RulesetId createGameId(comparableType comparable);
    Ruleset& createGame(comparableType comparable);

private:
	std::vector<std::shared_ptr<comparableType>> database;

    RulesetId nextId = 0;
};

#endif //CGSYNCH_2_RULESETDATABASE_H
