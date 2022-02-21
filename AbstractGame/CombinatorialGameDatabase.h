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
#include "CombinatorialGame.h"
#include "CombinatorialGameUtil.h"

/** Class that implements a database for [Combinatorial Games].
 * Each combinatorial game that is created, is saved inside this database.
 * When a new game is created with the same left and right options as one that already exists,
 * we instead return the existing one. That way, we don't have to do any calculation more than once.
 * Also has very very basic support for generating integers
 */
class CGDatabase {
public:
	CGDatabase();
    void print(std::ostream& os);

    /**
     * Checks if a game with these options exists. If it does, it returns the ID of that game.
     * If it doesn't, create such a game and then return the new ID.
     */
    GameId getGameId(const std::unordered_set<GameId>& left, const std::unordered_set<GameId>& right);
    CombinatorialGame& getGame(const std::unordered_set<GameId>& left, const std::unordered_set<GameId>& right);
    CombinatorialGame& idToGame(GameId id) { return *existingGames.at(id); }

    CombinatorialGame& getZero() { return idToGame(zeroId); }
	CombinatorialGame& getInteger(int value);
//	const std::unordered_map<GameId,int>& getSavedIntegers() { return savedIntegers; }

    const GameId zeroId = 0;

private:
    std::vector<std::shared_ptr<CombinatorialGame>> existingGames;


//	std::unordered_map<GameId,int> savedIntegers;
//	boost::bimaps::bimap<GameId, int, boost::container::allocator<int>> savedIntegers;
};
std::ostream& operator<<(std::ostream& os, CGDatabase database);

#define ID_TO_GAME(gameId) (cgDatabase.idToGame(gameId))
#define GET_GAME(left, right) (cgDatabase.getGame(left, right))
extern CGDatabase cgDatabase;



#endif //CGSYNCH_2_COMBINATORIALGAMEDATABASE_H
