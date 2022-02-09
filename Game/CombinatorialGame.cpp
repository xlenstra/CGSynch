//
// Created by ardour on 03-02-22.
//

#include <unordered_set>
#include <utility>
#include <iostream>
#include "CombinatorialGame.h"


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
    std::unordered_set<GameId> leftOptions,
    std::unordered_set<GameId> rightOptions,
    GameId id
) : leftOptions(std::move(leftOptions)), rightOptions(std::move(rightOptions)), id(id) {}


CombinatorialGame::CombinatorialGame(const CombinatorialGame& other) :
    leftOptions(other.leftOptions),
    rightOptions(other.rightOptions),
    id(other.id)
{
	copyCache(other);
}


WinningPlayer CombinatorialGame::getWinner() {
	if (cachedWinner != WinningPlayer::NONE) return cachedWinner;
	WinningPlayer winner;
	if (rightOptions.empty() && leftOptions.empty()) {
		winner = WinningPlayer::PREVIOUS;
	} else {
		bool leftHasWinningMove = false;
		bool rightHasWinningMove = false;
		for (const auto& gameId : leftOptions) {
			WinningPlayer leftWinner = GET_GAME(gameId).getWinner();
			if (leftWinner == WinningPlayer::LEFT || leftWinner == WinningPlayer::PREVIOUS) {
				leftHasWinningMove = true;
				break;
			}
		}
		for (const auto& gameId : rightOptions) {
			WinningPlayer rightWinner = GET_GAME(gameId).getWinner();
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
	cachedWinner = winner;
	return winner;
}

GameId CombinatorialGame::operator-() {
	std::unordered_set<GameId> newRightOptions;
	newRightOptions.reserve(leftOptions.size());
	for (const auto& leftOption : leftOptions) {
		newRightOptions.insert((-GET_GAME(leftOption)));
	}
	std::unordered_set<GameId> newLeftOptions;
	newLeftOptions.reserve(rightOptions.size());
	for (const auto& rightOption: rightOptions) {
		newLeftOptions.insert((-GET_GAME(rightOption)));
	}
    return GAME(newLeftOptions, newRightOptions).getId();
}

GameId CombinatorialGame::operator+(const CombinatorialGame& other) {
	std::unordered_set<GameId> newLeftOptions;
	std::unordered_set<GameId> newRightOptions;
	newLeftOptions.reserve(leftOptions.size() + other.leftOptions.size());
	newRightOptions.reserve(rightOptions.size() + other.rightOptions.size());
	for (const auto& leftOption : leftOptions) {
		newLeftOptions.insert(GET_GAME(leftOption) + other);
	}
	for (const auto& leftOption : other.leftOptions) {
		newLeftOptions.insert(*this + GET_GAME(leftOption));
	}
	for (const auto& rightOption: rightOptions) {
		newRightOptions.insert(GET_GAME(rightOption) + other);
	}
	for (const auto& rightOption: other.rightOptions) {
		newRightOptions.insert(*this + GET_GAME(rightOption));
	}
    return GAME(newLeftOptions, newRightOptions).getId();
}

GameId CombinatorialGame::operator-(const CombinatorialGame& other) {
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
	if (!displayString.empty())
        displayString = other.displayString;
}

std::string CombinatorialGame::getDisplayString() {
    if (!displayString.empty()) return displayString;
    displayString = "{";
    for (const auto &leftId: leftOptions) {
        displayString += GET_GAME(leftId).getDisplayString() + ",";
    }
    if (!leftOptions.empty()) displayString += "\b"; // remove trailing ,
    displayString += "|";
    for (const auto &rightId: rightOptions) {
        displayString += GET_GAME(rightId).getDisplayString() + ",";
    }
    if (!rightOptions.empty()) displayString += "\b"; // remove trailing ,
    displayString += "}";
    return displayString;
}

size_t CombinatorialGame::getBirthday() {
    if (leftOptions.empty() && rightOptions.empty()) return 0;
    size_t maxFound = 0;
    for (const auto& leftId : leftOptions) {
        maxFound = std::max(maxFound, GET_GAME(leftId).getBirthday());
    }
    for (const auto& rightId: rightOptions) {
        maxFound = std::max(maxFound, GET_GAME(rightId).getBirthday());
    }
    return maxFound + 1;
}


