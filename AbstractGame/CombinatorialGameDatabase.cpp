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

GameId CGDatabase::createGameId(const std::unordered_set<GameId>& left, const std::unordered_set<GameId>& right) {
    if (left.empty() && right.empty()) return zeroId;
    for (const auto& game : existingGames) {
        if (game->getLeftOptions() == left && game->getRightOptions() == right)
			return game->getId();
    }

    existingGames.emplace_back(std::make_shared<CombinatorialGame>(left, right, existingGames.size()));
    return existingGames.size()-1;
}

CombinatorialGame& CGDatabase::createGame(
	const std::unordered_set<GameId>& left,
	const std::unordered_set<GameId>& right
) {
    return getGame(createGameId(left, right));
}




CGDatabase cgDatabase;