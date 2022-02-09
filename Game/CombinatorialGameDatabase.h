//
// Created by ardour on 08-02-22.
//
#pragma once

#ifndef CGSYNCH_2_COMBINATORIALGAMEDATABASE_H
#define CGSYNCH_2_COMBINATORIALGAMEDATABASE_H


#include <vector>
#include <unordered_set>
#include "CombinatorialGame.h"

typedef size_t GameId;
class CombinatorialGame;

class CGDatabase {
public:
	CGDatabase();
//	~CGDatabase();

    GameId createGameId(std::unordered_set<GameId> left, std::unordered_set<GameId> right);
    CombinatorialGame& createGame(std::unordered_set<GameId> left, std::unordered_set<GameId> right);
    CombinatorialGame& getGame(GameId id) { return existingGames.at(id); }

    CombinatorialGame& getZero() { return getGame(0); }

private:
    std::vector<CombinatorialGame> existingGames;
};

#define GAME(game) (cgDatabase.getGame(game))
extern CGDatabase cgDatabase;



#endif //CGSYNCH_2_COMBINATORIALGAMEDATABASE_H
