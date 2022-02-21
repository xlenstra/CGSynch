////
//// Created by ardour on 08-02-22.
////
#include "CombinatorialGameDatabase.h"

CGDatabase::CGDatabase() {
	existingGames.emplace_back(
		std::make_shared<CombinatorialGame>(
			std::unordered_set<GameId>(),
		    std::unordered_set<GameId>(),
	        0
		)
	);
//	savedIntegers[0] = 0;
	existingGames.reserve(16);
}

void CGDatabase::print(std::ostream &os) {
    os << std::string("[");
    for (auto& game : existingGames) {
        os << game->getDisplayString() << std::string(", ");
    }
    os << std::string("\b") << std::string("\b"); // delete trailing comma and space
    os << std::string("]");
}

std::ostream& operator<<(std::ostream& os, CGDatabase database) {
    database.print(os);
    return os;
}

GameId CGDatabase::getGameId(const std::unordered_set<GameId>& left, const std::unordered_set<GameId>& right) {
    if (left.empty() && right.empty()) return zeroId;
    for (const auto& game : existingGames) {
        if (game->getLeftOptions() == left && game->getRightOptions() == right)
			return game->getId();
    }

    existingGames.emplace_back(std::make_shared<CombinatorialGame>(left, right, existingGames.size()));
    return existingGames.size()-1;
}

CombinatorialGame& CGDatabase::getGame(
	const std::unordered_set<GameId>& left,
	const std::unordered_set<GameId>& right
) {
    return idToGame(getGameId(left, right));
}

CombinatorialGame& CGDatabase::getInteger(int value) {
	if (value == 0) {
		return getZero();
	} else if (value > 0) {
		CombinatorialGame& newGame = getGame({getInteger(value - 1).getId()}, {});
		newGame.copyCache({std::to_string(value), WinningPlayer::LEFT, newGame.getId(), -1ul, true });
		return newGame;
	} else if (value < 0) {
		CombinatorialGame& newGame = getGame({}, {getInteger(value + 1).getId()});
		newGame.copyCache({std::to_string(value), WinningPlayer::RIGHT, newGame.getId(), -1ul, true});
		return newGame;
	} else {
		throw(std::domain_error(std::to_string(value) + " is not a valid integer!"));
	}
}


CGDatabase cgDatabase;