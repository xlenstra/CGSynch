//
// Created by ardour on 18-02-22.
//

#include "RulesetCherries.h"

#include <utility>

// Initialize static member variables
template<> std::shared_ptr<RulesetDatabase<CherriesPosition, RulesetCherries>> RulesetDatabase<CherriesPosition, RulesetCherries>::instance = nullptr;
template<> std::vector<std::shared_ptr<RulesetCherries>> RulesetDatabase<CherriesPosition, RulesetCherries>::database = {};
template<> std::unordered_map<CherriesPosition, RulesetId> RulesetDatabase<CherriesPosition, RulesetCherries>::transpositionTable = {};
// Get a global variable for the actual database
// TODO: inline this
std::shared_ptr<RulesetDatabase<CherriesPosition, RulesetCherries>> cherriesDatabase = RulesetDatabase<CherriesPosition, RulesetCherries>::getInstance();

std::ostream& operator<<(std::ostream& os, CherriesPosition position) {
	static const std::string white = "W";
	static const std::string black = "B";
	static const std::string none = " ";
	for (const auto& unconnectedLine : position) {
		for (const auto& stone : unconnectedLine) {
			switch (stone) {
				case StoneColour::WHITE:
					os << white;
					break;
				case StoneColour::BLACK:
					os << black;
					break;
				case StoneColour::NONE:
				default:
					break;
			}
		}
		os << none;
	}
	return os;
}

RulesetCherries::RulesetCherries(CherriesPosition position) : position(std::move(position)) {}

void RulesetCherries::explore() {
	for (const auto& unconnectedLine : position) {
		// Copy position
		CherriesPosition copy = position;
		copy.erase(unconnectedLine);
		CherriesPosition secondCopy = copy;

		std::deque<StoneColour> replacement = unconnectedLine;
		replacement.pop_front();
		if (!replacement.empty())
			copy.insert(replacement);
		if (unconnectedLine.front() == StoneColour::WHITE) {
			leftOptions.insert(cherriesDatabase->createGameId(RulesetCherries(copy)));
		} else {
			rightOptions.insert(cherriesDatabase->createGameId(RulesetCherries(copy)));
		}

		replacement = unconnectedLine;
		replacement.pop_back();
		if (!replacement.empty())
			secondCopy.insert(replacement);
		if (unconnectedLine.back() == StoneColour::WHITE) {
			leftOptions.insert(cherriesDatabase->createGameId(RulesetCherries(secondCopy)));
		} else {
			rightOptions.insert(cherriesDatabase->createGameId(RulesetCherries(secondCopy)));
		}
	}
	explored = true;
}

std::unordered_set<CherriesPosition> RulesetCherries::getTranspositions() const {
	std::unordered_set<CherriesPosition> transpositions;
	std::unordered_set<size_t> sectionsToReverse;
	size_t depth = 0;
	addTranspositionsRecursively(transpositions, sectionsToReverse, depth);
	return transpositions;
}

void RulesetCherries::addTranspositionsRecursively(std::unordered_set<CherriesPosition>& transpositions, std::unordered_set<size_t>& sectionsToReverse, const size_t& depth) const {
	if (depth == position.size()) {
		CherriesPosition transposition {};
		int i = 0;
		for (const auto& component : position) {
			if (sectionsToReverse.contains(i)) {
				std::deque<StoneColour> reversedComponent = component;
				std::reverse(reversedComponent.begin(), reversedComponent.end());
				transposition.insert(reversedComponent);
			} else {
				transposition.insert(component);
			}
			++i;
		}
		transpositions.insert(transposition);
		return;
	}

	addTranspositionsRecursively(transpositions, sectionsToReverse, depth+1);
	sectionsToReverse.insert(depth);
	addTranspositionsRecursively(transpositions, sectionsToReverse, depth+1);
	sectionsToReverse.erase(depth);
}

CherriesPosition RulesetCherries::getAnyTransposition() const {
	return position;
}

std::string RulesetCherries::getDisplayString() {
	if (!displayString.empty()) return displayString;
	if (position.empty()) return "";
	for (const auto& component : position) {
		for (const auto& character : component) {
			switch (character) {
				case StoneColour::WHITE:
					displayString += "W";
					break;
				case StoneColour::BLACK:
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

RulesetCherries& createCherriesPosition(const std::string& inputString) {
	std::istringstream input(inputString);
	CherriesPosition position;
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

	RulesetCherries ruleset = RulesetCherries(position);
	return cherriesDatabase->createGame(ruleset);
}