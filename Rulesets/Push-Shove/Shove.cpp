//
// Created by Xander Lenstra on 09/03/2022.
//

#include <utility>
#include <ranges>

#include "Shove.h"
#include "CombinatorialGame/CombinatorialGame.h"

CreateDatabase(PushShovePosition, Shove, shoveDatabase);

Shove::Shove(PushShovePosition position) : position(std::move(position)) {
	while (!this->position.empty() && this->position.back() == PieceColour::NONE) {
		this->position.pop_back();
	}
}

std::string Shove::getDisplayString() {
	if (!displayString.empty()) return displayString;
	if (position.empty()) return "";
	for (const auto& square : position) {
		displayString += pieceColourToChar(square);
	}
	return displayString;
}

PushShovePosition Shove::getAnyTransposition() const {
	return position;
}

std::unordered_set<PushShovePosition> Shove::getTranspositions() const {
	return { position };
}

void Shove::exploreAlternating() {
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
		if (*currentSquareIt == PieceColour::BLUE) {
			leftOptions.push_back(shoveDatabase->getOrInsertGameId(Shove(positionCopy)));
		} else {
			rightOptions.push_back(shoveDatabase->getOrInsertGameId(Shove(positionCopy)));
		}

		++currentSquareIt;
	}
}

void Shove::exploreSynched() {
	std::vector<size_t> leftOptions;
	std::vector<size_t> rightOptions;
	for (size_t i = 0; i < position.size(); ++i) {
		if (position[i] == PieceColour::BLUE)
			leftOptions.push_back(i);
		else if (position[i] == PieceColour::RED)
			rightOptions.push_back(i);
	}

	for (const auto& leftOption : leftOptions) {
		std::vector<GameId> blueOptions;
		for (const auto& rightOption : rightOptions) {
			PushShovePosition positionCopy = position;
			size_t lowestSquare = std::min(leftOption, rightOption);
			size_t highestSquare = std::max(leftOption, rightOption);
			std::copy(position.begin()+2, position.begin() + lowestSquare + 2, positionCopy.begin());
			std::copy(position.begin() + lowestSquare + 1, position.begin() + highestSquare + 1, positionCopy.begin() + lowestSquare);
			positionCopy[highestSquare] = PieceColour::NONE;
			if (lowestSquare >= 1)
				positionCopy[lowestSquare-1] = PieceColour::NONE;
			blueOptions.push_back(shoveDatabase->getOrInsertGameId(Shove(positionCopy)));
		}
		synchedOptions.options.push_back(blueOptions);
	}
}

// Source: LiP
bool Shove::tryToDetermineAlternatingId() {
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
		alternatingId = CGDatabase::getInstance().getDyadicRational(gameValue).getId();
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
			alternatingId = CGDatabase::getInstance().getDyadicRational(gameValue).getId();
			return true;
		}
	}

	// So now we have ended with two pieces of different colour. Next, we use the general formula.
	long long denominator = 1; // r
	while (!positionWithoutEmpties.empty()) {
		long long numerator = pieceColourToSign(positionWithoutEmpties.back()) * (int) simplifiedPosition.size();
		gameValue += DyadicRational(numerator, denominator);
		positionWithoutEmpties.pop_back();
		simplifiedPosition.pop_back();
		while (simplifiedPosition.back() == PieceColour::NONE) {
			simplifiedPosition.pop_back();
		}
		denominator *= 2;
		if (denominator >= 4294967296ll) {
			std::cout << "Automatic shove value calculation failed; position has too large a denominator as a fraction. Trying manual search." << std::endl;
			return false;
		}
	}
	alternatingId = CGDatabase::getInstance().getDyadicRational(gameValue).getId();
	return true;
}

bool Shove::determineDecidedSynchedValue() {
	long long lastRedPos = -1;
	long long lastBluePos = -1;
	for (size_t i = 0; i < position.size(); ++i) {
		switch (position[i]) {
			case PieceColour::BLUE:
				lastBluePos = i;
				break;
			case PieceColour::RED:
				lastRedPos = i;
				break;
			case PieceColour::NONE:
				break;
		}
		if (lastBluePos != -1 && lastRedPos != -1)
			return false;
	}
	synchedId = SGDatabase::getInstance().getDecidedGameWithValueId((double) (lastBluePos - lastRedPos));
	return true;
}


Shove& createShovePosition(const std::string& inputString) {
	PushShovePosition position;
	for (const auto& character : inputString) {
		position.push_back(charToPieceColour(character));
	}
	Shove potentialGame = Shove(position);
	return shoveDatabase->getOrInsertGame(potentialGame);
}