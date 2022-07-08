//
// Created by Xander Lenstra on 03-02-22.
//

#ifndef CGSYNCH_2_COMBINATORIALGAME_H
#define CGSYNCH_2_COMBINATORIALGAME_H

#include <compare>
#include <optional>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "CombinatorialGameDatabase.h"
#include "CombinatorialGameUtil.h"
#include "DyadicRational.h"
#include "GameUtil.h"


/** The cached data of each game */
struct CGCacheBlock {
	std::string displayString = "";
	WinningPlayer cachedWinner = WinningPlayer::NONE;
	std::optional<size_t> birthDay = {};
	AlternatingId canonicalFormId = -1ul;
	AlternatingId negativeFormId = -1ul;
	std::optional<bool> isInteger = {};
    std::optional<bool> isNumber = {};
    std::optional<DyadicRational> numberValue = {};
	std::unordered_map<AlternatingId, AlternatingId> subtractionCache = {};
	std::unordered_map<AlternatingId, AlternatingId> additionCache = {};
	std::unordered_map<AlternatingId, std::partial_ordering> compareCache = {};
};

/** A node in the tree of abstract combinatorial games.
 * Is as such a combinatorial game itself, with its left and right options saved in the class.
 * Each of these is saved in the [cgDatabase], the only instance of the Combinatorial Game Database.
 * This database also ensures that each game is only saved once, so all results calculated for one
 * instance can also be used when that instance is created in another spot.
 * To facilitate this, DO NOT create this using its constructor, but instead use [cgDatabase.idToGame(left, right)].
 *
 * The game must receive the id's of its left and right children when constructed.
 * As a result, this tree must be built from the ground up.
 */
class CombinatorialGame {
public:

    // DO NOT USE THIS! Instead, use cgDatabase.getOrInsertGame(leftOptions, rightOptions);
    CombinatorialGame(std::set<AlternatingId> leftOptions, std::set<AlternatingId> rightOptions, AlternatingId id);
	CombinatorialGame(const CombinatorialGame& other);

    /** Sets the cache of this game to something else.
     * Potentially useful when these values can be computed more easily than using their definition
     * */
    void setCache(const CGCacheBlock& other);

    /** Get a string representation of this game in mathematical notation. Not reversible. */
	std::string getDisplayString() const;

    /** Get the winner of this game */
	WinningPlayer getWinner() const;
    /** Get the birthday of this game */
    size_t getBirthday() const;

    /** Get the id's of the left options of this game, which can be turned into games in the [cgDatabase] */
	const std::set<AlternatingId>& getLeftOptions() const { return leftOptions; }
    /** Get the id's of the right options of this game, which can be turned into games in the <A NAME="cgDatabase"> cgDatabase</A> */
	const std::set<AlternatingId>& getRightOptions() const { return rightOptions; }

    /** Get the canonical, most simplified, form of this game */
	CombinatorialGame& getCanonicalForm() const;
    /** Returns whether this game is in canonical form */
    bool isInCanonicalForm() const;
    /** Is this game an integer in canonical form?
     * Note that this function returns false if it's an integer in a different form (e.g. {1|3}).
     */
	bool isCanonicalInteger() const;
    /** Is this game a number? */
    bool isNumber() const;
    /** Get the number value of this game, using the simplest number rule */
    std::optional<DyadicRational> getNumberValue() const;

    /** Get the negative of this game */
	CombinatorialGame& operator-() const;
    /** Get the disjunctive sum of this game and another */
	CombinatorialGame& operator+(const CombinatorialGame& other) const;
    /** Get the disjunctive subtraction of this game and another */
	CombinatorialGame& operator-(const CombinatorialGame& other) const;
	/** Get whether two games are isomorphic. Use [a \<=> b == 0] to test for equality in the combinatorial game sense. */
	bool operator==(const CombinatorialGame& other) const;
    /** Get the relative partial ordering of this game and another.
     * For equality, use [a \<=>b == 0]; for incomparable use [a\<=> b == std::partial_ordering::incomparable]
     */
	std::partial_ordering operator<=>(const CombinatorialGame& other) const;

    /** Gets the id of this game as saved in the [cgDatabase] */
	[[nodiscard]] size_t getId() const { return id; }

private:

	std::set<AlternatingId> leftOptions;
	std::set<AlternatingId> rightOptions;
    const size_t id;

	/** Only caches results, so does not change the data structure */
	mutable CGCacheBlock cacheBlock;

    /** Returns the canonical form if it is already calculated; otherwise returns this */
	CombinatorialGame& getSimplestAlreadyCalculatedForm() const;
    bool _isNumber() const;

	bool isCanonicalNumber() const;

	CGDatabase& cgDatabase = CGDatabase::getInstance();
};




#endif //CGSYNCH_2_COMBINATORIALGAME_H
