//
// Created by ardour on 03-02-22.
//

#ifndef CGSYNCH_2_SYNCHRONOUSGAME_H
#define CGSYNCH_2_SYNCHRONOUSGAME_H

#include <optional>
#include <unordered_set>
#include "SynchronizedGameUtil.h"
#include "Rational.h"

struct SGCacheBlock {
	std::optional<double> value;
	std::unordered_set<WinningPlayer> winners;
	std::optional<size_t> birthday;
};


class SynchronizedGame {
public:

	SynchronizedGame(SynchedMatrix synchedMatrix, SynchedId id);

	[[nodiscard]] SynchedId getId() const;
	std::unordered_set<WinningPlayer> getWinners();
	double getValue();
	size_t getBirthday();

private:

	SGCacheBlock cacheBlock;
	SynchedMatrix matrix;
	SynchedId id;

	SGDatabase& sgDatabase;

};





#endif //CGSYNCH_2_SYNCHRONOUSGAME_H
