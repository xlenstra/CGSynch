//
// Created by s1935534 on 09/03/2022.
//

#include "Shove.h"

#include <utility>

// Initialize static member variables
template<> std::shared_ptr<GameDatabase<PushShovePosition, Shove>> GameDatabase<PushShovePosition, Shove>::instance = nullptr;
template<> std::vector<std::shared_ptr<Shove>> GameDatabase<PushShovePosition, Shove>::database = {};
template<> std::unordered_map<PushShovePosition, GameId> GameDatabase<PushShovePosition, Shove>::transpositionTable = {};
// Get a global variable for the actual database
// TODO: inline this
std::shared_ptr<GameDatabase<PushShovePosition, Shove>> shoveDatabase = GameDatabase<PushShovePosition, Shove>::getInstance();

Shove::Shove(PushShovePosition position) : position(std::move(position)) {
	while (!this->position.empty() && this->position.back() == PieceColour::NONE) {
		this->position.pop_back();
	}
}

std::string Shove::getDisplayString() {
	if (!displayString.empty()) return displayString;
	if (position.empty()) return "";
	for (const auto& square : position) {
		switch (square) {
			case PieceColour::RED:
				displayString += "R";
				break;
			case PieceColour::BLUE:
				displayString += "B";
				break;
			case PieceColour::NONE:
				displayString += " ";
				break;
			default:
				break;
		}
	}
	return displayString;
}

PushShovePosition Shove::getAnyTransposition() const {
	return position;
}

std::unordered_set<PushShovePosition> Shove::getTranspositions() const {
	return { position };
}

void Shove::explore() {
	auto currentSquareIt = position.begin();
	for (size_t i = 0; i < position.size(); ++i) {
		if (*currentSquareIt == PieceColour::NONE) continue;

		PushShovePosition positionCopy = position;
		// Copy the part from the second square to the current square, but one tile to the left.
		std::copy(position.begin()+1, currentSquareIt+1, positionCopy.begin());
		positionCopy[i] = PieceColour::NONE;
		if (*currentSquareIt == PieceColour::BLACK || *currentSquareIt == PieceColour::BLUE) {
			leftOptions.insert(shoveDatabase->getOrInsertGameId(Shove(positionCopy)));
		} else {
			rightOptions.insert(shoveDatabase->getOrInsertGameId(Shove(positionCopy)));
		}

		++currentSquareIt;
	}
	explored = true;
}


Shove& createShovePosition(const std::string& inputString) {
	PushShovePosition position;
	for (const auto& character : inputString) {
		switch(character) {
			case 'B':
				position.push_back(PieceColour::BLUE);
				break;
			case 'R':
			case 'W':
				position.push_back(PieceColour::RED);
				break;
			case ' ':
				position.push_back(PieceColour::NONE);
			default:
				break;
		}
	}
	Shove potentialGame = Shove(position);
	return shoveDatabase->getOrInsertGame(potentialGame);
}