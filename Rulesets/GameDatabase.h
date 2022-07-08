//
// Created by Xander Lenstra on 15-02-22.
//

#ifndef CGSYNCH_2_GAMEDATABASE_H
#define CGSYNCH_2_GAMEDATABASE_H

#include <unordered_set>

#include "RulesetUtil.h"
#include "AbstractGame.h"

/** Class that is a database for a type of Game
 * The game must have a representation for each position that is both hashable and equality comparable
 * Whenever a new position of this game is found, it and all its transpositions are inserted into the database.
 * If then the same position (or a transposition of it) is found at a later time,
 * this database can be queried for this and returns the ID of the original game.
 * This makes it possible not to have to do calculations more than once.
 *
 * This class is a singleton; of each templated version there exists only one instance.
 * A ptr to this instance is returned by [RulesetDatabase\<\>::getInstance()]
 */
template<isPosition Position, isGame<Position> Game>
class GameDatabase {
public:
	/** Return the one and only instance with these templates */
	static std::shared_ptr<GameDatabase<Position, Game>> getInstance();

	/** Turns an id into a game */
    static Game& idToGame(GameId id);
	/** Returns the id of a known game of which this is a known transposition,
	 * or inserts it into the database if its not
	 */
	static GameId getOrInsertGameId(const Game& ruleset);
	/** Returns the game of which this is a known transposition,
	 * or inserts it into the database if its not
	 */
	static Game& getOrInsertGame(Game& ruleset);

    /** Returns the ID of the game this position is a known transposition of, or -1 if it is unknown */
	static GameId isKnownTransposition(Position transposition);

	static size_t size() { return database.size(); }

    /** PRINT */
	friend std::ostream& operator<<(std::ostream& os, GameDatabase<Position, Game> database) {
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
	static std::vector<std::shared_ptr<Game>> database;
	static std::shared_ptr<GameDatabase<Position, Game>> instance;
	GameDatabase() = default;


	static std::unordered_map<Position, GameId> transpositionTable;
};




// Templated class, so member functions must go in header file


template<isPosition comparableType, isGame<comparableType> Ruleset>
std::shared_ptr<GameDatabase<comparableType, Ruleset>> GameDatabase<comparableType, Ruleset>::getInstance() {
	if (instance) return instance;
	instance = std::make_shared<GameDatabase<comparableType, Ruleset>>(GameDatabase<comparableType, Ruleset>());
	return instance;
}

template<isPosition comparableType, isGame<comparableType> Ruleset>
Ruleset& GameDatabase<comparableType, Ruleset>::idToGame(GameId id) {
	return *(database.at(id));
}

template<isPosition comparableType, isGame<comparableType> Ruleset>
GameId GameDatabase<comparableType, Ruleset>::getOrInsertGameId(const Ruleset& ruleset) {
	// Get any transposition of the position
	const comparableType firstTransposition = ruleset.getAnyTransposition();
	if (transpositionTable.contains(firstTransposition))
		return transpositionTable[firstTransposition];

	// It's not in the known transpositions database, so add it and all its transpositions
	database.emplace_back(std::make_shared<Ruleset>(ruleset));
	GameId id = database.size() - 1;
	database.back()->setGameId(id);
	for (const auto& transposition : ruleset.getTranspositions()) {
		transpositionTable[transposition] = id;
	}
	return id;
}

template<isPosition comparableType, isGame<comparableType> Ruleset>
Ruleset& GameDatabase<comparableType, Ruleset>::getOrInsertGame(Ruleset& ruleset) {
	return idToGame(getOrInsertGameId(ruleset));
}

template<isPosition comparableType, isGame<comparableType> Ruleset>
GameId GameDatabase<comparableType, Ruleset>::isKnownTransposition(comparableType transposition) {
	if (transpositionTable.contains(transposition))
		return transpositionTable[transposition];
	return -1;
}


#endif //CGSYNCH_2_GAMEDATABASE_H
