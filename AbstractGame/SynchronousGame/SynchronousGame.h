//
// Created by ardour on 03-02-22.
//

#ifndef CGSYNCH_2_SYNCHRONOUSGAME_H
#define CGSYNCH_2_SYNCHRONOUSGAME_H

#include "SynchronizedGameUtil.h"
#include "Rational.h"

struct SGCacheBlock {
	Rational value;
};


class SynchronizedGame {
public:

	SynchronizedGame(SynchedMatrix synchedMatrix, SynchedId id);

	[[nodiscard]] SynchedId getId() const;

private:

	SynchedMatrix matrix;
	SynchedId id;
};





#endif //CGSYNCH_2_SYNCHRONOUSGAME_H
