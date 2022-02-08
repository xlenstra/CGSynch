//
// Created by ardour on 03-02-22.
//

#include <unordered_set>
#include <utility>
#include <iostream>
#include "CombinatorialGame.h"
#include "CombinatorialGameDatabase.h"

size_t next_unused_id = 0;

std::ostream& operator<<(std::ostream& os, WinningPlayer winningPlayer) {
	static const std::string LEFT = "LEFT";
	static const std::string RIGHT = "RIGHT";
	static const std::string PREVIOUS = "PREVIOUS";
	static const std::string NEXT = "NEXT";
	static const std::string NONE = "NONE";
	switch (winningPlayer) {
		case WinningPlayer::LEFT: os << LEFT; break;
		case WinningPlayer::RIGHT: os << RIGHT; break;
		case WinningPlayer::PREVIOUS: os << PREVIOUS; break;
		case WinningPlayer::NEXT: os << NEXT; break;
		case WinningPlayer::NONE: os << NONE; break;
	}
	return os;
}


CombinatorialGame::CombinatorialGame(
	std::unordered_set<CombinatorialGame*>& rightOptions,
	std::unordered_set<CombinatorialGame*>& leftOptions
) : leftOptions(std::move(leftOptions)), rightOptions(std::move(rightOptions)), id(next_unused_id) {
	next_unused_id++;
}


CombinatorialGame::CombinatorialGame(const CombinatorialGame& other) {
	leftOptions = other.leftOptions;
	rightOptions = other.rightOptions;
	copyCache();
}


WinningPlayer CombinatorialGame::getWinner() {
	if (cachedWinner != WinningPlayer::NONE) return cachedWinner;
	WinningPlayer winner;
	if (rightOptions.empty() && leftOptions.empty()) {
		winner = WinningPlayer::PREVIOUS;
	} else {
		bool leftHasWinningMove = false;
		bool rightHasWinningMove = false;
		for (auto& game : leftOptions) {
			WinningPlayer leftWinner = game.getWinner();
			if (leftWinner == WinningPlayer::LEFT || leftWinner == WinningPlayer::PREVIOUS) {
				leftHasWinningMove = true;
				break;
			}
		}
		for (auto& game : rightOptions) {
			WinningPlayer rightWinner = game.getWinner();
			if (rightWinner == WinningPlayer::RIGHT || rightWinner == WinningPlayer::PREVIOUS) {
				rightHasWinningMove = true;
				break;
			}
		}
		if (leftHasWinningMove && rightHasWinningMove) winner = WinningPlayer::NEXT;
		else if (leftHasWinningMove) winner = WinningPlayer::LEFT;
		else if (rightHasWinningMove) winner = WinningPlayer::RIGHT;
		else winner = WinningPlayer::PREVIOUS;
	}
	std::cout << "Winner has been found to be " << winner << std::endl;
	cachedWinner = winner;
	return winner;
}

CombinatorialGame* CombinatorialGame::operator-() const {
	std::unordered_set<CombinatorialGame*> newRightOptions;
	newRightOptions.reserve(leftOptions.size());
	for (const auto& leftOption : leftOptions) {
		newRightOptions.insert(cgDatabase.getGame(-*leftOption));
	}
	std::unordered_set<CombinatorialGame*> newLeftOptions;
	newLeftOptions.reserve(rightOptions.size());
	for (const auto& rightOption: rightOptions) {
		newLeftOptions.insert(cgDatabase.getGame(-*rightOption));
	}
	return cgDatabase.getGameWithSets(newLeftOptions,newRightOptions);
}

CombinatorialGame* CombinatorialGame::operator+(const CombinatorialGame& other) const {
//	std::unordered_set<CombinatorialGame> newLeftOptions;
//	std::unordered_set<CombinatorialGame> newRightOptions;
//	newLeftOptions.reserve(leftOptions.size() + other.leftOptions.size());
//	newRightOptions.reserve(rightOptions.size() + other.rightOptions.size());
//	for (const auto& leftOption : leftOptions) {
//		newLeftOptions.insert(leftOption + other);
//	}
//	for (const auto& leftOption : other.leftOptions) {
//		newLeftOptions.insert(*this + leftOption);
//	}
//	for (const auto& rightOption: rightOptions) {
//		newRightOptions.insert(rightOption + other);
//	}
//	for (const auto& rightOption: other.rightOptions) {
//		newRightOptions.insert(*this + rightOption);
//	}
//	return CombinatorialGame(newLeftOptions, newRightOptions);
}

CombinatorialGame* CombinatorialGame::operator-(const CombinatorialGame& other) const {
//	std::unordered_set<CombinatorialGame> newLeftOptions;
//	std::unordered_set<CombinatorialGame> newRightOptions;
//
//	return CombinatorialGame(newLeftOptions, newRightOptions);
}

bool CombinatorialGame::operator==(const CombinatorialGame& other) const {
	return leftOptions == other.leftOptions && rightOptions == other.rightOptions;
}

void CombinatorialGame::copyCache(const CombinatorialGame& other) {
	cachedWinner = std::max(other.cachedWinner, cachedWinner);
	if (!displayName.empty())
		displayName = other.displayName;
}
