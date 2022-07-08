//
// Created by Xander Lenstra on 31/03/2022.
//

#include <iostream>
#include "SynchronizedGameDatabase.h"
#include "SynchronousGame.h"

std::unique_ptr<SGDatabase> SGDatabase::instance = nullptr;

SGDatabase::SGDatabase() {
	// Add the zero game
	SynchronizedGame zeroGame = SynchronizedGame(SynchedMatrix(), 0);
	SGCacheBlock zeroCacheBlock;
	zeroCacheBlock.value = 0.0;
	zeroCacheBlock.winners = { WinningPlayer::DRAW };
	zeroCacheBlock.birthday = 0;
	zeroGame.setCache(zeroCacheBlock);
	existingGames.push_back(std::make_unique<SynchronizedGame>(zeroGame));
}

SynchedId SGDatabase::getGameId(const SynchedMatrix& synchedMatrix) {
	if (synchedMatrix.leftOptions.empty() && synchedMatrix.rightOptions.empty()) return 0;
	// Todo: This should instead check if there is an `equivalent' matrix contained in [games].
	// See the bottom of this file for an example algorithm
	if (matrixToIdMap.contains(synchedMatrix))
		return matrixToIdMap[synchedMatrix];

	existingGames.emplace_back(std::make_unique<SynchronizedGame>(synchedMatrix, existingGames.size()));
	return existingGames.size()-1;
}

SynchronizedGame& SGDatabase::getGame(const SynchedMatrix& synchedMatrix) {
	return idToGame(getGameId(synchedMatrix));
}

SynchedId SGDatabase::getDecidedGameWithValueId(double value) {
	if (value == 0)
		return 0;
	SynchedMatrix synchedMatrix;
	SynchronizedGame game = SynchronizedGame(SynchedMatrix(), existingGames.size());
	std::unordered_set<WinningPlayer> winners;
	if (value > 0) winners.insert(WinningPlayer::LEFT);
	else if (value < 0) winners.insert(WinningPlayer::RIGHT);
	game.setCache({
		value,
		winners,
		0
	});
	existingGames.push_back(std::make_unique<SynchronizedGame>(game));
	return game.getId();
}

SGDatabase& SGDatabase::getInstance() {
	if (instance == nullptr)
		instance = std::unique_ptr<SGDatabase>(new SGDatabase());
	return *instance;
}


// EXAMPLE ALGORITHM FOR CHECKING EQUIVALENCY OF TWO MATRICES A AND B
//
// Check if not same dimensions -> false
// Check if not same elements as an unordered_multiset -> false
//
// Take the first row [row1] of matrix A
// For each row [row2] of matrix B: (N)
//    if [row1] has NOT the same elements as [row2] -> continue
//    for mapping [map()] from [row1] to [row2]: (N!)
//        for [row3] of matrix A (skipping [row1]): (N)
//            if there does not exist a previously unused [row4] of B unequal to [row2] such that [map(row3)] == row4 -> try different mapping (N)
//        return true;

// Possibly save for easy comparison later?