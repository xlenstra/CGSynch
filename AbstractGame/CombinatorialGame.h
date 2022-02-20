//
// Created by ardour on 03-02-22.
//

#ifndef CGSYNCH_2_COMBINATORIALGAME_H
#define CGSYNCH_2_COMBINATORIALGAME_H

#include <vector>
#include <string>
#include <unordered_set>
#include <compare>
#include "CombinatorialGameDatabase.h"
#include "util.h"

// Many of these functions are marked as 'const'. However, not all of them actually are.
// 'const' only means the games themselves are not changed, but indirectly they may call
// functions which cache more properties of this game.

typedef size_t GameId;

enum class WinningPlayer {
	LEFT,
	PREVIOUS,
	NEXT,
	RIGHT,
	NONE = -1,
};

struct CGCacheBlock {
	CGCacheBlock(
		std::string displayString = "",
		WinningPlayer cachedWinner = WinningPlayer::NONE,
		GameId canonicalFormId = -1ul,
		GameId negativeFormId = -1ul,
		const std::optional<bool>& cachedIsInteger = std::optional<bool>{}
	);

	std::string displayString = "";
	WinningPlayer cachedWinner = WinningPlayer::NONE;
	GameId canonicalFormId = -1ul;
	GameId negativeFormId = -1ul;
	std::optional<bool> cachedIsInteger;
};

std::ostream& operator<<(std::ostream &os, WinningPlayer winningPlayer);

class CombinatorialGame {
public:

    // DO NOT USE THIS! Instead, use cgDatabase.createGame(leftOptions, rightOptions);
    CombinatorialGame(std::unordered_set<GameId> leftOptions, std::unordered_set<GameId> rightOptions, GameId id);
	CombinatorialGame(const CombinatorialGame& other);

    void copyCache(const CGCacheBlock& other);

	std::string getDisplayString();

	WinningPlayer getWinner();
    size_t getBirthday();

	const std::unordered_set<GameId>& getLeftOptions() const { return leftOptions; }
	const std::unordered_set<GameId>& getRightOptions() const { return rightOptions; }

	CombinatorialGame& getCanonicalForm();
	bool isInteger();
	std::optional<int> getIntegerValue();

	CombinatorialGame& operator-();
	CombinatorialGame& operator+(CombinatorialGame& other);
	CombinatorialGame& operator-(CombinatorialGame& other);
	/** == means isomorphic, use `a \<=> b == 0` to test for equality in the combinatorial game sense. */
	bool operator==(const CombinatorialGame& other) const;
	std::partial_ordering operator<=>(const CombinatorialGame& other) const;

	[[nodiscard]] size_t getId() const {
		return id;
	}

private:

	std::unordered_set<GameId> leftOptions;
	std::unordered_set<GameId> rightOptions;
//    std::unordered_set<GameId> parents;
    const size_t id;


	CGCacheBlock cacheBlock;

	CombinatorialGame& getSimplestAlreadyCalculatedForm() const;
};




#endif //CGSYNCH_2_COMBINATORIALGAME_H
