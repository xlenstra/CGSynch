//
// Created by s1935534 on 09/03/22.
//

#include "StackCherries.h"

#include <utility>
#include <iostream>

// Initialize static member variables
template<> std::shared_ptr<GameDatabase<StackCherriesPosition, StackCherries>> GameDatabase<StackCherriesPosition, StackCherries>::instance = nullptr;
template<> std::vector<std::shared_ptr<StackCherries>> GameDatabase<StackCherriesPosition, StackCherries>::database = {};
template<> std::unordered_map<StackCherriesPosition, GameId> GameDatabase<StackCherriesPosition, StackCherries>::transpositionTable = {};
// Get a global variable for the actual database
// TODO: inline this
std::shared_ptr<GameDatabase<StackCherriesPosition, StackCherries>> stackCherriesDatabase = GameDatabase<StackCherriesPosition, StackCherries>::getInstance();

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
			leftOptions.insert(stackCherriesDatabase->getOrInsertGameId(StackCherries(copy)));
		} else {
			rightOptions.insert(stackCherriesDatabase->getOrInsertGameId(StackCherries(copy)));
		}
	}
	alternatingExplored = true;
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