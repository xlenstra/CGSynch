//
// Created by ardour on 03-02-22.
//

#include "GameUtil.h"
#include "GurobiSolver.h"
#include "SynchronousGame.h"
#include "SynchronizedGameDatabase.h"

#include <utility>

SynchedId SynchronizedGame::getId() const {
	return id;
}

SynchronizedGame::SynchronizedGame(
	SynchedMatrix synchedMatrix,
	SynchedId id
) : matrix(std::move(synchedMatrix)),
	id(id),
	sgDatabase(SGDatabase::getInstance())
{

}

double SynchronizedGame::getValue() {
	if (cacheBlock.value)
		return *cacheBlock.value;
	if (matrix.rightMoveCount == 0)
		return (double) matrix.leftMoveCount;
	if (matrix.leftMoveCount == 0)
		return - (double) matrix.rightMoveCount;
	Matrix<double> values(matrix.matrix.getWidth(), matrix.matrix.getHeight());
	for (size_t i = 0; i < matrix.matrix.size(); ++i) {
		for (size_t j = 0; j < matrix.matrix[i].size(); ++j) {
			values[i][j] = sgDatabase.idToGame(matrix.matrix[i][j]).getValue();
		}
	}

	GurobiSolver solver = GurobiSolver(values);
	cacheBlock.value = solver.solve();
	return *cacheBlock.value;
}

std::unordered_set<WinningPlayer> SynchronizedGame::getWinners() {
	if (!cacheBlock.winners.empty())
		return cacheBlock.winners;

	if (matrix.leftMoveCount == 0 && matrix.rightMoveCount == 0)
		return { WinningPlayer::DRAW };
	if (matrix.rightMoveCount == 0)
		return { WinningPlayer::LEFT };
	if (matrix.leftMoveCount == 0)
		return { WinningPlayer::RIGHT };

	for (const auto& row : matrix.matrix) {
		for (const auto& childId : row) {
			std::unordered_set<WinningPlayer> winners = sgDatabase.idToGame(childId).getWinners();
			cacheBlock.winners.insert(winners.begin(), winners.end());
		}
	}
	return cacheBlock.winners;
}

size_t SynchronizedGame::getBirthday() {
	if (cacheBlock.birthday)
		return *cacheBlock.birthday;

	if (matrix.leftMoveCount == 0 || matrix.rightMoveCount == 0)
		return 0;
	for (const auto& row : matrix.matrix) {
		for (const auto& childId : row) {
			*cacheBlock.birthday = std::max(*cacheBlock.birthday, sgDatabase.idToGame(childId).getBirthday() + 1);
		}
	}
	return *cacheBlock.birthday;
}

bool SynchronizedGame::operator==(const SynchronizedGame& other) {
	return id == other.id;
}
