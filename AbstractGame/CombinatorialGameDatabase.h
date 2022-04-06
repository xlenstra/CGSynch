//
// Created by ardour on 08-02-22.
//
#pragma once

#ifndef CGSYNCH_2_COMBINATORIALGAMEDATABASE_H
#define CGSYNCH_2_COMBINATORIALGAMEDATABASE_H


//#include <boost/bimap.hpp>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
//#include "CombinatorialGame.h"
#include "CombinatorialGameUtil.h"
#include "DyadicRational.h"

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
    AbstractId getGameId(const std::unordered_set<AbstractId>& left, const std::unordered_set<AbstractId>& right);
	/** Same as <code><em>getGameId</em></code>, but returns a reference to the game instead of its ID.
	 * That is, it checks if a game with the provided left and right options exists.
	 * If it does, it returns a reference to that game.
	 * Otherwise, it creates that game and returns the new ID.
	 */
    CombinatorialGame& getGame(const std::unordered_set<AbstractId>& left, const std::unordered_set<AbstractId>& right);
	/** Get the game corresponding to a given ID */
    CombinatorialGame& idToGame(AbstractId id) { return *existingGames.at(id); }

	/** Get the game corresponding to 0, i.e., neither player has a move */
    CombinatorialGame& getZeroGame() { return idToGame(zeroId); }
	/** Get the game that is in canonical form and has integer value equal to <code><em>value</em></code> */
	CombinatorialGame& getInteger(int value);
//	const std::unordered_map<AbstractId,int>& getSavedIntegers() { return savedIntegers; }
    /** Gets the game in canoncial form that has dyadic rational value equal to <code><em>numerator/denominator</em></code> */
    CombinatorialGame& getDyadicRational(int numerator, int denominator);
	CombinatorialGame& getDyadicRational(const DyadicRational& dyadicRational);


	/** the id of the game corresponding to 0, in which neither player has a move */
    const AbstractId zeroId = 0;

private:
	CGDatabase();
    std::vector<std::unique_ptr<CombinatorialGame>> existingGames;

    AbstractId _getDyadicRational(int numerator, int denominator);

	static CGDatabase instance;
//	std::unordered_map<AbstractId,int> savedIntegers;
//	boost::bimaps::bimap<AbstractId, int, boost::container::allocator<int>> savedIntegers;
};


std::ostream& operator<<(std::ostream& os, const CGDatabase& database);

/** Turns an abstract ID into a game */
#define ID_TO_GAME(abstractId) (CGDatabase::getInstance().idToGame(abstractId))
/** Turns two sets of left and right options resp. into a game with these options */
#define GET_GAME(left, right) (CGDatabase::getInstance().getGame(left, right))



#endif //CGSYNCH_2_COMBINATORIALGAMEDATABASE_H
