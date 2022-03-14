//
// Created by s1935534 on 09/03/22.
//

#include "StackCherries.h"

#include <utility>
#include <iostream>

// Initialize static member variables
template<> std::shared_ptr<GameDatabase<StackCherriesPosition, StackCherriesGame>> GameDatabase<StackCherriesPosition, StackCherriesGame>::instance = nullptr;
template<> std::vector<std::shared_ptr<StackCherriesGame>> GameDatabase<StackCherriesPosition, StackCherriesGame>::database = {};
template<> std::unordered_map<StackCherriesPosition, GameId> GameDatabase<StackCherriesPosition, StackCherriesGame>::transpositionTable = {};
// Get a global variable for the actual database
// TODO: inline this
std::shared_ptr<GameDatabase<StackCherriesPosition, StackCherriesGame>> stackCherriesDatabase = GameDatabase<StackCherriesPosition, StackCherriesGame>::getInstance();

StackCherriesGame::StackCherriesGame(StackCherriesPosition position) : position(std::move(position)) {}

void StackCherriesGame::explore() {
	for (auto it = position.begin(); it != position.end(); ++it) {
		const auto& unconnectedLine = *it;
		// Copy position
		StackCherriesPosition copy = position;
		auto elementToRemove = copy.begin();
		auto distance = std::distance(position.begin(), it);
		std::advance(elementToRemove, distance);
		copy.erase(elementToRemove);

		std::deque<StoneColour> replacement = unconnectedLine;
		replacement.pop_front();
		if (!replacement.empty())
			copy.insert(replacement);
		if (unconnectedLine.front() == StoneColour::BLACK) {
			leftOptions.insert(stackCherriesDatabase->getOrInsertGameId(StackCherriesGame(copy)));
		} else {
			rightOptions.insert(stackCherriesDatabase->getOrInsertGameId(StackCherriesGame(copy)));
		}
	}
	explored = true;
}

std::unordered_set<StackCherriesPosition> StackCherriesGame::getTranspositions() const {
	return {position};
}

StackCherriesPosition StackCherriesGame::getAnyTransposition() const {
	return position;
}

std::string StackCherriesGame::getDisplayString() {
	if (!displayString.empty()) return displayString;
	if (position.empty()) return "";
	for (const auto& component : position) {
		for (const auto& character : component) {
			switch (character) {
				case StoneColour::WHITE:
				case StoneColour::RED:
					displayString += "W";
					break;
				case StoneColour::BLACK:
				case StoneColour::BLUE:
					displayString += "B";
					break;
				case StoneColour::NONE:
					displayString += " ";
					break;
			}
		}
		displayString += " ";
	}
	displayString += "\b";
	return displayString;
}


StackCherriesGame& createStackCherriesPosition(const std::string& inputString) {
	std::istringstream input(inputString);
	StackCherriesPosition position;
	std::deque<StoneColour> component;
	for (const auto& character : inputString) {
		switch (character) {
			case 'W':
			case 'w':
				component.push_back(StoneColour::WHITE);
				break;
			case 'B':
			case 'b':
				component.push_back(StoneColour::BLACK);
				break;
			case ' ':
				if (!component.empty()) {
					position.insert(component);
					component = std::deque<StoneColour>();
				}
				break;
			default:
				break;
		}
	}
	if (!component.empty()) {
		position.insert(component);
	}

	StackCherriesGame ruleset = StackCherriesGame(position);
	return stackCherriesDatabase->getOrInsertGame(ruleset);
}