//
// Created by Xander Lenstra on 08-02-22.
//
#include "CombinatorialGameDatabase.h"
#include "CombinatorialGame.h"

CGDatabase CGDatabase::instance = CGDatabase();

CGDatabase::CGDatabase() {
	existingGames.emplace_back(
		std::make_unique<CombinatorialGame>(
			std::set<AlternatingId>(),
			std::set<AlternatingId>(),
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

AlternatingId CGDatabase::getGameId(const std::set<AlternatingId>& left, const std::set<AlternatingId>& right) {
    if (left.empty() && right.empty()) return zeroId;
	std::pair<std::set<AlternatingId>, std::set<AlternatingId>> optionsPair = std::make_pair(left, right);
	if (setsToIdMap.contains(optionsPair))
		return setsToIdMap[optionsPair];

    existingGames.emplace_back(std::make_unique<CombinatorialGame>(left, right, existingGames.size()));
    return existingGames.size()-1;
}

CombinatorialGame& CGDatabase::getGame(
	const std::set<AlternatingId>& left,
	const std::set<AlternatingId>& right
) {
    return idToGame(getGameId(left, right));
}

CombinatorialGame& CGDatabase::getInteger(long long value) {
	if (value == 0) {
		return getZeroGame();
	} else if (savedIntegers.contains(value)) {
		return idToGame(savedIntegers.at(value));
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
		savedIntegers[value] = newGame.getId();
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
		savedIntegers[value] = newGame.getId();
		return newGame;
	} else {
		throw(std::domain_error(std::to_string(value) + " is not a valid integer!"));
	}
}

CombinatorialGame& CGDatabase::getDyadicRational(long long numerator, long long denominator) {
    if (denominator == 0)
        throw(std::domain_error("0 is not a power of 2!"));
    if (denominator < 0)
        throw(std::domain_error(std::to_string(denominator) + " is a negative number, and thus not allowed as a denominator!"));
    if (!std::has_single_bit((unsigned long long)denominator)) // bit magic
        throw(std::domain_error(std::to_string(denominator) + " is not a power of 2!"));
    return idToGame(_getDyadicRational(numerator, denominator));
}

AlternatingId CGDatabase::_getDyadicRational(long long numerator, long long denominator) {
    if (denominator == 1) {
	    return getInteger(numerator).getId();
    }

	std::pair<long long, long long> fractionAsPair = std::make_pair(numerator, denominator);
	if (savedFractions.contains(fractionAsPair)) {
		return savedFractions[fractionAsPair];
	} else if (numerator % 2 == 0) {
		return _getDyadicRational(numerator / 2, denominator / 2);
    } else {
		AlternatingId newGameId = getGameId(
		    { _getDyadicRational((numerator - 1) / 2, denominator / 2) },
		    { _getDyadicRational((numerator + 1) / 2, denominator / 2) }
	    );
		savedFractions[fractionAsPair] = newGameId;
		idToGame(newGameId).setCache(
			{
				{},
				{},
				{},
				newGameId,
				-1ul,
				false,
				true,
				DyadicRational(numerator,denominator),
			}
		);
		return newGameId;
    }
}

CombinatorialGame& CGDatabase::getDyadicRational(const DyadicRational& dyadicRational) {
	return getDyadicRational(dyadicRational.numerator, dyadicRational.denominator);
}