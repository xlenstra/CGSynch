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
	const SynchedMatrix& matrix = matrixIds[id];
	return *games[matrix];
}

SynchronizedGame& SGDatabase::getGame(const SynchedMatrix& synchedMatrix) {
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
