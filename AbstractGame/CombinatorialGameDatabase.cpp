////
//// Created by ardour on 08-02-22.
////
#include "CombinatorialGameDatabase.h"
#include "CombinatorialGame.h"

CGDatabase CGDatabase::instance = CGDatabase();

CGDatabase::CGDatabase() {
	existingGames.emplace_back(
		std::make_unique<CombinatorialGame>(
			std::unordered_set<AbstractId>(),
			std::unordered_set<AbstractId>(),
			0
		)
	);
    existingGames.back()->setCache({
        "0",
        WinningPlayer::PREVIOUS,
		0,
        0,
        0,
        true,
        true,
        DyadicRational(0,1)
    });
	existingGames.reserve(1024); // So we don't have to copy over a lot
}

void CGDatabase::print(std::ostream &os) const {
    os << std::string("[");
    for (auto& game : existingGames) {
        os << game->getDisplayString() << std::string(", ");
    }
    os << std::string("\b") << std::string("\b"); // delete trailing comma and space
    os << std::string("]");
}

std::ostream& operator<<(std::ostream& os, const CGDatabase& database) {
    database.print(os);
    return os;
}

AbstractId CGDatabase::getGameId(const std::unordered_set<AbstractId>& left, const std::unordered_set<AbstractId>& right) {
    if (left.empty() && right.empty()) return zeroId;
    for (const auto& game : existingGames) {
        if (game->getLeftOptions() == left && game->getRightOptions() == right)
			return game->getId();
    }

    existingGames.emplace_back(std::make_unique<CombinatorialGame>(left, right, existingGames.size()));
    return existingGames.size()-1;
}

CombinatorialGame& CGDatabase::getGame(
	const std::unordered_set<AbstractId>& left,
	const std::unordered_set<AbstractId>& right
) {
    return idToGame(getGameId(left, right));
}

CombinatorialGame& CGDatabase::getInteger(int value) {
	if (value == 0) {
		return getZeroGame();
	} else if (value > 0) {
		CombinatorialGame& newGame = getGame({getInteger(value - 1).getId()}, {});
        newGame.setCache(
            {
                std::to_string(value),
                WinningPlayer::LEFT,
				value,
                newGame.getId(),
                -1ul,
                true,
                true,
                DyadicRational(value, 1)
            }
        );
		return newGame;
	} else if (value < 0) {
		CombinatorialGame& newGame = getGame({}, {getInteger(value + 1).getId()});
        newGame.setCache(
            {
                std::to_string(value),
                WinningPlayer::RIGHT,
				-value,
                newGame.getId(),
                -1ul,
                true,
                true,
                DyadicRational(value, 1)
            }
        );
		return newGame;
	} else {
		throw(std::domain_error(std::to_string(value) + " is not a valid integer!"));
	}
}

CombinatorialGame& CGDatabase::getDyadicRational(int numerator, int denominator) {
    if (denominator == 0)
        throw(std::domain_error("0 is not a power of 2!"));
    if (denominator < 0)
        throw(std::domain_error(std::to_string(denominator) + " is a negative number, and thus not allowed as a denominator!"));
    if (!std::has_single_bit((unsigned)denominator)) // bit magic
        throw(std::domain_error(std::to_string(denominator) + " is not a power of 2!"));
    return idToGame(_getDyadicRational(numerator, denominator));
}

AbstractId CGDatabase::_getDyadicRational(int numerator, int denominator) {
    if (denominator == 1)
        return getInteger(numerator).getId();
    if (numerator % 2 == 0)
        return _getDyadicRational(numerator/2, denominator/2);
    return getGameId(
        { _getDyadicRational((numerator-1)/2,denominator/2) },
        { _getDyadicRational((numerator+1)/2,denominator/2) }
    );
}

CombinatorialGame& CGDatabase::getDyadicRational(const DyadicRational& dyadicRational) {
	return getDyadicRational(dyadicRational.numerator, dyadicRational.denominator);
}