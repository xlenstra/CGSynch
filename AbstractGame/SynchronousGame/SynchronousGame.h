//
// Created by Xander Lenstra on 03-02-22.
//

#ifndef CGSYNCH_2_SYNCHRONOUSGAME_H
#define CGSYNCH_2_SYNCHRONOUSGAME_H

#include <optional>
#include <unordered_set>
#include "SynchronizedGameUtil.h"
#include "GameUtil.h"
#include "Rational.h"

struct SGCacheBlock {
	std::optional<double> value;
	std::unordered_set<WinningPlayer> winners;
	std::optional<size_t> birthday;
};


class SynchronizedGame {
public:

	SynchronizedGame() = delete;
	SynchronizedGame(SynchedMatrix synchedMatrix, SynchedId id);

	[[nodiscard]] SynchedId getId() const;
	std::unordered_set<WinningPlayer> getWinners();
	double getValue(bool printMatrix = false);
	size_t getBirthday();

	void setCache(const SGCacheBlock& other);

	bool operator==(const SynchronizedGame& other);

private:

	SGCacheBlock cacheBlock;
	SynchedMatrix matrix;
	SynchedId id;
};





#endif //CGSYNCH_2_SYNCHRONOUSGAME_H
