//
// Created by ardour on 03-02-22.
//

#ifndef CGSYNCH_2_COMBINATORIALGAME_H
#define CGSYNCH_2_COMBINATORIALGAME_H

#include <vector>
#include <string>
#include <unordered_set>

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
	CombinatorialGame(std::unordered_set<CombinatorialGame*>& rightOptions, std::unordered_set<CombinatorialGame*>& leftOptions);
	CombinatorialGame(const CombinatorialGame& other);
	void copyCache(const CombinatorialGame& other);

	std::string getDisplayString() {
		return "0";
	}

	WinningPlayer getWinner();

	const std::unordered_set<CombinatorialGame*>& getLeftOptions() const { return leftOptions; }
	const std::unordered_set<CombinatorialGame*>& getRightOptions() const { return rightOptions; }


	CombinatorialGame* operator-() const;
	CombinatorialGame* operator+(const CombinatorialGame& other) const;
	CombinatorialGame* operator-(const CombinatorialGame& other) const;
	// == means isomorphic, use .equals() to test for equality.
	bool operator==(const CombinatorialGame& other) const;

	[[nodiscard]] size_t getId() const {
		return id;
	}

private:
	CombinatorialGame() = default;

	std::unordered_set<CombinatorialGame*> leftOptions;
	std::unordered_set<CombinatorialGame*> rightOptions;
	std::string displayName;
	const size_t id;

	WinningPlayer cachedWinner = WinningPlayer::NONE;
};







#endif //CGSYNCH_2_COMBINATORIALGAME_H
