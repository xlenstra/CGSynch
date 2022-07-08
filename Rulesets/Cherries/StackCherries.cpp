//
// Created by Xander Lenstra on 09/03/22.
//

#include "StackCherries.h"

#include <utility>
#include <iostream>

CreateDatabase(StackCherriesPosition, StackCherries, stackCherriesDatabase);

StackCherries::StackCherries(StackCherriesPosition position) : position(std::move(position)) {}

void StackCherries::exploreAlternating() {
	for (auto it = position.begin(); it != position.end(); ++it) {
		const auto& unconnectedLine = *it;
		// Copy position
		StackCherriesPosition copy = position;
		auto elementToRemove = copy.begin();
		auto distance = std::distance(position.begin(), it);
		std::advance(elementToRemove, distance);
		copy.erase(elementToRemove);

		std::deque<PieceColour> replacement = unconnectedLine;
		replacement.pop_front();
		if (!replacement.empty())
			copy.insert(replacement);
		if (unconnectedLine.front() == PieceColour::BLUE) {
			leftOptions.push_back(stackCherriesDatabase->getOrInsertGameId(StackCherries(copy)));
		} else {
			rightOptions.push_back(stackCherriesDatabase->getOrInsertGameId(StackCherries(copy)));
		}
	}
}

void StackCherries::exploreSynched() {
	Matrix<GameId> positions;

	int positionElementCount = 0;
	size_t blueMovesFound = 0;
	// First find a move for left
	for (const auto& blueUnconnectedLine : position) {
		if (blueUnconnectedLine.front() != PieceColour::BLUE) {
			++positionElementCount;
			continue;
		}
		++blueMovesFound;
		// Copy position
		CherriesPosition positionWithoutBlue = position;
		auto elementToRemove = positionWithoutBlue.begin();
		std::advance(elementToRemove, positionElementCount);
		positionWithoutBlue.erase(elementToRemove);

		std::vector<GameId> redOptionsForBlueFront;

		// Next, find a move for right in the original position
		for (const auto& redUnconnectedLine : position) {
			if (redUnconnectedLine.front() != PieceColour::RED) continue;
			// Then find where this section is stored in the position where red already played
			size_t elementCount = 0;
			CherriesPosition positionWithoutRedAndBlue = positionWithoutBlue;
			for (const auto& redInBlueIt : positionWithoutRedAndBlue) {
				if (redInBlueIt == redUnconnectedLine) break;
				++elementCount;
			}
			// Remove the segment we played on as well
			elementToRemove = positionWithoutRedAndBlue.begin();
			std::advance(elementToRemove, elementCount);
			positionWithoutRedAndBlue.erase(elementToRemove);

			// Then re-add both segments after removing the stones we played on
			std::deque<PieceColour> blueReplacement = blueUnconnectedLine;
			blueReplacement.pop_front();

			CherriesPosition positionWithRemovedParts = positionWithoutRedAndBlue;
			std::deque<PieceColour> redReplacement = redUnconnectedLine;
			redReplacement.pop_front();

			if (!redReplacement.empty()) positionWithRemovedParts.insert(redReplacement);
			if (!blueReplacement.empty()) positionWithRemovedParts.insert(blueReplacement);
			redOptionsForBlueFront.push_back(stackCherriesDatabase->getOrInsertGameId(StackCherries(positionWithRemovedParts)));
		}

		if (blueUnconnectedLine.front() == PieceColour::BLUE)
			positions.push_back(redOptionsForBlueFront);
		++positionElementCount;
	}
}

std::unordered_set<StackCherriesPosition> StackCherries::getTranspositions() const {
	return {position};
}

StackCherriesPosition StackCherries::getAnyTransposition() const {
	return position;
}

std::string StackCherries::getDisplayString() {
	if (!displayString.empty()) return displayString;
	if (position.empty()) return "";
	for (const auto& component : position) {
		for (const auto& character : component) {
			displayString += pieceColourToChar(character);
		}
		displayString += " ";
	}
	displayString += "\b";
	return displayString;
}

bool StackCherries::determineDecidedSynchedValue() {
	long long whiteStoneCount = 0;
	long long blackStoneCount = 0;
	long long whiteEdgeCount = 0;
	long long blackEdgeCount = 0;
	for (const auto& component : position) {
		for (const auto& piece : component) {
			switch (piece) {
				case PieceColour::BLUE:
					++blackStoneCount;
					break;
				case PieceColour::RED:
					++whiteStoneCount;
					break;
				case PieceColour::NONE:
					break;
			}
		}
		if (component[0] == PieceColour::BLUE) {
			++blackEdgeCount;
		} else {
			++whiteEdgeCount;
		}
		if (blackEdgeCount * whiteEdgeCount != 0)
			return false;
	}
	if (blackStoneCount * whiteStoneCount != 0) {
		if (!synchedGamesParser::ignoreNonSeparable && !synchedGamesParser::rulesetsForWhichUndecidableErrorWasShown.contains("Cherries")) {
			std::cout << "A decided Stack Cherries position was analyzed that had an undecided option." << std::endl
			          << "In general, this is not allowed. However, we give a value to it anyway in case you want it." << std::endl
			          << "This warning will only be shown once per session for this game."
					  << "Type 'ignoreNonSeparable' to disable this warning for all games." << std::endl << std::endl << std::endl;
			synchedGamesParser::rulesetsForWhichUndecidableErrorWasShown.insert("Cherries");
		}
		synchedId = SGDatabase::getInstance().getDecidedGameWithValueId(blackEdgeCount - whiteEdgeCount);
		return true;
	}
	synchedId = SGDatabase::getInstance().getDecidedGameWithValueId(blackStoneCount - whiteStoneCount);
	return true;
}


StackCherries& createStackCherriesPosition(const std::string& inputString) {
	std::istringstream input(inputString);
	StackCherriesPosition position;
	std::deque<PieceColour> component;
	for (const auto& character : inputString) {
		PieceColour colourToAdd = charToPieceColour(character);
		if (colourToAdd == PieceColour::NONE) {
			if (!component.empty()) {
				position.insert(component);
				component = std::deque<PieceColour>();
			}
		} else {
			component.push_back(colourToAdd);
		}
	}
	if (!component.empty()) {
		position.insert(component);
	}

	StackCherries ruleset = StackCherries(position);
	return stackCherriesDatabase->getOrInsertGame(ruleset);
}