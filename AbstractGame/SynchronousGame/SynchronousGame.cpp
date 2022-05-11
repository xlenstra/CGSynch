//
// Created by ardour on 03-02-22.
//

#include "SynchronousGame.h"

#include <utility>

SynchedId SynchronizedGame::getId() const {
	return id;
}

SynchronizedGame::SynchronizedGame(SynchedMatrix synchedMatrix, SynchedId id) : matrix(std::move(synchedMatrix)), id(id) {}
