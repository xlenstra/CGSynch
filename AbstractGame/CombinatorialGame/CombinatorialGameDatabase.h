//
// Created by Xander Lenstra on 08-02-22.
//
#pragma once

#ifndef CGSYNCH_2_COMBINATORIALGAMEDATABASE_H
#define CGSYNCH_2_COMBINATORIALGAMEDATABASE_H


#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <boost/container_hash/hash.hpp>
#include "CombinatorialGameUtil.h"
#include "DyadicRational.h"



namespace std {
	template<>
	struct hash<std::pair<long long, long long>> {
		size_t operator()(const std::pair<long long, long long>& pair) const {
			size_t hashValue = pair.first;
			boost::hash_combine(hashValue, pair.second);
			return hashValue;
		}
	};
}


/** Class that implements a database for [Combinatorial Games].
 * Each combinatorial game that is created, is saved inside this database.
 * When a new game is created with the same left and right options as one that already exists,
 * we instead return the existing one. That way, we don't have to do any calculation more than once.
 * Also has very very basic support for generating integers
 */
class CGDatabase {
public:
	static CGDatabase& getInstance() { return instance; };
	CGDatabase(const CGDatabase& other) = delete;
    void print(std::ostream& os) const;

    /** Checks if a game with these options exists. If it does, it returns the ID of that game.
     * If it doesn't, it creates such a game and then return the new ID.
     */
    AlternatingId getGameId(const std::set<AlternatingId>& left, const std::set<AlternatingId>& right);
	/** Same as <code><em>getGameId</em></code>, but returns a reference to the game instead of its ID.
	 * That is, it checks if a game with the provided left and right options exists.
	 * If it does, it returns a reference to that game.
	 * Otherwise, it creates that game and returns the new ID.
	 */
    CombinatorialGame& getGame(const std::set<AlternatingId>& left, const std::set<AlternatingId>& right);
	/** Get the game corresponding to a given ID */
    CombinatorialGame& idToGame(AlternatingId id) { return *existingGames.at(id); }

	/** Get the game corresponding to 0, i.e., neither player has a move */
    CombinatorialGame& getZeroGame() { return idToGame(zeroId); }
	/** Get the game that is in canonical form and has integer value equal to <code><em>value</em></code> */
	CombinatorialGame& getInteger(long long value);
//	const std::unordered_map<AbstractId,int>& getSavedIntegers() { return savedIntegers; }
    /** Gets the game in canoncial form that has dyadic rational value equal to <code><em>numerator/denominator</em></code> */
    CombinatorialGame& getDyadicRational(long long numerator, long long denominator);
	CombinatorialGame& getDyadicRational(const DyadicRational& dyadicRational);


	/** the id of the game corresponding to 0, in which neither player has a move */
    const AlternatingId zeroId = 0;

private:
	CGDatabase();
    std::vector<std::unique_ptr<CombinatorialGame>> existingGames;
	std::unordered_map<std::pair<std::set<AlternatingId>,std::set<AlternatingId>>, AlternatingId> setsToIdMap;

    AlternatingId _getDyadicRational(long long numerator, long long denominator);

	static CGDatabase instance;

	std::unordered_map<long long, AlternatingId> savedIntegers;
	std::unordered_map<std::pair<long long, long long>, AlternatingId> savedFractions;
};


std::ostream& operator<<(std::ostream& os, const CGDatabase& database);

/** Turns an abstract ID into a game */
#define ID_TO_GAME(abstractId) (CGDatabase::getInstance().idToGame(abstractId))
/** Turns two sets of left and right options resp. into a game with these options */
#define GET_GAME(left, right) (CGDatabase::getInstance().getGame(left, right))


// Yes, this needs to go here. Not at the top, as then we get recursive dependence and problems.
#include "CombinatorialGame.h"


#endif //CGSYNCH_2_COMBINATORIALGAMEDATABASE_H
