//
// Created by ardour on 15-02-22.
//

#include "RulesetDatabase.h"

//template<ComparableType comparableType, isRuleset<comparableType> Ruleset>
//RulesetDatabase<comparableType, Ruleset>::RulesetDatabase() {
//
//}

template<ComparableType comparableType, isRuleset<comparableType> Ruleset>
Ruleset& RulesetDatabase<comparableType, Ruleset>::getGame(RulesetId id) {
    return database.at(id);
}

template<ComparableType comparableType, isRuleset<comparableType> Ruleset>
Ruleset& RulesetDatabase<comparableType, Ruleset>::createGame(comparableType comparable) {
    return getGame(createGameId(comparable));
}

template<ComparableType comparableType, isRuleset<comparableType> Ruleset>
RulesetId RulesetDatabase<comparableType, Ruleset>::createGameId(comparableType comparable) {
    database.emplace_back(std::make_shared<CombinatorialGame>(comparable));
    return database.size()-1;
}

