//
// Created by Xander Lenstra on 03-02-22.
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
	id(id)
{
	if (id == -1ul) {
		std::cout << "Creating a game with ID -1!";
	}
}

double SynchronizedGame::getValue(bool printMatrix) {
	if (cacheBlock.value)
		return *cacheBlock.value;

	if (matrix.leftOptions.size() * matrix.rightOptions.size() == 0) {
		throw std::logic_error("Ruleset has no way to determine the value of a position with only moves of one player!\nPlease implement this in the function `tryToDetermineSynchedId()`.");
	}
	Matrix<double> values(matrix.matrix.getWidth(), matrix.matrix.getHeight());
	for (size_t i = 0; i < matrix.matrix.size(); ++i) {
		for (size_t j = 0; j < matrix.matrix[i].size(); ++j) {
			values[i][j] = SGDatabase::getInstance().idToGame(matrix.matrix[i][j]).getValue();
			if (printMatrix)
				std::cout << values[i][j] << ',';
		}
		if (printMatrix)
			std::cout << std::endl;
	}

	GurobiSolver solver = GurobiSolver(values);
	cacheBlock.value = solver.solve();
	return *cacheBlock.value;
}

std::unordered_set<WinningPlayer> SynchronizedGame::getWinners() {
	if (!cacheBlock.winners.empty())
		return cacheBlock.winners;

	if (matrix.leftOptions.empty() && matrix.rightOptions.empty())
		return { WinningPlayer::DRAW };
	if (matrix.rightOptions.empty())
		return { WinningPlayer::LEFT };
	if (matrix.leftOptions.empty())
		return { WinningPlayer::RIGHT };

	for (const auto& row : matrix.matrix) {
		for (const auto& childId : row) {
			std::unordered_set<WinningPlayer> winners = SGDatabase::getInstance().idToGame(childId).getWinners();
			cacheBlock.winners.insert(winners.begin(), winners.end());
		}
	}
	return cacheBlock.winners;
}

size_t SynchronizedGame::getBirthday() {
	if (cacheBlock.birthday)
		return *cacheBlock.birthday;

	if (matrix.leftOptions.empty() && matrix.rightOptions.empty())
		return 0;
	for (const auto& row : matrix.matrix) {
		for (const auto& childId : row) {
			*cacheBlock.birthday = std::max(*cacheBlock.birthday, SGDatabase::getInstance().idToGame(childId).getBirthday() + 1);
		}
	}
	return *cacheBlock.birthday;
}

bool SynchronizedGame::operator==(const SynchronizedGame& other) {
	return id == other.id;
}

void SynchronizedGame::setCache(const SGCacheBlock& other) {
	cacheBlock.value = other.value;
	cacheBlock.winners = other.winners;
	cacheBlock.birthday = other.birthday;
}
