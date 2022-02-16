//
// Created by ardour on 15-02-22.
//

#ifndef CGSYNCH_2_RULESETDATABASE_H
#define CGSYNCH_2_RULESETDATABASE_H

#include "Ruleset.h"

template<typename T, typename U>
concept isRuleset = std::is_base_of<Ruleset<U>,T>::value;


template<ComparableType comparableType, isRuleset<comparableType> Ruleset>
class RulesetDatabase {
public:
	RulesetDatabase() = default;

private:
	std::vector<std::shared_ptr<Ruleset>> database;
};

#endif //CGSYNCH_2_RULESETDATABASE_H
