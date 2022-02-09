////
//// Created by ardour on 08-02-22.
////
#include "CombinatorialGameDatabase.h"

GameId CGDatabase::createGameId(std::unordered_set<GameId> left, std::unordered_set<GameId> right) {
    existingGames.emplace_back(left, right, existingGames.size());
    return existingGames.size()-1;
}

CombinatorialGame& CGDatabase::createGame(std::unordered_set<GameId> left, std::unordered_set<GameId> right) {
    return getGame(createGameId(left, right));
}

CGDatabase::CGDatabase() {
    existingGames.emplace_back(std::unordered_set<GameId>(), std::unordered_set<GameId>(), 0);
}

CGDatabase cgDatabase;