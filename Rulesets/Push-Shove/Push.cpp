//
// Created by s1935534 on 09/03/2022.
//

#include "Push.h"

#include <utility>

// Initialize static member variables
template<> std::shared_ptr<GameDatabase<PushShovePosition, Push>> GameDatabase<PushShovePosition, Push>::instance = nullptr;
template<> std::vector<std::shared_ptr<Push>> GameDatabase<PushShovePosition, Push>::database = {};
template<> std::unordered_map<PushShovePosition, GameId> GameDatabase<PushShovePosition, Push>::transpositionTable = {};
// Get a global variable for the actual database
// TODO: inline this
std::shared_ptr<GameDatabase<PushShovePosition, Push>> pushDatabase = GameDatabase<PushShovePosition, Push>::getInstance();


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
				leftOptions.insert(pushDatabase->getOrInsertGameId(Push(positionCopy)));
			} else {
				rightOptions.insert(pushDatabase->getOrInsertGameId(Push(positionCopy)));
			}
		} else {
			lastEmptySquareIt = currentSquareIt;
			lastEmptySquareIndex = (int)i;
		}

		++currentSquareIt;
	}
	alternatingExplored = true;
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
	synchedOptions.leftMoveCount = leftOptions.size();
	synchedOptions.rightMoveCount = rightOptions.size();

	for (const auto& leftOption : leftOptions) {
		std::vector<GameId> redOptions;
		size_t indexOfFirstEmptyLeftOfLeftMove = 0ul;
		// We find the first empty square left of the current left move
		for (size_t i = leftOption; i != -1ul; --i) {
			if (position[i] == PieceColour::NONE) {
				indexOfFirstEmptyLeftOfLeftMove = i;
				break;
			}
		}

		// Then we create the position where that entire segment is moved one square to the left
		PushShovePosition positionCopy = position;
		std::copy(
			position.begin() + (long) indexOfFirstEmptyLeftOfLeftMove + 1,
			position.begin() + (long) leftOption + 1,
			positionCopy.begin() + (long) indexOfFirstEmptyLeftOfLeftMove
		);
		positionCopy[leftOption] = PieceColour::NONE;

		// Then for each right move
		for (const auto& rightOption : rightOptions) {
			// If that right piece is already moved, we don't have to do anything and can add it to the vector of completed positions
			if (indexOfFirstEmptyLeftOfLeftMove < rightOption && rightOption < leftOption) {
				redOptions.push_back(pushDatabase->getOrInsertGameId(Push(positionCopy)));
				continue;
			}

			// Otherwise, we find the first empty square left of that move
			size_t indexOfFirstEmptyLeftOfRightMove = 0ul;
			for (size_t i = rightOption; i != -1ul; --i) {
				if (position[i] == PieceColour::NONE) {
					indexOfFirstEmptyLeftOfRightMove = i;
					break;
				}
			}

			// And copy that segment one square over.
			PushShovePosition positionCopyCopy = positionCopy;
			std::copy(
				positionCopy.begin() + (long) indexOfFirstEmptyLeftOfRightMove + 1,
				positionCopy.begin() + (long) rightOption + 1,
				positionCopyCopy.begin() + (long) indexOfFirstEmptyLeftOfRightMove
			);
			positionCopy[rightOption] = PieceColour::NONE;

			// And add it to the vector of options.
			redOptions.push_back(pushDatabase->getOrInsertGameId(Push(positionCopyCopy)));
		}
		synchedOptions.options.push_back(redOptions);
	}


	synchedExplored = true;
}


Push& createPushPosition(const std::string& inputString) {
	PushShovePosition position;
	for (const auto& character : inputString) {
		position.push_back(charToPieceColour(character));
	}
	Push potentialGame = Push(position);
	return pushDatabase->getOrInsertGame(potentialGame);
}