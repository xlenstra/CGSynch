//
// Created by ardour on 03-02-22.
//

#ifndef CGSYNCH_2_COMBINATORIALGAME_H
#define CGSYNCH_2_COMBINATORIALGAME_H

#include <vector>
#include <string>
#include <unordered_set>
#include "CombinatorialGameDatabase.h"

typedef size_t GameId;

enum class WinningPlayer {
	LEFT,
	PREVIOUS,
	NEXT,
	RIGHT,
	NONE = -1,
};

std::ostream& operator<<(std::ostream &os, WinningPlayer winningPlayer);


class CombinatorialGame {
public:

    // DO NOT USE THIS! Instead, use cgDatabase.createGame(leftOptions, rightOptions);
    CombinatorialGame(std::unordered_set<GameId> leftOptions, std::unordered_set<GameId> rightOptions, GameId id);
	CombinatorialGame(const CombinatorialGame& other);

    void copyCache(const CombinatorialGame& other);

	std::string getDisplayString();

	WinningPlayer getWinner();
    size_t getBirthday();

	const std::unordered_set<GameId>& getLeftOptions() const { return leftOptions; }
	const std::unordered_set<GameId>& getRightOptions() const { return rightOptions; }


	GameId operator-();
    GameId operator+(const CombinatorialGame& other);
    GameId operator-(const CombinatorialGame& other);
	// == means isomorphic, use .equals() to test for equality.
	bool operator==(const CombinatorialGame& other) const;

	[[nodiscard]] size_t getId() const {
		return id;
	}

private:
	CombinatorialGame() = default;

	std::unordered_set<GameId> leftOptions;
	std::unordered_set<GameId> rightOptions;
    std::unordered_set<GameId> parents;
    const size_t id;

	std::string displayString;
	WinningPlayer cachedWinner = WinningPlayer::NONE;
};







#endif //CGSYNCH_2_COMBINATORIALGAME_H
