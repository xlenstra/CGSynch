//
// Created by ardour on 15-02-22.
//

#ifndef CGSYNCH_2_RULESETDATABASE_H
#define CGSYNCH_2_RULESETDATABASE_H

#include <unordered_set>

#include "RulesetUtil.h"
#include "AbstractRuleset.h"



typedef size_t RulesetId;

template<ComparableType comparableType, isRuleset<comparableType> Ruleset>
class RulesetDatabase {
public:
	static std::shared_ptr<RulesetDatabase<comparableType, Ruleset>> getInstance();

    static Ruleset& getGame(RulesetId id);
	static RulesetId createGameId([[maybe_unused]] const Ruleset& ruleset);
	static Ruleset& createGame(Ruleset& ruleset);

	static RulesetId isKnownTransposition(comparableType transposition);


	friend std::ostream& operator<<(std::ostream& os, RulesetDatabase<comparableType, Ruleset> database) {
		if (database.database.empty()) {
			os << "[]";
			return os;
		}
		os << "[{";
		for (const auto& position : database.database) {
			os << position->getDisplayString() << "}, {";
		}
		os << "\b\b\b]";
		return os;
	};


private:
	static std::shared_ptr<RulesetDatabase<comparableType, Ruleset>> instance;
	RulesetDatabase() = default;
	static std::vector<std::shared_ptr<Ruleset>> database;
	static std::unordered_map<comparableType, RulesetId> transpositionTable;
};




// Templated class, so member functions must go in header file


template<ComparableType comparableType, isRuleset<comparableType> Ruleset>
std::shared_ptr<RulesetDatabase<comparableType, Ruleset>> RulesetDatabase<comparableType, Ruleset>::getInstance() {
	if (instance) return instance;
	instance = std::make_shared<RulesetDatabase<comparableType, Ruleset>>(RulesetDatabase<comparableType, Ruleset>());
	return instance;
}

template<ComparableType comparableType, isRuleset<comparableType> Ruleset>
Ruleset& RulesetDatabase<comparableType, Ruleset>::getGame(RulesetId id) {
	return *(database.at(id));
}

template<ComparableType comparableType, isRuleset<comparableType> Ruleset>
RulesetId RulesetDatabase<comparableType, Ruleset>::createGameId(const Ruleset& ruleset) {
	// Get any transposition of the position
	const comparableType firstTransposition = ruleset.getAnyTransposition();
	if (transpositionTable.contains(firstTransposition))
		return transpositionTable[firstTransposition];

	// It's not in the known transpositions database, so add it and all its transpositions
	database.emplace_back(std::make_shared<Ruleset>(ruleset));
	RulesetId id = database.size() - 1;
	for (const auto& transposition : ruleset.getTranspositions()) {
		transpositionTable[transposition] = id;
	}
	return id;
}

template<ComparableType comparableType, isRuleset<comparableType> Ruleset>
Ruleset& RulesetDatabase<comparableType, Ruleset>::createGame(Ruleset& ruleset) {
	return getGame(createGameId(ruleset));
}

template<ComparableType comparableType, isRuleset<comparableType> Ruleset>
RulesetId RulesetDatabase<comparableType, Ruleset>::isKnownTransposition(comparableType transposition) {
	if (transpositionTable.contains(transposition))
		return transpositionTable[transposition];
	return -1;
}


#endif //CGSYNCH_2_RULESETDATABASE_H
