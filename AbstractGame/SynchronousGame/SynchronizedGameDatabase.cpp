//
// Created by s1935534 on 31/03/2022.
//

#include "SynchronizedGameDatabase.h"
#include "SynchronousGame.h"

SGDatabase SGDatabase::instance = SGDatabase();

SynchedId SGDatabase::getGameId(const SynchedMatrix& synchedMatrix) {
	return getGame(synchedMatrix).getId();
}

SynchronizedGame& SGDatabase::idToGame(const SynchedId& id) {
	const SynchedMatrix& matrix = matrixIds.at(id);
	return *games[matrix];
}

SynchronizedGame& SGDatabase::getGame(const SynchedMatrix& synchedMatrix) {
	// Todo: This should instead check if there is an `equivalent' matrix contained in [games].
	// See the bottom of this file for an example algorithm
	if (!games.contains(synchedMatrix)) {
		games[synchedMatrix] = std::make_unique<SynchronizedGame>(SynchronizedGame(synchedMatrix, firstUnusedId));
		matrixIds.push_back(synchedMatrix);
		++firstUnusedId;
	}
	return *games.at(synchedMatrix);
}

SGDatabase::SGDatabase() {
	// Add the zero game
	getGame({{},0,0});
}


// EXAMPLE ALGORITHM FOR CHECKING EQUIVALENCY OF TWO MATRICES A AND B
//
// Check if not same dimensions -> false
// Check if not same elements -> false
//
// Take the first row [row1] of matrix A
// For each row [row2] of matrix B: (N)
//    if [row1] has NOT the same elements as [row2] -> continue
//    for mapping [map()] from [row1] to [row2]: (N!)
//        for [row3] of matrix A (skipping [row1]): (N)
//            if there does not exist a previously unused [row4] of B unequal to [row2] such that [map(row3)] != row4 -> try different mapping (N)
//        return true;

// Possibly save for easy comparison later?