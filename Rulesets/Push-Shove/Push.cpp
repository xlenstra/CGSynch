//
// Created by Xander Lenstra on 09/03/2022.
//

#include "Push.h"

#include <utility>

CreateDatabase(PushShovePosition, Push, pushDatabase);

Push::Push(PushShovePosition position) : position(std::move(position)) {
	while (!this->position.empty() && this->position.back() == PieceColour::NONE) {
		this->position.pop_back();
	}
}

std::string Push::getDisplayString() {
	if (!displayString.empty()) return displayString;
	if (position.empty()) return "";
	for (const auto& square : position) {
		displayString += pieceColourToChar(square);
	}
	return displayString;
}

PushShovePosition Push::getAnyTransposition() const {
	return position;
}

std::unordered_set<PushShovePosition> Push::getTranspositions() const {
	return { position };
}

void Push::exploreAlternating() {
	int lastEmptySquareIndex = 0;
	auto lastEmptySquareIt = position.begin();
	auto currentSquareIt = position.begin();
	// We loop from left to right, and every time we find any piece, we try every move with it.
	for (size_t i = 0; i < position.size(); ++i) {
		if (*currentSquareIt != PieceColour::NONE) {
			PushShovePosition positionCopy = position;
			// Copy the part from the second square to the current square, but one tile to the left.
			std::copy(lastEmptySquareIt+1, currentSquareIt+1, positionCopy.begin() + lastEmptySquareIndex);
			positionCopy[i] = PieceColour::NONE;
			if (*currentSquareIt == PieceColour::BLUE) {
				leftOptions.push_back(pushDatabase->getOrInsertGameId(Push(positionCopy)));
			} else {
				rightOptions.push_back(pushDatabase->getOrInsertGameId(Push(positionCopy)));
			}
		} else {
			lastEmptySquareIt = currentSquareIt;
			lastEmptySquareIndex = (int)i;
		}

		++currentSquareIt;
	}
}

void Push::exploreSynched() {
	std::vector<size_t> leftOptions;
	std::vector<size_t> rightOptions;
	for (size_t i = 0; i < position.size(); ++i) {
		if (position[i] == PieceColour::BLUE)
			leftOptions.push_back(i);
		else if (position[i] == PieceColour::RED)
			rightOptions.push_back(i);
	}

	for (const auto& leftOption : leftOptions) {
		std::vector<GameId> redOptions;
		for (const auto& rightOption : rightOptions) {

			// We first move the left-most, that is smallest, hand
			size_t smallestOption = std::min(leftOption, rightOption);
			size_t largestOption = std::max(leftOption, rightOption);

			// Find the first empty left of it
			size_t indexOfFirstEmptyLeftOfFirstMove = 0ul;
			for (size_t i = smallestOption; i != -1ul; --i) {
				if (position[i] == PieceColour::NONE) {
					indexOfFirstEmptyLeftOfFirstMove = i;
					break;
				}
			}

			// And copy the segment between them one square over.
			PushShovePosition positionCopy = position;
			std::copy(
				position.begin() + (long) indexOfFirstEmptyLeftOfFirstMove + 1,
				position.begin() + (long) smallestOption + 1,
				positionCopy.begin() + (long) indexOfFirstEmptyLeftOfFirstMove
			);
			positionCopy[smallestOption] = PieceColour::NONE;

			// Then do the same for the right-most, that is largest, hand
			size_t indexOfFirstEmptyLeftOfSecondMove = 0ul;
			for (size_t i = largestOption; i != -1ul; --i) {
				if (positionCopy[i] == PieceColour::NONE) {
					indexOfFirstEmptyLeftOfSecondMove = i;
					break;
				}
			}

			std::copy(
				positionCopy.begin() + (long) indexOfFirstEmptyLeftOfSecondMove + 1,
				positionCopy.begin() + (long) largestOption + 1,
				positionCopy.begin() + (long) indexOfFirstEmptyLeftOfSecondMove
			);
			positionCopy[largestOption] = PieceColour::NONE;

			redOptions.push_back(pushDatabase->getOrInsertGameId(Push(positionCopy)));
		}
		synchedOptions.options.push_back(redOptions);
	}
}

bool Push::determineDecidedSynchedValue() {
	long long blueTotal = 0;
	long long redTotal = 0;
	for (size_t i = 0; i < position.size(); ++i) {
		switch (position[i]) {
			case PieceColour::BLUE:
				blueTotal += i + 1;
				break;
			case PieceColour::RED:
				redTotal += i + 1;
			case PieceColour::NONE:
			default:
				break;
		}
	}
	if (blueTotal * redTotal != 0)
		return false;
	synchedId = SGDatabase::getInstance().getDecidedGameWithValueId((double) (blueTotal - redTotal));
	return true;
}


Push& createPushPosition(const std::string& inputString) {
	PushShovePosition position;
	for (const auto& character : inputString) {
		position.push_back(charToPieceColour(character));
	}
	Push potentialGame = Push(position);
	return pushDatabase->getOrInsertGame(potentialGame);
}