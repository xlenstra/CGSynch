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

CombinatorialGame& CombinatorialGame::operator-() const {
	if (id == cgDatabase.zeroId) return cgDatabase.getZero();
	std::unordered_set<GameId> newRightOptions {};
	newRightOptions.reserve(leftOptions.size());
	for (const auto& leftOption : leftOptions) {
		newRightOptions.insert((-GET_GAME(leftOption)).getId());
	}
	std::unordered_set<GameId> newLeftOptions;
	newLeftOptions.reserve(rightOptions.size());
	for (const auto& rightOption: rightOptions) {
		newLeftOptions.insert((-GET_GAME(rightOption)).getId());
	}
    return CREATE_GAME(newLeftOptions, newRightOptions);
}

CombinatorialGame& CombinatorialGame::operator+(const CombinatorialGame& other) const {
	if (id == cgDatabase.zeroId) return GET_GAME(other.getId());
	if (other.getId() == cgDatabase.zeroId) return GET_GAME(id);
	std::unordered_set<GameId> newLeftOptions {};
	std::unordered_set<GameId> newRightOptions {};
	newLeftOptions.reserve(leftOptions.size() + other.leftOptions.size());
	newRightOptions.reserve(rightOptions.size() + other.rightOptions.size());
	for (const auto& leftOption : leftOptions) {
		newLeftOptions.insert((GET_GAME(leftOption) + other).getId());
	}
	for (const auto& leftOption : other.leftOptions) {
		newLeftOptions.insert((*this + GET_GAME(leftOption)).getId());
	}
	for (const auto& rightOption: rightOptions) {
		newRightOptions.insert((GET_GAME(rightOption) + other).getId());
	}
	for (const auto& rightOption: other.rightOptions) {
		newRightOptions.insert((*this + GET_GAME(rightOption)).getId());
	}
    return CREATE_GAME(newLeftOptions, newRightOptions);
}

CombinatorialGame& CombinatorialGame::operator-(const CombinatorialGame& other) const {
	if (other.getId() == cgDatabase.zeroId) return GET_GAME(id);
	if (id == cgDatabase.zeroId) return -other;
	if (id == other.getId()) return cgDatabase.getZero();
	std::unordered_set<GameId> newLeftOptions {};
	std::unordered_set<GameId> newRightOptions {};
	newLeftOptions.reserve(leftOptions.size() + other.leftOptions.size());
	newRightOptions.reserve(rightOptions.size() + other.rightOptions.size());
	for (const auto& leftOption : leftOptions) {
		newLeftOptions.insert((GET_GAME(leftOption) - other).getId());
	}
	for (const auto& rightOption: other.rightOptions) {
		newLeftOptions.insert((*this - GET_GAME(rightOption)).getId());
	}
	for (const auto& rightOption: rightOptions) {
		newRightOptions.insert((GET_GAME(rightOption) - other).getId());
	}
	for (const auto& leftOption : other.leftOptions) {
		newRightOptions.insert((*this - GET_GAME(leftOption)).getId());
	}
	return CREATE_GAME(newLeftOptions, newRightOptions);
}

bool CombinatorialGame::operator==(const CombinatorialGame& other) const {
	return id == other.id; // ID's imply isomorphism by how the database is structured
}


std::partial_ordering CombinatorialGame::operator<=>(const CombinatorialGame& other) const {
	CombinatorialGame& leftGame = this->getSimplestAlreadyCalculatedForm();
	CombinatorialGame& rightGame = this->getSimplestAlreadyCalculatedForm();
	GameId differenceGame;
	bool swappedGames = false;
	if (leftGame.getBirthday() < rightGame.getBirthday())
		differenceGame = (leftGame - rightGame).getId();
	else {
		differenceGame = (rightGame - leftGame).getId();
		swappedGames = true;
	}

	switch (GET_GAME(differenceGame).getWinner()) {
		case WinningPlayer::LEFT:
			if (swappedGames)
				return std::partial_ordering::less;
			return std::partial_ordering::greater;
		case WinningPlayer::PREVIOUS:
			return std::partial_ordering::equivalent;
		case WinningPlayer::NEXT:
			return std::partial_ordering::unordered;
		case WinningPlayer::RIGHT:
			if (swappedGames)
				return std::partial_ordering::greater;
			return std::partial_ordering::less;
		case WinningPlayer::NONE:
		default:
			throw(std::domain_error("Invalid game: " + (*this-other).getDisplayString()));
	}
}

void CombinatorialGame::copyCache(const CombinatorialGame& other) {
	cachedWinner = std::max(other.cachedWinner, cachedWinner);
	if (!displayString.empty())
        displayString = other.displayString;
}

