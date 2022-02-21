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
#include "CombinatorialGameUtil.h"
#include "util.h"

/** Player that wins a game */
enum class WinningPlayer {
	LEFT,
	PREVIOUS,
	NEXT,
	RIGHT,
	NONE = -1,
};
std::ostream& operator<<(std::ostream &os, WinningPlayer winningPlayer);

/** The cached data of each game */
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

/** A node in the tree of abstract combinatorial games.
 * Is as such a combinatorial game itself, with its left and right options saved in the class.
 * Each of these is saved in the [cgDatabase], the only instance of the Combinatorial Game Database.
 * This database also ensures that each game is only saved once, so all results calculated for one
 * instance can also be used when that instance is created in another spot.
 * To facilitate this, DO NOT create this using its constructor, but instead use [cgDatabase.getGame(left, right)].
 *
 * The game must receive the id's of its left and right children when constructed.
 * As a result, this tree must be built from the ground up.
 */
class CombinatorialGame {
public:

    // DO NOT USE THIS! Instead, use cgDatabase.createGame(leftOptions, rightOptions);
    CombinatorialGame(std::unordered_set<GameId> leftOptions, std::unordered_set<GameId> rightOptions, GameId id);
	CombinatorialGame(const CombinatorialGame& other);

    /** Copies the cache (rarely useful, as each unique abstract game is initialized only once) */
    void copyCache(const CGCacheBlock& other);

    /** Get a string representation of this game in mathematical notation. Not reversible. */
	std::string getDisplayString();

    /** Get the winner of this game */
	WinningPlayer getWinner();
    /** Get the birthday of this game */
    size_t getBirthday();

    /** Get the id's of the left options of this game, which can be turned into games in the [cgDatabase] */
	const std::unordered_set<GameId>& getLeftOptions() const { return leftOptions; }
    /** Get the id's of the right options of this game, which can be turned into games in the [cgDatabase] */
	const std::unordered_set<GameId>& getRightOptions() const { return rightOptions; }

    /** Get the canonical, most simplified, form of this game */
	CombinatorialGame& getCanonicalForm();
    /** Is this game an integer */
	bool isInteger();
    /** Get the integer value of this game, or [std::optional<int>{}] if it isn't a game */
	std::optional<int> getIntegerValue();

    /** Get the negative of this game */
	CombinatorialGame& operator-();
    /** Get the disjunctive sum of this game and another */
	CombinatorialGame& operator+(CombinatorialGame& other);
    /** Get the disjunctive subtraction of this game and another */
	CombinatorialGame& operator-(CombinatorialGame& other);
	/** Get whether two games are isomorphic. Use [a \<=> b == 0] to test for equality in the combinatorial game sense. */
	bool operator==(const CombinatorialGame& other) const;
    /** Get the relative partial ordering of this game and another.
     * For equality, use [a \<=>b == 0]; for incomparable use [a\<=> b == std::partial_ordering::incomparable]
     */
	std::partial_ordering operator<=>(const CombinatorialGame& other) const;

    /** Gets the id of this game as saved in the [cgDatabase] */
	[[nodiscard]] size_t getId() const { return id; }

private:

	std::unordered_set<GameId> leftOptions;
	std::unordered_set<GameId> rightOptions;
//    std::unordered_set<GameId> parents;
    const size_t id;


	CGCacheBlock cacheBlock;

    /** Returns the canonical form if it is already calculated; otherwise returns this */
	CombinatorialGame& getSimplestAlreadyCalculatedForm() const;
};




#endif //CGSYNCH_2_COMBINATORIALGAME_H
