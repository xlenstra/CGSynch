//
// Created by Xander Lenstra on 31/03/2022.
//

#ifndef CGSYNCH_2_SYNCHRONIZEDGAMEDATABASE_H
#define CGSYNCH_2_SYNCHRONIZEDGAMEDATABASE_H

#include "SynchronizedGameUtil.h"

class SGDatabase {
public:
	static SGDatabase& getInstance();
	SGDatabase(const SGDatabase& other) = delete;
	void operator=(const SGDatabase& other) = delete;

	SynchedId getGameId(const SynchedMatrix& synchedMatrix);
	SynchronizedGame& idToGame(const SynchedId& id) { return *existingGames.at(id); };
	SynchronizedGame& getGame(const SynchedMatrix& synchedMatrix);

	SynchedId getDecidedGameWithValueId(double value);

private:
	SGDatabase();
	static std::unique_ptr<SGDatabase> instance;

	std::vector<std::unique_ptr<SynchronizedGame>> existingGames;
	std::unordered_map<SynchedMatrix, SynchedId> matrixToIdMap;
};

#include "SynchronousGame.h"


#endif //CGSYNCH_2_SYNCHRONIZEDGAMEDATABASE_H