std::string CombinatorialGame::getDisplayString() {
	if (id == cgDatabase.zeroId) {
		displayString = "0";
		return displayString;
	}
	if (isInteger()) {
		if (rightOptions.empty()) {
			displayString = std::to_string((int)getBirthday());
			return displayString;
		} else if (leftOptions.empty()) {
			displayString = std::to_string(-((int)getBirthday()));
			return displayString;
		}
	}
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

CombinatorialGame& CombinatorialGame::getCanonicalForm() {
	if (canonicalFormId != GameId(-1)) return GET_GAME(canonicalFormId);
	if (getWinner() == WinningPlayer::PREVIOUS) return GET_GAME(0);
	std::cout << "Started calculation of canonical form of game " << id << std::endl;

	std::unordered_set<GameId> newLeftOptions {};
	std::unordered_set<GameId> newRightOptions {};
	// First we convert all our options to canonical form
	for (const auto& leftId : leftOptions) {
		newLeftOptions.insert(GET_GAME(leftId).getCanonicalForm().getId());
	}
	for (const auto& rightId : rightOptions) {
		newRightOptions.insert(GET_GAME(rightId).getCanonicalForm().getId());
	}

	// Then we delete any dominated positions
	std::unordered_set<GameId> undominatedLeftOptions {};
	std::unordered_set<GameId> undominatedRightOptions {};
	for (const auto& leftId : newLeftOptions) {
		bool shouldAdd = true;
		for (const auto& otherLeftId : newLeftOptions) {
			if (leftId != otherLeftId && GET_GAME(leftId) < GET_GAME(otherLeftId)) {
				shouldAdd = false;
				break;
			}
		}
		for (const auto& alreadyAddedLeftId : undominatedLeftOptions) {
			if (leftId != alreadyAddedLeftId && GET_GAME(alreadyAddedLeftId) <=> GET_GAME(leftId) == 0) {
				shouldAdd = false;
				break;
			}
		}
		if (shouldAdd)
			undominatedLeftOptions.insert(leftId);
	}
	for (const auto& rightId : newRightOptions) {
		bool shouldAdd = true;
		for (const auto& otherRightId : newRightOptions) {
			if (rightId != otherRightId && GET_GAME(rightId) > GET_GAME(otherRightId)) {
				shouldAdd = false;
				break;
			}
		}
		for (const auto& alreadyAddedRightId : undominatedRightOptions) {
			if (rightId != alreadyAddedRightId && GET_GAME(alreadyAddedRightId) <=> GET_GAME(rightId) == 0) {
				shouldAdd = false;
				break;
			}
		}
		if (shouldAdd)
			undominatedRightOptions.insert(rightId);
	}

	// Lastly, we do reversibility
	std::unordered_set<GameId> finalLeftOptions {};
	std::unordered_set<GameId> finalRightOptions {};
	bool anyChangesWithReversibility = false;
	if (id == 43) {
		std::cout << "id=43" << std::endl;
	}
	for (const auto& leftId : undominatedLeftOptions) {
		CombinatorialGame& leftOption = GET_GAME(leftId);
		bool shouldAddLeftId = true;
		for (const auto& rightId : leftOption.getRightOptions()) {
			CombinatorialGame& leftRightOption = GET_GAME(rightId);
			if (leftRightOption <= *this) {
				anyChangesWithReversibility = true;
				shouldAddLeftId = false;
				finalLeftOptions.insert(leftRightOption.leftOptions.begin(), leftRightOption.leftOptions.end());
				break;
			}
		}
		if (shouldAddLeftId)
			finalLeftOptions.insert(leftId);
	}
	for (const auto rightId : undominatedRightOptions) {
		CombinatorialGame& rightOption = GET_GAME(rightId);
		bool shouldAddRightId = true;
		for (const auto& leftId : rightOption.getLeftOptions()) {
			CombinatorialGame& rightLeftOption = GET_GAME(leftId);
			if (rightLeftOption >= *this) {
				anyChangesWithReversibility = true;
				shouldAddRightId = false;
				finalRightOptions.insert(rightLeftOption.rightOptions.begin(), rightLeftOption.rightOptions.end());
				break;
			}
		}
		if (shouldAddRightId)
			finalRightOptions.insert(rightId);
	}

	if (anyChangesWithReversibility) {
		// Whelp, more options were added, and so presumably more options to remove by domination or reversibility.
		// Time to start over!
		CombinatorialGame& newGame = CREATE_GAME(finalLeftOptions, finalRightOptions).getCanonicalForm();
		canonicalFormId = newGame.getId();
		std::cout << "Canonical form of game " << id << " is game " << canonicalFormId << "!" << std::endl;
		return newGame;
	}

	CombinatorialGame& newGame = CREATE_GAME(finalLeftOptions, finalRightOptions);
	canonicalFormId = newGame.getId();
	std::cout << "Canonical form of game " << id << " is game " << canonicalFormId << "!" << std::endl;
	return newGame;
}

CombinatorialGame& CombinatorialGame::getSimplestAlreadyCalculatedForm() const {
	if (canonicalFormId != GameId(-1))
		return GET_GAME(canonicalFormId);
	return GET_GAME(id);
}

bool CombinatorialGame::isInteger() {
	if (id == cgDatabase.zeroId) return true;
	if (cachedIsInteger) return cachedIsInteger.value();
	cachedIsInteger = false;
	if (rightOptions.empty() && leftOptions.size() == 1) {
		cachedIsInteger = GET_GAME(*leftOptions.begin()).isInteger();
	}
	if (leftOptions.empty() && rightOptions.size() == 1) {
		cachedIsInteger = GET_GAME(*rightOptions.begin()).isInteger();
	}
	return cachedIsInteger.value();
}
