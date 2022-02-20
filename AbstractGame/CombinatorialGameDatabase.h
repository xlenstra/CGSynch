//
// Created by ardour on 08-02-22.
//
#pragma once

#ifndef CGSYNCH_2_COMBINATORIALGAMEDATABASE_H
#define CGSYNCH_2_COMBINATORIALGAMEDATABASE_H


#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <boost/bimap.hpp>
#include "CombinatorialGame.h"

typedef size_t GameId;
class CombinatorialGame;

class CGDatabase {
public:
	CGDatabase();
//	~CGDatabase();
    void print(std::ostream& os);

    GameId createGameId(const std::unordered_set<GameId>& left, const std::unordered_set<GameId>& right);
    CombinatorialGame& createGame(const std::unordered_set<GameId>& left, const std::unordered_set<GameId>& right);
    CombinatorialGame& getGame(GameId id) { return *existingGames.at(id); }

    CombinatorialGame& getZero() { return getGame(zeroId); }
	CombinatorialGame& getInteger(int value);
//	const std::unordered_map<GameId,int>& getSavedIntegers() { return savedIntegers; }

    const GameId zeroId = 0;

private:
    std::vector<std::shared_ptr<CombinatorialGame>> existingGames;


//	std::unordered_map<GameId,int> savedIntegers;
//	boost::bimaps::bimap<GameId, int, boost::container::allocator<int>> savedIntegers;
};
std::ostream& operator<<(std::ostream& os, CGDatabase database);

#define GET_GAME(gameId) (cgDatabase.getGame(gameId))
#define CREATE_GAME(left, right) (cgDatabase.createGame(left, right))
extern CGDatabase cgDatabase;



#endif //CGSYNCH_2_COMBINATORIALGAMEDATABASE_H
