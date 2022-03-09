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
	while (!this->position.empty() && this->position.back() == StoneColour::NONE) {
		this->position.pop_back();
	}
}

std::string Push::getDisplayString() {
	if (!displayString.empty()) return displayString;
	if (position.empty()) return "";
	for (const auto& square : position) {
		switch (square) {
			case StoneColour::RED:
				displayString += "R";
				break;
			case StoneColour::BLUE:
				displayString += "B";
				break;
			case StoneColour::NONE:
				displayString += " ";
				break;
			default:
				break;
		}
	}
	return displayString;
}

PushShovePosition Push::getAnyTransposition() const {
	return position;
}

std::unordered_set<PushShovePosition> Push::getTranspositions() const {
	return { position };
}

void Push::explore() {
	int lastEmptySquareIndex = 0;
	auto lastEmptySquareIt = position.begin();
	auto currentSquareIt = position.begin();
	// We loop from left to right, and every time we find any piece, we try every move with it.
	for (size_t i = 0; i < position.size(); ++i) {
		if (*currentSquareIt != StoneColour::NONE) {
			PushShovePosition positionCopy = position;
			// Copy the part from the second square to the current square, but one tile to the left.
			std::copy(lastEmptySquareIt+1, currentSquareIt+1, positionCopy.begin() + lastEmptySquareIndex);
			positionCopy[i] = StoneColour::NONE;
			if (*currentSquareIt == StoneColour::BLACK || *currentSquareIt == StoneColour::BLUE) {
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
	explored = true;
}


Push& createPushPosition(const std::string& inputString) {
	PushShovePosition position;
	for (const auto& character : inputString) {
		switch(character) {
			case 'B':
				position.push_back(StoneColour::BLUE);
				break;
			case 'R':
			case 'W':
				position.push_back(StoneColour::RED);
				break;
			case ' ':
				position.push_back(StoneColour::NONE);
			default:
				break;
		}
	}
	Push potentialGame = Push(position);
	return pushDatabase->getOrInsertGame(potentialGame);
}