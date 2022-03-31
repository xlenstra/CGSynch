//
// Created by s1935534 on 09/03/2022.
//

#include <utility>
#include <ranges>

#include "Shove.h"

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
			case PieceColour::WHITE:
				displayString += "R";
				break;
			case PieceColour::BLUE:
			case PieceColour::BLACK:
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
		if (*currentSquareIt == PieceColour::NONE) {
			++currentSquareIt;
			continue;
		}

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

// Source: LIP
bool Shove::tryToDetermineAbstractForm() {
	DyadicRational gameValue = DyadicRational(0);
	PushShovePosition simplifiedPosition = position;
	PushShovePosition positionWithoutEmpties;
	std::copy_if(
		position.begin(),
		position.end(),
		std::back_inserter(positionWithoutEmpties),
		[] (const auto& piece) { return piece != PieceColour::NONE; }
	);
	// First check if we only have a single piece; then the value is just the board size.
	if (positionWithoutEmpties.size() == 1) {
		gameValue += pieceColourToSign(positionWithoutEmpties.back()) * (int) simplifiedPosition.size();
		abstractForm = cgDatabase.getDyadicRational(gameValue).getId();
		return true;
	}

	// Remove all pieces from the end with equal colours, and add the value of these pieces together.
	while (positionWithoutEmpties[positionWithoutEmpties.size()-1] == positionWithoutEmpties[positionWithoutEmpties.size()-2]) {
		gameValue += pieceColourToSign(positionWithoutEmpties.back()) * (int) simplifiedPosition.size();
		positionWithoutEmpties.pop_back();
		simplifiedPosition.pop_back();
		while (simplifiedPosition.back() == PieceColour::NONE) {
			simplifiedPosition.pop_back();
		}
		// If all pieces are the same size, we're done.
		if (positionWithoutEmpties.size() < 2) {
			gameValue += pieceColourToSign(positionWithoutEmpties.back()) * (int) simplifiedPosition.size();
			abstractForm = cgDatabase.getDyadicRational(gameValue).getId();
			return true;
		}
	}

	// So now we have ended with two pieces of different colour. Next, we use the general formula.
	int denominator = 1; // r
	while (!positionWithoutEmpties.empty()) {
		int numerator = pieceColourToSign(positionWithoutEmpties.back()) * (int) simplifiedPosition.size();
		gameValue += DyadicRational(numerator, denominator);
		positionWithoutEmpties.pop_back();
		simplifiedPosition.pop_back();
		while (simplifiedPosition.back() == PieceColour::NONE) {
			simplifiedPosition.pop_back();
		}
		denominator *= 2;
	}
	abstractForm = cgDatabase.getDyadicRational(gameValue).getId();
	return true;
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