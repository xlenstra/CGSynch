//
// Created by s1935534 on 31/03/2022.
//

#ifndef CGSYNCH_2_SYNCHRONIZEDGAMEDATABASE_H
#define CGSYNCH_2_SYNCHRONIZEDGAMEDATABASE_H

#include "SynchronizedGameUtil.h"

class SGDatabase {
public:
	static SGDatabase& getInstance() { return instance; };
	SGDatabase(const SGDatabase& other) = delete;

	SynchedId getGameId(const SynchedMatrix& synchedMatrix);
	SynchronizedGame& idToGame(const SynchedId& id);
	SynchronizedGame& getGame(const SynchedMatrix& synchedMatrix);

private:
	SGDatabase();
	static SGDatabase instance;

	int firstUnusedId = 0;

	std::vector<SynchedMatrix> matrixIds;
	std::unordered_map<SynchedMatrix, std::unique_ptr<SynchronizedGame>> games;
};

#include "SynchronousGame.h"


#endif //CGSYNCH_2_SYNCHRONIZEDGAMEDATABASE_H
