//
// Created by ardour on 08-02-22.
//

#include "CombinatorialGameDatabase.h"

CGDatabase::~CGDatabase() {
	for (auto [key, value] : database) {
		delete value;
	}
}

CombinatorialGame* CGDatabase::getGameWithSets(
	const std::unordered_set<CombinatorialGame*>& leftOptions,
	const std::unordered_set<CombinatorialGame*>& rightOptions
) {
	std::pair<std::unordered_set<CombinatorialGame*>, std::unordered_set<CombinatorialGame*>> positionSets =
		std::make_pair(leftOptions, rightOptions);
}


CombinatorialGame* CGDatabase::getGame(CombinatorialGame* game) {
	std::pair<std::unordered_set<CombinatorialGame*>, std::unordered_set<CombinatorialGame*>> positionSets =
		std::make_pair(game->getLeftOptions(), game->getRightOptions());
	if (database.contains(positionSets)) {
		return database[positionSets];
	} else {
		database[positionSets] = game;
		return game;
	}
}


