////
//// Created by ardour on 08-02-22.
////
#include "CombinatorialGameDatabase.h"

void CGDatabase::print(std::ostream &os) {
    static const std::string open_bracket = "[";
    static const std::string comma = ", ";
    static const std::string backSpace = "\b";
    static const std::string closing_bracket = "]";
    os << open_bracket;
    for (auto& game : existingGames) {
        os << game.getDisplayString() << comma;
    }
    os << backSpace << backSpace; // delete trailing comma and space
    os << closing_bracket;
}

std::ostream& operator<<(std::ostream& os, CGDatabase database) {
    database.print(os);
    return os;
}

GameId CGDatabase::createGameId(const std::unordered_set<GameId>& left, const std::unordered_set<GameId>& right) {
    if (left.empty() && right.empty()) return zeroId;
    for (const auto& game : existingGames) {
        if (game.getLeftOptions() == left && game.getRightOptions() == right) return game.getId();
    }

    existingGames.emplace_back(left, right, existingGames.size());
    return existingGames.size()-1;
}

CombinatorialGame& CGDatabase::createGame(const std::unordered_set<GameId>& left, const std::unordered_set<GameId>& right) {
    return getGame(createGameId(left, right));
}

CGDatabase::CGDatabase() {
    existingGames.emplace_back(std::unordered_set<GameId>(), std::unordered_set<GameId>(), 0);
}



CGDatabase cgDatabase;